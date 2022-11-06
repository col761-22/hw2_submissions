#include "graph-utils.hpp"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

void readIndex(
    string &graphsFilename,
    unordered_map<tuple<int, int, int>, unordered_set<int>, EdgeHash>& edgeIdxs,
    vector<vector<graph_type>>& features,
    vector<vector<unordered_set<int>>>& featureIdxs
)
{
    ifstream edgeIdxsFile( "edge-idxs.txt" );
    ifstream featuresFile( "features.txt" );
    ifstream featureIdxsFile( "feature-idxs.txt" );

    string line;
    int x;
    getline( edgeIdxsFile, graphsFilename );
    getline( edgeIdxsFile, line );
    edgeIdxs.reserve(stoi(line));
    while ( getline( edgeIdxsFile, line ) )
    {
        istringstream ss( line );
        char c;
        if ( !( ss >> c ) || c != '#' ) continue;
        int source, dest, label;
        ss >> source >> dest >> label;
        tuple<int, int, int> edge = make_tuple( source, dest, label );

        unordered_set<int> idxs;
        int size;
        ss >> size;
        idxs.reserve(size);
        for ( int i = 0; i < size; i++ )
        {
            getline( edgeIdxsFile, line );
            ss.str( line );
            ss.clear();
            ss >> x;
            idxs.insert( x );
        }
        edgeIdxs[edge] = idxs;
    }
    // FEATURES
    getline(featuresFile,line);
    istringstream ss(line);
    int numFeatures;
    ss >> numFeatures;
    features.resize(numFeatures);
    int k=0;
    while(getline(featuresFile,line))
    {
        if (line != "$$") continue;
        int size;
        getline(featuresFile,line);
        ss.str(line);
        ss.clear();
        ss >> size;
        features[k].resize(size);
        for(int i=0; i<size; i++)
        {
            getline(featuresFile,line);
            ss.str(line);
            ss.clear();
            char c;
            if (!(ss >> c) || c!='#') continue;
            
            graph_type G;
            int numVertices, numEdges;
            ss >> numVertices >> numEdges;
            unordered_map<int,graph_type::vertex_descriptor> nameToVertex;
            for(int j=0; j<numVertices; j++)
            {
                getline(featuresFile,line);
                ss.str(line);
                ss.clear();
                int x;
                ss >> x;
                graph_type::vertex_descriptor v = boost::add_vertex(vertex_prop(x),G);
                nameToVertex[x] = v;
            }
            for(int j=0; j<numEdges; j++)
            {
                getline(featuresFile,line);
                ss.str(line);
                ss.clear();
                int source,dest,label;
                ss >> source >> dest >> label;
                boost::add_edge(nameToVertex[source], nameToVertex[dest], edge_prop(label), G);
            }

            features[k][i] = G; 
        }
        k++;
    }
    // FEATURE IDXS
    getline(featureIdxsFile,line);
    ss.str(line);
    ss.clear();
    int numFeatureIdxs;
    ss >> numFeatureIdxs;
    featureIdxs.resize(numFeatureIdxs);
    k = 0;
    while(getline(featureIdxsFile,line))
    {
        if (line != "$$") continue;
        getline(featureIdxsFile,line);
        ss.str(line);
        ss.clear();
        int numSets;
        ss >> numSets;
        featureIdxs[k].resize(numSets);
        for(int i=0; i<numSets; i++)
        {
            getline(featureIdxsFile,line);
            ss.str(line); ss.clear();
            char c;
            if (!(ss >> c) || c!='#') continue;

            unordered_set<int> idxs;
            int size;
            ss >> size;
            idxs.reserve(size);
            for(int j=0; j<size; j++)
            {
                getline(featureIdxsFile,line);
                ss.str(line);
                ss.clear();
                int x;
                ss >> x;
                idxs.insert(x);
            }
            featureIdxs[k][i] = idxs; 
        }
        k++;
    }
};

void prune1( graph_type& query, vector<graph_type>& graphs, unordered_map<tuple<int, int, int>, unordered_set<int>, EdgeHash>& edgeIdxs, unordered_set<int>& candidates )
{

    graph_type::edge_iterator first, last;
    unordered_set<tuple<int, int, int>, EdgeHash> seenEdges;

    auto es = boost::edges( query );

    for ( auto eit = es.first; eit != es.second; eit++ )
    {
        int source, dest, label;
        source = boost::get( boost::vertex_name, query )[boost::source( *eit, query )];
        dest = boost::get( boost::vertex_name, query )[boost::target( *eit, query )];
        label = boost::get( boost::edge_name, query )[*eit];
        tuple<int, int, int> edge = make_tuple( source, dest, label );

        if ( seenEdges.find( edge ) == seenEdges.end() )
        {
            seenEdges.insert( edge );
            if ( edgeIdxs.find( edge ) == edgeIdxs.end() )
            {
                unordered_set<int> empty;
                candidates = empty;
                return;
            }
            getIntersection( candidates, edgeIdxs[edge] );
        }

    }

    return;
}

