#include<iostream>
#include<bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>

using namespace boost;
using namespace std;

int main(int argc, char* argv[]){
    string graphset = argv[1];
    ifstream graphs(argv[1]);

    if(!graphs.is_open()){
        cout << "Unable to read the file" << "\n";
        return 0;
    }
    vector<pair<int,int>> graphid;              // index, id of the graph.
    vector<string> file;
    string line;
    set<string> labl;

    int is_pos = 0;
    int is_count = 0;
    int ind = 0;
    while(getline(graphs, line)){
        //cout << "lineee" << line << "\n";
        file.push_back(line);
        if(line[0] == '#'){
            int id = stoi(line.substr(1, line.size()-1));
            graphid.push_back(pair<int, int>(file.size()-1, id));
        }
        if(is_pos = 1 && is_count > 0){
            labl.insert(line);
            is_count -= 1;
        }
        else{
            is_pos = 0;
            is_count = 0;
        }

        if(ind > 0 && file[ind-1][0] == '#'){
            is_pos =  1;
            is_count = stoi(line);
        }
        ind++;
    }
    
    graphs.close();

    vector<string> labels;
    for(auto it = labl.begin(); it != labl.end(); it++){
        labels.push_back(*it);
    }

    system("mkdir Dump");
    /*Opening a index file to store the modified version of the given graphs.*/
    ofstream wf;
    wf.open("Dump/index.txt");
    if(!wf){
        cout << "Cannot open file" << "\n";
        return 0;
    }

    ind = 0;
    for(int i= 0; i< graphid.size(); i++){
        wf << "t # " << i << "\n";

        int start = graphid[i].first;
        int vertices = stoi(file[start+1]);

        for(int j= 0; j< vertices; j++){         
            int v_label = lower_bound(labels.begin(), labels.end(),file[start+2+j]) - labels.begin();
            wf << "v " << j << " " << v_label << "\n";
        }

        int edges = stoi(file[start+2+vertices]);

        for(int j= 0; j< edges; j++){
            stringstream ss(file[start+3+vertices+j]);
            int left_index;
            int right_index;
            ss >> left_index ;
            ss >> right_index ;
            int edge_label;
            ss >> edge_label;
            wf << "e " << left_index << " " << right_index << " " << edge_label << "\n";
        }

    }
    wf.close();
    /*--------------------------------------------------------------------------------------------------------*/


    /* Executing gspan on the formatted graph and dumping all the information into dump.txt file */
    system("./gSpan6/gSpan -f Dump/index.txt -s 0.5 -o -i >> Dump/dump.txt");
    /*---------------------------------------------------------------------------------------*/

    ofstream vlabels("Dump/vertex_labels.txt");
    for(int i = 0; i< labels.size(); i++){
        vlabels << labels[i] << "\n";                // Saving all the vertex labels encountered in to a vertex_labels file.
    }
    vlabels.close();

    ofstream graphids("Dump/graph_ids.txt");
    for(int i= 0; i< graphid.size(); i++){
        graphids << graphid[i].first << " " << graphid[i].second << "\n";
    }
    graphids.close();

    return 0;
}
