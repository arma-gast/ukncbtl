// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "diskimage.h"
#include "rad50.h"
#include "hardimage.h"


//////////////////////////////////////////////////////////////////////
// Предварительные объявления функций программы

void PrintWelcome();
bool ParseCommandLine(int argc, _TCHAR* argv[]);
void PrintUsage();
void PrintTableHeader();
void PrintTableFooter();
void DecodeImageCatalog();
void FreeImageCatalog();
void PrepareTrack(int nSide, int nTrack);
void DoPrintCatalogDirectory();
void DoExtractFile();
bool DoAddFile();
bool DoHardList();
void DoHardExtractPartition();
void UpdateCatalogSegment(CVolumeCatalogSegment* pSegment);


//////////////////////////////////////////////////////////////////////
// Глобальные переменные

LPCTSTR g_sCommand = NULL;
LPCTSTR g_sImageFileName = NULL;
LPCTSTR g_sFileName = NULL;
bool    g_okHardCommand = false;
LPCTSTR g_sPartition = NULL;
int     g_nPartition = -1;

CDiskImage g_diskimage;
CHardImage g_hardimage;
CVolumeInformation g_volumeinfo;
WORD g_segmentBuffer[512];

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
    if (g_okHardCommand)
    {
        if (!g_hardimage.Attach(g_sImageFileName))
        {
            wprintf(_T("Failed to open the image file.\n"));
            return 255;
        }
    }
    else
    {
        if (!g_diskimage.Attach(g_sImageFileName))
        {
            wprintf(_T("Failed to open the image file.\n"));
            return 255;
        }
        // Разбор Home Block и чтение каталога диска
        DecodeImageCatalog();
    }

    // Main task
    if (g_sCommand[0] == _T('l'))
        DoPrintCatalogDirectory();
    else if (g_sCommand[0] == _T('e'))
        DoExtractFile();
    else if (g_sCommand[0] == _T('a'))
        DoAddFile();
    else if (g_sCommand[0] == _T('h') && g_sCommand[1] == _T('l'))
        DoHardList();
    else if (g_sCommand[0] == _T('h') && g_sCommand[1] == _T('x'))
        DoHardExtractPartition();

    // Завершение работы с файлом
    if (g_okHardCommand)
    {
        g_hardimage.Detach();
    }
    else
    {
        FreeImageCatalog();
        g_diskimage.Detach();
    }

    return 0;
}

void PrintWelcome()
{
    wprintf(_T("RT11DSK Utility  by Nikita Zimin  [%S %S]\n\n"), __DATE__, __TIME__);
}

bool ParseCommandLine(int argc, _TCHAR* argv[])
{
    for (int argn = 1; argn < argc; argn++)
    {
        LPCTSTR arg = argv[argn];
        if (arg[0] == _T('-') || arg[0] == _T('/'))
        {
            //TODO
            wprintf(_T("Unknown option: %s\n"), arg);
            return false;
        }
        else
        {
            if (g_sCommand == NULL)
                g_sCommand = arg;
            else if (g_sImageFileName == NULL)
                g_sImageFileName = arg;
            else if (g_sCommand[0] == _T('h') && g_sPartition == NULL)
                g_sPartition = arg;
            else if (g_sFileName == NULL)
                g_sFileName = arg;
            else
            {
                wprintf(_T("Unknown param: %s\n"), arg);
                return false;
            }
        }
    }

    // Parsed options validation
    if (g_sCommand == NULL)
    {
        wprintf(_T("Command not specified.\n"));
        return false;
    }
    if (g_sCommand[0] == _T('h'))
    {
        g_okHardCommand = true;
        if (g_sCommand[1] == 0)
        {
            wprintf(_T("H-command not specified.\n"));
            return false;
        }
        if (g_sCommand[1] != _T('l') && g_sCommand[1] != _T('x'))
        {
            wprintf(_T("Unknown H-command: %s.\n"), g_sCommand);
            return false;
        }
        if (g_sPartition != NULL)
        {
            g_nPartition = _wtoi(g_sPartition);
        }

        return true;
    }
    if (g_sCommand[0] != _T('l') && g_sCommand[0] != _T('e') && g_sCommand[0] != _T('a'))
    {
        wprintf(_T("Unknown command: %s.\n"), g_sCommand);
        return false;
    }
    if (g_sImageFileName == NULL)
    {
        wprintf(_T("Image file not specified.\n"));
        return false;
    }

    return true;
}

