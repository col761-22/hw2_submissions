#include <bits/stdc++.h>
#include <sys/stat.h>
using namespace std;

int main(int argc, char* argv[])
{
    
    // include flags
    // --type: graphs/patterns
    // --input: input file
    // --selected_patterns: file containing selected patterns

    // parse arguments
    if(argc < 5)
    {
        cout << "Usage: " << argv[0] << " --type [graphs/patterns] --input [input file] --selected_patterns [file containing selected patterns for patterns type]" << "\n";
        return 0;
    }
    string type = argv[2];
    string input_file = argv[4];
    string selected_patterns_file = "";
    if(type == "patterns")
    {
        if(argc < 7)
        {
            cout << "Usage: " << argv[0] << " --type [graphs/patterns] --input [input file] --selected_patterns [file containing selected patterns for patterns type]" << "\n";
            return 0;
        }
        selected_patterns_file = argv[6];
    }

    set<int> selected_patterns;
    if(type == "patterns")
    {
        ifstream selected_patterns_file_stream(selected_patterns_file);
        int pattern_id;
        while(selected_patterns_file_stream >> pattern_id)
            selected_patterns.insert(pattern_id);
        selected_patterns_file_stream.close();
    }
    // read input in gspan format
    string line;
    // create a directory to save graph files if it doesn't exist
    string dir = "graphs";
    if(type == "patterns")
        dir = "patterns";
    if(mkdir(dir.c_str(), 0777) == -1){
        cout << "Warning: Directory already exists\n";
    }
    // create instream from input file
    ifstream input(input_file);
    unordered_map<int,int> node_to_label;
    vector<vector<pair<int,int>>> edges;
    string graph_id = "";
    bool ignore = false;
    while(getline(input, line))
    {
        // cout << line << endl;
        if(line.empty()) continue;
        if(line[0] == 't')
        {
            // save the previous graph
            if(node_to_label.size() > 0)
            {
                // save the graph
                string graph_file = dir + "/" + graph_id + ".txt";
                ofstream graph_file_stream(graph_file);
                graph_file_stream << node_to_label.size() << "\n";
                // cout << node_to_label.size() << endl;
                for(int i = 0; i<node_to_label.size(); i++)
                {
                    graph_file_stream << i << " " << node_to_label[i] << "\n";
                }
                // cout << "here" << endl;
                for(int i = 0; i<node_to_label.size(); i++)
                {
                    // cout << i << endl;
                    graph_file_stream << edges[i].size() << "\n";
                    // cout << edges[i].size() << "\n";
                    for(auto edge: edges[i])
                    {
                        graph_file_stream << i << " " << edge.first << " " << edge.second << "\n";
                    }
                }
                graph_file_stream.close();
            }
            // clear the data structures
            node_to_label.clear();
            edges.clear();
            // split line by space
            vector<string> tokens;
            stringstream ss(line);
            string token;
            while(getline(ss, token, ' '))
                tokens.push_back(token);
            // get the graph id
            graph_id = tokens[2];
            // cout << graph_id << endl;
            if(type == "patterns")
            {
                int pattern_id = stoi(graph_id.substr(0, graph_id.find(" ")));
                if(selected_patterns.find(pattern_id) == selected_patterns.end())
                    ignore = true;
                else
                    ignore = false;
            }
        }
        else if(line[0] == 'v')
        {
            if(ignore) continue;
            // add a node
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while(getline(ss, token, ' '))
                tokens.push_back(token);
            int node_id = stoi(tokens[1]);
            int label = stoi(tokens[2]);
            node_to_label[node_id] = label;
            edges.push_back(vector<pair<int,int>>());
        }
        else if(line[0] == 'e')
        {
            if(ignore) continue;
            // add an edge
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while(getline(ss, token, ' '))
                tokens.push_back(token);
            int node1 = stoi(tokens[1]);
            int node2 = stoi(tokens[2]);
            int label = stoi(tokens[3]);
            edges[node1].push_back({node2, label});
            edges[node2].push_back({node1, label});
        }
    }
    // save the last graph  
    if(node_to_label.size() > 0)
    {
        // save the graph
        string graph_file = dir + "/" + graph_id + ".txt";
        ofstream graph_file_stream(graph_file);
        graph_file_stream << node_to_label.size() << "\n";
        for(int i = 0; i<node_to_label.size(); i++)
        {
            graph_file_stream << i << " " << node_to_label[i] << "\n";
        }
        for(int i = 0; i<node_to_label.size(); i++)
        {
            graph_file_stream << edges[i].size() << "\n";
            for(auto edge: edges[i])
            {
                graph_file_stream << i << " " << edge.first << " " << edge.second << "\n";
            }
        }
        graph_file_stream.close();
    }


    return 0;
}