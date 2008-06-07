// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "diskimage.h"
#include "rad50.h"


//////////////////////////////////////////////////////////////////////
// ��������� ������, �������������� ���������� � ���� RT-11

struct CVolumeCatalogEntry;
struct CVolumeCatalogSegment;

// ��������� ��� �������� ���������� � ����
struct CVolumeInformation
{
    char volumeid[13];
    char ownername[13];
    char systemid[13];
    WORD firstcatalogblock;
    WORD systemversion;
    WORD catalogextrawords;
    WORD catalogentrylength;
    WORD catalogentriespersegment;
    WORD catalogsegmentcount;
    WORD lastopenedsegment;
    // ������ ���������
    CVolumeCatalogSegment* catalogsegments;
    WORD catalogentriescount;  // ���������� �������� ������� ��������, ������� ����������� ENDMARK
};

// ��������� ������ ��� �������� ��������
struct CVolumeCatalogSegment
{
public:
    WORD segmentblock;  // ���� �� �����, � ������� ���������� ���� ������� ��������
    WORD entriesused;   // ���������� ������������� ������� ��������
public:
    WORD nextsegment;   // ����� ���������� ��������
    WORD start;         // ����� �����, � �������� ���������� ����� ����� ��������
    // ������ ������� ��������, �������� � ����������� ��������� ���-�� ������� ��� ����� ��������
    CVolumeCatalogEntry* catalogentries;
};

// ��������� ��� �������� ����������� ������ ��������
struct CVolumeCatalogEntry
{
public:  // ����������� ���� ������
    WORD status;    // See RT11_STATUS_xxx constants
    WORD datepac;   // ����������� ���� ����
    WORD start;     // File start block number
    WORD length;    // File length in 512-byte blocks
public:  // ������������� ���� ������
    TCHAR name[7];  // File name - 6 characters
    TCHAR ext[4];   // File extension - 3 characters

public:
    CVolumeCatalogEntry();
    void Unpack(WORD const * pSrc, WORD filestartblock);  // ���������� ������ �� ��������
    void Pack(WORD* pDest);   // �������� ������ � �������
    void Print();  // ������ ������ �������� �� �������
};

CVolumeCatalogEntry::CVolumeCatalogEntry()
{
    status = 0;
    memset(name, 0, sizeof(name));
    memset(ext, 0, sizeof(ext));
    start = length = 0;
}

void CVolumeCatalogEntry::Unpack(WORD const * pCatalog, WORD filestartblock)
{
    start = filestartblock;
    status = pCatalog[0];
    WORD namerad50[3];
    namerad50[0] = pCatalog[1];
    namerad50[1] = pCatalog[2];
    namerad50[2] = pCatalog[3];
    length  = pCatalog[4];
    datepac = pCatalog[6];

    if (status != RT11_STATUS_EMPTY && status != RT11_STATUS_ENDMARK)
    {
        r50asc(6, namerad50, name);
        name[6] = 0;
        r50asc(3, namerad50 + 2, ext);
        ext[3] = 0;
    }
}

void CVolumeCatalogEntry::Pack(WORD* pCatalog)
{
    pCatalog[0] = status;
    if (status == RT11_STATUS_EMPTY || status == RT11_STATUS_ENDMARK)
    {
        memset(pCatalog + 1, 0, sizeof(WORD) * 3);
    }
    else
    {
        WORD namerad50[3];
        irad50(6, name, namerad50);
        irad50(3, ext,  namerad50 + 2);
        memcpy(pCatalog + 1, namerad50, sizeof(namerad50));
    }
    pCatalog[4] = length;
    pCatalog[5] = 0;  // Used only for tentative files
    pCatalog[6] = datepac;
}

void CVolumeCatalogEntry::Print()
{
    if (status == RT11_STATUS_EMPTY)
        wprintf(_T("< UNUSED >  %5d            %5d %8d\n"),
                length, start, length * RT11_BLOCK_SIZE);
    else
    {
        TCHAR datestr[10];
        rtDateStr(datepac, datestr);
        wprintf(_T("%s.%s  %5d  %s %5d %8d\n"),
                name, ext, length, datestr, start, length * RT11_BLOCK_SIZE);
    }
}


//////////////////////////////////////////////////////////////////////
// ��������������� ���������� ������� ���������

