/*
This file will be used as helper file to support other functions (provides support uitility)
*/

#include <bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <boost/graph/vf2_sub_graph_iso.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#define maxL 13
#define gammaThreshold 1.0500
#define M 400

using namespace std::chrono;
using namespace std;
using namespace boost;

//The following definition are vertices, edges, and their maps as specified in the boost::vf2 library

typedef property<edge_name_t, int> edge_prop;
typedef property<vertex_name_t, int, property<vertex_index_t, int>> vertex_prop;
typedef adjacency_list<vecS, vecS, undirectedS, vertex_prop, edge_prop> graph_type;
typedef property_map<graph_type, vertex_name_t>::type vertex_name_map_t;
typedef property_map_equivalent<vertex_name_map_t, vertex_name_map_t> vertex_comp_t;
typedef property_map<graph_type, edge_name_t>::type edge_name_map_t;
typedef property_map_equivalent<edge_name_map_t, edge_name_map_t> edge_comp_t;

//define the callback function for the correspondences between graph1 and graph2 that
//instructs to return false when isomorphism fails

template<typename Graph1, typename Graph2> struct vf2_callback{

    vf2_callback(Graph1 g, Graph2 f){
    }
    template <typename CorrespondenceMap1To2,
          typename CorrespondenceMap2To1>
    bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1 g) const {
        return false;
    }
};

bool isomorphismchecker(graph_type g1, graph_type g2){

    vertex_comp_t vertex_comp =
        make_property_map_equivalent(get(vertex_name, g1), get(vertex_name, g2));
    edge_comp_t edge_comp =
        make_property_map_equivalent(get(edge_name, g1), get(edge_name, g2));
    vf2_callback<graph_type , graph_type> callback(g1,g2);
    return vf2_subgraph_iso(g1, g2, callback, vertex_order_by_mult(g1),
                       edges_equivalent(edge_comp).vertices_equivalent(vertex_comp));
}

bool checker(string &a)
{
  for(char x: a)
  {
    if(!isdigit(x))
      return false;
  }

  return true;
}

void setIntersection(std::unordered_set<int> &s1, std::unordered_set<int> &s2)
{
  std::unordered_set<int> retter;

  for(int x: s2)
  {
    if(s1.find(x)!=s1.end())
      retter.insert(x);
  }

  s1 = retter;
}

void readDataset(string inpfile, vector<graph_type> &v, std::unordered_map<string, int> &mp, std::unordered_map<int, int> &graph_mp)
{
  //Note that each type of node has a label and hence we continue to use those labels and add more labels
  //when not found in the set of labels
  //v is the vector of graphs we are concerned with
  //filename is the file we shall be reading the input from

  ifstream ifile(inpfile);
  int graph_counter = 0;
  int vertex_counter = 1;
  bool check = true;

  string line;
  while(getline(ifile, line))
  {
    if(line.length()==0)
      continue;

    if(line[0]=='#')
    {
      int graph_id = stoi(line.substr(1));
      int node_count=0, edge_count=0;
      graph_type G;

      while(getline(ifile, line) && line.length()!=0)
      {
        if(checker(line))
        {
          if(check)
          {
            node_count = stoi(line);
            check = false;
          }
          else
          {
            edge_count = stoi(line);
            check = true;
          }
        }
        else if(!check)
        {
          if(mp.find(line)==mp.end())
          {
            mp[line] = vertex_counter++;
          }

          add_vertex(vertex_prop(mp[line]), G);
        }
        else
        {
          istringstream iss(line);

          string a, b, c;
          iss >> a >> b >> c;

          if(mp.find(c)==mp.end())
          {
            mp[c] = vertex_counter++;
          }

          if(a < b)
            add_edge(stoi(a), stoi(b), edge_prop(mp[c]), G);
          else
            add_edge(stoi(b), stoi(a), edge_prop(mp[c]), G);
        }
      }

      v.push_back(G);
      graph_mp[graph_counter++] = graph_id;
    }
  }

  ifile.close();
}

