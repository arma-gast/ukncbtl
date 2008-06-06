// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "diskimage.h"
#include "rad50.h"


//////////////////////////////////////////////////////////////////////
// Структуры данных, представляющие информацию о томе RT-11

struct CVolumeCatalogEntry;

// Структура для хранения информации о томе
struct CVolumeInformation {
    char volumeid[13];
    char ownername[13];
    char systemid[13];
    WORD firstcatalogblock;
    WORD systemversion;
    WORD catalogextrawords;
    WORD catalogentriespersegment;
    WORD catalogsegmentcount;
    WORD lastopenedsegment;
    // Массив записей каталога, размером в максимально возможное кол-во записей для этого кол-ва сегментов каталога
    CVolumeCatalogEntry* catalogentries;
    WORD catalogentriescount;  // Количество валидных записей каталога, включая завершающую ENDMARK
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
void DecodeImageCatalog();
void FreeImageCatalog();
void PrepareTrack(int nSide, int nTrack);
void DoPrintCatalogDirectory();
void DoExtractFile();
BOOL DoAddFile();


//////////////////////////////////////////////////////////////////////
// Глобальные переменные

LPCTSTR g_sCommand = NULL;
LPCTSTR g_sImageFileName = NULL;
LPCTSTR g_sFileName = NULL;

CDiskImage g_diskimage;

CVolumeInformation g_volumeinfo;


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
    if (!g_diskimage.Attach(g_sImageFileName))
        return 255;

    // Разбор Home Block и чтение каталога диска
    DecodeImageCatalog();

    // Main task
    if (g_sCommand[0] == _T('l'))
        DoPrintCatalogDirectory();
    else if (g_sCommand[0] == _T('e'))
        DoExtractFile();
    else if (g_sCommand[0] == _T('a'))
    {
        DoPrintCatalogDirectory();  //DEBUG
        DoAddFile();
    }

    // Завершение работы с файлом
    FreeImageCatalog();
    g_diskimage.Detach();

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
    if (g_sCommand[0] != _T('l') && g_sCommand[0] != _T('e') && g_sCommand[0] != _T('a'))
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
    wprintf(_T("Usage:  rt11dsk a <ImageFile> <FileName>  - add file\n"));
    wprintf(_T("\n  <ImageFile> is UKNC disk image in .dsk or .rtd format\n"));
}


//////////////////////////////////////////////////////////////////////

// Разбор Home Block и чтение каталога диска.
void DecodeImageCatalog()
{
    memset(&g_volumeinfo, 0, sizeof(g_volumeinfo));

    // Разбор Home Block
    BYTE* pHomeSector = g_diskimage.GetBlock(1);
    WORD nFirstCatalogBlock = pHomeSector[0724];  // Это должен быть блок номер 6
    if (nFirstCatalogBlock == 0) nFirstCatalogBlock = 6;
    g_volumeinfo.firstcatalogblock = nFirstCatalogBlock;
    g_volumeinfo.systemversion = pHomeSector[0726];
    LPCSTR sVolumeId = (LPCSTR) pHomeSector + 0730;
    strncpy_s(g_volumeinfo.volumeid, 13, sVolumeId, 12);
    LPCSTR sOwnerName = (LPCSTR) pHomeSector + 0744;
    strncpy_s(g_volumeinfo.ownername, 13, sOwnerName, 12);
    LPCSTR sSystemId = (LPCSTR) pHomeSector + 0760;
    strncpy_s(g_volumeinfo.systemid, 13, sSystemId, 12);

    // Разбор первого блока каталога
    WORD* pCatalogSector = (WORD*) g_diskimage.GetBlock(nFirstCatalogBlock);
    g_volumeinfo.catalogsegmentcount = pCatalogSector[0];
    g_volumeinfo.lastopenedsegment = pCatalogSector[2];
    WORD nExtraBytesLength = pCatalogSector[3];
    WORD nExtraWordsLength = (nExtraBytesLength + 1) / 2;
    g_volumeinfo.catalogextrawords = nExtraWordsLength;
    WORD nEntryLength = 7 + nExtraWordsLength;  // Total catalog entry length, in words
    WORD nEntriesPerSegment = (512 - 5) / nEntryLength;
    g_volumeinfo.catalogentriespersegment = nEntriesPerSegment;

    // Allocate memory for catalog entry list
    g_volumeinfo.catalogentries = (CVolumeCatalogEntry*) malloc(
        sizeof(CVolumeCatalogEntry) * nEntriesPerSegment * g_volumeinfo.catalogsegmentcount);
    memset(g_volumeinfo.catalogentries, 0,
        sizeof(CVolumeCatalogEntry) * nEntriesPerSegment * g_volumeinfo.catalogsegmentcount);

    //TODO: Для заголовка самого первого сегмента каталога существует правило:
    //      если удвоить содержимое слова 1 и к результату прибавить начальный блок каталога (обычно 6),
    //      то получиться содержимое слова 5. Таким образом RT-11 отличает свой каталог от чужого.

    WORD nCatalogEntriesCount = 0;
    WORD nCatalogSegmentNumber = 1;
    CVolumeCatalogEntry* pEntry = g_volumeinfo.catalogentries;

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
            nCatalogEntriesCount++;

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
        pCatalogSector = (WORD*) g_diskimage.GetBlock(nCatalogBlock);
        nCatalogSegmentNumber = nNextSegment;
    }

    pEntry->status = RT11_STATUS_ENDMARK;
    g_volumeinfo.catalogentriescount = nCatalogEntriesCount;
}

