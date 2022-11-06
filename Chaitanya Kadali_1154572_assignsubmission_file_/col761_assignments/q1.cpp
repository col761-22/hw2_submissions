#include<iostream>
#include<bits/stdc++.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]){
    //cout << "HI" << "\n";


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

    int ind = 0;
    int is_pos = 0;
    int is_count = 0;
    while(getline(graphs, line)){
        file.push_back(line);
        if(line[0] == '#'){
            //cout << line << "\n";
            int id = stoi(line.substr(1, line.size()-1));
            graphid.push_back(pair<int, int>(ind, id));
        }
        if(is_pos = 1 && is_count > 0){
            //cout << line.size() << "\n";
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
    wf.open("Dump/gsp_gas.txt");
    ofstream fsp("Dump/fsp");
    if(!wf){
        cout << "Cannot open gsp_gas file" << "\n";
        return 0;
    }
    if(!fsp){
        cout << "Cannot open fsp file" << "\n";
        return 0;
    }
    /*--------------------------------------------------------------------------------------------------------*/

    ind = 0;
    cout << "Number of graphs are " << graphid.size() << "\n";
    for(int i= 0; i< graphid.size(); i++){
        wf << "t # " << i << "\n";


        int start = graphid[i].first;
        int vertices = stoi(file[start+1]);
        fsp << "t #" << vertices << "\n";

        for(int j= 0; j< vertices; j++){         
            int v_label = lower_bound(labels.begin(), labels.end(),file[start+2+j]) - labels.begin();
            wf << "v " << j << " " << v_label << "\n";
            fsp << "v " << j << " " << v_label << "\n";
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
            fsp << "u " << left_index << " " << right_index << " " << edge_label << "\n";
        }


    }

    wf.close();
    fsp.close();

    vector<int> gspan(5,0);
    vector<int> gaston(5,0);
    vector<int> fsg_v(5,0);
    vector<double> threshold = {0.05, 0.1, 0.25,0.5,0.95};

    ofstream tgas("Dump/time_gaston.txt");
    ofstream tfsg("Dump/time_fsg.txt");
    ofstream tgspan("Dump/time_gspan.txt");

    for(int i= 4; i >= 0; i--){
        double support = threshold[i]*graphid.size();
        /*Time for gaston */
        auto start = high_resolution_clock::now();
        string temp = "./gaston-1.1/gaston " + to_string(support) + " Dump/gsp_gas.txt >> Dump/dump.txt";
        const char* command = temp.c_str();
        system(command);
        auto end = high_resolution_clock::now();
        gaston[i] = duration_cast<microseconds>(end-start).count()*0.000001;
        tgas << gaston[i] << " ";
        /*--------------------------------------------------------------------------------------*/

        /*Time for fsg */
        start = high_resolution_clock::now();
        temp = "./pafi/Linux/fsg -s " + to_string(threshold[i]*100) + " -pt Dump/fsp >> Dump/dump.txt";
        command = temp.c_str();
        system(command);
        end = high_resolution_clock::now();
        fsg_v[i] = duration_cast<microseconds>(end-start).count()*0.000001;
        tfsg << fsg_v[i] << " ";
        /*--------------------------------------------------------------------------------------*/

        /*Time for fsp */
        start = high_resolution_clock::now();
        temp = "./gSpan6/gSpan -f Dump/gsp_gas.txt -s " + to_string(threshold[i]) + " -o -i >> Dump/dump.txt" ;
        command = temp.c_str();
        system(command);
        end = high_resolution_clock::now();
        gspan[i] = duration_cast<microseconds>(end-start).count()*0.000001; 
        tgspan << gspan[i] << " ";
        /*--------------------------------------------------------------------------------------*/
    }

    tgas.close();
    tfsg.close();
    tgspan.close();

    for(int i= 0; i< 5; i++){
        cout << "gaston " << gaston[i] << "\n";
        cout << "gspan " << gspan[i] << "\n";
        cout << "fsp " << fsg_v[i] << "\n";
    }

    return 0;

}
