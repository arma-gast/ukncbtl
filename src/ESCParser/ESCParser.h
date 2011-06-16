/*  This file is part of UKNCBTL.
    UKNCBTL is free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
    UKNCBTL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License along with
UKNCBTL. If not, see <http://www.gnu.org/licenses/>. */

#ifndef _ESCPARSER_H_
#define _ESCPARSER_H_

#include <iostream>

extern unsigned short RobotronFont[];


//////////////////////////////////////////////////////////////////////

enum
{
    OUTPUT_DRIVER_UNKNOWN = 0,
    OUTPUT_DRIVER_SVG = 1,
    OUTPUT_DRIVER_POSTSCRIPT = 2,
};

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
    bool m_fontsp;      // Шрифт вразрядку
    bool m_fontdo;      // Двойная печать
    bool m_fontfe;      // Жирный шрифт
    bool m_fontks;      // Сжатый шрифт
    bool m_fontel;      // Шрифт "элита"
    bool m_fontun;      // Подчеркивание
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
