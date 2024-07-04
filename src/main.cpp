#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
    struct
    {
        int testNumber = -1; // enum would be better here, but let's keep it simple
        string inputFilePath;
        string outputFilePath;
    } parsedArgs;

    // basic argument parsing
    vector<string> args(argv, argv + argc);
    for (size_t i = 1; i < args.size(); i++)
    {
        if (args[i] == "-i" && ++i < args.size())
        {
            parsedArgs.inputFilePath = args[i];
        }
        else if (args[i] == "-o" && ++i < args.size())
        {
            parsedArgs.outputFilePath = args[i];
        }
        else if (args[i] == "-t" && ++i < args.size())
        {
            try
            {
                parsedArgs.testNumber = stoi(args[i]);
                if (parsedArgs.testNumber < 1 || parsedArgs.testNumber > 3)
                {
                    cerr << "Error: Test number must be between 1 and 3 inclusive, got '" << args[i] << "'." << endl;
                    exit(-1);
                }
            }
            catch (const invalid_argument& e)
            {
                cerr << "Error: Unsigned integer expected after the '-t' switch, got '" << args[i] << "'." << endl;
                exit(-1);
            }
            catch (const out_of_range& e)
            {
                cerr << "Error: Specified width is out of range with '" << args[i] << "'." << endl;
                exit(-1);
            }
        }
    }

    if (parsedArgs.inputFilePath.empty() || parsedArgs.outputFilePath.empty() || parsedArgs.testNumber == -1)
    {
        cerr << "Error: Missing required arguments." << endl;
        exit(-1);
    }

    switch (parsedArgs.testNumber)
    {
    case 1:
        ifstream inputFile(parsedArgs.inputFilePath, ios::binary);
        if (!inputFile.is_open())
        {
            cerr << "Error: Could not open input file '" << parsedArgs.inputFilePath << "'." << endl;
            exit(-1);
        }
        inputFile.seekg(0, ios::end);
        size_t fileSizeInts = (static_cast<size_t>(inputFile.tellg()) + sizeof(int) - 1) / sizeof(int); // assume that the file contains only integers
        inputFile.seekg(0, ios::beg);
        vector<int> inputVector(fileSizeInts); // reserve space for the vector
        inputFile.read(reinterpret_cast<char *>(inputVector.data()), fileSizeInts * sizeof(int)); // assume that the endianness is correct
        inputFile.close();

        int closestToZero = getClosestToZero(inputVector);
        ofstream outputFile(parsedArgs.outputFilePath, ios::binary);
        if (!outputFile.is_open())
        {
            cerr << "Error: Could not open output file '" << parsedArgs.outputFilePath << "'." << endl;
            exit(-1);
        }
        // write the result to the output file in ASCII
        outputFile << closestToZero;
        outputFile.close();
        break;
    }

    return 0;
}
