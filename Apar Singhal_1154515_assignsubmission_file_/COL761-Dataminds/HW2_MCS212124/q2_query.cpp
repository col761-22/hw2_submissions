#include "q2.hpp"

std::unordered_map<string, int> labelMap;
std::unordered_map<int, int> graphIndexToID;
vector<graphType> graphs;

void isSubGraphIsoMorphic(graphType G1, graphType G2, string s, bool &val)
{
    vertexCompT vertexComp = make_property_map_equivalent(get(vertex_name, G1), get(vertex_name, G2));
    string value = "";
    edgeCompT edgeComp = make_property_map_equivalent(get(edge_name, G1), get(edge_name, G2));
    value = s;
    vf3_callback<graphType, graphType> callback(G1, G2);
    val =vf2_subgraph_mono(G1, G2, callback, vertex_order_by_mult(G1), edges_equivalent(edgeComp).vertices_equivalent(vertexComp));
}

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

void takeIntersectionSet(std::unordered_set<int> &s1, std::unordered_set<int> &s2, bool val)
{
    if (val)
    {
        vector<int> removeSet;
        for(auto it : s1)
        {
            if(s2.find(it) != s2.end())
            {
                string a = "do nothing";
            }
            else
            {
                removeSet.push_back(it);
            }
        }
        for(auto it: removeSet)
        {
            s1.erase(it);
        }
    }
}

int cleanCandidateSet(std::unordered_set<int> &candidateSet, std::unordered_set<int> &featureGraph, bool val)
{
    takeIntersectionSet(candidateSet, featureGraph, val);
    return 0;
}

int getGraphsFromStandardInput(string file, vector<graphType> &graphs,std::unordered_map<string,int> &labelToCT,std::unordered_map<int,int> &graphIndexToID, high_resolution_clock::time_point beginTime)
{
    ifstream inFile;
    inFile.open(file); 
    stringstream strStream;
    strStream << inFile.rdbuf(); 
    string str = strStream.str(); 
    vector<string> data;
    split(str,'\n', data);

    int fileCT = 0;
    int ctGraph = 0,ctLabel = 1;
    while(fileCT < data.size())
    {
        string dataline = data[fileCT];
        if (dataline.size() <= 0)
        {
            string a = "do nothing";
        }
        else
        {
            if (dataline[0] == '#')
            {
                graphType G;
                int numNodes = 0, numEdges = 0;
                int graphID = stoi(dataline.substr(1));

                fileCT++;
                numNodes = stoi(data[fileCT]);
                int i =0;
                while(i < numNodes)
                {
                    fileCT++;
                    string label = data[fileCT];
                    if(labelToCT.find(label) != labelToCT.end())
                    {
                        string a = "do nothing";
                    }
                    else
                    {
                        labelToCT[label] = ctLabel;
                        ctLabel++;
                    }
                    add_vertex(propNode(labelToCT[label]), G);
                    i++;
                }

                fileCT++;
                numEdges = stoi(data[fileCT]);
                i=0;
                while(i < numEdges)
                {
                    int beginNode;
                    int endNode;
                    string edgeLabel;
                    fileCT ++;
                    istringstream line_stream(data[fileCT]);
                    line_stream >> beginNode >> endNode >> edgeLabel;
                    if (beginNode <= endNode)
                    {
                        string a = "do nothing";
                    }
                    else
                    {
                        int temp = 0;
                        temp = beginNode;
                        beginNode = endNode; 
                        endNode =temp;
                    }
                    if(labelToCT.find(edgeLabel) != labelToCT.end())
                    {
                        string a = "do nothing";
                    }
                    else
                    {
                        labelToCT[edgeLabel] = ctLabel;
                        ctLabel = ctLabel+ 1;
                    }
                    add_edge(beginNode, endNode, propEdge(labelToCT[edgeLabel]), G); 
                    i++;
                }
                graphs.push_back(G);
                graphIndexToID[ctGraph] = graphID;
                ctGraph++;
            }    
        }
        fileCT += 1;  
    }
    return 0;
}

