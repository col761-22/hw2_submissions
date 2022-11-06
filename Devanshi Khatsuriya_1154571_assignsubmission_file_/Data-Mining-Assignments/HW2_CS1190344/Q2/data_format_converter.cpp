#include <bits/stdc++.h>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2) {
        cout << "Usage: ./data_format_converter <path_to_yeast_input_file>" << endl;
        return 0;
    }
    string yeast_input_file = argv[1];
    unordered_map<string, int> vertex_label_to_num_map;
    int label = 1; // we start assigning labels from 1

    // open input and output files
    ifstream input(yeast_input_file);
    ofstream output1("data_for_gspan.txt");
    ofstream mapping("label_mapping.txt");

    int max_num_vertices = 0;
    
    string line;
    long long num_graphs = 0;
    while(getline(input, line)) {
        if(line[0] == '#') {
            num_graphs++;
            string graph_id = line.substr(1);
            output1 << "t # " << graph_id << endl;
            getline(input, line);
            int num_vertices = stoi(line);
            max_num_vertices = max(max_num_vertices, num_vertices);
            for(int vertex_id=0; vertex_id<num_vertices; vertex_id++) {
                getline(input, line);
                string vertex_label = line;
                if(vertex_label_to_num_map[vertex_label]==0) {
                    // cout << vertex_label << " " << label << '\n';
                    vertex_label_to_num_map[vertex_label] = label++;
                    // write the mapping to label_mapping.json
                    mapping << vertex_label << " " << vertex_label_to_num_map[vertex_label] << '\n';
                }
                // output1 << "v " << vertex_id << " " << vertex_label << endl;
                output1 << "v " << vertex_id << " " << vertex_label_to_num_map[vertex_label] << endl;
            }
            getline(input, line);
            int num_edges = stoi(line);
            for(int i=0; i<num_edges; i++) {
                getline(input, line);
                stringstream ss(line);
                string src, dst, edge_label;
                ss >> src >> dst >> edge_label;
                output1 << "e " << src << " " << dst << " " << edge_label << endl;
            }
        }
    }

    cout << "Number of graphs: " << num_graphs << '\n';
    cout << "Max. no. of nodes: " << max_num_vertices << '\n';

    // close all files
    input.close();
    output1.close();
    mapping.close();

    return 0;
}