void readGspanOutput(string inpfile, vector<vector<graph_type>> &v, vector<vector<std::unordered_set<int>>> &graph_mp)
{
  ifstream ifile(inpfile);
  string line;

  while(getline(ifile, line))
  {
    if(line.length()==0)
      continue;

    if(line[0]=='t')
    {
      int edge_count=0;
      graph_type G;

      while(getline(ifile, line) && line.length()!=0)
      {
        if(line[0]=='v')
        {
          istringstream iss(line);
          string a, b, c;
          iss >> a >> b >> c;

          add_vertex(vertex_prop(stoi(c)), G);
        }
        else if(line[0]=='e')
        {
          edge_count++;
          istringstream iss(line);
          string a, b, c, d;
          iss >> a >> b >> c >> d;

          if(b < c)
            add_edge(stoi(b), stoi(c), edge_prop(stoi(d)), G);
          else
            add_edge(stoi(c), stoi(b), edge_prop(stoi(d)), G);
        }
        else if(line[0]=='x')
        {
          istringstream iss(line);
          string word;
          iss >> word;
          std::unordered_set<int> temp;

          while(iss >> word)
          {
            temp.emplace(stoi(word));
          }

          if(edge_count <= maxL)
          {
            v[edge_count].push_back(G);
            graph_mp[edge_count].push_back(temp);
          }
        }
      }
    }
  }

  ifile.close();
}

void pushAllFeatures(string v1_label, string v2_label, string elabel, vector<vector<graph_type>> &all_features)
{
  graph_type temp_graph;  //to be inserted in all_features as single edge graphs
  add_vertex(vertex_prop(stoi(v1_label)), temp_graph);
  add_vertex(vertex_prop(stoi(v2_label)), temp_graph);
  add_edge(0, 1, edge_prop(stoi(elabel)), temp_graph);
  all_features[1].push_back(temp_graph);
}

void pustToGraphMap(graph_type &G, vector<vector<std::unordered_set<int>>> &graph_mp, std::unordered_map<string, std::unordered_set<int>> &edge_gIndices)
{
  graph_type::edge_iterator e1, e2;
  for(tie(e1, e2) = edges(G); e1!=e2; e1++)
  {
    string v1_label = to_string(get(vertex_name, G)[source(*e1, G)]);
    string v2_label = to_string(get(vertex_name, G)[target(*e1, G)]);
    string elabel = to_string(get(edge_name, G)[*e1]);

    string gidx = "";

    if(v1_label < v2_label)
      gidx = v1_label + "@" + v2_label + "@" + elabel;
    else
      gidx = v2_label + "@" + v1_label + "@" + elabel;

    graph_mp[1].push_back(edge_gIndices[gidx]);
  }
}

double findGamma(graph_type &G, int sizeOfG, vector<vector<graph_type>> &all_features, vector<vector<std::unordered_set<int>>> &graph_mp)
{
  std::unordered_set<int> featureSet;
  bool check = true;
  for(int i=1; i<=num_edges(G); i++)
  {
    for(int j=0; j<all_features[i].size(); j++)
    {
      // cout << "Here at i: " << i << ", and j: " << j << endl;
      if(isomorphismchecker(all_features[i][j], G))
      {
        if(check)
        {
          featureSet = graph_mp[i][j];
          check = false;
        }
        else
        {
          setIntersection(featureSet, graph_mp[i][j]);
        }
      }
    }
  }

  return (1.000*featureSet.size())/sizeOfG;
}

void eliminateRedundantGraphs(graph_type &G, std::unordered_set<int> &validGraphSet, vector<vector<graph_type>> &all_features, vector<vector<std::unordered_set<int>>> &graph_mp)
{
  int edge_count = num_edges(G);
  for(int i=0; i<all_features.size(); i++)
  {
    if(i>edge_count)
      break;

    for(int j=0; j<all_features[i].size(); j++)
    {
      if(isomorphismchecker(all_features[i][j], G))
        setIntersection(validGraphSet, graph_mp[i][j]);
    }
  }
}

void fillQueries(string inpfile, vector<graph_type> &v, std::unordered_map<string, int> &mp)
{
  ifstream ifile(inpfile);
  string line;
  bool check = true;
  int vertex_counter = mp.size()+1;

  while(getline(ifile, line))
  {
    if(line.length()==0)
      continue;

    if(line[0]=='#')
    {
      check = false;
      int graph_id = stoi(line.substr(1));
      graph_type G;

      while(getline(ifile, line) && line.length()!=0)
      {
        if(checker(line))
        {
          if(check)
            check = false;
          else
            check = true;
        }
        else if(check)
        {
          if(mp.find(line)==mp.end())
            mp[line] = vertex_counter++;

          add_vertex(vertex_prop(mp[line]), G);
        }
        else
        {
          istringstream iss(line);
          string a, b, c;
          iss >> a >> b >> c;

          if(mp.find(c)==mp.end())
            mp[c] = vertex_counter++;

          if(a < b)
            add_edge(stoi(a), stoi(b), edge_prop(mp[c]), G);
          else
            add_edge(stoi(b), stoi(a), edge_prop(mp[c]), G);
        }
      }

      v.push_back(G);
    }
  }

  ifile.close();
}

