/*
 * Name: driver.cpp
 * Project name: CPE 400 Final Project
 * Date of Creation: November 25, 2021
 * Memeber Names:
 *        Cooper Flourens
 *        Mark Graham
 */

// NOTE: Mark Graham is solely responsible for the mess below.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

const int size = 100;

int main()
{

    // Gets file input of graph information and checks if file opens properly.
    ifstream inFile;
    inFile.open("output.txt");
    if(!(inFile.is_open()))
    {
        cerr << "Input file failed to open!" << endl;
        return -1;
    }

    // Opens the file for the analysis of the graph.
    ofstream outFile;
    outFile.open("pathAnalysis.txt");
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

    // Gets connection weights for node to node UDP (time in ms) and places
    // them into an array.
    for(int i = 0; endCondition && i < size; i++)
    {

        getline(inFile, line, '\n');
        if(line[0] == '[')
        {
            endCondition = false;
        }

        // Gets the values of the connection nodes and the weight.
        for(int j = 0; endCondition && j < line.length(); j++)
        {
            if(line[j] >= '0' && line[j] <= '9')
            {
                // Gets value of double digits at the end.
                if(line[j+2] == '}' && placement <= 2)
                {
                    int value = line[j] - '0';
                    value *= 10;
                    value += line[j+1] - '0';
                    connectionArray[i][placement] = value;
                    placement++;
                }
                // Gets the value of digits
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

    // Gets k-shortest paths and places the into an array.
    int row = 0, col = 0, num = 0;
    for(int i = 1; i < line.length(); i++)
    {
        // Checks to see if the paths have ended.
        if(line[i] == ']' && line[i+1] != ']')
        {
            pathSize++;
            pathLengths[row] = num;
            num = 0;
            row++;
            col = 0;
        }

        // Places values from path into an array (paths).
        if(line[i] >= '0' && line[i] <= '9')
        {
            int value = line[i] - '0';
            paths[row][col] = value;
            num++;
            col++;
        }

    }

    int TCP = 0, UDP = 0;
    float failing[pathSize] = {0.0}, prob = .1;
    col = 0;

    outFile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << endl << "Path Anaylsis:" << endl
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << endl;

    // Row for the path currently on.
    for(int i = 0; i < pathSize; i++)
    {
        if(i != 0 && i < pathSize)
        {
            outFile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                    << endl;
        }
        outFile << "Path: " << flush;

        // Col for the path currently on.
        for(int j = 0; j < pathLengths[i]-1; j++)
        {

            // Row for the connections currently on.
            for(int k = 0; k < connectionSize; k++)
            {
                // Checks if the connections are in the path, then adds weight
                // to the TCP and adjusts for TCP.
                if(connectionArray[k][0] == paths[i][j] && connectionArray[k][1] == paths[i][j+1] || connectionArray[k][0] == paths[i][j+1] && connectionArray[k][1] == paths[i][j])
                {
                    // Beginning of fence post problem
                    if(j == 0)
                    {
                        outFile << paths[i][j] << flush;
                    }

                    if(j != pathLengths[i]-1)
                    {
                        outFile << " - " << flush;
                    }
                    //^^^^^ Ending of fence post problem ^^^^^

                    // Outputs the path node and updates the UDP, TCP, and
                    // probability of failure.
                    outFile << paths[i][j+1] << flush;
                    UDP += connectionArray[k][2];
                    TCP += UDP + 3;
                    failing[i] += prob;
                    break;
                }
            }
        }

        // Outputs to the file for the data of the current path.
        failing[i] += prob;
        outFile << endl << "NOTE: TCP = UDP + (3ms * number of nodes in path)"
                << endl << "UDP: " << UDP << endl
                << "TCP: " << TCP << endl
                << "Probability of Failure: " << failing[i] * 100
                << "%" << endl;

        // Resets values for next path.
        UDP = 0;
        TCP = 0;
    }

    outFile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << endl << "Path Failure Probability Anaylsis:" << endl
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl ;

    for(int i = 0; i < pathSize; i++)
    {
        for(int j = i ; j < pathSize; j++)
        {
            if(i == j)
            {
                outFile << "Path: " << i + 1 << endl << "Failure Probability: "
                        << failing[i] * 100 << "%" << endl
                        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                        << endl;
            }
            else
            {
                outFile << "Path: " << i+1 << " & " << j+1<< endl
                        << "Failure Probability: "
                        << (failing[i] * failing[j]) * 100 << "%" << endl
                        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                        << endl;
            }
        }
    }

    // Closes the files and terminates the program with a success.
    inFile.close();
    outFile.close();
    return 0;
}
