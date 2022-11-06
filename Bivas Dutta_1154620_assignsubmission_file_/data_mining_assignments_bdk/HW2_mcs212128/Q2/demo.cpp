#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <iterator>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;

typedef property<edge_name_t, int> edge_prop;
typedef property<vertex_name_t, int, property<vertex_index_t, int> > vertex_prop;

typedef adjacency_list<vecS, vecS, undirectedS, vertex_prop, edge_prop> Graph;

typedef property_map<Graph, vertex_name_t>::type vertex_name_map_t;
typedef property_map_equivalent<vertex_name_map_t, vertex_name_map_t> vertex_comp_t;

typedef property_map<Graph, edge_name_t>::type edge_name_map_t;
typedef property_map_equivalent<edge_name_map_t, edge_name_map_t> edge_comp_t;
Graph gsmall,glarge;

std::vector<std::string> split(std::string str)
{
    std::vector<std::string> res;
    std::string word = "";
    for (auto x : str)
    {
        if (x == ' ')
        {
            res.push_back(word);
            // cout << word << endl;
            word = "";
        }
        else {
            word = word + x;
        }
    }
    res.push_back(word);
    return res;
}

bool is_subgraph_isomorphic(Graph smallGraph, Graph bigGraph)
{
    vertex_comp_t vertex_comp =
        make_property_map_equivalent(get(vertex_name, smallGraph), get(vertex_name, bigGraph));
    edge_comp_t edge_comp =
        make_property_map_equivalent(get(edge_name, smallGraph), get(edge_name, bigGraph));
    vf2_print_callback<Graph, Graph> callback(smallGraph, bigGraph);
    bool res = vf2_subgraph_iso(smallGraph, bigGraph, callback, vertex_order_by_mult(smallGraph),
        edges_equivalent(edge_comp).vertices_equivalent(vertex_comp));
    return res;
}

int main(int argc,char *argv[])
{  
    std::fstream f(argv[1]);
    std::vector<std::string> token;
    int i=0;
    int n=0;    
    while (f)
    {
        std::string line;
        if(i==0)
        { 
            getline(f,line);
            i++;
            std::stringstream geek(line);
            geek>>n;
            // n=> # of vertices
            // std::cout<<line<<' '<<n<<std::endl;
            while (n)
            {
                n--;
                getline(f,line);
                token=split(line) ;
                add_vertex(vertex_prop(stoi(token[1])),gsmall);
            }  
        }
        else{
            getline(f,line);
            std::stringstream geek(line);
            geek>>n;
            while(n)
            {
                n--;
                getline(f,line);
                token=split(line);
                // std::cout<<token[0]<<' '<<token[1]<<' '<<token[2]<<std::endl;
                add_edge(stoi(token[0]), stoi(token[1]), edge_prop(stoi(token[2])),gsmall);
            }
        }
    }
    // std::cout<<"done gsmall"<<std::endl;

    std::fstream g(argv[2]);
    i=0;
    n=0;
    while (g)
    {
        std::string line;
        if(i==0)
        { 
            getline(g,line);
            i++;
            std::stringstream geek(line);
            geek>>n;
            // n=> # of vertices
            // std::cout<<line<<' '<<n<<std::endl;
            while (n)
            {
                n--;
                getline(g,line);
                token=split(line) ;
                add_vertex(vertex_prop(stoi(token[1])),glarge);
            }  
        }
        else{
            getline(g,line);
            std::stringstream geek(line);
            geek>>n;
            while(n)
            {
                n--;
                getline(g,line);
                token=split(line);
                // std::cout<<token[0]<<' '<<token[1]<<' '<<token[2]<<std::endl;
                add_edge(stoi(token[0]), stoi(token[1]), edge_prop(stoi(token[2])),glarge);
            }
        }
    }
    std::cout<<' ';
    if (is_subgraph_isomorphic(gsmall,glarge)){
        std::cout<<"done"<<std::endl;
    }
    else{
        std::cout<<"not"<<std::endl;
    }

}