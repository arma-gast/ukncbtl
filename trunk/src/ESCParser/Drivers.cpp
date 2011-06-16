
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
    m_output << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << r << "\" />" << std::endl;
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
