#include <bits/stdc++.h>
#include <sys/stat.h>
#include <ctime>

#include "vf2lib/include/argraph.h"
#include "vf2lib/include/argedit.h"
#include "vf2lib/include/vf2_sub_state.h"
#include "vf2lib/include/match.h"
#include "vf2lib/include/argloader.h"
#include "vf2lib/include/allocpool.h"

#define MAXNODES 70000
// TODO: set max nodes
// ANS: in vf2, max nodes in graph can be 65535
node_id ni1[MAXNODES];
node_id ni2[MAXNODES];

using namespace std;

bool isContained(string feature_vector, string graph_feature) {
    int n = feature_vector.size();
    for(int i=0; i<n; i++) {
        if(feature_vector[i] == '1' && graph_feature[i] != '1') {
            return false;
        }
    }
    return true;
}

void graphs_to_vf3_format(string input_file, string label_file, string query_gIDs_output) {
	// input_file is query_path
	// label_file is label_mapping_file_path
	// query_gIDs_output is query_gIDs_output_path

	// read label file
	unordered_map<string, int> label_map;
	ifstream label_file_in(label_file);
	string line;
	int max_label_id = 0;
	while(getline(label_file_in, line)) {
		stringstream ss(line);
		string label;
		int label_id;
		ss >> label >> label_id;
		label_map[label] = label_id;
		max_label_id = max(max_label_id, label_id);
	}
	label_file_in.close();

	// read input file
	ifstream input_file_in(input_file);
	if(input_file_in.fail()) {
        cout << "Error: Query file not found.\n";
		return;
    }
	ofstream query_gIDs_output_out(query_gIDs_output);
	// make directory for query graphs
	string query_graphs_dir = "query_graphs";
	if(mkdir(query_graphs_dir.c_str(), 0777) == -1) {
		cout << "Directory " << query_graphs_dir << " already exists" << "\n";
	}
	else {
		cout << "Directory " << query_graphs_dir << " created" << "\n";
	}
	string query_gID;
	while(getline(input_file_in, line)) {
		// check if empty line
		if(line.empty()) continue;
		// check if first character is #
		if(line[0] == '#'){
			// read query graph ID
			query_gID = line.substr(1);
			query_gIDs_output_out << query_gID << "\n";
		}
		else continue;
		// create query graph file
		string query_graph_file = query_graphs_dir + "/" + query_gID + ".txt";
		ofstream query_graph_file_out(query_graph_file);

		int num_vertices = 0;
		int num_edges = 0;
		// read number of vertices
		getline(input_file_in, line);
		stringstream ss(line);
		ss >> num_vertices;
		query_graph_file_out << num_vertices << "\n";
		for(int vertex_id = 0; vertex_id<num_vertices; vertex_id++)
		{
			// read vertex
			getline(input_file_in, line);
			stringstream ss(line);
			string label;
			ss >> label;
			// output vertex
			if(label_map.find(label) == label_map.end())
			{
				label_map[label] = max_label_id + 1;
				max_label_id++;
			}
			query_graph_file_out << vertex_id << " " << label_map[label] << "\n";
		}
		// read number of edges
		getline(input_file_in, line);
		stringstream sse(line);
		sse >> num_edges;
		vector<vector<pair<int, int>>> adj_list(num_vertices);
		for(int edge_id = 0; edge_id<num_edges; edge_id++)
		{
			// read edge
			getline(input_file_in, line);
			stringstream sss(line);
			int vertex_id1, vertex_id2, edge_label;
			sss >> vertex_id1 >> vertex_id2 >> edge_label;
			// output edge
			adj_list[vertex_id1].push_back(make_pair(vertex_id2, edge_label));
			adj_list[vertex_id2].push_back(make_pair(vertex_id1, edge_label));
		}
		// output edges
		for(int vertex_id = 0; vertex_id<num_vertices; vertex_id++)
		{
			query_graph_file_out << adj_list[vertex_id].size() << "\n";
			for(auto edge: adj_list[vertex_id])
			{
				query_graph_file_out << vertex_id << " " << edge.first << " " << edge.second << "\n";
			}
		}
		query_graph_file_out.close();			
	}
	input_file_in.close();
	query_gIDs_output_out.close();
}

class Node {
public:
	int label;
};

// class NodeDestroyer: public AttrDestroyer {
// public:
// 	virtual void destroy(void *p) {
// 		delete p;
// 	}
// };

class NodeComparator: public AttrComparator {
public:
	NodeComparator() {}
	virtual bool compatible(void *na, void *nb) {
		Node *a = (Node *)na;
		Node *b = (Node *)nb;
		return a->label == b->label;
	}
};

istream& operator>>(istream& in, Node &n) {
	in >> n.label;
	return in;
}

ostream& operator<<(ostream& out, Node &n) {
	out << n.label;
	return out;
}

class Edge {
public:
	int label;
};

// class EdgeDestroyer: public AttrDestroyer {
// public:
// 	virtual void destroy(void *p) {
// 		delete p;
// 	}
// };

