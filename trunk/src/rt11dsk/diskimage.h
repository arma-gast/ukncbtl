// diskimage.h : Disk image utilities headers

struct CCachedBlock;
struct CVolumeInformation;
struct CVolumeCatalogSegment;

//////////////////////////////////////////////////////////////////////

/* Size of each RT-11 disk block, 512 or 0x200 bytes */
#define RT11_BLOCK_SIZE         512

#define NETRT11_IMAGE_HEADER_SIZE  256


//////////////////////////////////////////////////////////////////////
// Структура для хранения информации о томе
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
    // Массив сегментов
    CVolumeCatalogSegment* catalogsegments;
    WORD catalogentriescount;  // Количество валидных записей каталога, включая завершающую ENDMARK

public:
    CVolumeInformation();
    ~CVolumeInformation();
};


//////////////////////////////////////////////////////////////////////
// Образ диска в формате .dsk либо .rtd

class CDiskImage
{
protected:
    FILE*           m_fpFile;
    bool            m_okReadOnly;
    long            m_lStartOffset;  // First block start offset in the image file
    int             m_nTotalBlocks;  // Total blocks in the image
    int             m_nCacheBlocks;  // Cache size in blocks
    CCachedBlock*   m_pCache;
    CVolumeInformation m_volumeinfo;

public:
    CDiskImage();
    ~CDiskImage();

public:
    bool Attach(LPCTSTR sFileName);
    void Detach();

public:
    int IsReadOnly() const { return m_okReadOnly; }
    int GetBlockCount() const { return m_nTotalBlocks; }

public:
    void PrintCatalogDirectory();
    void PrintTableHeader();
    void PrintTableFooter();
    void* GetBlock(int nBlock);
    void MarkBlockChanged(int nBlock);
    void FlushChanges();
    void DecodeImageCatalog();
    void UpdateCatalogSegment(CVolumeCatalogSegment* pSegment);
    void SaveEntryToExternalFile(LPCTSTR sFileName);
    void AddFileToImage(LPCTSTR sFileName);

private:
    long GetBlockOffset(int nBlock) const;

};


//////////////////////////////////////////////////////////////////////