int getQueryGraphsFromStandardInput(string file, string st, vector<graphType> &graphs,std::unordered_map<string,int> &labelToCT,std::vector<int> &support)
{
    string type = st;
    int fileCT = 0;
    int ctGraph = 0;
    int ctLabel = 0;
    ifstream inFile;
    inFile.open(file); 
    stringstream strStream;
    strStream << inFile.rdbuf(); 
    string str = strStream.str(); 

    vector<string> data;
    split(str,'\n', data);

    
    ctLabel = labelToCT.size()+1;
    while(fileCT < data.size())
    {
        int numNodes;
        int numEdges;
        string dataline = data[fileCT];
        if (dataline.size() <= 0)
        {
            string a = "do nothing";
        }
        else
        {
            if (dataline[0] == '#')
            {
                graphType G;
                numNodes = 0;
                numEdges = 0;
                int graphID = std::stoi(dataline.substr(1));
                fileCT++;
                numNodes = stoi(data[fileCT]);
                int i =0;
                while(i < numNodes)
                {
                    fileCT++;
                    string label = data[fileCT];
                    if(labelToCT.find(label) != labelToCT.end())
                    {
                        string a = "do nothing";
                    }
                    else
                    {
                        labelToCT[label] = ctLabel;
                        ctLabel++;
                    }
                    add_vertex(propNode(labelToCT[label]), G);
                    i++;
                }

                fileCT++;
                if(data[fileCT][0] != '\n')
                {
                    if(data[fileCT][0]!= '#')
                    {    
                        numEdges = stoi(data[fileCT]);
                        i = 0;
                        while(i < numEdges)
                        {
                            int beginNode;
                            int endNode;
                            string edgeLabel;
                            fileCT ++;
                            istringstream lineStream(data[fileCT]);
                            lineStream >> beginNode >> endNode >> edgeLabel;
                            if (beginNode <= endNode)
                            {
                                string a = "do nothing";
                            }
                            else
                            {
                                int temp = 0;
                                temp = beginNode;
                                beginNode = endNode; 
                                endNode =temp;
                            }
                            if(labelToCT.find(edgeLabel) != labelToCT.end())
                            {
                                string a = "do nothing";
                            }
                            else
                            {
                                labelToCT[edgeLabel] = ctLabel;
                                ctLabel += 1;
                            }
                            add_edge(beginNode, endNode, propEdge(labelToCT[edgeLabel]), G); 
                            i++;
                        }
                    }
                }
                else
                {
                    fileCT = fileCT -1;
                }
                graphs.push_back(G);
                support.push_back(graphID);
                ctGraph++;
            }
        }
        fileCT += 1;       
    }
    return 0;
}

int pruneUsingDiscriminativeGraphs(graphType &gph, vector<graphType> subGraphs, std::unordered_set<int> &candidateSet, vector<vector<graphType>> &featureGraphs, vector<vector<std::unordered_set<int>>> &featureGraphsTids)
{
    int i =0;
    while(i < featureGraphs.size())
    {
        if(i <= num_edges(gph))
        {
            int j = 0;
            while (j < featureGraphs[i].size())
            {
                bool val;
                isSubGraphIsoMorphic(featureGraphs[i][j], gph, "", val);
                if(val)
                {
                    takeIntersectionSet(candidateSet, featureGraphsTids[i][j], true);
                }
                j++;
            }
        }
        i++;
    }
    return 0;
}

int pruneUsingOneEdgeFeatures(vector<graphType> &graphs, string s, graphType &gph, std::unordered_map<string, std::unordered_set<int>> &binaryFeatureIndex, std::unordered_set<int> &candidateSet, vector<graphType> &subGraphs)
{
    graphType:: edge_iterator edgeItr, end;
    std::unordered_set<string> temp;
    for(tie(edgeItr,end) = edges(gph); edgeItr != end; edgeItr++)
    {
        string label = "";
        string beginNode = "";
        string endNode = "";
        label = std::to_string(get(edge_name, gph)[*edgeItr]);
        beginNode = std::to_string(get(vertex_name, gph)[source(*edgeItr, gph)]);
        endNode = std::to_string(get(vertex_name, gph)[target(*edgeItr, gph)]);
        string indexKey = "";
        string node1 = beginNode;
        string node2 = endNode;
        string edgeLabel = label;
        if (node1 > node2)
        {
            indexKey = indexKey + node2 + "#" + node1+ "#" + edgeLabel;
        }
        indexKey = indexKey + node1 + "#" + node2 + "#" + edgeLabel;

        if(temp.find(indexKey) == temp.end())
        {
            temp.insert(indexKey);
            if (binaryFeatureIndex.find(indexKey) != binaryFeatureIndex.end())
            {
                std::unordered_set<int> featureGraph = binaryFeatureIndex[indexKey];
                int tt = cleanCandidateSet(candidateSet, featureGraph, true);
            }
            else
            {
                std::unordered_set<int> dummySet;
                candidateSet = dummySet;
                return 0;
            }
        }
    }
    return 0;

}