void FreeImageCatalog()
{
    free(g_volumeinfo.catalogentries);
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
    CVolumeCatalogEntry* pEntry = g_volumeinfo.catalogentries;

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
    CVolumeCatalogEntry* pEntry = g_volumeinfo.catalogentries;
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
        BYTE* pData = g_diskimage.GetBlock(filestart + blockpos);
        size_t nBytesWritten = fwrite(pData, sizeof(BYTE), RT11_BLOCK_SIZE, foutput);
        //TODO: Check if nBytesWritten < RT11_BLOCK_SIZE
    }

    fclose(foutput);

    wprintf(_T("\nDone.\n"));
}

// Помещение файла в образ.
// Алгоритм:
//   Помещаемый файл считывается в память
//   Перебираются все записи каталога, пока не будет найдена пустая запись большей или равной длины
//   Если нужно, в конце каталога создается новая пустая запись
//   В файл образа прописывается измененная запись каталога
//   Если нужно, в файл образа прописывается новая пустая запись каталога
//   В файл образа прописываются блоки нового файла
//NOTE: Пока НЕ обрабатываем ситуацию открытия нового блока каталога - выходим по ошибке
//NOTE: Пока НЕ проверяем что файл с таким именем уже есть, и НЕ выдаем ошибки
BOOL DoAddFile()
{
    // Открываем помещаемый файл на чтение
    HANDLE hFile = ::CreateFile(g_sFileName,
            GENERIC_READ, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        wprintf(_T("Failed to open file."));
        return FALSE;
    }

    // Определяем длину файла, с учетом округления до полного блока
    DWORD dwFileLength = ::GetFileSize(hFile, NULL);  // Точная длина файла
    WORD nFileSizeBlocks =  // Требуемая ширина свободного места в блоках
        (WORD) (dwFileLength + RT11_BLOCK_SIZE - 1) / RT11_BLOCK_SIZE;
    DWORD dwFileSize =  // Длина файла с учетом округления до полного блока
        ((DWORD) nFileSizeBlocks) * RT11_BLOCK_SIZE;
    //TODO: Проверка на файл нулевой длины
    //TODO: Проверка, не слишком ли длинный файл для этого тома

    // Выделяем память и считываем данные файла
    PVOID pFileData = malloc(dwFileSize);
    memset(pFileData, 0, dwFileSize);
    DWORD dwBytesRead;
    ::ReadFile(hFile, pFileData, dwFileLength, &dwBytesRead, NULL);
    //TODO: Проверка на ошибки чтения
    ::CloseHandle(hFile);

    wprintf(_T("File size is %ld bytes or %d blocks\n"), dwFileLength, nFileSizeBlocks);

    // Перебираются все записи каталога, пока не будет найдена пустая запись длины >= dwFileLength
    //TODO: Выделить в отдельную функцию и искать наиболее подходящую запись, с минимальной разницей по длине
    CVolumeCatalogEntry* pEntry = g_volumeinfo.catalogentries;
    while (pEntry->status != RT11_STATUS_ENDMARK)
    {
        if (pEntry->status == RT11_STATUS_EMPTY && pEntry->length >= nFileSizeBlocks)
            break;
        pEntry++;
    }
    if (pEntry->status == RT11_STATUS_ENDMARK)
    {
        wprintf(_T("Empty catalog entry with %d or more blocks not found\n"), nFileSizeBlocks);
        free(pFileData);
        return FALSE;
    }
    wprintf(_T("Found < UNUSED > catalog entry with %d blocks\n"), pEntry->length);

    // Определяем, нужна ли новая запись каталога
    BOOL okNeedNewCatalogEntry = (pEntry->length != nFileSizeBlocks);
    CVolumeCatalogEntry* pNewEntry = NULL;
    CVolumeCatalogEntry* pNewEndmarkEntry = NULL;
    if (okNeedNewCatalogEntry)
    {
        // Проверяем, нужно ли для новой записи каталога открывать новый блок каталога
        if (g_volumeinfo.catalogentriescount % g_volumeinfo.catalogentriespersegment == 0)
        {
            wprintf(_T("We have to open new catalog segment - not implemented now, sorry.\n"));
            free(pFileData);
            return FALSE;
        }

        // Определяем, в какие записи каталога мы должны прописать новую пустую запись и новую ENDMARK
        pNewEntry = g_volumeinfo.catalogentries + g_volumeinfo.catalogentriescount - 1;
        pNewEndmarkEntry = pNewEntry + 1;
        // Заполнить данные новой записи каталога
        pNewEntry->status = RT11_STATUS_EMPTY;
        pNewEntry->start = pEntry->start + nFileSizeBlocks;
        pNewEntry->length = pEntry->length - nFileSizeBlocks;
        //TODO: pNewEntry->datestr = 
        // Заполнить данные новой ENDMARK
        pNewEndmarkEntry->status = RT11_STATUS_ENDMARK;
    }

    // Изменяем существующую запись каталога
    pEntry->length = nFileSizeBlocks;
    //TODO: pEntry->name = 
    //TODO: pEntry->ext = 
    //TODO: pEntry->datestr = 
    pEntry->status = RT11_STATUS_PERM;

    // Сохраняем новый файл поблочно
    WORD nFileStartBlock = pEntry->start;  // Начиная с какого блока размещается новый файл
    WORD nBlock = nFileStartBlock;
    for (int block = 0; block < nFileSizeBlocks; block++)
    {
        BYTE* pFileBlockData = ((BYTE*) pFileData) + block * RT11_BLOCK_SIZE;
        BYTE* pData = g_diskimage.GetBlock(nBlock);
        memcpy(pData, pFileBlockData, RT11_BLOCK_SIZE);

        //TODO: Сообщить что блок был изменен
        
        nBlock++;
    }
    free(pFileData);

    if (okNeedNewCatalogEntry)
    {
        //TODO: Сохраняем новую запись каталога
        //TODO: Сохраняем новую ENDMARK
    }

    return TRUE;
}


//////////////////////////////////////////////////////////////////////
