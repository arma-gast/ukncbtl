// diskimage.h : Disk image utilities headers

//////////////////////////////////////////////////////////////////////

/* Size of each RT-11 disk block, 512 or 0x200 bytes */
#define RT11_BLOCK_SIZE         512

#define NETRT11_IMAGE_HEADER_SIZE  256

struct CCachedBlock;
struct CVolumeCatalogSegment;

//////////////////////////////////////////////////////////////////////
// Образ диска в формате .dsk либо .rtd

class CDiskImage
{
protected:
    FILE*           m_fpFile;
    bool            m_okReadOnly;
    bool            m_okNetRT11Image;
    int             m_nTotalBlocks;
    int             m_nCacheBlocks;  // Cache size in blocks
    CCachedBlock*   m_pCache;

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
// Структуры данных, представляющие информацию о томе RT-11

/* Types for rtFileEntry 'status' */
#define RT11_STATUS_TENTATIVE   256     /* Temporary file */
#define RT11_STATUS_EMPTY       512     /* Marks empty space */
#define RT11_STATUS_PERM        1024    /* A "real" file */
#define RT11_STATUS_ENDMARK     2048    /* Marks the end of file entries */

struct CVolumeCatalogEntry;
struct CVolumeCatalogSegment;

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
};

// Структура данных для сегмента каталога
struct CVolumeCatalogSegment
{
public:
    WORD segmentblock;  // Блок на диске, в котором расположен этот сегмент каталога
    WORD entriesused;   // Количество использованых записей каталога
public:
    WORD nextsegment;   // Номер следующего сегмента
    WORD start;         // Номер блока, с которого начинаются файлы этого сегмента
    // Массив записей каталога, размером в максимально возможное кол-во записей для этого сегмента
    CVolumeCatalogEntry* catalogentries;
};

// Структура для хранения разобранной строки каталога
struct CVolumeCatalogEntry
{
public:  // Упакованные поля записи
    WORD status;    // See RT11_STATUS_xxx constants
    WORD datepac;   // Упакованное поле даты
    WORD start;     // File start block number
    WORD length;    // File length in 512-byte blocks
public:  // Распакованные поля записи
    TCHAR name[7];  // File name - 6 characters
    TCHAR ext[4];   // File extension - 3 characters

public:
    CVolumeCatalogEntry();
    void Unpack(WORD const * pSrc, WORD filestartblock);  // Распаковка записи из каталога
    void Pack(WORD* pDest);   // Упаковка записи в каталог
    void Print();  // Печать строки каталога на консоль
};


//////////////////////////////////////////////////////////////////////


extern CVolumeInformation g_volumeinfo;


//////////////////////////////////////////////////////////////////////