void PrintWelcome();
BOOL ParseCommandLine(int argc, _TCHAR* argv[]);
void PrintUsage();
void PrintTableHeader();
void PrintTableFooter();
void DecodeImageCatalog();
void FreeImageCatalog();
void PrepareTrack(int nSide, int nTrack);
void DoPrintCatalogDirectory();
void DoExtractFile();
BOOL DoAddFile();
void UpdateCatalogSegment(CVolumeCatalogSegment* pSegment);


//////////////////////////////////////////////////////////////////////
// ���������� ����������

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

    // ����������� � ����� ������
    if (!g_diskimage.Attach(g_sImageFileName))
        return 255;

    // ������ Home Block � ������ �������� �����
    DecodeImageCatalog();

    // Main task
    if (g_sCommand[0] == _T('l'))
        DoPrintCatalogDirectory();
    else if (g_sCommand[0] == _T('e'))
        DoExtractFile();
    else if (g_sCommand[0] == _T('a'))
        DoAddFile();

    // ���������� ������ � ������
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

// ������ Home Block � ������ �������� �����.
void DecodeImageCatalog()
{
    memset(&g_volumeinfo, 0, sizeof(g_volumeinfo));

    // ������ Home Block
    BYTE* pHomeSector = g_diskimage.GetBlock(1);
    WORD nFirstCatalogBlock = pHomeSector[0724];  // ��� ������ ���� ���� ����� 6
    if (nFirstCatalogBlock == 0) nFirstCatalogBlock = 6;
    g_volumeinfo.firstcatalogblock = nFirstCatalogBlock;
    g_volumeinfo.systemversion = pHomeSector[0726];
    LPCSTR sVolumeId = (LPCSTR) pHomeSector + 0730;
    strncpy_s(g_volumeinfo.volumeid, 13, sVolumeId, 12);
    LPCSTR sOwnerName = (LPCSTR) pHomeSector + 0744;
    strncpy_s(g_volumeinfo.ownername, 13, sOwnerName, 12);
    LPCSTR sSystemId = (LPCSTR) pHomeSector + 0760;
    strncpy_s(g_volumeinfo.systemid, 13, sSystemId, 12);

    // ������ ������� ����� ��������
    WORD* pCatalogSector = (WORD*) g_diskimage.GetBlock(nFirstCatalogBlock);
    g_volumeinfo.catalogsegmentcount = pCatalogSector[0];
    g_volumeinfo.lastopenedsegment = pCatalogSector[2];
    WORD nExtraBytesLength = pCatalogSector[3];
    WORD nExtraWordsLength = (nExtraBytesLength + 1) / 2;
    g_volumeinfo.catalogextrawords = nExtraWordsLength;
    WORD nEntryLength = 7 + nExtraWordsLength;  // Total catalog entry length, in words
    g_volumeinfo.catalogentrylength = nEntryLength;
    WORD nEntriesPerSegment = (512 - 5) / nEntryLength;
    g_volumeinfo.catalogentriespersegment = nEntriesPerSegment;

    // �������� ������ ��� ������ ���������
    g_volumeinfo.catalogsegments = (CVolumeCatalogSegment*) malloc(
        sizeof(CVolumeCatalogSegment) * g_volumeinfo.catalogsegmentcount);
    memset(g_volumeinfo.catalogsegments, 0,
        sizeof(CVolumeCatalogSegment) * g_volumeinfo.catalogsegmentcount);

    //TODO: ��� ��������� ������ ������� �������� �������� ���������� �������:
    //      ���� ������� ���������� ����� 1 � � ���������� ��������� ��������� ���� �������� (������ 6),
    //      �� ���������� ���������� ����� 5. ����� ������� RT-11 �������� ���� ������� �� ������.

    WORD nCatalogEntriesCount = 0;
    WORD nCatalogSegmentNumber = 1;
    CVolumeCatalogSegment* pSegment = g_volumeinfo.catalogsegments;

    WORD nCatalogBlock = nFirstCatalogBlock;
    for (;;)
    {
        pSegment->segmentblock = nCatalogBlock;

        WORD nStartBlock = pCatalogSector[4];  // ����� �����, � �������� ���������� ����� ����� ��������
        pSegment->start = pCatalogSector[4];
        //wprintf(_T("Segment %d start block: %d\n"), nCatalogSegmentNumber, nStartBlock);
        pSegment->nextsegment = pCatalogSector[1];
        //wprintf(_T("Next segment:           %d\n"), pSegment->nextsegment);

        // �������� ������ ��� ������ ��������
        pSegment->catalogentries = (CVolumeCatalogEntry*) malloc(
            sizeof(CVolumeCatalogEntry) * nEntriesPerSegment);
        memset(pSegment->catalogentries, 0,
            sizeof(CVolumeCatalogEntry) * nEntriesPerSegment);

        CVolumeCatalogEntry* pEntry = pSegment->catalogentries;
        WORD* pCatalog = pCatalogSector + 5;  // ������ �������� ������
        WORD nFileStartBlock = nStartBlock;
        WORD entriesused = 0;
        for (;;)  // ���� �� ������� ������� �������� ��������
        {
            nCatalogEntriesCount++;

            pEntry->Unpack(pCatalog, nFileStartBlock);

            if (pEntry->status == RT11_STATUS_ENDMARK)
                break;

            nFileStartBlock += pEntry->length;
            pEntry++;
            pCatalog += nEntryLength;
            if (pCatalog - pCatalogSector > 256 * 2 - nEntryLength)  // ������� ����������
                break;
        }
        pSegment->entriesused = entriesused;

        if (pSegment->nextsegment == 0) break;  // ����� ������� ���������

        // ��������� � ���������� �������� ��������
        nCatalogBlock = nFirstCatalogBlock + (pSegment->nextsegment - 1) * 2;
        pCatalogSector = (WORD*) g_diskimage.GetBlock(nCatalogBlock);
        nCatalogSegmentNumber = pSegment->nextsegment;
        pSegment++;
    }

    g_volumeinfo.catalogentriescount = nCatalogEntriesCount;
}

