#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <cmath>
#include <sstream>
#include <iostream>

using namespace std;

void split(std::string const &str, const char delim,
            std::vector<std::string> &splitStrings)
{
    // construct a stream from the string
    std::stringstream ss(str);
 
    std::string s;
    while (std::getline(ss, s, delim)) {
        splitStrings.push_back(s);
    }
}

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    string inputFile = argv[1];
    ifstream inFile;
    inFile.open(inputFile);
    stringstream strStream;
    strStream << inFile.rdbuf();
    string str = strStream.str();

    vector<string> data;
    split(str,'\n', data);
    int ctLabel = 1,fileCT = 0, numNodes = 0, numEdges = 0;

    string outputFile = argv[2];
    ofstream oFile(outputFile);
    unordered_map<string, int> labelToCT;
     
    while(fileCT < data.size())
    {
        string lineData = data[fileCT];
        if(lineData.size()>0)
        {
            if(lineData[0] == '#')
            {
                numNodes = 0;
                numEdges = 0;
                string graphID = lineData.substr(1);
                oFile << "t # " << graphID << "\n";
                fileCT = fileCT +1;
                numNodes = stoi(data[fileCT]);
                int i =0;
                while (i < numNodes)
                {
                    fileCT = fileCT + 1;
                    string label = data[fileCT];               
                    if (labelToCT.find(label) == labelToCT.end())
                    {
                        labelToCT[label] = ctLabel;
                        ctLabel = ctLabel + 1;
                    }
                    oFile << "v " << i << " " << labelToCT[label] << "\n";
                    i=i+1;
                }

                fileCT = fileCT +1;
                numEdges = stoi(data[fileCT]);
                i = 0;
                while (i < numEdges)
                {
                    int startNode;
                    int endNode;
                    string edgeLabel;
                    fileCT = fileCT + 1;
                    istringstream lineStream(data[fileCT]);
                    lineStream >> startNode >> endNode >> edgeLabel;           
                    if (endNode < startNode)
                    {
                        int temp = 0;
                        temp = startNode;
                        startNode = endNode;
                        endNode = temp;
                    }
                    if (labelToCT.find(edgeLabel) != labelToCT.end())
                    {
                        string s = "Do Nothing";
                    }
                    else
                    {
                        labelToCT[edgeLabel] = ctLabel;
                        ctLabel = ctLabel + 1;
                    }
                    oFile << "e " << startNode << " " << endNode << " " << labelToCT[edgeLabel] << "\n";
                    i=i+1;
                }
            }
        }
        fileCT = fileCT+1;
    }

}