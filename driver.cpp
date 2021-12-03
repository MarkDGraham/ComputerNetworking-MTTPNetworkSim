#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

const int size = 100;

int main()
{

    ifstream inFile;
    inFile.open("output.txt");
    if(!(inFile.is_open()))
    {
        cerr << "Input file failed to open!" << endl;
        return -1;
    }

    ofstream outFile;
    outFile.open("results.txt");
    if(!(outFile.is_open()))
    {
        cerr << "Output file failed to open!" << endl;
        return -1;
    }

    int connectionArray[size][3], connectionSize = 0;
    int paths[10][10], pathSize = 0;
    int pathLengths[10] = {0};
    string line = "";
    int placement = 0;

    bool endCondition = true;

    // Gets connection weights for node to node UDP
    for(int i = 0; endCondition && i < size; i++)
    {

        getline(inFile, line, '\n');
        if(line[0] == '[')
        {
            endCondition = false;
        }

        for(int j = 0; endCondition && j < line.length(); j++)
        {
            if(line[j] >= '0' && line[j] <= '9')
            {
                if(line[j+2] == '}' && placement <= 2)
                {
                    int value = line[j] - '0';
                    value *= 10;
                    value += line[j+1] - '0';
                    connectionArray[i][placement] = value;
                    placement++;
                }
                else if(placement <= 2)
                {
                    int value = line[j] - '0';
                    connectionArray[i][placement] = value;
                    placement++;
                }
            }
        }
        connectionSize++;
        placement = 0;
    }

    // Gets k-shortest paths
    int row = 0, col = 0, num = 0;
    for(int i = 1; i < line.length(); i++)
    {
        if(line[i] == ']' && line[i+1] != ']')
        {
            pathSize++;
            pathLengths[row] = num;
            num = 0;
            row++;
        }

        if(line[i] >= '0' && line[i] <= '9')
        {
            int value = line[i] - '0';
            paths[row][col] = value;
            num++;
            col++;
        }
    }

    int TCP = 0, UDP = 0;
    float failing = 0;
    col = 0;

    // Row for the path currently on.
    for(int i = 0; i < pathSize; i++)
    {
        // Col for the path currently on.
        for(int j = 0; j < pathLengths[i]-1; j++)
        {
            // Row for the connections currently on.
            for(int k = 0; k < connectionSize; k++)
            {
                // Bug is that second connection does notwant to add to the UDP
                // weight. I believe it has something to do with th loop.
                if(connectionArray[k][0] == paths[i][j] && connectionArray[k][1] == paths[i][j+1])
                {
                    UDP += connectionArray[k][2];
                    break;
                }
            }
        }
        UDP = 0;
    }

    inFile.close();
    outFile.close();
    return 0;
}
