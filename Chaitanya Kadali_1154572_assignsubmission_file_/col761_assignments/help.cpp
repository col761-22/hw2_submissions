#include<iostream>
#include<bits/stdc++.h>
#include<chrono>
#include<boost/unordered_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>


using namespace boost;
using namespace std;
using namespace std::chrono;

typedef property< edge_name_t, int > edgeii_property;
typedef property< vertex_name_t, string, property< vertex_index_t, int > > vertexii_property;           
typedef adjacency_list< setS, vecS, undirectedS, vertexii_property,edgeii_property> graph_typ; 
typedef property_map< graph_typ, vertex_name_t >::type vertex_name_map_t;
typedef property_map_equivalent< vertex_name_map_t, vertex_name_map_t >vertex_comp_t; 
typedef property_map< graph_typ, edge_name_t >::type edge_name_map_t;
typedef property_map_equivalent< edge_name_map_t, edge_name_map_t >edge_comp_t;



template <typename Graph1, typename Graph2> class my_call_back {
public:
    my_call_back(const Graph1& graph1, const Graph2& graph2) : graph1_(graph1), graph2_(graph2) {}

    template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
    bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) {
        return true;
    }
    std::vector <std::vector <std::pair<int, int>>> get_setvmap() { return set_of_vertex_iso_map; }

private:
    const Graph1& graph1_;
    const Graph2& graph2_;
    std::vector <std::vector <std::pair<int, int>>> set_of_vertex_iso_map;
    std::vector <std::pair<int, int>> vertex_iso_map;   
};

bool are_they_isomorphic(graph_typ &graph1, graph_typ &graph2){
    vertex_comp_t vertex_comp = make_property_map_equivalent(get(vertex_name, graph1), get(vertex_name, graph2));

    edge_comp_t edge_comp = make_property_map_equivalent(get(edge_name, graph1), get(edge_name, graph2));
    my_call_back< graph_typ, graph_typ > callback(graph1, graph2);
    bool actual = vf2_subgraph_iso(graph1, graph2, callback, vertex_order_by_mult(graph1),
                edges_equivalent(edge_comp).vertices_equivalent(vertex_comp));

    return actual;
}

 

int main(int argc, char *argv[]){
    

    /*--------------------------------------------------------------------------------------------------*/

    /* Reading the vertex labels file and storing it in labels array. Labels are sorted in lexicographical order. */
    vector<string> labels;
    ifstream vlabel("Dump/vertex_labels.txt");                   
    string line = "";                                   
    while(getline(vlabel, line)){
        //cout << line << "\n";
        labels.push_back(line);
    }
    vlabel.close();
    /*---------------------------------------------------------------------------------------*/

    /* Reading the Graph_ids file and storing it in graph_id array */
    ifstream gid("Dump/graph_ids.txt");
    line = "";
    vector<pair<int, int>> graph_id;
    while(getline(gid,line)){
        stringstream ss(line);
        pair<int, int> temp;
        ss >> temp.first;
        ss >> temp.second;
        //cout << temp.first << " " << temp.second << "\n";
        graph_id.push_back(temp);
    }
    gid.close();

    /*---------------------------------------------------------------------------------------*/

    /* Reading the index structure and storing it in a data structure */
    vector<graph_typ> index_graphs;
    vector<set<int>> index_lists;
    ifstream file("Dump/index.txt.fp");
	if(!file.is_open()){
		cout << "Please build the index structures first" << "\n";
		return 0;
	}
    line = "";
    while(getline(file, line)){
        int finallygod = index_graphs.size();
        if(line.size() == 0){
            continue;
        }
        else if(line[0] == 't'){
            int s = index_graphs.size();
            index_graphs.resize(s+1);
        }
        else if(line[0] == 'v'){
            stringstream ss(line);
            int vert;
            int label;
            string dump;
            ss >> dump;
            ss >> vert;
            ss >> label;
            add_vertex(vertexii_property(labels[label]), index_graphs[finallygod-1]);
        }
        else if(line[0] == 'e'){
            stringstream ss(line);
            string dump;
            int start;
            int end;
            int label;
            ss >> dump;
            ss >> start;
            ss >> end;
            ss >> label;
            add_edge(start, end, edgeii_property(label), index_graphs[finallygod-1]);
        }
        else if(line[0] == 'x'){
            stringstream ss(line);
            set<int> useful;
            string dump;
            ss >> dump;
            int temp;
            while(ss >> temp){useful.insert(temp);}
            index_lists.push_back(useful); 
        }
    }
    file.close();

    /*--------------------------------------------------------------------------*/

    /*Graphs database....................................*/
    vector<graph_typ> database(graph_id.size());
    ifstream datab("Dump/index.txt");
    string lin = "";
    int ind = -1;
    while(getline(datab, lin)){
        if(lin[0] == 't'){
            ind += 1;
            continue;
        }
        else if(lin[0] == 'v'){
            stringstream ss(lin);
            string dump;
            ss >> dump;
            int vert;
            int label;
            ss >> vert;
            ss >> label;
            add_vertex(vertexii_property(labels[label]), database[ind]);
        }

        else if(lin[0] == 'e'){
            stringstream ss(lin);
            string dump;
            int left;
            int right;
            int label;
            ss >> dump;
            ss >> left;
            ss >> right;
            ss >> label;
            add_edge(left, right, edgeii_property(label), database[ind]);
        }

    }
    datab.close();


    set<int> dummY;
    for(int i= 0; i< graph_id.size(); i++){
        dummY.insert(i);
    }

    //string present_query = argv[1];
    string present_query;
    cout << "Please provide the path to your query file: ";
    cin >> present_query;
    auto start = high_resolution_clock::now();
    ifstream quer(present_query);
    ofstream final_out("output_CS5190435.txt");
    line = "";
    while(getline(quer,line) && line[0] == '#'){
        graph_typ query;

        getline(quer, line);
        int vertices = stoi(line);
        //cout << "vertices: " << vertices << "\n";
        for(int j= 0; j< vertices; j++){
            getline(quer,line);
            add_vertex(vertexii_property(line),query);
        }

        getline(quer, line);
        int edges = stoi(line);

        //cout << "edges " << edges << "\n";
        for(int j= 0; j< edges; j++){
            getline(quer,line);
            stringstream ss(line);
            int left;
            int right;
            int label;
            ss >> left;
            ss>> right;
            ss >> label;
            add_edge(left, right, edgeii_property(label), query);
        }

        set<int> candidates = dummY;

        for(int j= 0; j< index_graphs.size(); j++){
            graph_typ curr = index_graphs[j];

            if(are_they_isomorphic(curr,query)){
                set<int> curr_list = index_lists[j];
                set<int> useful;

                set_intersection(candidates.begin(),candidates.end(),curr_list.begin(),curr_list.end(),
                    std::inserter(useful,useful.begin()));

                candidates = useful;
            }

        }

        for(auto lt = candidates.begin(); lt != candidates.end(); lt++){
            int curr_pos = *(lt);
            graph_typ present = database[curr_pos];
 
            if(are_they_isomorphic(query,present) ){
                final_out << graph_id[curr_pos].second << "\t";
            }

        }
        final_out << "\n";
        //graphfile.close();
        if(!getline(quer,line)){break;}
    }

    final_out.close();
    quer.close();

    auto end = high_resolution_clock::now();

    cout << "querying time: "<< duration_cast<microseconds>(end-start).count()*0.001 << " milli sec" <<"\n";

    return 0;

}
