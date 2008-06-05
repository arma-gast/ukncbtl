// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rad50.h"


//////////////////////////////////////////////////////////////////////


/* Types for rtFileEntry 'status' */
#define RT11_STATUS_TENTATIVE   256     /* Temporary file */
#define RT11_STATUS_EMPTY       512     /* Marks empty space */
#define RT11_STATUS_PERM        1024    /* A "real" file */
#define RT11_STATUS_ENDMARK     2048    /* Marks the end of file entries */
/* Size of each RT-11 disk block, 512 or 0x200 bytes */
#define RT11_BLOCK_SIZE         512
#define RT11_TRACK_SIZE         5120
#define NETRT11_IMAGE_HEADER_SIZE  256


//////////////////////////////////////////////////////////////////////


// Структура для хранения информации о томе
struct CVolumeInformation {
    char volumeid[13];
    char ownername[13];
    char systemid[13];
    WORD catalogsegmentcount;
    WORD lastopenedsegment;
};

// Структура для хранения разобранной строки каталога
struct CVolumeCatalogEntry {
    WORD status;  // See RT11_STATUS_xxx constants
    TCHAR name[7];  // File name - 6 characters
    TCHAR ext[4];   // File extension - 3 characters
    TCHAR datestr[10];
    WORD start;     // File start block number
    WORD length;    // File length in 512-byte blocks
};


//////////////////////////////////////////////////////////////////////
// Предварительные объявления функций программы

void PrintWelcome();
BOOL ParseCommandLine(int argc, _TCHAR* argv[]);
void PrintUsage();
BOOL AttachImage();
void DetachImage();
void DecodeImageCatalog();
void PrepareTrack(int nSide, int nTrack);
BYTE* GetSector(int nSector);
BYTE* GetBlock(int nBlock);
void DoPrintCatalogDirectory();
void DoExtractFile();


//////////////////////////////////////////////////////////////////////
// Глобальные переменные

LPCTSTR g_sCommand = NULL;
LPCTSTR g_sImageFileName = NULL;
LPCTSTR g_sFileName = NULL;

BOOL g_okNetRT11Image = FALSE;
HANDLE g_hFile = INVALID_HANDLE_VALUE;
BYTE g_TrackData[RT11_TRACK_SIZE];
int g_nCurrentSide = -1;
int g_nCurrentTrack = -1;

CVolumeInformation g_volumeinfo;
CVolumeCatalogEntry* g_pCatalogEntries = NULL;


//////////////////////////////////////////////////////////////////////


int _tmain(int argc, _TCHAR* argv[])
{
    PrintWelcome();

    if (!ParseCommandLine(argc, argv))
    {
        PrintUsage();
        return 255;
    }

    // Подключение к файлу образа
    if (!AttachImage())
        return 255;

    // Разбор Home Block и чтение каталога диска
    DecodeImageCatalog();

    // Main task
    if (g_sCommand[0] == _T('l'))
        DoPrintCatalogDirectory();
    else if (g_sCommand[0] == _T('e'))
        DoExtractFile();

    // Завершение работы с файлом
    DetachImage();

    return 0;
}

void PrintWelcome()
{
    wprintf(_T("RT11DSK Utility  by Nikita Zeemin  [%S %S]\n\n"), __DATE__, __TIME__);
}

BOOL ParseCommandLine(int argc, _TCHAR* argv[])
{
    for (int argn = 1; argn < argc; argn++)
    {
        LPCTSTR arg = argv[argn];
        if (arg[0] == _T('-') || arg[0] == _T('/'))
        {
            //TODO
            wprintf(_T("Unknown option: %s\n"), arg);
            return FALSE;
        }
        else
        {
            if (g_sCommand == NULL)
                g_sCommand = arg;
            else if (g_sImageFileName == NULL)
                g_sImageFileName = arg;
            else if (g_sFileName == NULL)
                g_sFileName = arg;
            else
            {
                wprintf(_T("Unknown param: %s\n"), arg);
                return FALSE;
            }
        }
    }

    // Parsed options validation
    if (g_sCommand == NULL)
    {
        wprintf(_T("Command not specified.\n"));
        return FALSE;
    }
    if (g_sCommand[0] != _T('l') && g_sCommand[0] != _T('e'))
    {
        wprintf(_T("Unknown command: %s.\n"), g_sCommand);
        return FALSE;
    }
    if (g_sImageFileName == NULL)
    {
        wprintf(_T("Image file not specified.\n"));
        return FALSE;
    }

    return TRUE;
}

