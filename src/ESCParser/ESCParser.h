
#ifndef _ESCPARSER_H_
#define _ESCPARSER_H_

#include <iostream>

extern unsigned short RobotronFont[];

// Printing qualities
#define DRAFTmode 0
#define LQmode    1

class EscInterpreter
{
private:
    const void* m_pdata;
    const unsigned char* m_pnext;
    long m_datalength;
    std::ostream& m_output;
    long m_datapos;
    int  m_x, m_y;  // Current position
    int  m_px, m_py;
    int  m_marginleft, m_margintop;
    int  m_shiftx, m_shifty;  // Shift for text printout
    int  m_printmode;
    bool m_endofpage;
    bool m_widefont;  // Шрифт вразрядку
    bool m_doublestrike;  // Двойная печать

public:
    EscInterpreter(const void* pdata, long datalength, std::ostream& output);
    bool InterpretNext();
    bool InterpretEscape();
    bool IsEndOfFile() const { return m_datapos >= m_datalength; }

protected:
    unsigned char GetNextByte();
    void PrinterReset();
    void printGR9(int dx);
    void printGR24(int dx);
    void PrintCharacter(unsigned char ch);
    void DrawStrike(float x, float y);
};


#endif // _ESCPARSER_H_