void FreeImageCatalog()
{
    free(g_volumeinfo.catalogsegments);
}

// ������ ����� �������� �����
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

// ���������� ����� �� ������ � ��������� ����.
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
        BYTE* pData = g_diskimage.GetBlock(filestart + blockpos);
        size_t nBytesWritten = fwrite(pData, sizeof(BYTE), RT11_BLOCK_SIZE, foutput);
        //TODO: Check if nBytesWritten < RT11_BLOCK_SIZE
    }

    fclose(foutput);

    wprintf(_T("\nDone.\n"));
}

// ��������� ����� � �����.
// ��������:
//   ���������� ���� ����������� � ������
//   ������������ ��� ������ ��������, ���� �� ����� ������� ������ ������ ������� ��� ������ �����
//   ���� �����, � ����� �������� ��������� ����� ������ ������
//   � ���� ������ ������������� ���������� ������ ��������
//   ���� �����, � ���� ������ ������������� ����� ������ ������ ��������
//   � ���� ������ ������������� ����� ������ �����
//NOTE: ���� �� ������������ �������� �������� ������ ����� �������� - ������� �� ������
//NOTE: ���� �� ��������� ��� ���� � ����� ������ ��� ����, � �� ������ ������
BOOL DoAddFile()
{
    // ��������� ���������� ���� �� ������
    HANDLE hFile = ::CreateFile(g_sFileName,
            GENERIC_READ, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        wprintf(_T("Failed to open file."));
        return FALSE;
    }

    // ���������� ����� �����, � ������ ���������� �� ������� �����
    DWORD dwFileLength = ::GetFileSize(hFile, NULL);  // ������ ����� �����
    WORD nFileSizeBlocks =  // ��������� ������ ���������� ����� � ������
        (WORD) (dwFileLength + RT11_BLOCK_SIZE - 1) / RT11_BLOCK_SIZE;
    DWORD dwFileSize =  // ����� ����� � ������ ���������� �� ������� �����
        ((DWORD) nFileSizeBlocks) * RT11_BLOCK_SIZE;
    //TODO: �������� �� ���� ������� �����
    //TODO: ��������, �� ������� �� ������� ���� ��� ����� ����

    // �������� ������ � ��������� ������ �����
    PVOID pFileData = malloc(dwFileSize);
    memset(pFileData, 0, dwFileSize);
    DWORD dwBytesRead;
    ::ReadFile(hFile, pFileData, dwFileLength, &dwBytesRead, NULL);
    //TODO: �������� �� ������ ������
    ::CloseHandle(hFile);

    wprintf(_T("File size is %ld bytes or %d blocks\n"), dwFileLength, nFileSizeBlocks);

    // ������������ ��� ������ ��������, ���� �� ����� ������� ������ ������ ����� >= dwFileLength
    //TODO: �������� � ��������� ������� � ������ �������� ���������� ������, � ����������� �������� �� �����
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
        return FALSE;
    }
    wprintf(_T("Found empty catalog entry with %d blocks:\n\n"), pFileEntry->length);
    PrintTableHeader();
    pFileEntry->Print();
    PrintTableFooter();

    // ����������, ����� �� ����� ������ ��������
    BOOL okNeedNewCatalogEntry = (pFileEntry->length != nFileSizeBlocks);
    CVolumeCatalogEntry* pEmptyEntry = NULL;
    if (okNeedNewCatalogEntry)
    {
        // ���������, ����� �� ��� ����� ������ �������� ��������� ����� ������� ��������
        if (pFileSegment->entriesused == g_volumeinfo.catalogentriespersegment)
        {
            wprintf(_T("New catalog segment needed - not implemented now, sorry.\n"));
            free(pFileData);
            return FALSE;
        }

        // �������� ������ �������� ������� � ������ �� ���� ������ - ����������� ����� ��� ����� ������
        int fileentryindex = (int) (pFileEntry - pFileSegment->catalogentries);
        int totalentries = g_volumeinfo.catalogentriespersegment;
        memmove(pFileEntry + 1, pFileEntry, (totalentries - fileentryindex - 1) * sizeof(CVolumeCatalogEntry));

        // ����� ������ ������ ��������
        pEmptyEntry = pFileEntry + 1;
        // ��������� ������ ����� ������ ��������
        pEmptyEntry->status = RT11_STATUS_EMPTY;
        pEmptyEntry->start = pFileEntry->start + nFileSizeBlocks;
        pEmptyEntry->length = pFileEntry->length - nFileSizeBlocks;
        pEmptyEntry->datepac = pFileEntry->datepac;
    }

    // �������� ������������ ������ ��������
    pFileEntry->length = nFileSizeBlocks;
    wcscpy_s(pFileEntry->name, 7, _T("TEST  "));  //TODO
    wcscpy_s(pFileEntry->ext, 4, _T("EXT"));  //TODO
    pFileEntry->datepac = 0;
    pFileEntry->status = RT11_STATUS_PERM;

    wprintf(_T("\nUpdated catalog entries:\n\n"));
    PrintTableHeader();
    pFileEntry->Print();
    if (pEmptyEntry != NULL) pEmptyEntry->Print();
    PrintTableFooter();

    // ��������� ����� ���� ��������
    wprintf(_T("\nWriting file data...\n"));
    WORD nFileStartBlock = pFileEntry->start;  // ������� � ������ ����� ����������� ����� ����
    WORD nBlock = nFileStartBlock;
    for (int block = 0; block < nFileSizeBlocks; block++)
    {
        BYTE* pFileBlockData = ((BYTE*) pFileData) + block * RT11_BLOCK_SIZE;
        BYTE* pData = g_diskimage.GetBlock(nBlock);
        memcpy(pData, pFileBlockData, RT11_BLOCK_SIZE);
        // �������� ��� ���� ��� �������
        g_diskimage.MarkTrackChanged();
        
        nBlock++;
    }
    free(pFileData);

    // ��������� ������� �������� �� ����
    wprintf(_T("Updating catalog segment...\n"));
    UpdateCatalogSegment(pFileSegment);

    g_diskimage.FlushChanges();

    wprintf(_T("\nDone.\n"));

    return TRUE;
}

void UpdateCatalogSegment(CVolumeCatalogSegment* pSegment)
{
    WORD* pData = (WORD*) g_diskimage.GetBlock(pSegment->segmentblock);

    pData += 5;  // ���������� ��������� ��������
    for (int entryno = 0; entryno < g_volumeinfo.catalogentriespersegment; entryno++)
    {
        CVolumeCatalogEntry* pEntry = pSegment->catalogentries + entryno;

        pEntry->Pack(pData);

        pData += g_volumeinfo.catalogentrylength;
    }

    g_diskimage.MarkTrackChanged();
}


//////////////////////////////////////////////////////////////////////
