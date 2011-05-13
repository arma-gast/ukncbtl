// diskimage.cpp : Disk image utilities

#include "stdafx.h"
#include "diskimage.h"

//////////////////////////////////////////////////////////////////////

struct CCachedBlock
{
    int     nBlock;
    BYTE*   pData;
    BOOL    bChanged;
    DWORD   dwLastUsage;  // GetTickCount() for last usage
};


//////////////////////////////////////////////////////////////////////

CDiskImage::CDiskImage()
{
    m_okReadOnly = m_okNetRT11Image = FALSE;
    m_hFile = INVALID_HANDLE_VALUE;
    m_nTotalBlocks = m_nCacheBlocks = 0;
    m_pCache = NULL;
}

CDiskImage::~CDiskImage()
{
    Detach();
}

BOOL CDiskImage::Attach(LPCTSTR sImageFileName)
{
    // Определяем, это .dsk-образ или .rtd-образ - по расширению файла
    LPCTSTR sImageFilenameExt = wcsrchr(sImageFileName, _T('.'));
    if (sImageFilenameExt != NULL && _wcsicmp(sImageFilenameExt, _T(".rtd")) == 0)
        m_okNetRT11Image = TRUE;

    // Try to open as Normal first, then as ReadOnly
    m_okReadOnly = FALSE;
    m_hFile = ::CreateFile(sImageFileName,
            GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        m_okReadOnly = TRUE;
        m_hFile = ::CreateFile(sImageFileName,
                GENERIC_READ, FILE_SHARE_READ, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
        if (m_hFile == INVALID_HANDLE_VALUE)
            return FALSE;
    }

    // Calculate m_TotalBlocks
    DWORD dwFileSize = ::GetFileSize(m_hFile, NULL);
    m_nTotalBlocks = dwFileSize / RT11_BLOCK_SIZE;

    // Allocate memory for the cache
    m_nCacheBlocks = 1024;  //NOTE: For up to 1024 blocks, for 512K of data
    if (m_nCacheBlocks > m_nTotalBlocks) m_nCacheBlocks = m_nTotalBlocks;
    m_pCache = (CCachedBlock*) ::LocalAlloc(LPTR, m_nCacheBlocks * sizeof(CCachedBlock));

    // Initial read: fill half of the cache
    int nBlocks = 10;
    for (int i = 1; i <= nBlocks; i++)
    {
        GetBlock(i);
    }

    return TRUE;
}

void CDiskImage::Detach()
{
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        FlushChanges();

        ::CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;

        // Free cached blocks data
        for (int i = 0; i < m_nCacheBlocks; i++)
        {
            if (m_pCache[i].pData != NULL)
                ::LocalFree(m_pCache[i].pData);
        }

        ::LocalFree(m_pCache);
    }
}

LONG CDiskImage::GetBlockOffset(int nBlock) const
{
    LONG foffset = ((LONG)nBlock) * RT11_BLOCK_SIZE;
    if (m_okNetRT11Image) foffset += NETRT11_IMAGE_HEADER_SIZE;
    return foffset;
}

void CDiskImage::FlushChanges()
{
    for (int i = 0; i < m_nCacheBlocks; i++)
    {
        if (!m_pCache[i].bChanged) continue;

        // Вычисляем смещение в файле образа
        long foffset = GetBlockOffset(m_pCache[i].nBlock);
        ::SetFilePointer(m_hFile, foffset, NULL, FILE_BEGIN);

        // Записываем блок
        ::SetFilePointer(m_hFile, foffset, NULL, FILE_BEGIN);
        DWORD dwBytesWritten;
        ::WriteFile(m_hFile, m_pCache[i].pData, RT11_BLOCK_SIZE, &dwBytesWritten, NULL);
        if (dwBytesWritten != RT11_BLOCK_SIZE)
        {
            wprintf(_T("Failed to write block number %d.\n"), m_pCache[i].nBlock);
            _exit(-1);
        }

        m_pCache[i].bChanged = FALSE;
    }
}

// Каждый блок - 256 слов, 512 байт
// nBlock = 1..???
BYTE* CDiskImage::GetBlock(int nBlock)
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
            ::LocalFree(m_pCache[iEmpty].pData);
            m_pCache[iEmpty].pData = NULL;
            m_pCache[iEmpty].nBlock = 0;
            m_pCache[iEmpty].bChanged = FALSE;
        }
    }

    if (iEmpty == -1)
    {
        wprintf(_T("Cache is full.\n"));
        _exit(-1);
    }

    m_pCache[iEmpty].nBlock = nBlock;
    m_pCache[iEmpty].bChanged = FALSE;
    m_pCache[iEmpty].pData = (BYTE*) ::LocalAlloc(LPTR, RT11_BLOCK_SIZE);
    m_pCache[iEmpty].dwLastUsage = ::GetTickCount();

    // Load the block data
    LONG foffset = GetBlockOffset(nBlock);
    ::SetFilePointer(m_hFile, foffset, NULL, FILE_BEGIN);
    DWORD dwBytesRead;
    ::ReadFile(m_hFile, m_pCache[iEmpty].pData, RT11_BLOCK_SIZE, &dwBytesRead, NULL);
    if (dwBytesRead != RT11_BLOCK_SIZE)
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