float getGamma(graphType &gph, vector<vector<graphType>> &gphs, std::unordered_set<int> &tids, vector<vector<graphType>> &featureGraphs, vector<vector<std::unordered_set<int>>> &featureGraphsTids)
{
    int gphSize = num_edges(gph);
    std::unordered_set<int> candidates;
    int i = 1;
    while (i < gphSize)
    {
        int j = 0;
        while (j < featureGraphs[i].size())
        {
            bool val;
            isSubGraphIsoMorphic(featureGraphs[i][j], gph, "", val);
            if(val)
            {
                if (candidates.size()!=0)
                {
                    takeIntersectionSet(candidates, featureGraphsTids[i][j], true);
                }
                else
                {
                    candidates = featureGraphsTids[i][j];
                }
            }
            j++;
        }
        i++;
    }
    float value = candidates.size()*1.00/tids.size();
    return value;
}

int getTimeSpentInSec(high_resolution_clock::time_point beginTime)
{
    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds> (endTime - beginTime);
    int val = duration.count();
    return val;
}

string getVectorToString(vector<int> &v, string app)
{
    std::stringstream ss;
    int i =0;
    while(i < v.size())
    {
        if (i != 0)
        {
            ss << "\t";
        }
        ss << app << v[i];
        i++;
    }
    std::string s = ss.str();
    return s;
}

vector<int> searchQueryInGraphs(graphType &gph, string st, vector<graphType> &graphs, std::unordered_map<string, std::unordered_set<int>> &binaryFeatureIndex, std::unordered_map<int, int> &graphIndexToID, vector<vector<graphType>> &featureGraphs, vector<vector<std::unordered_set<int>>> &featureGraphsTids, bool flag  = true)
{
    string b = st;
    std::unordered_set<int> candidateGraphsId;
    int i =0;
    while (i < graphs.size())
    {
        candidateGraphsId.insert(i);
        i++;
    }
    vector<graphType> subGraphs;
    int tt = pruneUsingOneEdgeFeatures(graphs, "", gph, binaryFeatureIndex, candidateGraphsId, subGraphs);
    if (candidateGraphsId.size() <= 1000)
    {
        string a = "do nothing";
    }
    else
    {
        cout << "Further pruning using Graph Index" << endl;
        int t = pruneUsingDiscriminativeGraphs(gph, subGraphs, candidateGraphsId, featureGraphs, featureGraphsTids);
    }
    vector<int> foundInVector;
    for(auto &it : candidateGraphsId)
    {
        bool val;
        isSubGraphIsoMorphic(gph, graphs[it],"", val);
        if(val)
        {
            foundInVector.push_back(it);
        }
    }
    if(flag)
    {
        i = 0;
        while(i < foundInVector.size())
        {
            foundInVector[i] = graphIndexToID[foundInVector[i]];
            i++;
        }
    }
    return foundInVector;
}


