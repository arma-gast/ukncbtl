// Settings.cpp

#include "stdafx.h"
#include "UKNCBTL.h"
#include <iostream>
#include <fstream>


/////////////////////////////////////////////////////////////////////////////

typedef std::map<TSTRING, TSETTINGSKEYVALUEMAP> TSETTINGSSECTIONMAP;

static TSETTINGSSECTIONMAP m_Settings_Sections;

const TCHAR* SETTINGS_SECTION_UKNCBTL = _T("UKNCBTL");
const TCHAR* SETTINGS_SECTION_SDL = _T("SDL");

void Settings_ParseIniFile(const TSTRING& sFileName)
{
    std::ifstream ifs(sFileName.c_str(), std::ifstream::in);

    TSETTINGSKEYVALUEMAP* pCurrentSection = 0;
    while (ifs.good())
    {
        char line[256];
        ifs.getline(line, 256);

        if (line[0] == 0) continue;  // Skip empty lines
        if (line[0] == '#' || line[0] == ';') continue;  // Skip comments

        if (line[0] == '[')  // Parse section header
        {
            TSTRING sSection(line + 1);
            for (;;)  // Remove trailing spaces and bracket
            {
                TCHAR ch = sSection.at(sSection.length() - 1);
                if (ch == _T(' ') || ch == _T('\t') || ch == _T(']'))
                    sSection.resize(sSection.length() - 1);
                else
                    break;
            }
            //printf("INI Section \'%s\'\n", sSection.c_str());
            pCurrentSection = &m_Settings_Sections[sSection];

            continue;
        }

        // Parse 'key=value' line
        TSTRING sLine(line);
        TSTRING::size_type eqpos = sLine.find_first_of(_T('='));
        if (eqpos == TSTRING::npos)
            continue;  // The line has no '=' char, skipping
        TSTRING sKey(sLine.substr(0, eqpos));
        //TODO: Trim leading/trailing spaces/tabs
        TSTRING sValue(sLine.substr(eqpos + 1));
        //TODO: Trim leading/trailing spaces/tabs
        //printf("INI KeyValue \'%s\' = \'%s\'\n", sKey.c_str(), sValue.c_str());
        if (pCurrentSection != 0)
            (*pCurrentSection)[sKey] = sValue;
    }

    ifs.close();
}

TSTRING Settings_GetValue(const TCHAR* section, const TCHAR* key)
{
    TSTRING sSection(section);
    TSETTINGSSECTIONMAP::iterator itsect = m_Settings_Sections.find(section);
    if (itsect == m_Settings_Sections.end())
        return TSTRING();

    TSTRING sKey(key);
    const TSETTINGSKEYVALUEMAP& sectmap = itsect->second;
    TSETTINGSKEYVALUEMAP::const_iterator itkeyval = sectmap.find(sKey);
    if (itkeyval == sectmap.end())
        return TSTRING();

    return itkeyval->second;
}

const TSETTINGSKEYVALUEMAP* Settings_GetKeyValues(const TCHAR* section)
{
    TSTRING sSection(section);
    TSETTINGSSECTIONMAP::iterator itsect = m_Settings_Sections.find(section);
    if (itsect == m_Settings_Sections.end())
        return 0;

    return &(itsect->second);
}


/////////////////////////////////////////////////////////////////////////////
