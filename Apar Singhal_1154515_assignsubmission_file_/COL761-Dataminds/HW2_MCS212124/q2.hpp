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
#include <chrono>
#include <algorithm>
#include <cstdio>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std::chrono;
using namespace std;
using namespace boost;

template<typename Graph1, typename Graph2> struct vf3_callback
{
    vf3_callback(Graph1 g, Graph2 h){}
    template<typename CorresponMapOneToTwo, typename CorresponMapTwoToOne>
    bool operator()(CorresponMapOneToTwo h, CorresponMapTwoToOne g) const
    {
        return false;
    }
};

typedef property<vertex_name_t, int, property<vertex_name_t, int>> propNode;
typedef property<edge_name_t, int> propEdge;
typedef adjacency_list<vecS, vecS, undirectedS, propNode, propEdge> graphType;
typedef property_map<graphType, edge_name_t>::type edgeNameMapT;
typedef property_map_equivalent<edgeNameMapT, edgeNameMapT> edgeCompT;
typedef property_map<graphType, vertex_name_t>::type vertexNameMapT;
typedef property_map_equivalent<vertexNameMapT, vertexNameMapT> vertexCompT;


