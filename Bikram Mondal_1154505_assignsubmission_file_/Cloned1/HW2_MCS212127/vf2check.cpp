#include <iostream>
#include <fstream>
#include <string>

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

inline std::string get_inp(std::string s) {
    if ((s[s.length() - 1] > 47 && s[s.length() - 1] < 58) || (s[s.length() - 1] > 64 && s[s.length() - 1] < 91) ||
        (s[s.length() - 1] > 96 && s[s.length() - 1] < 123)) {
        return s;
    }
    return s.substr(0, s.length() - 1);
}

void splitedge(std::string str, int val[])
{
    std::string word = "";
    int i = 0;
    for (auto x : str)
    {
        if (x == ' ')
        {
            val[i++] = stoi(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    val[i++] = stoi(word);
}

std::unordered_map<std::string,int> m;
int z=1;

int main()
{
    std::string queryfile;
    std::fstream qn("queryfile_BHK.txt");
    getline(qn,queryfile);
    qn.close();
    std::fstream q(queryfile);
    std::ofstream out("output_MCS212127.txt");
    int querycount =1;
    while (q) {
        Graph gsmall;
        std::string gid;
        getline(q, gid);
        gid = get_inp(gid);
        if (gid.length() < 1) {
            continue;
        }
        std::string t;
        getline(q,t);
        t = get_inp(t);
        int v= stoi(t);
        for(int i=0;i<v;i++){
            getline(q,t);
            t = get_inp(t);
            if(m.find(t)==m.end()){
                m[t]=z++;
            }
            add_vertex(vertex_prop(m[t]), gsmall);
        }
        getline(q,t);
        //t= get_inp(t);
        int e = stoi(t);
        for(int i=0;i<e;i++){
            getline(q,t);
            t= get_inp(t);
            int val[3]={0};
            splitedge(t,val);
            add_edge(val[1], val[0], edge_prop(val[2]), gsmall);
        }
        std::string name = "q"+std::to_string(querycount)+".txt";
        querycount++;
        std::fstream freq(name);
        while(freq){
            Graph glarge;
            //string gid;
            getline(freq, gid);
            gid = get_inp(gid);
            if (gid.length() < 1) {
                continue;
            }
            //string t;
            getline(freq,t);
            t = get_inp(t);
            v= stoi(t);
            for(int i=0;i<v;i++){
                getline(freq,t);
                t = get_inp(t);
                if(m.find(t)==m.end()){
                    m[t]=z++;
                }
                add_vertex(vertex_prop(m[t]), glarge);
            }
            getline(freq,t);
            t= get_inp(t);
            e = stoi(t);
            for(int i=0;i<e;i++){
                getline(freq,t);
                t= get_inp(t);
                int val[3]={0};
                splitedge(t,val);
                add_edge(val[1], val[0], edge_prop(val[2]), glarge);
            }
            if(is_subgraph_isomorphic(gsmall, glarge)){
                out <<gid<<'\t';
            }
        }
        out<<'\n';
    }
        
}