void PrintUsage()
{
    wprintf(_T("\n"));
    wprintf(_T("Usage:  rt11dsk l <ImageFile>  - list image contents\n"));
    wprintf(_T("Usage:  rt11dsk e <ImageFile> <FileName>  - extract file\n"));
    wprintf(_T("\n  <ImageFile> is UKNC disk image in .dsk or .rtd format\n"));
}


//////////////////////////////////////////////////////////////////////

// Подготовка образа диска к работе - открытие файла итп.
BOOL AttachImage()
{
    // Определяем, это .dsk-образ или .rtd-образ - по расширению файла
    g_okNetRT11Image = FALSE;
    LPCTSTR sImageFilenameExt = wcsrchr(g_sImageFileName, _T('.'));
    if (sImageFilenameExt != NULL && _wcsicmp(sImageFilenameExt, _T(".rtd")) == 0)
        g_okNetRT11Image = TRUE;

    g_hFile = ::CreateFile(g_sImageFileName,
            GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_hFile == INVALID_HANDLE_VALUE)
    {
        wprintf(_T("Failed to open file."));
        return FALSE;
    }

    return TRUE;
}

// Завершение работы с образом диска - сохранение последних изменений, закрытие файла итп.
void DetachImage()
{
    ::CloseHandle(g_hFile);
    g_hFile = INVALID_HANDLE_VALUE;
    free(g_pCatalogEntries);
}

// Разбор Home Block и чтение каталога диска.
void DecodeImageCatalog()
{
    memset(&g_volumeinfo, 0, sizeof(g_volumeinfo));

    // Разбор Home Block
    BYTE* pHomeSector = GetBlock(1);
    WORD nFirstCatalogBlock = pHomeSector[0724];  // Это должен быть блок номер 6
    if (nFirstCatalogBlock == 0) nFirstCatalogBlock = 6;
    WORD nSystemVersion = pHomeSector[0726];
    LPCSTR sVolumeId = (LPCSTR) pHomeSector + 0730;
    strncpy_s(g_volumeinfo.volumeid, 13, sVolumeId, 12);
    LPCSTR sOwnerName = (LPCSTR) pHomeSector + 0744;
    strncpy_s(g_volumeinfo.ownername, 13, sOwnerName, 12);
    LPCSTR sSystemId = (LPCSTR) pHomeSector + 0760;
    strncpy_s(g_volumeinfo.systemid, 13, sSystemId, 12);

    // Разбор первого блока каталога
    WORD* pCatalogSector = (WORD*) GetBlock(nFirstCatalogBlock);
    g_volumeinfo.catalogsegmentcount = pCatalogSector[0];
    g_volumeinfo.lastopenedsegment = pCatalogSector[2];
    WORD nExtraBytesLength = pCatalogSector[3];
    WORD nExtraWordsLength = (nExtraBytesLength + 1) / 2;
    WORD nEntryLength = 7 + nExtraWordsLength;  // Total catalog entry length, in words
    WORD nEntriesPerSegment = (512 - 5) / nEntryLength;

    // Allocate memory for catalog entry list
    g_pCatalogEntries = (CVolumeCatalogEntry*) malloc(
        sizeof(CVolumeCatalogEntry) * nEntriesPerSegment * g_volumeinfo.catalogsegmentcount);
    memset(g_pCatalogEntries, 0,
        sizeof(CVolumeCatalogEntry) * nEntriesPerSegment * g_volumeinfo.catalogsegmentcount);

    //TODO: Для заголовка самого первого сегмента каталога существует правило:
    //      если удвоить содержимое слова 1 и к результату прибавить начальный блок каталога (обычно 6),
    //      то получиться содержимое слова 5. Таким образом RT-11 отличает свой каталог от чужого.

    WORD nCatalogSegmentNumber = 1;
    CVolumeCatalogEntry* pEntry = g_pCatalogEntries;

    for (;;)
    {
        WORD nStartBlock = pCatalogSector[4];  // Номер блока, с которого начинаются файлы этого сегмента
        //wprintf(_T("Segment %d start block: %d\n"), nCatalogSegmentNumber, nStartBlock);
        WORD nNextSegment = pCatalogSector[1];
        //wprintf(_T("Next segment:           %d\n"), nNextSegment);

        WORD* pCatalog = pCatalogSector + 5;  // Начало описаний файлов
        WORD nFileStartBlock = nStartBlock;
        for (;;)  // Цикл по записям данного сегмента каталога
        {
            WORD status = pCatalog[0];

            WORD namerad50[3];
            namerad50[0] = pCatalog[1];
            namerad50[1] = pCatalog[2];
            namerad50[2] = pCatalog[3];
            WORD length  = pCatalog[4];
            WORD datepac = pCatalog[6];

            if (status == RT11_STATUS_ENDMARK)
                break;
            if (status == RT11_STATUS_EMPTY)
            {
                pEntry->status = status;
                pEntry->length = length;
                pEntry->start = nFileStartBlock;

                pEntry++;
            }
            else
            {
                pEntry->status = status;
                pEntry->length = length;
                pEntry->start = nFileStartBlock;
                r50asc(6, namerad50, pEntry->name);
                pEntry->name[6] = 0;
                r50asc(3, namerad50 + 2, pEntry->ext);
                pEntry->ext[3] = 0;

                rtDateStr(datepac, pEntry->datestr);

                pEntry++;
            }

            nFileStartBlock += length;
            pCatalog += nEntryLength;
            if (pCatalog - pCatalogSector > 256 * 2 - nEntryLength)  // Сегмент закончился
                break;
        }

        if (nNextSegment == 0) break;  // Конец цепочки сегментов

        // Переходим к следующему сегменту каталога
        WORD nCatalogBlock = nFirstCatalogBlock + (nNextSegment - 1) * 2;
        pCatalogSector = (WORD*) GetBlock(nCatalogBlock);
        nCatalogSegmentNumber = nNextSegment;
    }

    pEntry->status = RT11_STATUS_ENDMARK;
}