void PrintUsage()
{
    wprintf(_T("\nUsage:\n"));
    wprintf(_T("  Disk image commands:\n"));
    wprintf(_T("    rt11dsk l <ImageFile>  - list image contents\n"));
    wprintf(_T("    rt11dsk e <ImageFile> <FileName>  - extract file\n"));
    wprintf(_T("    rt11dsk a <ImageFile> <FileName>  - add file\n"));
    wprintf(_T("  Hard disk image commands:\n"));
    wprintf(_T("    rt11dsk hl <HddImage>  - list HDD image partitions\n"));
    wprintf(_T("    rt11dsk hx <HddImage> <Partn> <FileName>  - extract partition to file\n"));
    wprintf(_T("  Parameters:\n"));
    wprintf(_T("    <ImageFile> is UKNC disk image in .dsk or .rtd format\n"));
    wprintf(_T("    <HddImage>  is UKNC hard disk image file name\n"));
    wprintf(_T("    <Partn>     is hard disk image partition number, 0..23\n"));
    wprintf(_T("    <FileName>  is a file name to read from or save to\n"));
}

static void PrintTableHeader()
{
    wprintf(_T(" Filename  Blocks  Date      Start    Bytes\n"));
    wprintf(_T("---------- ------  --------- ----- --------\n"));
}
static void PrintTableFooter()
{
    wprintf(_T("---------- ------  --------- ----- --------\n"));
}


//////////////////////////////////////////////////////////////////////

// Разбор Home Block и чтение каталога диска.
void DecodeImageCatalog()
{
    memset(&g_volumeinfo, 0, sizeof(g_volumeinfo));

    // Разбор Home Block
    BYTE* pHomeSector = (BYTE*) g_diskimage.GetBlock(1);
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
    BYTE* pBlock1 = (BYTE*) g_diskimage.GetBlock(nFirstCatalogBlock);
    memcpy(g_segmentBuffer, pBlock1, 512);
    BYTE* pBlock2 = (BYTE*) g_diskimage.GetBlock(nFirstCatalogBlock + 1);
    memcpy(g_segmentBuffer + 256, pBlock2, 512);
    WORD* pCatalogSector = g_segmentBuffer;
    g_volumeinfo.catalogsegmentcount = pCatalogSector[0];
    g_volumeinfo.lastopenedsegment = pCatalogSector[2];
    WORD nExtraBytesLength = pCatalogSector[3];
    WORD nExtraWordsLength = (nExtraBytesLength + 1) / 2;
    g_volumeinfo.catalogextrawords = nExtraWordsLength;
    WORD nEntryLength = 7 + nExtraWordsLength;  // Total catalog entry length, in words
    g_volumeinfo.catalogentrylength = nEntryLength;
    WORD nEntriesPerSegment = (512 - 5) / nEntryLength;
    g_volumeinfo.catalogentriespersegment = nEntriesPerSegment;

    // Получаем память под список сегментов
    g_volumeinfo.catalogsegments = (CVolumeCatalogSegment*) ::malloc(
        sizeof(CVolumeCatalogSegment) * g_volumeinfo.catalogsegmentcount);
    memset(g_volumeinfo.catalogsegments, 0,
        sizeof(CVolumeCatalogSegment) * g_volumeinfo.catalogsegmentcount);

    //TODO: Для заголовка самого первого сегмента каталога существует правило:
    //      если удвоить содержимое слова 1 и к результату прибавить начальный блок каталога (обычно 6),
    //      то получиться содержимое слова 5. Таким образом RT-11 отличает свой каталог от чужого.

    WORD nCatalogEntriesCount = 0;
    WORD nCatalogSegmentNumber = 1;
    CVolumeCatalogSegment* pSegment = g_volumeinfo.catalogsegments;

    WORD nCatalogBlock = nFirstCatalogBlock;
    for (;;)
    {
        pSegment->segmentblock = nCatalogBlock;

        WORD nStartBlock = pCatalogSector[4];  // Номер блока, с которого начинаются файлы этого сегмента
        pSegment->start = pCatalogSector[4];
        //wprintf(_T("Segment %d start block: %d\n"), nCatalogSegmentNumber, nStartBlock);
        pSegment->nextsegment = pCatalogSector[1];
        //wprintf(_T("Next segment:           %d\n"), pSegment->nextsegment);

        // Выделяем память под записи сегмента
        pSegment->catalogentries = (CVolumeCatalogEntry*) ::malloc(
            sizeof(CVolumeCatalogEntry) * nEntriesPerSegment);
        memset(pSegment->catalogentries, 0,
            sizeof(CVolumeCatalogEntry) * nEntriesPerSegment);

        CVolumeCatalogEntry* pEntry = pSegment->catalogentries;
        WORD* pCatalog = pCatalogSector + 5;  // Начало описаний файлов
        WORD nFileStartBlock = nStartBlock;
        WORD entriesused = 0;
        for (;;)  // Цикл по записям данного сегмента каталога
        {
            nCatalogEntriesCount++;

            pEntry->Unpack(pCatalog, nFileStartBlock);

            if (pEntry->status == RT11_STATUS_ENDMARK)
                break;

            nFileStartBlock += pEntry->length;
            pEntry++;
            pCatalog += nEntryLength;
            if (pCatalog - pCatalogSector > 256 * 2 - nEntryLength)  // Сегмент закончился
                break;
        }
        pSegment->entriesused = entriesused;

        if (pSegment->nextsegment == 0) break;  // Конец цепочки сегментов

        // Переходим к следующему сегменту каталога
        nCatalogBlock = nFirstCatalogBlock + (pSegment->nextsegment - 1) * 2;
        pBlock1 = (BYTE*) g_diskimage.GetBlock(nCatalogBlock);
        memcpy(g_segmentBuffer, pBlock1, 512);
        pBlock2 = (BYTE*) g_diskimage.GetBlock(nCatalogBlock + 1);
        memcpy(g_segmentBuffer + 256, pBlock2, 512);
        pCatalogSector = g_segmentBuffer;
        nCatalogSegmentNumber = pSegment->nextsegment;
        pSegment++;
    }

    g_volumeinfo.catalogentriescount = nCatalogEntriesCount;
}

