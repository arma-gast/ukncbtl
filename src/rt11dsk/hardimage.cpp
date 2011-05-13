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

static BYTE g_hardbuffer[512];

CHardImage::CHardImage()
{
    m_okReadOnly = false;
    m_fpFile = NULL;
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
            return true;
    }

    // Read first 512 bytes
    long lBytesRead = ::fread(g_hardbuffer, 1, 512, m_fpFile);
    if (lBytesRead != 512)
    {
        wprintf(_T("Failed to read first 512 bytes of the hard disk image file.\n"));
        _exit(-1);
    }

    //TODO: Calculate and verify checksum

    m_nSectorsPerTrack = g_hardbuffer[0];
    m_nSidesPerTrack = g_hardbuffer[1];

    // Count partitions
    int count = 0;
    for (int i = 1; i < 254; i++)
    {
        WORD blocks = *((WORD*)g_hardbuffer + i);
        if (blocks == 0) break;
        count++;
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

void CHardImage::PrintPartitionTable()
{
    wprintf(_T("  #  Blocks  Bytes     Start     \n"));
    wprintf(_T("---  ------  --------  ----------\n"));

    for (int i = 0; i < m_nPartitions; i++)
    {
        m_pPartitionInfos[i].Print(i + 1);
    }
}


//////////////////////////////////////////////////////////////////////

void CPartitionInfo::Print(int number)
{
    long bytes = ((long)blocks) * 512;
    wprintf(_T("%3d  %6d %9ld  0x%08lx\n"), number, blocks, bytes, offset);
}


//////////////////////////////////////////////////////////////////////
