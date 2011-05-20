// hardimage.h : HDD image utilities headers

//////////////////////////////////////////////////////////////////////

#define RT11_BLOCK_SIZE         512

enum HDDDriverType
{
    HDD_DRIVER_UNKNOWN = 0,
    HDD_DRIVER_ID = 1,
    HDD_DRIVER_WD = 2,
};

struct CPartitionInfo;

class CHardImage
{
protected:
    FILE*           m_fpFile;
    bool            m_okReadOnly;
    bool            m_okInverted;       // Inverted image
    long            m_lFileSize;
    HDDDriverType   m_drivertype;
    int             m_nSectorsPerTrack;
    BYTE            m_nSidesPerTrack;
    int             m_nPartitions;
    CPartitionInfo* m_pPartitionInfos;

public:
    CHardImage();
    ~CHardImage();

public:
    bool Attach(LPCTSTR sFileName);
    void Detach();

public:
    int IsReadOnly() const { return m_okReadOnly; }
    int GetPartitionCount() const { return m_nPartitions; }

public:
    void PrintImageInfo();
    void PrintPartitionTable();
    void SavePartitionToFile(int partition, LPCTSTR filename);
    void UpdatePartitionFromFile(int partition, LPCTSTR filename);
    void InvertImage();
};

//////////////////////////////////////////////////////////////////////
