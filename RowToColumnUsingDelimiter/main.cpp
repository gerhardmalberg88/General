#include <iostream>
#include <iomanip>
#include <fstream>

int main(int argc, char *argv[])
{
    std::string lineStringFromConvertableFile;
    std::string delimiterString = "TIMESTAMP";
    int detectedDelimiter = 0;
    // Process arguments (optional)
    std::cout << "argv = " << argc << std::endl;
    for (int count{0}; count < argc; ++count)
    {
        std::cout << count << " " << argv[count] << std::endl;
    }
    // Get convertable text file using program arguments
    std::ifstream convertableFile;
    convertableFile.open(argv[1]);
    if (!convertableFile) 
    {
        std::cout << "Unable to open file";
        exit(1);
    }
    // Open file to write processed data
    std::ofstream convertedFile("convertedFile2.txt");
    if (!convertedFile) 
    {
        std::cout << "Unable to open file";
        exit(1);
    }
    // Process and convert text from row type to column type by using delimiter                                                
    while (getline (convertableFile, lineStringFromConvertableFile)) 
    {
        if(lineStringFromConvertableFile == delimiterString || lineStringFromConvertableFile == "Starting Loop")
        {
            // Write next line when delimiter is detected
            std::cout << "" << std::endl;
            convertedFile << "\n";
            detectedDelimiter = detectedDelimiter + 1;
        }
        // Output the text from the file
        std::cout << lineStringFromConvertableFile;
        convertedFile << lineStringFromConvertableFile;
    }
    convertableFile.close();
    convertedFile.close();
    std::cout << "\n\n\n***************Converting finished succesfully " << std::endl;
    std::cout << "***************Delimiters count " << detectedDelimiter << std::endl;
    std::cout << "***************Converted file name converted.txt "<< std::endl; 
    return 0;
    // Write text to other file
}