int main(int argc, char **argv)
{
  vector<graph_type> v;
  std::unordered_map<string, int> mp;
  std::unordered_map<int, int> graph_tag_mp;

  readDataset(argv[1], v, mp, graph_tag_mp);

  vector<vector<graph_type>> gspan_graphs(maxL+1);
  vector<vector<std::unordered_set<int>>> gspan_graph_mp(maxL+1);

  readGspanOutput(argv[2], gspan_graphs, gspan_graph_mp);

  // for(int i=0; i<=maxL; i++)
  // {
  //   cout << "Number of graphs with " << i << " edges are: " << gspan_graphs[i].size() << endl;
  // }
  //
  // cout << "Number of graphs are: " << v.size() << endl;

  vector<vector<graph_type>> all_features(maxL+1);
  vector<vector<std::unordered_set<int>>> graph_mp(maxL+1);
  std::unordered_map<string, std::unordered_set<int>> edge_gIndices;

  //pushing in all the edges as features to construct further discriminative features
  graph_type::edge_iterator e1, e2;
  int graph_counter = -1;
  for(auto &G: v)
  {
    graph_counter++;
    for(tie(e1, e2) = edges(G); e1!=e2; e1++)
    {
      string v1_label = to_string(get(vertex_name, G)[source(*e1, G)]);
      string v2_label = to_string(get(vertex_name, G)[target(*e1, G)]);
      string elabel = to_string(get(edge_name, G)[*e1]);

      string gidx = "";

      if(v1_label < v2_label)
        gidx = v1_label + "@" + v2_label + "@" + elabel;
      else
        gidx = v2_label + "@" + v1_label + "@" + elabel;

      if(edge_gIndices.find(gidx)==edge_gIndices.end())
      {
        std::unordered_set<int> temp;
        temp.insert(graph_counter);

        edge_gIndices[gidx] = temp;
        pushAllFeatures(v1_label, v2_label, elabel, all_features);
      }
      else
      {
        edge_gIndices[gidx].insert(graph_counter);
      }
    }
  }

  for(auto &G: all_features[1])
    pustToGraphMap(G, graph_mp, edge_gIndices);

  // for(int i=0; i<=maxL; i++)
  //   cout << "Size of " << i << " edge features is: " << gspan_graphs[i].size() << endl;

  //Construct the whole feature set from the 1 edge graphs now from the gSpan set

  int feature_counter=0;
  bool check1 = false;
  for(int i=2; i<=maxL; i++)
  {
    if(check1)
      break;

    for(int j=0; j<gspan_graphs[i].size(); j++)
    {
      //g_val is the gamme value for this feature
      if(feature_counter <= M)
      {
        double g_val = findGamma(gspan_graphs[i][j], gspan_graph_mp[i][j].size(), all_features, graph_mp);

        if(g_val >= gammaThreshold)
        {
          feature_counter++;
          all_features[i].push_back(gspan_graphs[i][j]);
          graph_mp[i].push_back(gspan_graph_mp[i][j]);
        }
      }
      else
      {
        check1 = true;
        break;
      }
    }
  }

  cout << "Index has been loaded!\n";
  vector<graph_type> queries;

  string inputQueryfile;
  cout << "Enter name of query file: ";
  cin >> inputQueryfile;

  clock_t start, end;

  start = clock();

  fillQueries(inputQueryfile, queries, mp);

  string outputQueryFile = "output_2021CSY7585.txt";
  ofstream ofile(outputQueryFile);

  for(auto &G: queries)
  {
    vector<int> ans;

    vector<int> validGraphs(v.size());
    iota(validGraphs.begin(), validGraphs.end(), 0);
    std::unordered_set<int> validGraphSet(validGraphs.begin(), validGraphs.end());

    eliminateRedundantGraphs(G, validGraphSet, all_features, graph_mp);

    for(int x: validGraphSet)
    {
      if(isomorphismchecker(G, v[x]))
        ans.push_back(graph_tag_mp[x]);
    }

    for(int i=0; i<ans.size(); i++)
    {
      if(i!=0)
        ofile << "\t";

      ofile << "#" << ans[i];
    }
    ofile << endl;
  }

  ofile.close();

  end = clock();

  double time_taken = double(end-start)/double(CLOCKS_PER_SEC);
  cout << "Time taken to run queries is: " << fixed << time_taken << setprecision(5);
  cout << " seconds " << endl;

  return 0;
}
