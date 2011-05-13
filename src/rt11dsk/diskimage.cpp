// diskimage.cpp : Disk image utilities

#include "stdafx.h"
#include "diskimage.h"

//////////////////////////////////////////////////////////////////////

struct CCachedBlock
{
    int     nBlock;
    void*   pData;
    bool    bChanged;
    DWORD   dwLastUsage;  // GetTickCount() for last usage
};


//////////////////////////////////////////////////////////////////////

CDiskImage::CDiskImage()
{
    m_okReadOnly = m_okNetRT11Image = FALSE;
    m_fpFile = NULL;
    m_nTotalBlocks = m_nCacheBlocks = 0;
    m_pCache = NULL;
}

CDiskImage::~CDiskImage()
{
    Detach();
}

bool CDiskImage::Attach(LPCTSTR sImageFileName)
{
    // ����������, ��� .dsk-����� ��� .rtd-����� - �� ���������� �����
    LPCTSTR sImageFilenameExt = wcsrchr(sImageFileName, _T('.'));
    if (sImageFilenameExt != NULL && _wcsicmp(sImageFilenameExt, _T(".rtd")) == 0)
        m_okNetRT11Image = true;

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

    // Calculate m_TotalBlocks
    ::fseek(m_fpFile, 0, SEEK_END);
    long lFileSize = ::ftell(m_fpFile);
    m_nTotalBlocks = lFileSize / RT11_BLOCK_SIZE;

    // Allocate memory for the cache
    m_nCacheBlocks = 1024;  //NOTE: For up to 1024 blocks, for 512K of data
    if (m_nCacheBlocks > m_nTotalBlocks) m_nCacheBlocks = m_nTotalBlocks;
    m_pCache = (CCachedBlock*) ::malloc(m_nCacheBlocks * sizeof(CCachedBlock));
    ::memset(m_pCache, 0, m_nCacheBlocks * sizeof(CCachedBlock));

    // Initial read: fill half of the cache
    int nBlocks = 10;
    for (int i = 1; i <= nBlocks; i++)
    {
        GetBlock(i);
    }

    return true;
}

void CDiskImage::Detach()
{
    if (m_fpFile != NULL)
    {
        FlushChanges();

        ::fclose(m_fpFile);
        m_fpFile = NULL;

        // Free cached blocks data
        for (int i = 0; i < m_nCacheBlocks; i++)
        {
            if (m_pCache[i].pData != NULL)
                ::free(m_pCache[i].pData);
        }

        ::free(m_pCache);
    }
}

long CDiskImage::GetBlockOffset(int nBlock) const
{
    long foffset = ((long)nBlock) * RT11_BLOCK_SIZE;
    if (m_okNetRT11Image) foffset += NETRT11_IMAGE_HEADER_SIZE;
    return foffset;
}

void CDiskImage::FlushChanges()
{
    for (int i = 0; i < m_nCacheBlocks; i++)
    {
        if (!m_pCache[i].bChanged) continue;

        // ��������� �������� � ����� ������
        long foffset = GetBlockOffset(m_pCache[i].nBlock);
        ::fseek(m_fpFile, foffset, SEEK_SET);

        // ���������� ����
        long lBytesWritten = ::fwrite(m_pCache[i].pData, 1, RT11_BLOCK_SIZE, m_fpFile);
        if (lBytesWritten != RT11_BLOCK_SIZE)
        {
            wprintf(_T("Failed to write block number %d.\n"), m_pCache[i].nBlock);
            _exit(-1);
        }

        m_pCache[i].bChanged = false;
    }
}

// ������ ���� - 256 ����, 512 ����
// nBlock = 1..???
void* CDiskImage::GetBlock(int nBlock)
{
    // First lookup the cache
    for (int i = 0; i < m_nCacheBlocks; i++)
    {
        if (m_pCache[i].nBlock == nBlock)
        {
            m_pCache[i].dwLastUsage = ::GetTickCount();
            return m_pCache[i].pData;
        }
    }

    // Find a free cache slot
    int iEmpty = -1;
    for (int i = 0; i < m_nCacheBlocks; i++)
    {
        if (m_pCache[i].nBlock == 0)
        {
            iEmpty = i;
            break;
        }
    }

    // If a free slot not found then release a slot
    if (iEmpty == -1)
    {
        // Find a non-changed cached block with oldest usage time
        int iCand = -1;
        DWORD maxdiff = 0;
        for (int i = 0; i < m_nCacheBlocks; i++)
        {
            if (!m_pCache[i].bChanged)
            {
                DWORD diff = GetTickCount() - m_pCache[i].dwLastUsage;
                if (diff > maxdiff)
                {
                    maxdiff = diff;
                    iCand = i;
                }
            }
        }
        if (iCand != -1)  // Found
        {
            ::free(m_pCache[iEmpty].pData);
            m_pCache[iEmpty].pData = NULL;
            m_pCache[iEmpty].nBlock = 0;
            m_pCache[iEmpty].bChanged = false;
        }
    }

    if (iEmpty == -1)
    {
        wprintf(_T("Cache is full.\n"));
        _exit(-1);
    }

    m_pCache[iEmpty].nBlock = nBlock;
    m_pCache[iEmpty].bChanged = false;
    m_pCache[iEmpty].pData = ::malloc(RT11_BLOCK_SIZE);
    ::memset(m_pCache[iEmpty].pData, 0, RT11_BLOCK_SIZE);
    m_pCache[iEmpty].dwLastUsage = ::GetTickCount();

    // Load the block data
    long foffset = GetBlockOffset(nBlock);
    ::fseek(m_fpFile, foffset, SEEK_SET);
    long lBytesRead = ::fread(m_pCache[iEmpty].pData, 1, RT11_BLOCK_SIZE, m_fpFile);
    if (lBytesRead != RT11_BLOCK_SIZE)
    {
        wprintf(_T("Failed to read block number %d.\n"), nBlock);
        _exit(-1);
    }

    return m_pCache[iEmpty].pData;
}

void CDiskImage::MarkBlockChanged(int nBlock)
{
    for (int i = 0; i < m_nCacheBlocks; i++)
    {
        if (m_pCache[i].nBlock != nBlock) continue;

        m_pCache[i].bChanged = TRUE;
        m_pCache[i].dwLastUsage = ::GetTickCount();
        break;
    }
}


//////////////////////////////////////////////////////////////////////
