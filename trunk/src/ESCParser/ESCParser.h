
#ifndef _ESCPARSER_H_
#define _ESCPARSER_H_

#include <iostream>

extern unsigned short RobotronFont[];


//////////////////////////////////////////////////////////////////////


class OutputDriver
{
protected:
    std::ostream& m_output;

public:
    OutputDriver(std::ostream& output) : m_output(output) { }

public:
    virtual void WriteBeginning() { }  // Overwrite if needed
    virtual void WriteEnding() { }  // Overwrite if needed
    virtual void WriteStrike(float x, float y, float r) = 0;  // Always overwrite
};

class OutputDriverSvg : public OutputDriver
{
public:
    OutputDriverSvg(std::ostream& output) : OutputDriver(output) { };

public:
    virtual void WriteBeginning();
    virtual void WriteEnding();
    virtual void WriteStrike(float x, float y, float r);
};

class OutputDriverPostScript : public OutputDriver
{
public:
    OutputDriverPostScript(std::ostream& output) : OutputDriver(output) { };

public:
    virtual void WriteBeginning();
    virtual void WriteEnding();
    virtual void WriteStrike(float x, float y, float r);
};


//////////////////////////////////////////////////////////////////////


class EscInterpreter
{
private:
    const void* m_pdata;
    const unsigned char* m_pnext;
    long m_datalength;
    OutputDriver& m_output;
    long m_datapos;
    int  m_x, m_y;      // Current position
    int  m_marginleft, m_margintop;
    int  m_shiftx, m_shifty;  // Shift for text printout
    bool m_printmode;   // false - DRAFT, true - LQ
    bool m_endofpage;
    bool m_fontsp;      // ����� ���������
    bool m_fontdo;      // ������� ������
    bool m_fontfe;      // ������ �����
    bool m_fontks;      // ������ �����
    bool m_fontel;      // ����� "�����"
    bool m_fontun;      // �������������
    bool m_superscript;
    bool m_subscript;

public:
    EscInterpreter(const void* pdata, long datalength, OutputDriver& output);
    bool InterpretNext();
    bool InterpretEscape();
    bool IsEndOfFile() const { return m_datapos >= m_datalength; }

protected:
    unsigned char GetNextByte();
    void UpdateShiftX();
    void PrinterReset();
    void printGR9(int dx);
    void printGR24(int dx);
    void PrintCharacter(unsigned char ch);
    void DrawStrike(float x, float y);
};


//////////////////////////////////////////////////////////////////////
#endif // _ESCPARSER_H_
