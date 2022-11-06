// Some code borrowed from Boost Graph Library Subgraph Isomorphism Example
// https://www.boost.org/doc/libs/1_80_0/libs/graph/example/vf2_sub_graph_iso_multi_example.cpp

#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/vf2_sub_graph_iso.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
#include <unordered_set>

struct EdgeHash {
    std::size_t operator()(const std::tuple<int,int,int> & edge) const;
};

typedef boost::property< boost::edge_name_t, int > edge_prop;
typedef boost::property< boost::vertex_name_t, int, boost::property< boost::vertex_index_t, int > > vertex_prop;
// Using a vecS graphs => the index maps are implicit.
typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, vertex_prop, edge_prop> graph_type;
// create predicates
typedef boost::property_map< graph_type, boost::vertex_name_t >::type vertex_name_map_t;
typedef boost::property_map_equivalent< vertex_name_map_t, vertex_name_map_t > vertex_comp_t;
typedef boost::property_map< graph_type, boost::edge_name_t >::type edge_name_map_t;
typedef boost::property_map_equivalent< edge_name_map_t, edge_name_map_t > edge_comp_t;

template<typename Graph1, typename Graph2> struct vf2_callback 
{
    vf2_callback(Graph1 f, Graph2 g);
    template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
    bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1 g) const;
};

bool checkSubgraphIsomorphism(graph_type G1, graph_type G2);

// read graphs from filename, store them in graphs.
// labelToInt - store mapping of string labels to ints (node and edge properties are ints)
// graphIds[n] = Id of n-th graph in the graphs file
void readGraphs(std::string filename,
                std::vector<graph_type>& graphs,
                std::unordered_map<std::string,int>& labelToInt,
                std::vector<int>& graphIds );

void getIntersection( std::unordered_set<int>& a, std::unordered_set<int>& b );

void readFrequentGraphs(
    std::string filename,
    std::vector<std::vector<graph_type>>& graphs,
    std::vector<std::vector<std::unordered_set<int>>>& graphIds
);

float getGamma(
    graph_type& g,
    std::unordered_set<int>& frequentIdxs,
    std::vector<std::vector<graph_type>>& features,
    std::vector<std::vector<std::unordered_set<int>>>& featureIdxs );
#endif