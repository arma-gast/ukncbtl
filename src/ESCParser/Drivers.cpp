/*  This file is part of UKNCBTL.
    UKNCBTL is free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
    UKNCBTL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License along with
UKNCBTL. If not, see <http://www.gnu.org/licenses/>. */

#include "ESCParser.h"

//////////////////////////////////////////////////////////////////////


void OutputDriverSvg::WriteBeginning()
{
    m_output << "<svg xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
}

void OutputDriverSvg::WriteEnding()
{
    m_output << "</svg>" << std::endl;
}

void OutputDriverSvg::WriteStrike(float x, float y, float r)
{
    float cx = x / 6.0f;
    float cy = y / 6.0f;
    float cr = r / 6.0f;
    m_output << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"" << cr << "\" />" << std::endl;
}


//////////////////////////////////////////////////////////////////////


void OutputDriverPostScript::WriteBeginning()
{
    m_output << "%!PS-Adobe-1.0" << std::endl;
    m_output << "0 850 translate 1 -1 scale" << std::endl;
    m_output << "0 setgray" << std::endl;
}

void OutputDriverPostScript::WriteEnding()
{
    m_output << "showpage" << std::endl;
    m_output << "%%EOF" << std::endl;
}

void OutputDriverPostScript::WriteStrike(float x, float y, float r)
{
    float cx = x / 10.0f;
    float cy = y / 10.0f;
    float cr = r / 10.0f;
    m_output << "newpath " << cx << " " << cy << " " << cr << " 0 360 arc fill" << std::endl;
}



//////////////////////////////////////////////////////////////////////
