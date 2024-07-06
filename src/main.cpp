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
                if (parsedArgs.testNumber < 1 || parsedArgs.testNumber > 4)
                {
                    cerr << "Error: Test number must be between 1 and 4 inclusive, got '" << args[i] << "'." << endl;
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

    ofstream outputFile(parsedArgs.outputFilePath, ios::binary);
    if (!outputFile.is_open())
    {
        cerr << "Error: Could not open output file '" << parsedArgs.outputFilePath << "'." << endl;
        exit(-1);
    }

    if (parsedArgs.testNumber == 3)
    {
        Trie trie(parsedArgs.inputFilePath);
        trie.print();
        cout << endl;
        trie.printASCII();
        cout << endl;

    #ifdef _APPROACH_3_
        #pragma omp parallel
    #endif
        {
        #ifdef _APPROACH_3_
            #pragma omp single
            cerr << "Number of threads: " << omp_get_num_threads() << endl;
            cerr << "Thread " << omp_get_thread_num() << " is working." << endl;
        
            #pragma omp single
        #endif
            {
                outputFile << getLevelSum(trie, 0) << endl;
                outputFile << getLevelSum(trie, 1) << endl;
                outputFile << getLevelSum(trie, 2) << endl;
                outputFile << getLevelSum(trie, 3) << endl;
                outputFile << getLevelSum(trie, 4) << endl;
                outputFile << getLevelSum(trie, 5) << endl;
                outputFile << getLevelSum(trie, 6) << endl;
                outputFile << getLevelSum(trie, 7) << endl;
                outputFile << getLevelSum(trie, 8) << endl;
                outputFile << getLevelSum(trie, 9) << endl;
                outputFile << getLevelSum(trie, 10) << endl;
                outputFile << getLevelSum(trie, 11) << endl;
                outputFile << getLevelSum(trie, 123) << endl;
            }
        }
    }
    else
    {
        ifstream inputFile(parsedArgs.inputFilePath, ios::binary);
        if (!inputFile.is_open())
        {
            cerr << "Error: Could not open input file '" << parsedArgs.inputFilePath << "'." << endl;
            exit(-1);
        }
        inputFile.seekg(0, ios::end);
        size_t fileSizeInts = (static_cast<size_t>(inputFile.tellg()) + sizeof(int) - 1) / sizeof(int); // assume that the file contains only integers
        inputFile.seekg(0, ios::beg);
        vector<int, AlignedAllocator<int>> inputVectorAligned(fileSizeInts);              // reserve space for the vector
        vector<int> &inputVector = reinterpret_cast<vector<int> &>(inputVectorAligned); // this should be avoided...
        inputFile.read(reinterpret_cast<char *>(inputVector.data()), fileSizeInts * sizeof(int)); // assume that the endianness is correct
        inputFile.close();


        switch (parsedArgs.testNumber)
        {
        case 1:
            {
                auto start = chrono::high_resolution_clock::now();
                int result = getClosestToZero(inputVector);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
                outputFile << result; // write the result to the output file in ASCII

                cerr << duration.count() << endl;
            }
            break;
        
        case 2:
            {
                auto start = chrono::high_resolution_clock::now();
                int result = countChunks(inputVector);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
                outputFile << result; // write the result to the output file in ASCII
                
                cerr << duration.count() << endl;
            }
            break;
        
        case 4:
            {// scope for the local variable
                vector<size_t> reversals = getReversalsToSort(inputVector);
                // write the result to the output file in binary
                outputFile.write(reinterpret_cast<char *>(reversals.data()), reversals.size() * sizeof(size_t));
            }
            break;
        }
    }
    outputFile.close();

    return 0;
}
