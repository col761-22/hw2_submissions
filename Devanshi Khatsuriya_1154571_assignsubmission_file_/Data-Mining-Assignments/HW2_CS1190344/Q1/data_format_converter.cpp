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
    ofstream output1("yeast_for_gspan.txt");
    ofstream output2("yeast_for_fsg.txt");
    ofstream output3("yeast_for_gaston.txt");
    
    string line;
    long long num_graphs = 0;
    while(getline(input, line)) {
        if(line[0] == '#') {
            num_graphs++;
            string graph_id = line.substr(1);
            output1 << "t # " << graph_id << endl;
            output2 << "t # " << graph_id << endl;
            output3 << "t # " << graph_id << endl;
            getline(input, line);
            int num_vertices = stoi(line);
            for(int vertex_id=0; vertex_id<num_vertices; vertex_id++) {
                getline(input, line);
                string vertex_label = line;
                if(vertex_label_to_num_map[vertex_label]==0) {
                    // cout << vertex_label << " " << label << '\n';
                    vertex_label_to_num_map[vertex_label] = label++;
                }
                // output1 << "v " << vertex_id << " " << vertex_label << endl;
                output1 << "v " << vertex_id << " " << vertex_label_to_num_map[vertex_label] << endl;
                output2 << "v " << vertex_id << " " << vertex_label << endl;
                output3 << "v " << vertex_id << " " << vertex_label_to_num_map[vertex_label] << endl;
            }
            getline(input, line);
            int num_edges = stoi(line);
            for(int i=0; i<num_edges; i++) {
                getline(input, line);
                stringstream ss(line);
                string src, dst, edge_label;
                ss >> src >> dst >> edge_label;
                output1 << "e " << src << " " << dst << " " << edge_label << endl;
                output2 << "u " << src << " " << dst << " " << edge_label << endl;
                output3 << "e " << src << " " << dst << " " << edge_label << endl;
            }
        }
    }

    cout << "Number of graphs: " << num_graphs << '\n';

    // close all files
    input.close();
    output1.close();
    output2.close();
    output3.close();

    return 0;
}