// diskimage.h : Disk image utilities headers

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
// Образ диска в формате .dsk либо .rtd

class CDiskImage
{
protected:
    BOOL    m_okNetRT11Image;
    HANDLE  m_hFile;
    BYTE    m_TrackData[RT11_TRACK_SIZE];
    int     m_nCurrentSide;
    int     m_nCurrentTrack;
    BOOL    m_okTrackChanged;

public:
    CDiskImage();
    ~CDiskImage();

public:
    BOOL Attach(LPCTSTR sFileName);
    void Detach();

public:
    BYTE* GetSector(int nSector);
    BYTE* GetBlock(int nBlock);
    void MarkTrackChanged() { m_okTrackChanged = TRUE; }
    void FlushChanges();

protected:
    void PrepareTrack(int nSide, int nTrack);

};


//////////////////////////////////////////////////////////////////////