// nSide = 0..1
// nTrack = 0..79
void PrepareTrack(int nSide, int nTrack)
{
    if (g_nCurrentSide == nSide && g_nCurrentTrack == nTrack)
        return;  // Повторного чтения не требуется

    memset(g_TrackData, 0, RT11_TRACK_SIZE);

    long foffset = long(nTrack * 2 + nSide) * RT11_TRACK_SIZE;  // File offset to read from
    if (g_okNetRT11Image) foffset += NETRT11_IMAGE_HEADER_SIZE;
    ::SetFilePointer(g_hFile, foffset, NULL, FILE_BEGIN);

    DWORD count;
    ::ReadFile(g_hFile, g_TrackData, RT11_TRACK_SIZE, &count, NULL);

    g_nCurrentSide = nSide;
    g_nCurrentTrack = nTrack;
}

// nSector = 1..10
BYTE* GetSector(int nSector)
{
    return g_TrackData + (nSector - 1) * RT11_BLOCK_SIZE;
}

// Переход к заданному блоку файла образа диска. Если нужно, выполняется чтение другой дорожки.
// Каждый блок - 256 слов, 512 байт
// Каждый track = 10 блоков
// nBlock = 1..???
BYTE* GetBlock(int nBlock)
{
    int nTrackAndSide = nBlock / 10;
    int nSector = nBlock % 10 + 1;
    int nTrack = nTrackAndSide / 2;
    int nSide = nTrackAndSide % 2;

    PrepareTrack(nSide, nTrack);

    return GetSector(nSector);
}

