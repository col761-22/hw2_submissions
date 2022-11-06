#include <fstream>
#include <sstream>
#include <iostream>
#include "graph-utils.hpp"
using namespace std;

#include <boost/functional/hash.hpp>
std::size_t EdgeHash::operator()( const std::tuple<int, int, int>& edge ) const
{
    return boost::hash_value( edge );
};

template<typename Graph1, typename Graph2>
vf2_callback<Graph1, Graph2>::vf2_callback( Graph1 f, Graph2 g ) {};

template <typename Graph1, typename Graph2>
template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
bool vf2_callback<Graph1, Graph2>::operator()( CorrespondenceMap1To2 f, CorrespondenceMap2To1 g ) const
{
    return false;
};

void readGraphs( string filename,
    vector<graph_type>& graphs,
    unordered_map<string, int>& labelToInt,
    vector<int>& graphIds )
{
    ifstream inFile( filename );
    int labelInt = 1; // integer to assign to next label 
    string line;
    while ( getline( inFile, line ) )
    {
        istringstream ss( line );
        char c;
        if ( !( ss >> c ) || c != '#' ) continue;

        graph_type G;
        // read graph id
        int graphId;
        ss >> graphId;
        graphIds.push_back( graphId );
        // cout << "#" << graphId << "\n";

        // read number of nodes
        getline( inFile, line );
        ss.str( line );
        ss.clear();
        int numNodes;
        ss >> numNodes;
        // cout << numNodes << "\n";

        // read node labels
        for ( int i = 0; i < numNodes; i++ )
        {
            getline( inFile, line );
            if ( labelToInt.find( line ) == labelToInt.end() ) labelToInt[line] = labelInt++;
            boost::add_vertex( vertex_prop( labelToInt[line] ), G );
            // cout << line << "\n";
        }

        // read number of edges
        getline( inFile, line );
        ss.str( line );
        ss.clear();
        int numEdges;
        ss >> numEdges;
        // cout << numEdges << "\n";

        // read edges
        for ( int i = 0; i < numEdges; i++ )
        {
            getline( inFile, line );
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
            if ( labelToInt.find( label ) == labelToInt.end() ) labelToInt[label] = labelInt++;
            boost::add_edge( source, dest, edge_prop( labelToInt[label] ), G );
            // cout << source << " " << dest << " " << label << "\n";
        }

        graphs.push_back( G ); // [TODO]: emplace_back or push_back?
    }
};

void readFrequentGraphs( string filename,
    vector<vector<graph_type>>& graphs,
    vector<vector<unordered_set<int>>>& graphIdxs
)
{
    ifstream inFile( filename );
    int maxSize = graphs.size();
    string line;
    while ( getline( inFile, line ) )
    {
        istringstream ss( line );
        char c;
        if ( !( ss >> c ) || c != 't' ) continue;

        graph_type G;

        string temp; 
        int tid;
        int support;
        ss >> temp;
        ss >> tid;
        ss >> temp;
        ss >> support;

        int source, dest, label;
        while ( getline( inFile, line ) )
        {
            istringstream ss1( line );
            if ( !( ss1 >> c ) ) continue;
            if ( c == 'v' )
            {
                ss1 >> source >> label;
                boost::add_vertex( vertex_prop( label ), G );
            }
            if ( c == 'e' )
            {
                ss1 >> source >> dest >> label;
                if ( source > dest )
                {
                    int temp = source;
                    source = dest;
                    dest = temp;
                }
                boost::add_edge( source, dest, edge_prop( label ), G );
            }
            if ( c == 'x' )
            {
                unordered_set<int> ids;
                while ( ss1 >> label )
                {
                    ids.insert( label );
                }
                int numEdges = boost::num_edges( G );
                if ( numEdges < maxSize ) // [TODO] : < or <=?
                {
                    graphs[numEdges].push_back( G );
                    graphIdxs[numEdges].push_back( ids );
                }
                break;
            }
        }
    }
    return;
}

void getIntersection( unordered_set<int>& a, unordered_set<int>& b )
{
    vector<int> excluded;
    for ( auto i : a )
    {
        if ( b.find( i ) == b.end() )
        {
            excluded.push_back( i );
        }
    }
    for ( int i = 0; i < excluded.size(); i++ )
    {
        a.erase( excluded[i] );
    }
}

bool checkSubgraphIsomorphism( graph_type G1, graph_type G2 )
{
    vertex_comp_t vertices = boost::make_property_map_equivalent( get( boost::vertex_name, G1 ), get( boost::vertex_name, G2 ) );
    edge_comp_t edges = boost::make_property_map_equivalent( get( boost::edge_name, G1 ), get( boost::edge_name, G2 ) );

    vf2_callback<graph_type, graph_type> callback( G1, G2 );
    return boost::vf2_subgraph_mono( G1, G2, callback, boost::vertex_order_by_mult( G1 ), boost::edges_equivalent( edges ).vertices_equivalent( vertices ) );

}

float getGamma(
    graph_type& g,
    unordered_set<int>& frequentIdxs,
    vector<vector<graph_type>>& features,
    vector<vector<std::unordered_set<int>>>& featureIdxs )
{

    int numEdges = boost::num_edges( g );
    unordered_set<int> candidates;
    for ( int i = 1; i <= numEdges; i++ )
    {
        for ( int j = 0; j < features[i].size(); j++ )
        {
            if ( checkSubgraphIsomorphism( features[i][j], g ) )
            {
                if ( candidates.size() == 0 )
                {
                    candidates = featureIdxs[i][j];
                }
                else
                {
                    getIntersection( candidates, featureIdxs[i][j] );
                }
            }
        }
    }
    return candidates.size() * 1.00 / frequentIdxs.size();

}