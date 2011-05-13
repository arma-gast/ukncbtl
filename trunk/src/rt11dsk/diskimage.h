// diskimage.h : Disk image utilities headers

//////////////////////////////////////////////////////////////////////

/* Size of each RT-11 disk block, 512 or 0x200 bytes */
#define RT11_BLOCK_SIZE         512

#define NETRT11_IMAGE_HEADER_SIZE  256

struct CCachedBlock;

//////////////////////////////////////////////////////////////////////
// Образ диска в формате .dsk либо .rtd

class CDiskImage
{
protected:
    HANDLE          m_hFile;
    BOOL            m_okReadOnly;
    BOOL            m_okNetRT11Image;
    int             m_nTotalBlocks;
    int             m_nCacheBlocks;  // Cache size in blocks
    CCachedBlock*   m_pCache;

public:
    CDiskImage();
    ~CDiskImage();

public:
    BOOL Attach(LPCTSTR sFileName);
    void Detach();

public:
    int IsReadOnly() const { return m_okReadOnly; }
    int GetBlockCount() const { return m_nTotalBlocks; }
    BYTE* GetBlock(int nBlock);
    void MarkBlockChanged(int nBlock);
    void FlushChanges();

private:
    LONG GetBlockOffset(int nBlock) const;

};


//////////////////////////////////////////////////////////////////////
