// diskimage.cpp : Disk image utilities

#include "stdafx.h"
#include "diskimage.h"

//////////////////////////////////////////////////////////////////////


CDiskImage::CDiskImage()
{
    m_okNetRT11Image = FALSE;
    m_hFile = INVALID_HANDLE_VALUE;
    m_nCurrentSide = -1;
    m_nCurrentTrack = -1;
}

CDiskImage::~CDiskImage()
{
    Detach();
}

BOOL CDiskImage::Attach(LPCTSTR sImageFileName)
{
    // ����������, ��� .dsk-����� ��� .rtd-����� - �� ���������� �����
    m_okNetRT11Image = FALSE;
    LPCTSTR sImageFilenameExt = wcsrchr(sImageFileName, _T('.'));
    if (sImageFilenameExt != NULL && _wcsicmp(sImageFilenameExt, _T(".rtd")) == 0)
        m_okNetRT11Image = TRUE;

    m_hFile = ::CreateFile(sImageFileName,
            GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_hFile == INVALID_HANDLE_VALUE)
        return FALSE;

    return TRUE;
}

void CDiskImage::Detach()
{
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        FlushChanges();

        ::CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
}

// nSide = 0..1
// nTrack = 0..79
void CDiskImage::PrepareTrack(int nSide, int nTrack)
{
    if (m_nCurrentSide == nSide && m_nCurrentTrack == nTrack)
        return;  // ���������� ������ �� ���������

    memset(m_TrackData, 0, RT11_TRACK_SIZE);

    long foffset = long(nTrack * 2 + nSide) * RT11_TRACK_SIZE;  // File offset to read from
    if (m_okNetRT11Image) foffset += NETRT11_IMAGE_HEADER_SIZE;
    ::SetFilePointer(m_hFile, foffset, NULL, FILE_BEGIN);

    DWORD count;
    ::ReadFile(m_hFile, m_TrackData, RT11_TRACK_SIZE, &count, NULL);
    //TODO: �������� �� ������ ������

    m_nCurrentSide = nSide;
    m_nCurrentTrack = nTrack;
    m_okTrackChanged = FALSE;
}

void CDiskImage::FlushChanges()
{
    if (!m_okTrackChanged) return;

    // ��������� �������� � ����� ������ - ������ �������
    long foffset = long(m_nCurrentTrack * 2 + m_nCurrentSide) * RT11_TRACK_SIZE;
    if (m_okNetRT11Image) foffset += NETRT11_IMAGE_HEADER_SIZE;
    ::SetFilePointer(m_hFile, foffset, NULL, FILE_BEGIN);

    // ���������� �������
    ::SetFilePointer(m_hFile, foffset, NULL, FILE_BEGIN);
    DWORD dwBytesWritten;
    ::WriteFile(m_hFile, &m_TrackData, RT11_TRACK_SIZE, &dwBytesWritten, NULL);
    //TODO: �������� �� ������ ������

    m_okTrackChanged = FALSE;
}

// ��������� ��������� �� ������ ��������� ������� ������� �������
// nSector = 1..10
BYTE* CDiskImage::GetSector(int nSector)
{
    return m_TrackData + (nSector - 1) * RT11_BLOCK_SIZE;
}

// ������� � ��������� ����� ����� ������ �����. ���� �����, ����������� ������ ������ �������.
// ������ ���� - 256 ����, 512 ����
// ������ track = 10 ������
// nBlock = 1..???
BYTE* CDiskImage::GetBlock(int nBlock)
{
    int nTrackAndSide = nBlock / 10;
    int nSector = nBlock % 10 + 1;
    int nTrack = nTrackAndSide / 2;
    int nSide = nTrackAndSide % 2;

    PrepareTrack(nSide, nTrack);

    return GetSector(nSector);
}


//////////////////////////////////////////////////////////////////////
