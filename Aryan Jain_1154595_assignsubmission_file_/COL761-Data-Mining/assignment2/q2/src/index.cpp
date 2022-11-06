#include <iostream>
#include <unordered_set>
#include <fstream>
#include <chrono>
#include <sstream>
#include "graph-utils.hpp"

using namespace std;

void writeIndex(
    string graphsFilename,
    unordered_map<tuple<int, int, int>, unordered_set<int>, EdgeHash>& edgeIdxs,
    vector<vector<graph_type>>& features,
    vector<vector<unordered_set<int>>>& featureIdxs
)
{
    ofstream edgeIdxsFile( "edge-idxs.txt" );
    ofstream featuresFile( "features.txt" );
    ofstream featureIdxsFile( "feature-idxs.txt" );

    // EDGE IDXS
    edgeIdxsFile << graphsFilename << "\n";
    edgeIdxsFile << edgeIdxs.size() << "\n";
    for ( auto& it : edgeIdxs )
    {
        const tuple<int, int, int>& edge = it.first;
        const unordered_set<int>& idxs = it.second;
        edgeIdxsFile << "#" << get<0>( edge ) << " " << get<1>( edge ) << " " << get<2>( edge ) << " " << idxs.size() << "\n";
        for ( auto& i : idxs ) edgeIdxsFile << i << "\n";
    }
    // FEATURES
    featuresFile << features.size() << "\n";
    for ( auto& it : features )
    {
        featuresFile << "$$\n" << it.size() << "\n";
        for ( auto& G : it )
        {
            featuresFile << "#" << boost::num_vertices( G ) << " " << boost::num_edges( G ) << "\n";
            auto vs = boost::vertices( G );
            for ( auto vit = vs.first; vit != vs.second; vit++ )
            {
                int label = boost::get( boost::vertex_name, G )[*vit];
                featuresFile << label << "\n";
            }
            auto es = boost::edges( G );
            for ( auto eit = es.first; eit != es.second; eit++ )
            {
                int label = boost::get( boost::edge_name, G )[*eit];
                int source = boost::get( boost::vertex_name, G )[boost::source( *eit, G )];
                int dest = boost::get( boost::vertex_name, G )[boost::target( *eit, G )];
                featuresFile << source << " " << dest << " " << label << "\n";
            }
        }
    }
    // FEATURE IDXS
    featureIdxsFile << featureIdxs.size() << "\n";
    for ( auto& it : featureIdxs )
    {
        featureIdxsFile << "$$\n" << it.size() << "\n";
        for ( auto& S : it )
        {
            featureIdxsFile << "#" << S.size() << "\n";
            for ( auto& x : S ) featureIdxsFile << x << "\n";
        }
    }
};

int main( int argc, char* argv[] )
{
    string graphsFile = argv[1];

    // READ GRAPHS, CONVERT LABELS TO INTS, STORE GRAPH IDS
    vector<graph_type> graphs;
    unordered_map<string, int> labelToInt;
    vector<int> graphIds;
    readGraphs( graphsFile,
        graphs,
        labelToInt, // [TODO]: Should we store intToLabel instead?
        graphIds );

    unordered_map<tuple<int, int, int>, unordered_set<int>, EdgeHash> edgeIdxs;

    // [param] [TODO]:tune
    int maxSize = 55; // maximum size feature graphs to consider
    vector<vector<graph_type>> features( maxSize + 1 );
    vector<vector<unordered_set<int>>> featureIdxs( maxSize + 1 );

    int graphIdx = 0;
    for ( auto& G : graphs )
    {
        auto es = boost::edges( G );
        for ( auto eit = es.first; eit != es.second; eit++ )
        {
            int label = boost::get( boost::edge_name, G )[*eit];
            int source = boost::get( boost::vertex_name, G )[boost::source( *eit, G )];
            int dest = boost::get( boost::vertex_name, G )[boost::target( *eit, G )];
            tuple<int, int, int> edge = make_tuple( source, dest, label );

            if ( edgeIdxs.find( edge ) == edgeIdxs.end() )
            {
                edgeIdxs[edge] = unordered_set<int>( { graphIdx } );
                graph_type edgeGraph;
                graph_type::vertex_descriptor v1 = boost::add_vertex(
                    vertex_prop( source ), edgeGraph );
                graph_type::vertex_descriptor v2 = boost::add_vertex(
                    vertex_prop( dest ), edgeGraph );
                boost::add_edge( v1, v2, edge_prop( label ), edgeGraph );
                features[1].emplace_back( edgeGraph );
            }
            else edgeIdxs[edge].insert( graphIdx );
        }
        graphIdx++;
    }

    for ( auto& G : features[1] )
    {
        auto es = boost::edges( G );
        for ( auto eit = es.first; eit != es.second; eit++ )
        {
            int label = boost::get( boost::edge_name, G )[*eit];
            int source = boost::get( boost::vertex_name, G )[boost::source( *eit, G )];
            int dest = boost::get( boost::vertex_name, G )[boost::target( *eit, G )];
            tuple<int, int, int> edge = make_tuple( source, dest, label );
            // [TODO] : emplace_back or push_back
            featureIdxs[1].push_back( edgeIdxs[edge] );
        }
    }

    string frequentGraphsFile = argv[2];
    vector<vector<graph_type>> frequentGraphs(maxSize+1);
    vector<vector<unordered_set<int>>> frequentGraphsIdxs(maxSize+1);

    readFrequentGraphs( frequentGraphsFile, frequentGraphs, frequentGraphsIdxs );

    int count = 0;
    float gammaThresh = 1.05; // [TODO] : param tune
    if ( graphs.size() <= 50000 )
    {
        gammaThresh = 1.1;
    }
    if ( graphs.size() <= 40000 )
    {
        gammaThresh = 1.2;
    }
    if ( graphs.size() < 30000 )
    {
        gammaThresh = 1.3;
    }

    for ( int i = 2; i <= maxSize; i++ )
    {
        for ( int j = 0; j < frequentGraphs[i].size(); j++ )
        {
            graph_type g = frequentGraphs[i][j];

            if ( count <= 500 ) // [TODO] : param tune
            {
                float gamma = getGamma( g, frequentGraphsIdxs[i][j], features, featureIdxs );

                if ( gammaThresh <= gamma ) // [TODO] : < or <=?
                {
                    count++;
                    features[i].push_back( g );
                    featureIdxs[i].push_back( frequentGraphsIdxs[i][j] );
                }
            }
        }
    }

    // SAVE INDEX TO DISK
    writeIndex(
        graphsFile,
        edgeIdxs,
        features,
        featureIdxs
    );
    cerr << "Index Creation Completed" << "\n";
};