void getGraphsSizeWiseFromGspan(string file, vector< vector< graphType> > &graphs,vector< vector< std::unordered_set<int>>> &graphsTIDs)
{
    ifstream inFile;
    inFile.open(file); 

    int tmp =0;
    int maxSize = graphs.size();
    cout <<" Max Size stored : " << maxSize << endl;

    stringstream strStream;
    strStream << inFile.rdbuf(); 
    string str = strStream.str(); 
    vector<string> data;
    split(str,'\n', data);

    int fileCT;
    int ctGraph;
    
    fileCT = 0;
    ctGraph = 0;
    while(fileCT < data.size())
    {
        string line = data[fileCT];
        if (line.size() <= 0)
        {
            string a = "do nothing";
        }
        else
        {
            if (line[0] == 't')
            {
                graphType G;
                int tid , supportCT;
                string strTp;
                string tid_s,supportCTString;
                istringstream lineStream(line);
                lineStream >> strTp >> strTp >> tid_s >> strTp >> supportCTString;
                
                fileCT++;
                int tp1,tp2;
                int tp3;

                tid = stoi(tid_s);
                supportCT = stoi(supportCTString);
                
                while(data[fileCT][0]=='v')
                {
                    istringstream lineStream(data[fileCT].substr(1));
                    lineStream >> tp1 >> tp2;
                    add_vertex(propNode(tp2), G);
                    fileCT ++;
                }
                while(data[fileCT][0] =='e')
                {
                    istringstream lineStream(data[fileCT].substr(1));
                    lineStream >> tp1 >> tp2 >> tp3;
                    if(tp1 > tp2){
                        int tmp = 0;
                        tmp = tp1;
                        tp1 = tp2;
                        tp2=tmp;
                    }
                    add_edge(tp1, tp2, propEdge(tp3), G); 
                    fileCT++;
                }
                std::unordered_set<int> tids;
                if(data[fileCT][0] =='x')
                {
                    istringstream lineStream(data[fileCT].substr(1));
                    while(lineStream >> tp1)
                    {
                        tids.insert(tp1);
                    }
                }
                int edgeCT = num_edges(G);
                if (edgeCT >= maxSize)
                {
                    string b = "do nothing";
                }
                else
                {
                    graphs[edgeCT].push_back(G);
                    graphsTIDs[edgeCT].push_back(tids);
                }
                ctGraph++;
            }
        }
        fileCT++;        
    }
    return ;
}
//////////////////////////////////////////////////////////////////////////////////

void tokenize(std::string const &str, const char delim,
            std::vector<std::string> &out)
{
    // construct a stream from the string
    std::stringstream ss(str);
 
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

void writebinaryFeatureIndex(std::unordered_map<string, std::unordered_set<int>> binaryFeatureIndex)
{
    ofstream myfile;
    myfile.open ("q2binaryFeatureIndex.txt");
    for(auto &it: binaryFeatureIndex){
        std::unordered_set<int> x = it.second;
        myfile << "1-" << it.first << endl;
        for (auto it = x.begin(); it != x.end(); ++it)
        {
            myfile << "2-" << *it << endl;
        }
        myfile<< "Next"<<endl;
    }
    myfile.close();
}


std::unordered_map<string, std::unordered_set<int>> readbinaryFeatureIndex()
{
    std::unordered_map<string, std::unordered_set<int>> binaryFeatureIndex;
    string line;
    const char delim = '-';
    std::unordered_set<int> x;
    string y;
    ifstream myfile ("q2binaryFeatureIndex.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            std::vector<std::string> out;
            tokenize(line, delim, out);
            if (out[0] == "Next")
            {
                binaryFeatureIndex.insert({y, x});
                x.clear();
            }
            else if (stoi(out[0]) == 1)
            {
                y= out[1];
            }
            else if (stoi(out[0]) == 2)
            {
                x.insert(stoi(out[1]));
            }
        }
        myfile.close();
    }

    return binaryFeatureIndex;
}

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
void writefeatureGraphsTids(vector<vector<std::unordered_set<int>>> featureGraphsTids)
{
    ofstream myfile;
    myfile.open ("q2featureGraphsTids.txt");
    for (int i = 0; i < featureGraphsTids.size(); i++)
    {
        for (int j = 0; j < featureGraphsTids[i].size(); j++)
        {
            for (auto it = featureGraphsTids[i][j].begin(); it != featureGraphsTids[i][j].end(); ++it)
            {
                myfile << *it << endl;
            }
            myfile << "Nextj" << endl;
        }
        myfile << "Nexti" << endl;
    }
    myfile.close();
}


