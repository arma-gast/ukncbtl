// hardimage.cpp : HDD image utilities

#include "stdafx.h"
#include "hardimage.h"

//////////////////////////////////////////////////////////////////////

struct CPartitionInfo
{
    long    offset;     // Offset from file start
    WORD    blocks;     // Size in blocks

public:
    void Print(int number);
};

//////////////////////////////////////////////////////////////////////

// Inverts 512 bytes in the buffer
static void InvertBuffer(void* buffer)
{
    DWORD* p = (DWORD*) buffer;
    for (int i = 0; i < 128; i++)
    {
        *p = ~(*p);
        p++;
    }
}

static DWORD GetHomeBlockChecksum(void* buffer)
{
    WORD* p = (WORD*) buffer;
    DWORD crc = 0;
    for (int i = 0; i < 255; i++)
    {
        crc += (DWORD) *p;
        p++;
    }
    crc += ((DWORD)*p) << 16;

    return crc;
}

//////////////////////////////////////////////////////////////////////

static BYTE g_hardbuffer[512];

CHardImage::CHardImage()
{
    m_okReadOnly = false;
    m_fpFile = NULL;
    m_drivertype = HDD_DRIVER_UNKNOWN;
    m_nSectorsPerTrack = m_nSidesPerTrack = m_nPartitions = 0;
    m_pPartitionInfos = NULL;
}

CHardImage::~CHardImage()
{
    Detach();
}

bool CHardImage::Attach(LPCTSTR sImageFileName)
{
    // Try to open as Normal first, then as ReadOnly
    m_okReadOnly = false;
    m_fpFile = ::_wfopen(sImageFileName, _T("r+b"));
    if (m_fpFile == NULL)
    {
        m_okReadOnly = true;
        m_fpFile = ::_wfopen(sImageFileName, _T("rb"));
        if (m_fpFile == NULL)
            return false;
    }

    // Get file size
    ::fseek(m_fpFile, 0, SEEK_END);
    m_lFileSize = ::ftell(m_fpFile);

    // Read first 512 bytes
    ::fseek(m_fpFile, 0, SEEK_SET);
    size_t lBytesRead = ::fread(g_hardbuffer, 1, 512, m_fpFile);
    if (lBytesRead != 512)
    {
        wprintf(_T("Failed to read first 512 bytes of the hard disk image file.\n"));
        _exit(-1);
    }

    // Check for inverted image
    BYTE test = 0xff;
    for (int i = 0x1f0; i <= 0x1fb; i++)
        test &= g_hardbuffer[i];
    m_okInverted = (test == 0xff);
    // Invert the buffer if needed
    if (m_okInverted)
        InvertBuffer(g_hardbuffer);

    // Calculate and verify checksum
    DWORD checksum = GetHomeBlockChecksum(g_hardbuffer);
    //wprintf(_T("Home block checksum is 0x%08lx.\n"), checksum);
    if (checksum != 0)
        wprintf(_T("Home block checksum is incorrect!\n"), checksum);

    m_nSectorsPerTrack = g_hardbuffer[0];
    m_nSidesPerTrack = g_hardbuffer[1];

    m_drivertype = HDD_DRIVER_UNKNOWN;
    WORD wdwaittime = ((WORD*)g_hardbuffer)[0122 / 2];
    WORD wdhidden = ((WORD*)g_hardbuffer)[0124 / 2];
    if (wdwaittime != 0 || wdhidden != 0)
        m_drivertype = HDD_DRIVER_WD;

    // Count partitions
    int count = 0;
    long totalblocks = 0;
    for (int i = 1; i < 24; i++)
    {
        WORD blocks = *((WORD*)g_hardbuffer + i);
        if (blocks == 0) break;
        if (blocks + totalblocks > (m_lFileSize / 512) - 1)
            break;
        count++;
        totalblocks += blocks;
    }

    m_nPartitions = count;
    if (m_nPartitions > 0)
    {
        m_pPartitionInfos = (CPartitionInfo*) ::malloc(sizeof(CPartitionInfo) * m_nPartitions);
        ::memset(m_pPartitionInfos, 0, sizeof(CPartitionInfo) * m_nPartitions);

        // Prepare m_pPartitionInfos
        long offset = 512;
        for (int i = 0; i < m_nPartitions; i++)
        {
            m_pPartitionInfos[i].offset = offset;
            WORD blocks = *((WORD*)g_hardbuffer + i + 1);
            m_pPartitionInfos[i].blocks = blocks;
            offset += blocks * 512;
        }
    }

    return true;
}

void CHardImage::Detach()
{
    if (m_fpFile != NULL)
    {
        ::fclose(m_fpFile);
        m_fpFile = NULL;
    }
}

void CHardImage::PrintImageInfo()
{
    wprintf(_T("Image file size: %ld bytes, %ld blocks\n"), m_lFileSize, m_lFileSize / 512);
    wprintf(_T("Disk geometry: %d sectors/track, %d heads\n"), m_nSectorsPerTrack, m_nSidesPerTrack);
}

void CHardImage::PrintPartitionTable()
{
    wprintf(_T("  #  Blocks  Bytes      Offset\n"));
    wprintf(_T("---  ------  ---------  ----------\n"));

    long blocks = 0;
    for (int i = 0; i < m_nPartitions; i++)
    {
        m_pPartitionInfos[i].Print(i);
        blocks += m_pPartitionInfos[i].blocks;
    }

    wprintf(_T("---  ------  ---------  ----------\n"));

    wprintf(_T("     %6d\n"), blocks);
}

void CHardImage::SavePartitionToFile(int partition, LPCTSTR filename)
{
    if (partition < 0 || partition >= m_nPartitions)
    {
        wprintf(_T("Wrong partition number specified.\n"));
        return;
    }

    // Open output file
    FILE* foutput = NULL;
    errno_t err = _wfopen_s(&foutput, filename, _T("wb"));
    if (err != 0)
    {
        wprintf(_T("Failed to open output file %s: error %d\n"), filename, err);
        return;
    }

    CPartitionInfo* pPartInfo = m_pPartitionInfos + partition;

    wprintf(_T("Extracting partition number %d to file %s\n"), partition, filename);
    wprintf(_T("Saving %d blocks, %ld bytes.\n"), pPartInfo->blocks, ((DWORD)pPartInfo->blocks) * RT11_BLOCK_SIZE);

    // Copy data
    ::fseek(m_fpFile, pPartInfo->offset, SEEK_SET);
    for (int i = 0; i < pPartInfo->blocks; i++)
    {
        size_t lBytesRead = ::fread(g_hardbuffer, sizeof(BYTE), RT11_BLOCK_SIZE, m_fpFile);
        if (lBytesRead != RT11_BLOCK_SIZE)
        {
            wprintf(_T("Failed to read disk image file.\n"));
            fclose(foutput);
            return;
        }

        if (m_okInverted)
            InvertBuffer(g_hardbuffer);

        size_t nBytesWritten = fwrite(g_hardbuffer, sizeof(BYTE), RT11_BLOCK_SIZE, foutput);
        if (nBytesWritten != RT11_BLOCK_SIZE)
        {
            wprintf(_T("Failed to write to output file.\n"));
            fclose(foutput);
            return;
        }
    }
    fclose(foutput);

    wprintf(_T("\nDone.\n"));
}


//////////////////////////////////////////////////////////////////////

void CPartitionInfo::Print(int number)
{
    long bytes = ((long)blocks) * 512;
    wprintf(_T("%3d  %6d %10ld  0x%08lx\n"), number, blocks, bytes, offset);
}


//////////////////////////////////////////////////////////////////////