class EdgeComparator: public AttrComparator {
public:
	EdgeComparator() {}
	virtual bool compatible(void *ea, void *eb) {
		Edge *a = (Edge *)ea;
		Edge *b = (Edge *)eb;
		return a->label == b->label;
	}
};

istream& operator>>(istream& in, Edge &e) {
	in >> e.label;
	return in;
}

ostream& operator<<(ostream& out, Edge &e) {
	out << e.label;
	return out;
}

bool subgraphIsomorphism(string query_graph_file, string graph_file) {
	// cout << "checking subgraph isomorphism " << query_graph_file << " " << graph_file << '\n';

	NewAllocator<Node> node_allocator1;
	NewAllocator<Edge> edge_allocator1;

	NewAllocator<Node> node_allocator2;
	NewAllocator<Edge> edge_allocator2;

	// ifstream in1("vf3lib/test/labeled.sub.grf");
	// ifstream in2("vf3lib/test/labeled.grf");
	ifstream in1(query_graph_file);
	ifstream in2(graph_file);

	StreamARGLoader<Node, Edge> loader1(&node_allocator1, &edge_allocator1, in1);
	StreamARGLoader<Node, Edge> loader2(&node_allocator2, &edge_allocator2, in2);

	// construct the graphs
	ARGraph<Node, Edge> query_graph(&loader1);
	ARGraph<Node, Edge> graph(&loader2);

	// TODO: do we need to set destroyers? was giving error in compilation.
	// query_graph.SetNodeDestroyer(new NodeDestroyer());
	// graph.SetNodeDestroyer(new NodeDestroyer());
	// query_graph.SetEdgeDestroyer(new EdgeDestroyer());
	// graph.SetEdgeDestroyer(new EdgeDestroyer());

	query_graph.SetNodeComparator(new NodeComparator());
	query_graph.SetEdgeComparator(new EdgeComparator());

	// create initial state of search space
	VF2SubState s0(&query_graph, &graph);

	int n;

	if(match(&s0, &n, ni1, ni2)) {
		// cout << "Matching found\n";
		return true;
	}
	else {
		// cout << "No matching found\n";
		return false;
	}
}

int main(int argc, char* argv[])
{
    cout << "Enter path to query graph: ";
    string query_path;
    cin >> query_path;
	// query_path = "sample_query.txt";

	time_t start_time = time(NULL);

	graphs_to_vf3_format(query_path, "label_mapping.txt", "query_graph_ids.txt");

	ifstream query_graph_ids_file("query_graph_ids.txt");
	vector<int> query_graph_ids;
	string line;
	while(getline(query_graph_ids_file, line)) {
		int qg_id = stoi(line);
		query_graph_ids.push_back(qg_id);
	}
	query_graph_ids_file.close();

	ifstream selected_patterns_file("selected_patterns.txt");
	vector<int> selected_patterns;
	while(getline(selected_patterns_file, line)) {
		int sp_id = stoi(line);
		selected_patterns.push_back(sp_id);
	}
	selected_patterns_file.close();

	ifstream feature_vectors_file("feature_vectors.txt");
	unordered_map<int, string> feature_vectors_map;
	while(getline(feature_vectors_file, line)) {
		stringstream ss(line);
		int g_id;
		string fv;
		ss >> g_id >> fv;
		feature_vectors_map[g_id] = fv;
	}
	feature_vectors_file.close();

	ofstream output("output.txt");

	string query_graph_folder = "query_graphs/";
	string pattern_folder = "patterns/";
	string graphs_folder = "graphs/";
	string txt = ".txt";

	// cout << query_graph_ids.size() << '\n';
	// cout << selected_patterns.size() << '\n';

	for(int i=0; i<query_graph_ids.size(); i++) {
		time_t s1 = time(NULL);
		int query_graph_id = query_graph_ids[i];
		cout << "Query Graph ID " << query_graph_id << '\n';
		string query_graph_file_path = query_graph_folder + to_string(query_graph_id) + txt;
		string feature_vector = "";
		for(int j=0; j<selected_patterns.size(); j++) {
			int selected_pattern_id = selected_patterns[j];
			string selected_patten_graph_path = pattern_folder + to_string(selected_pattern_id) + txt;
			if(subgraphIsomorphism(selected_patten_graph_path, query_graph_file_path)) {
				feature_vector.append("1");
			}
			else {
				feature_vector.append("0");
			}
		}
		// cout << feature_vector << '\n';
		for(auto it=feature_vectors_map.begin(); it!=feature_vectors_map.end(); it++) {
			string graph_feature = it->second;
			string graph_id = to_string(it->first);
			if(isContained(feature_vector, graph_feature)) {
				string graph_path = graphs_folder + graph_id + txt;
				// cout << graph_path << '\n';
				if(subgraphIsomorphism(query_graph_file_path, graph_path)) {
					output << graph_id << '\t';
				}
			}
		}
		output << '\n';
		time_t e1 = time(NULL);
		cout << "Time Taken in Query Graph "<< i << ": " << difftime(e1, s1) << " seconds" << '\n';
	}

	output.close();

	time_t end_time = time(NULL);
	cout << "\nTotal Time Taken: " << difftime(end_time, start_time) << " seconds" << '\n';

    return 0;
}