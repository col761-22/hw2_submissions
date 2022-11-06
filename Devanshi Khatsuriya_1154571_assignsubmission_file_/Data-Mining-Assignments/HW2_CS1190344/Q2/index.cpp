#include <bits/stdc++.h>

using namespace std;

mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// create a function to add two numbers

int main(int argc, char* argv[]){
    string gSpan_output = argv[1];
    int M = atoi(argv[2]);
    string graph_Dataset = argv[3];

    // get the number of frequent patterns generated
    // count the number of lines starting with "t" in the gSpan output file


    // open the gSpan output file
    ifstream input(gSpan_output);
    string line;
    int num_patterns = 0;
    // int curr_pattern_size = 0;
    // vector<pair<int,int>> pattern_sizes;
    // while(getline(input, line)) {
    //     if(line.empty()) continue;
    //     if(line[0] == 't') {
    //         if(curr_pattern_size > 0)pattern_sizes.push_back({curr_pattern_size, num_patterns-1});
    //         num_patterns++;
    //         curr_pattern_size = 0;
    //     }
    //     if(line[0] == 'v') {
    //         curr_pattern_size++;
    //     }
    // }
    // pattern_sizes.push_back({curr_pattern_size, num_patterns-1});
    // sort(pattern_sizes.begin(), pattern_sizes.end());
    while(getline(input, line)) {
        if(line.empty()) continue;
        if(line[0] == 't') {
            num_patterns++;
        }
    }

    M = min(M, num_patterns);
    // int mid = pattern_sizes.size()/2;
    // int l = max(0, mid-M/2);
    // int r = l + M;
    int l = 0;
    int r = M;
    cout << "l " << l << " r " << r << "\n";


    input.close();
    cout << "Number of frequent patterns: " << num_patterns << endl;

    vector<int> pattern_ids(num_patterns);
    for(int i = 0; i < num_patterns; i++) {
        pattern_ids[i] = i;
    }
    // shuffle the pattern ids
    // shuffle(pattern_ids.begin(), pattern_ids.end(), rng);
    
    unordered_map<int,bool> selected_patterns;
    for(int i = l; i < r; i++) {
        selected_patterns[i] = true;
    }
    // cout << M << endl;
    unordered_map<int,string> dummy_id_to_feature_vector;
    unordered_map<int,string> dummy_id_to_graph_id;
    // read the graph dataset and initialize the graph_id_to_feature_vector map with strings of length M filled with 0.
    ifstream graph(graph_Dataset);
    int dummy_id = 0;
    while(getline(graph, line)) {
        if(line.empty()) continue;
        if(line[0] == '#') {
            string graph_id = line.substr(1);
            dummy_id_to_feature_vector[dummy_id] = string(M, '0');
            dummy_id_to_graph_id[dummy_id] = graph_id;
            dummy_id++;
        }
    }
    graph.close();

    // cout << dummy_id << endl;
    // read the gSpan output file and fill the feature vectors
    ifstream input2(gSpan_output);
    int cur_pattern = -1;
    while(getline(input2,line)){
        if(line.empty() || line[0] != 't') continue;
        if(line[0] == 't'){
            // t # 0 * 64110
            // split the line by space
            vector<string> tokens;
            stringstream check1(line);
            string intermediate;
            while(getline(check1, intermediate, ' ')) {
                tokens.push_back(intermediate);
            }
            int pattern_id = stoi(tokens[2]);
            if(selected_patterns.find(pattern_id) == selected_patterns.end()) 
                continue;

            cur_pattern++;
            // cout << cur_pattern << endl;
            while(getline(input2,line)){
                if(line.empty() or line[0] != 'x') continue;
                line = line.substr(2);

                stringstream check2(line);
                while(getline(check2, intermediate, ' ')) {
                    int tdummy_id = stoi(intermediate);
                    dummy_id_to_feature_vector[tdummy_id][cur_pattern] = '1';
                }
                break;
            }
        }
    }
    // write the feature vectors to output file
    ofstream output("feature_vectors.txt");
    for(auto it = dummy_id_to_feature_vector.begin(); it != dummy_id_to_feature_vector.end(); it++) {
        output << dummy_id_to_graph_id[it->first] << " " << it->second << endl;
    }
    output.close();

    // write the selected patterns for future use
    ofstream output2("selected_patterns.txt");
    vector<int> selected_patterns_vec;
    for(auto it = selected_patterns.begin(); it != selected_patterns.end(); it++) {
        selected_patterns_vec.push_back(it->first);
    }
    sort(begin(selected_patterns_vec), end(selected_patterns_vec));
    for(int i = 0; i < selected_patterns_vec.size(); i++) {
        output2 << selected_patterns_vec[i] << endl;
    }
}