// hardimage.h : HDD image utilities headers

//////////////////////////////////////////////////////////////////////

struct CPartitionInfo;

class CHardImage
{
protected:
    FILE*           m_fpFile;
    bool            m_okReadOnly;
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
    void PrintPartitionTable();
};

//////////////////////////////////////////////////////////////////////