void FreeImageCatalog()
{
    ::free(g_volumeinfo.catalogsegments);
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
    PrintTableHeader();

    WORD nFilesCount = 0;
    WORD nBlocksCount = 0;
    WORD nFreeBlocksCount = 0;
    for (int segmno = 0; segmno < g_volumeinfo.catalogsegmentcount; segmno++)
    {
        CVolumeCatalogSegment* pSegment = g_volumeinfo.catalogsegments + segmno;
        if (pSegment->catalogentries == NULL) continue;
        
        for (int entryno = 0; entryno < g_volumeinfo.catalogentriespersegment; entryno++)
        {
            CVolumeCatalogEntry* pEntry = pSegment->catalogentries + entryno;

            if (pEntry->status == RT11_STATUS_ENDMARK) break;
            if (pEntry->status == 0) continue;
            pEntry->Print();
            if (pEntry->status == RT11_STATUS_EMPTY)
                nFreeBlocksCount += pEntry->length;
            else
            {
                nFilesCount++;
                nBlocksCount += pEntry->length;
            }
        }
    }

    PrintTableFooter();
    wprintf(_T(" %d files, %d blocks\n"), nFilesCount, nBlocksCount);
    wprintf(_T(" %d free blocks\n\n"), nFreeBlocksCount);
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
    CVolumeCatalogEntry* pFileEntry = NULL;
    for (int segmno = 0; segmno < g_volumeinfo.catalogsegmentcount; segmno++)
    {
        CVolumeCatalogSegment* pSegment = g_volumeinfo.catalogsegments + segmno;
        if (pSegment->catalogentries == NULL) continue;
        
        for (int entryno = 0; entryno < g_volumeinfo.catalogentriespersegment; entryno++)
        {
            CVolumeCatalogEntry* pEntry = pSegment->catalogentries + entryno;

            if (pEntry->status == RT11_STATUS_ENDMARK) break;
            if (pEntry->status == 0) continue;
            if (pEntry->status == RT11_STATUS_EMPTY) continue;

            if (_wcsnicmp(filename, pEntry->name, 6) == 0 &&
                _wcsnicmp(fileext, pEntry->ext, 3) == 0)
            {
                pFileEntry = pEntry;
                break;
            }
        }
    }
    if (pFileEntry == NULL)
    {
        wprintf(_T("Filename not found: %s\n"), g_sFileName);
        return;
    }
    wprintf(_T("Extracting file:\n\n"));
    PrintTableHeader();
    pFileEntry->Print();
    PrintTableFooter();

    WORD filestart = pFileEntry->start;
    WORD filelength = pFileEntry->length;

    FILE* foutput = NULL;
    errno_t err = _wfopen_s(&foutput, g_sFileName, _T("wb"));
    if (err != 0)
    {
        wprintf(_T("Failed to open output file %s: error %d\n"), g_sFileName, err);
        return;
    }

    for (WORD blockpos = 0; blockpos < filelength; blockpos++)
    {
        BYTE* pData = (BYTE*) g_diskimage.GetBlock(filestart + blockpos);
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
bool DoAddFile()
{
    // Parse g_sFileName
    LPCTSTR sFilenameExt = wcsrchr(g_sFileName, _T('.'));
    if (sFilenameExt == NULL)
    {
        wprintf(_T("Wrong filename format: %s\n"), g_sFileName);
        return false;
    }
    size_t nFilenameLength = sFilenameExt - g_sFileName;
    if (nFilenameLength == 0 || nFilenameLength > 6)
    {
        wprintf(_T("Wrong filename format: %s\n"), g_sFileName);
        return false;
    }
    size_t nFileextLength = wcslen(g_sFileName) - nFilenameLength - 1;
    if (nFileextLength == 0 || nFileextLength > 3)
    {
        wprintf(_T("Wrong filename format: %s\n"), g_sFileName);
        return false;
    }
    TCHAR filename[7];
    for (int i = 0; i < 6; i++) filename[i] = _T(' ');
    for (WORD i = 0; i < nFilenameLength; i++) filename[i] = g_sFileName[i];
    filename[6] = 0;
    _wcsupr_s(filename, 7);
    TCHAR fileext[4];
    for (int i = 0; i < 3; i++) fileext[i] = _T(' ');
    for (WORD i = 0; i < nFileextLength; i++) fileext[i] = sFilenameExt[i + 1];
    fileext[3] = 0;
    _wcsupr_s(fileext, 4);

    // Открываем помещаемый файл на чтение
    FILE* fpFile = ::_wfopen(g_sFileName, _T("rb"));
    if (fpFile == NULL)
    {
        wprintf(_T("Failed to open the file."));
        return false;
    }

    // Определяем длину файла, с учетом округления до полного блока
    ::fseek(fpFile, 0, SEEK_END);
    LONG lFileLength = ::ftell(fpFile);  // Точная длина файла
    WORD nFileSizeBlocks =  // Требуемая ширина свободного места в блоках
        (WORD) (lFileLength + RT11_BLOCK_SIZE - 1) / RT11_BLOCK_SIZE;
    DWORD dwFileSize =  // Длина файла с учетом округления до полного блока
        ((DWORD) nFileSizeBlocks) * RT11_BLOCK_SIZE;
    //TODO: Проверка на файл нулевой длины
    //TODO: Проверка, не слишком ли длинный файл для этого тома

    // Выделяем память и считываем данные файла
    PVOID pFileData = ::malloc(dwFileSize);
    memset(pFileData, 0, dwFileSize);
    ::fseek(fpFile, 0, SEEK_SET);
    size_t lBytesRead = ::fread(pFileData, 1, lFileLength, fpFile);
    if (lBytesRead != lFileLength)
    {
        wprintf(_T("Failed to read the file.\n"));
        _exit(-1);
    }
    ::fclose(fpFile);

    wprintf(_T("File size is %ld bytes or %d blocks\n"), lFileLength, nFileSizeBlocks);

    // Перебираются все записи каталога, пока не будет найдена пустая запись длины >= dwFileLength
    //TODO: Выделить в отдельную функцию и искать наиболее подходящую запись, с минимальной разницей по длине
    CVolumeCatalogEntry* pFileEntry = NULL;
    CVolumeCatalogSegment* pFileSegment = NULL;
    for (int segmno = 0; segmno < g_volumeinfo.catalogsegmentcount; segmno++)
    {
        CVolumeCatalogSegment* pSegment = g_volumeinfo.catalogsegments + segmno;
        if (pSegment->catalogentries == NULL) continue;
        
        for (int entryno = 0; entryno < g_volumeinfo.catalogentriespersegment; entryno++)
        {
            CVolumeCatalogEntry* pEntry = pSegment->catalogentries + entryno;

            if (pEntry->status == RT11_STATUS_ENDMARK) break;
            if (pEntry->status == 0) continue;

            if (pEntry->status == RT11_STATUS_EMPTY && pEntry->length >= nFileSizeBlocks)
            {
                pFileEntry = pEntry;
                pFileSegment = pSegment;
                break;
            }
        }
    }
    if (pFileEntry == NULL)
    {
        wprintf(_T("Empty catalog entry with %d or more blocks not found\n"), nFileSizeBlocks);
        free(pFileData);
        return false;
    }
    wprintf(_T("Found empty catalog entry with %d blocks:\n\n"), pFileEntry->length);
    PrintTableHeader();
    pFileEntry->Print();
    PrintTableFooter();

    // Определяем, нужна ли новая запись каталога
    BOOL okNeedNewCatalogEntry = (pFileEntry->length != nFileSizeBlocks);
    CVolumeCatalogEntry* pEmptyEntry = NULL;
    if (okNeedNewCatalogEntry)
    {
        // Проверяем, нужно ли для новой записи каталога открывать новый сегмент каталога
        if (pFileSegment->entriesused == g_volumeinfo.catalogentriespersegment)
        {
            wprintf(_T("New catalog segment needed - not implemented now, sorry.\n"));
            free(pFileData);
            return false;
        }

        // Сдвигаем записи сегмента начиная с пустой на одну вправо - освобождаем место под новую запись
        int fileentryindex = (int) (pFileEntry - pFileSegment->catalogentries);
        int totalentries = g_volumeinfo.catalogentriespersegment;
        memmove(pFileEntry + 1, pFileEntry, (totalentries - fileentryindex - 1) * sizeof(CVolumeCatalogEntry));

        // Новая пустая запись каталога
        pEmptyEntry = pFileEntry + 1;
        // Заполнить данные новой записи каталога
        pEmptyEntry->status = RT11_STATUS_EMPTY;
        pEmptyEntry->start = pFileEntry->start + nFileSizeBlocks;
        pEmptyEntry->length = pFileEntry->length - nFileSizeBlocks;
        pEmptyEntry->datepac = pFileEntry->datepac;
    }

    // Изменяем существующую запись каталога
    pFileEntry->length = nFileSizeBlocks;
    wcscpy_s(pFileEntry->name, 7, filename);
    wcscpy_s(pFileEntry->ext, 4, fileext);
    pFileEntry->datepac = 0;
    pFileEntry->status = RT11_STATUS_PERM;

    wprintf(_T("\nCatalog entries to update:\n\n"));
    PrintTableHeader();
    pFileEntry->Print();
    if (pEmptyEntry != NULL) pEmptyEntry->Print();
    PrintTableFooter();

    // Сохраняем новый файл поблочно
    wprintf(_T("\nWriting file data...\n"));
    WORD nFileStartBlock = pFileEntry->start;  // Начиная с какого блока размещается новый файл
    WORD nBlock = nFileStartBlock;
    for (int block = 0; block < nFileSizeBlocks; block++)
    {
        BYTE* pFileBlockData = ((BYTE*) pFileData) + block * RT11_BLOCK_SIZE;
        BYTE* pData = (BYTE*) g_diskimage.GetBlock(nBlock);
        memcpy(pData, pFileBlockData, RT11_BLOCK_SIZE);
        // Сообщаем что блок был изменен
        g_diskimage.MarkBlockChanged(nBlock);
        
        nBlock++;
    }
    free(pFileData);

    // Сохраняем сегмент каталога на диск
    wprintf(_T("Updating catalog segment...\n"));
    UpdateCatalogSegment(pFileSegment);

    g_diskimage.FlushChanges();

    wprintf(_T("\nDone.\n"));

    return true;
}

void UpdateCatalogSegment(CVolumeCatalogSegment* pSegment)
{
    BYTE* pBlock1 = (BYTE*) g_diskimage.GetBlock(pSegment->segmentblock);
    memcpy(g_segmentBuffer, pBlock1, 512);
    BYTE* pBlock2 = (BYTE*) g_diskimage.GetBlock(pSegment->segmentblock + 1);
    memcpy(g_segmentBuffer + 256, pBlock2, 512);
    WORD* pData = g_segmentBuffer;

    pData += 5;  // Пропускаем заголовок сегмента
    for (int entryno = 0; entryno < g_volumeinfo.catalogentriespersegment; entryno++)
    {
        CVolumeCatalogEntry* pEntry = pSegment->catalogentries + entryno;

        pEntry->Pack(pData);

        pData += g_volumeinfo.catalogentrylength;
    }

    memcpy(pBlock1, g_segmentBuffer, 512);
    g_diskimage.MarkBlockChanged(pSegment->segmentblock);
    memcpy(pBlock2, g_segmentBuffer + 256, 512);
    g_diskimage.MarkBlockChanged(pSegment->segmentblock + 1);
}

bool DoHardList()
{
    g_hardimage.PrintImageInfo();
    wprintf(_T("\n"));
    g_hardimage.PrintPartitionTable();
    wprintf(_T("\n"));

    return true;
}

void DoHardExtractPartition()
{
    g_hardimage.SavePartitionToFile(g_nPartition, g_sFileName);
}


//////////////////////////////////////////////////////////////////////