vector<vector<std::unordered_set<int>>> readfeatureGraphsTids()
{
    vector<vector<std::unordered_set<int>>> featureGraphsTids;
    string line;
    ifstream myfile ("q2featureGraphsTids.txt");
    std::unordered_set<int> x;
    vector<std::unordered_set<int>> y;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if (line == "Nextj")
            {
                y.push_back(x);
                x.clear();
            }
            else if (line == "Nexti")
            {
                featureGraphsTids.push_back(y);
                y.clear();
            }
            else
            {
                x.insert(stoi(line));
            }
        }
        myfile.close();
    }
    return featureGraphsTids;
}

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
void writegraphIndexToID(std::unordered_map<int, int> graphIndexToID)
{
    ofstream myfile;
    myfile.open ("q2graphIndexToID.txt");
    for(auto &it: graphIndexToID){
        myfile << it.first << " " << it.second << endl;
    }
    myfile.close();
}

std::unordered_map<int, int> readgraphIndexToID()
{
    std::unordered_map<int, int> graphIndexToID;
    string line;
    ifstream myfile ("q2graphIndexToID.txt");
    const char delim = ' ';
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            std::vector<std::string> out;
            tokenize(line, delim, out);
            int x= stoi(out[0]);
            int y= stoi(out[1]);
            graphIndexToID.insert({x, y});
        }
        myfile.close();
    }

    return graphIndexToID;
}

//////////////////////////////////////////////////////////////////////////////////

void writegraphs(vector<graphType> graphs)
{
    ofstream myfile;
    myfile.open ("q2graphs.txt");
    graphType x;
    for (int i = 0; i < graphs.size(); i++)
    {
        x = graphs[i];
        myfile<<"t # "<<num_vertices(x)<<" "<<num_edges(x)<<endl;
        {
            graphType::vertex_iterator vitr,vend;
            vector<int> v(num_vertices(x));
            for(tie(vitr,vend)=vertices(x);vitr!=vend;vitr++)
            {
                int index=get(vertex_index,x)[*vitr];
                int vertexLabel=get(vertex_name,x)[*vitr];
                v[index]=vertexLabel;
            }
            myfile<<"v ";
            for(int i:v)
            {
                myfile<<i<<" ";
            }
            myfile<<endl;
        }
        {
            graphType::edge_iterator eitr,eend;
            for(tie(eitr,eend)=edges(x);eitr!=eend;eitr++){
                int u=get(vertex_index,x)[source(*eitr,x)];
                int v=get(vertex_index,x)[target(*eitr,x)];
                int edge_label=get(edge_name, x)[*eitr];
                if(u>v)
                {
                    swap(u,v);
                }
                // edge notifier
                myfile<<"e "<<u<<" "<<v<<" "<<edge_label<<endl;
            }
        }
        myfile << "Next" << endl;
    }
    myfile.close();
}

vector<graphType> readgraphs()
{
    vector<graphType> graphs;
    string line;
    ifstream myfile ("q2graphs.txt");
    graphType x;
    int numVertex;
    int numEdges;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if (line == "Next")
            {
                graphs.push_back(x);
                x.clear();
            }
            else
            {
                const char delim = ' ';
                std::vector<std::string> out;
                tokenize(line, delim, out);
                if (out[0] == "t" && out[1] == "#")
                {
                    numVertex = stoi(out[2]);
                    numEdges = stoi(out[3]);
                }
                else if (out[0] == "v")
                {
                    for(int i=0;i<numVertex;i++)
                    {
                        int vertexLabel = stoi(out[i+1]);
                        add_vertex(propNode(vertexLabel),x);
                    }
                }
                else if (out[0] == "e")
                {
                    for(int i=0;i<numEdges;i++)
                    {
                        int u = stoi(out[1]),v = stoi(out[2]);
                        int edge_label = stoi(out[3]);
                        add_edge(u,v,propEdge(edge_label),x);
                    }
                }
            }
        }
        myfile.close();
    }
    return graphs;
}

