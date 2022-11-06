#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);
    string inFileName = argv[1];
    string outFileName = argv[2];
    ifstream inFile(inFileName);
    ofstream outFile(outFileName);

    int labelInt = 1; 
    unordered_map<string,int> labelToInt;
    string line;
    while(getline(inFile, line))
    {
        istringstream ss(line);
        char c;
        if (!(ss >> c) || c != '#') continue;

        // write graph id
        outFile << "t # " << ss.str() << "\n";
        // write nodes
        getline(inFile,line);
        int numNodes = stoi(line);
        for(int i=0; i<numNodes; i++)
        {
            getline(inFile,line);
            if (labelToInt.find(line)==labelToInt.end()) labelToInt[line] = labelInt++;
            outFile << "v " << i << " " << labelToInt[line] << "\n";
        }
        // write edges
        getline(inFile,line);
        int numEdges = stoi(line);
        for(int i=0; i<numEdges; i++)
        {
            getline(inFile,line);
            ss.str(line);
            ss.clear();
            int source,dest;
            string label;
            ss >> source >> dest >> label;
            if (source > dest)
            {
                int temp = source;
                source = dest;
                dest = temp;
            }
            if (labelToInt.find(label)==labelToInt.end()) labelToInt[label] = labelInt++;
            outFile << "e " << source << " " << dest << " " << labelToInt[label] << "\n";
        }
    }
}