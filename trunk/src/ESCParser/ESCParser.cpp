
#define _CRT_SECURE_NO_WARNINGS

#include "ESCParser.h"
#include <iostream>

const char* g_InputFileName = 0;
void* g_InputData = 0;

void main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Input file is not specified." << std::endl;
        return;
    }
    g_InputFileName = argv[1];

    // Open and read the file
    FILE* fpfile = ::fopen(g_InputFileName, "rb");
    if (fpfile == 0)
    {
        std::cerr << "Failed to open the input file." << std::endl;
        return;
    }
    
    // Get file size
    ::fseek(fpfile, 0, SEEK_END);
    long filesize = ::ftell(fpfile);
    ::fseek(fpfile, 0, SEEK_SET);

    // Allocate memory
    g_InputData = ::malloc(filesize);

    // Read the file
    ::fread(g_InputData, 1, filesize, fpfile);
    //TODO: Check bytes read

    ::fclose(fpfile);

    std::cout << "<svg xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

    // Initialize the emulator
    EscInterpreter intrpr(g_InputData, filesize, std::cout);
    // Run the emulator
    while (true)
    {
        if (!intrpr.InterpretNext())
            break;
    }

    std::cout << "</svg>" << std::endl;

    // Free the memory
    ::free(g_InputData);  g_InputData = 0;
}