void writefeatureGraphs(vector<vector<graphType>> featureGraphs)
{
    ofstream myfile;
    graphType x; 
    myfile.open ("q2featureGraphs.txt");
    for (int i = 0; i < featureGraphs.size(); i++)
    {
        for (int j = 0; j < featureGraphs[i].size(); j++)
        {
            x = featureGraphs[i][j];
            myfile<<"t # "<<num_vertices(x)<<" "<<num_edges(x)<<endl;
            {
                graphType::vertex_iterator vitr,vend;
                vector<int> v(num_vertices(x));
                for(tie(vitr,vend)=vertices(x);vitr!=vend;vitr++)
                {
                    int index=get(vertex_index,x)[*vitr];
                    int vertexLabel=get(vertex_name,x)[*vitr];
                    v[index]=vertexLabel;
                }
                myfile<<"v ";
                for(int i:v)
                {
                    myfile<<i<<" ";
                }
                myfile<<endl;
            }
            {
                graphType::edge_iterator eitr,eend;
                for(tie(eitr,eend)=edges(x);eitr!=eend;eitr++){
                    int u=get(vertex_index,x)[source(*eitr,x)];
                    int v=get(vertex_index,x)[target(*eitr,x)];
                    int edge_label=get(edge_name, x)[*eitr];
                    if(u>v)
                    {
                        swap(u,v);
                    }
                    // edge notifier
                    myfile<<"e "<<u<<" "<<v<<" "<<edge_label<<endl;
                }
            }
            myfile << "Nextj" << endl;
        }
        myfile << "Nexti" << endl;
    }
    myfile.close();
}

vector<vector<graphType>> readfeatureGraphs()
{
    vector<vector<graphType>> featureGraphs;
    string line;
    graphType x;
    vector<graphType> y;
    int numVertex;
    int numEdges;
    ifstream myfile ("q2featureGraphs.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if (line == "Nextj")
            {
                y.push_back(x);
                x.clear();
            }
            else if(line == "Nexti")
            {
                featureGraphs.push_back(y);
                y.clear();
            }
            else
            {
                const char delim = ' ';
                std::vector<std::string> out;
                tokenize(line, delim, out);
                if (out[0] == "t" && out[1] == "#")
                {
                    numVertex = stoi(out[2]);
                    numEdges = stoi(out[3]);
                }
                else if (out[0] == "v")
                {
                    for(int i=0;i<numVertex;i++)
                    {
                        int vertexLabel = stoi(out[i+1]);
                        add_vertex(propNode(vertexLabel),x);
                    }
                }
                else if (out[0] == "e")
                {
                    for(int i=0;i<numEdges;i++)
                    {
                        int u = stoi(out[1]),v = stoi(out[2]);
                        int edge_label = stoi(out[3]);
                        add_edge(u,v,propEdge(edge_label),x);
                    }
                }
            }
        }
        myfile.close();
    }
    return featureGraphs;
}

int main(int argc, char** argv)
{
    std::unordered_map<string, std::unordered_set<int>> binaryFeatureIndex = readbinaryFeatureIndex();
    vector<vector<std::unordered_set<int>>> featureGraphsTids = readfeatureGraphsTids();
    std::unordered_map<int, int> graphIndexToID = readgraphIndexToID();
    vector<vector<graphType>> featureGraphs = readfeatureGraphs();
    vector<graphType> graphs = readgraphs();

    string queryFilename;
    string queryOutputFilename = "output_MCS212124.txt";
    cout  << "Enter Query File"<< endl;
    cin >> queryFilename;
    auto beginTime = high_resolution_clock::now();

    std::vector<int> querySupports;
    vector<graphType> queryGraphs;
    int t = getQueryGraphsFromStandardInput(queryFilename, "txt", queryGraphs, labelMap, querySupports);
    
    ofstream ofOut(queryOutputFilename);
    cout << "Query Graph Size : " << queryGraphs.size() << endl;
    int i = 0;
    while (i < queryGraphs.size())
    {
        graphType gph = queryGraphs[i];
        vector<int> foundIn = searchQueryInGraphs(gph, "", graphs, binaryFeatureIndex, graphIndexToID, featureGraphs, featureGraphsTids, true);
        if (foundIn.size() <= 0)
        {
            ofOut << endl;
        }
        else
        {
            ofOut << getVectorToString(foundIn, "#") << endl;
        }
        cout << "Detected in " << foundIn.size() << " Actual " << querySupports[i] << endl;
        cout << "Time Taken : " << getTimeSpentInSec(beginTime) << endl;
        i++;
    }
    cout << "Time Taken : " << getTimeSpentInSec(beginTime) << endl;
    ofOut.close();

    return 0;
}



