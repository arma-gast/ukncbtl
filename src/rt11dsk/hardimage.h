// hardimage.h : HDD image utilities headers

//////////////////////////////////////////////////////////////////////

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
    void PrintImageInfo();
    void PrintPartitionTable();
};

//////////////////////////////////////////////////////////////////////