// Печать всего каталога диска
void DoPrintCatalogDirectory()
{
    wprintf(_T(" Volume: %S\n"), g_volumeinfo.volumeid);
    wprintf(_T(" Owner:  %S\n"), g_volumeinfo.ownername);
    wprintf(_T(" System: %S\n"), g_volumeinfo.systemid);
    wprintf(_T("\n"));
    wprintf(_T(" %d available segments, last opened segment: %d\n"), g_volumeinfo.catalogsegmentcount, g_volumeinfo.lastopenedsegment);
    wprintf(_T("\n"));
    wprintf(_T(" Filename  Blocks  Date      Start    Bytes\n"));
    wprintf(_T("---------- ------  --------- ----- --------\n"));

    WORD nFilesCount = 0;
    WORD nBlocksCount = 0;
    WORD nFreeBlocksCount = 0;
    CVolumeCatalogEntry* pEntry = g_pCatalogEntries;

    while (pEntry->status != RT11_STATUS_ENDMARK)
    {
        if (pEntry->status == RT11_STATUS_EMPTY)
        {
            wprintf(_T("< UNUSED >  %5d            %5d %8d\n"),
                    pEntry->length, pEntry->start, pEntry->length * RT11_BLOCK_SIZE);
            nFreeBlocksCount += pEntry->length;
        }
        else
        {
            wprintf(_T("%s.%s  %5d  %s %5d %8d\n"),
                    pEntry->name, pEntry->ext, pEntry->length, pEntry->datestr, pEntry->start, pEntry->length * RT11_BLOCK_SIZE);
            nFilesCount++;
            nBlocksCount += pEntry->length;
        }

        pEntry++;
    }

    wprintf(_T("---------- ------  --------- ----- --------\n"));
    wprintf(_T(" %d files, %d blocks\n"), nFilesCount, nBlocksCount);
    wprintf(_T(" %d free blocks\n"), nFreeBlocksCount);
    wprintf(_T("\n"));
}

// Извлечение файла из образа в отдельный файл.
void DoExtractFile()
{
    // Parse g_sFileName
    LPCTSTR sFilenameExt = wcsrchr(g_sFileName, _T('.'));
    if (sFilenameExt == NULL)
    {
        wprintf(_T("Wrong filename format: %s\n"), g_sFileName);
        return;
    }
    size_t nFilenameLength = sFilenameExt - g_sFileName;
    if (nFilenameLength == 0 || nFilenameLength > 6)
    {
        wprintf(_T("Wrong filename format: %s\n"), g_sFileName);
        return;
    }
    size_t nFileextLength = wcslen(g_sFileName) - nFilenameLength - 1;
    if (nFileextLength == 0 || nFileextLength > 3)
    {
        wprintf(_T("Wrong filename format: %s\n"), g_sFileName);
        return;
    }
    TCHAR filename[7];
    for (int i = 0; i < 6; i++) filename[i] = _T(' ');
    for (WORD i = 0; i < nFilenameLength; i++) filename[i] = g_sFileName[i];
    filename[6] = 0;
    TCHAR fileext[4];
    for (int i = 0; i < 3; i++) fileext[i] = _T(' ');
    for (WORD i = 0; i < nFileextLength; i++) fileext[i] = sFilenameExt[i + 1];
    fileext[3] = 0;

    // Search for the filename/fileext
    CVolumeCatalogEntry* pEntry = g_pCatalogEntries;
    while (pEntry->status != RT11_STATUS_ENDMARK)
    {
        if (_wcsnicmp(filename, pEntry->name, 6) == 0 &&
            _wcsnicmp(fileext, pEntry->ext, 3) == 0)
            break;
        pEntry++;
    }
    if (pEntry->status == RT11_STATUS_ENDMARK)
    {
        wprintf(_T("Filename not found: %s\n"), g_sFileName);
        return;
    }
    wprintf(_T("Extracting file:\n\n"));
    wprintf(_T(" Filename  Blocks  Date      Start    Bytes\n\n"));
    wprintf(_T("%s.%s  %5d  %s %5d %8d\n"),
            pEntry->name, pEntry->ext, pEntry->length, pEntry->datestr, pEntry->start, pEntry->length * RT11_BLOCK_SIZE);

    WORD filestart = pEntry->start;
    WORD filelength = pEntry->length;

    FILE* foutput = NULL;
    errno_t err = _wfopen_s(&foutput, g_sFileName, _T("wb"));
    if (err != 0)
    {
        wprintf(_T("Failed to open output file %s: error %d\n"), g_sFileName, err);
        return;
    }

    for (WORD blockpos = 0; blockpos < filelength; blockpos++)
    {
        BYTE* pData = GetBlock(filestart + blockpos);
        size_t nBytesWritten = fwrite(pData, sizeof(BYTE), RT11_BLOCK_SIZE, foutput);
        //TODO: Check if nBytesWritten < RT11_BLOCK_SIZE
    }

    fclose(foutput);

    wprintf(_T("\nDone.\n"));
}


//////////////////////////////////////////////////////////////////////