void pruneDiscriminative( graph_type& query, unordered_set<int>& candidates, vector<vector<graph_type>>& features, vector<vector<unordered_set<int>>>& featureIdxs )
{

    for ( int i = 2; i < features.size(); i++ )
    {
        if ( i <= boost::num_edges( query ) )
        {
            for ( int j = 0; j < features[i].size(); j++ )
            {
                if ( checkSubgraphIsomorphism( features[i][j], query ) )
                {
                    getIntersection( candidates, featureIdxs[i][j] );
                }
            }
        }
    }

    return;
}

void searchQuery( graph_type& query, vector<graph_type>& graphs, unordered_map<tuple<int, int, int>, unordered_set<int>, EdgeHash>& edgeIdxs, vector<int>& graphIds, vector<vector<graph_type>>& features, vector<vector<unordered_set<int>>>& featureIdxs, vector<int> &searchOutput )
{
    unordered_set<int> candidates;
    candidates.reserve(graphs.size());
    for ( int i = 0; i < graphs.size(); i++ )
    {
        candidates.insert( i );
    }

    prune1(query, graphs, edgeIdxs, candidates);

    if ( candidates.size() > 1000 )
    {
        pruneDiscriminative( query, candidates, features, featureIdxs );
    }

    for ( auto &it : candidates )
    {
        if ( checkSubgraphIsomorphism( query, graphs[it] ) )
        {
            searchOutput.push_back( graphIds[it] );
        }
    }

    return;
}

int main( int argc, char* argv[] )
{

    vector<int> graphIds;
    unordered_map<string, int> labelToInt;
    unordered_map<tuple<int, int, int>, unordered_set<int>, EdgeHash> edgeIdxs;
    vector<vector<graph_type>> features;
    vector<vector<unordered_set<int>>> featureIdxs;
    string graphsFile;
    readIndex( graphsFile, edgeIdxs, features, featureIdxs );

    vector<graph_type> graphs;
    readGraphs( graphsFile, graphs, labelToInt, graphIds );
    cout << "Index loading completed" << "\n";

    string queryFilename, outputFilename;
    outputFilename = "output_CS1190334.txt";
    cout << "Enter query file name :" << "\n";
    cin >> queryFilename;

    auto start = chrono::high_resolution_clock::now();

    ifstream queryFile( queryFilename );
    ofstream outFile( outputFilename );
    string line;
    vector<int> searchOutput;
    while ( getline( queryFile, line ) )
    {
        searchOutput.clear();
        istringstream ss( line );
        char c;
        if ( !( ss >> c ) || c != '#' ) continue;

        graph_type query;
        int graphId;
        ss >> graphId;

        getline( queryFile, line );
        ss.str( line );
        ss.clear();
        int numNodes;
        ss >> numNodes;

        for ( int i = 0; i < numNodes; i++ )
        {
            getline( queryFile, line );

            if ( labelToInt.find( line ) == labelToInt.end() )
            {
                for ( int j = i + 1; j < numNodes; j++ )
                {
                    getline( queryFile, line );
                }

                getline( queryFile, line );
                ss.str( line );
                ss.clear();
                int numEdges;
                ss >> numEdges;

                for ( int j = 0; j < numEdges; j++ )
                {
                    getline( queryFile, line );
                }

                goto output;

            }
            boost::add_vertex( vertex_prop( labelToInt[line] ), query );
        }


        getline( queryFile, line );
        ss.str( line );
        ss.clear();
        int numEdges;
        ss >> numEdges;

        for ( int i = 0; i < numEdges; i++ )
        {
            getline( queryFile, line );
            ss.str( line );
            ss.clear();
            int source, dest;
            string label;
            ss >> source >> dest >> label;

            if ( source > dest )
            {
                int temp = source;
                source = dest;
                dest = temp;
            }

            if ( labelToInt.find( label ) == labelToInt.end() )
            {
                for ( int j = i + 1; j < numEdges; j++ )
                {
                    getline( queryFile, line );
                }

                goto output;
            }
            boost::add_edge( source, dest, edge_prop( labelToInt[label] ), query );
        }


        searchQuery(
            query,
            graphs,
            edgeIdxs,
            graphIds,
            features,
            featureIdxs,
            searchOutput
        );

    output:
        for ( auto& i : searchOutput )
        {
            outFile << i << "\t";
        }
        outFile << "\n";

    }


    queryFile.close();
    outFile.close();
    cout << chrono::duration_cast< chrono::milliseconds >
        ( chrono::high_resolution_clock::now() - start ).count() << "\n";
    
}
