#include "messages.h"
#include "def.h"

#include <iostream>
#include <fstream>


/***********************************************************
    constructor
************************************************************/
Messages::Messages () {}


/***********************************************************
    shows usage guide
************************************************************/
void Messages::help (void)
{
    std::cout
            << "\nSynopsis:\n"
            << "    phoenix [OPTION]... -t [TARGET] -r [REFERENCE]\n"
            << "\n"
            << "Options:\n"
            << "    -h,  --help\n"
            << "         usage guide\n"
            << "\n"
            << "    -V,  --version\n"
            << "         version number\n"
            << "\n"
            << "    -v,  --verbose\n"
            << "         verbose mode (more information)\n"
            << "\n"
            << "    -i,  --inverted_repeat\n"
            << "         considers inverted repeats\n"
            << "\n"
            << "    -n [NUMBER],  --number [NUMBER]\n"
            << "         number of something.\n"
            << "         requires an integer number (NUMBER)\n"
            << "\n"
            << "    -d [NUMBER],  --fnumber [NUMBER]\n"
            << "         number of something.\n"
            << "         requires a float number (NUMBER)\n"
            << "\n"
            << "    -t [TARGET],  --target [TARGET]\n"
            << "         target file\n"
            << "\n"
            << "    -r [REFERENCE],  --reference [REFERENCE]\n"
            << "         reference file\n\n";
}


/***********************************************************
    shows version number
************************************************************/
void Messages::version ()
{
    std::cout
      <<"\n"
      <<"              ====================\n"
      <<"              ==  PHOENIX v" << VERSION_PHOENIX << "." << RELEASE_PHOENIX << "  ==\n"
      <<"              ====================\n";
}


/***********************************************************
    shows verbose mode
************************************************************/
void Messages::verbose (void)
{
    std::cout << "verbose mode.\n"; // for test
}


/***********************************************************
    shows inverted_repeat mode
************************************************************/
void Messages::inverted_repeat (void)
{
    std::cout << "inverted repeat mode.\n"; // for test
}


/***********************************************************
    shows integer number mode
************************************************************/
void Messages::number (int num)
{
    std::cout << "integer number mode (n=" << num << ")\n"; // for test
}


/***********************************************************
    shows float number mode
************************************************************/
void Messages::fnumber (float num)
{
    std::cout << "float number mode (d=" << num << ")\n"; // for test
}


/***********************************************************
    reads target file
************************************************************/
void Messages::targetRead (std::string fileName)
{
    // file opened
    std::ifstream targetFile(fileName, std::ios::in);

    if (!targetFile)
    {
        std::cerr << "Option 't': target file ("
                  << fileName << ") could not be opened.\n";
        return;
    }

    std::string strEachLine;
    std::string strDataset;
    
    while (std::getline(targetFile, strEachLine))
        strDataset += strEachLine;

    // show the dataset
    std::cout << "Dataset: " << strDataset << std::endl;
    
    targetFile.close();
    // file closed
}


/***********************************************************
    reads reference file
************************************************************/
void Messages::referenceRead (std::string fileName)
{
    // file opened
    std::ifstream targetFile(fileName, std::ios::in);
    
    if (!targetFile)
    {
        std::cerr << "Option 't': target file ("
                  << fileName << ") could not be opened.\n";
        return;
    }
    
    std::string strEachLine;
    std::string strDataset;
    
    while (std::getline(targetFile, strEachLine))
        strDataset += strEachLine;
    
    // show the dataset
    std::cout << "Dataset: " << strDataset << std::endl;
    
    targetFile.close();
    // file closed
}