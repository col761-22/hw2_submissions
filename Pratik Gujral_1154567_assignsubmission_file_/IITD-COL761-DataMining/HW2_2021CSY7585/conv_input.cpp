#include <bits/stdc++.h>
#include <iostream>

using namespace std;

int total_graphs = 0;

bool check(string &a)
{
  for(char x: a)
  {
    if(!isdigit(x))
      return false;
  }

  return true;
}

void convToGspan(string orig_file, string inp_file)
{
  ifstream ifile(orig_file);
  ofstream ofile(inp_file);

  bool ctr = false;
  std::unordered_map<string, int> mp;
  int v_count = 0;
  int tot_count = 1;

  string line;
  while(getline(ifile, line))
  {
    istringstream iss(line);

    if(line.length()==0)
    {
      continue;
    }
    else if(line[0]=='#')
    {
      v_count = 0;

      if(total_graphs>0)
        ofile << "\n";

      string temp = "t # " + to_string(total_graphs++) + "\n";
      ofile << temp;
    }
    else if(check(line))
    {
      if(ctr)
        ctr = false;
      else
        ctr = true;
    }
    else if(ctr)
    {
      if(mp.find(line)==mp.end())
      {
        mp[line] = tot_count++;
      }

      string temp = "v " + to_string(v_count++) + " " + to_string(mp[line]) + "\n";
      ofile << temp;
    }
    else
    {
      istringstream iss(line);
      string a, b, c;

      iss >> a >> b >> c;

      if(mp.find(c)==mp.end())
      {
        mp[c] = tot_count++;
      }

      string temp;
      if(a < b)
        temp = "e " + a + " " + b + " " + to_string(mp[c]) + "\n";
      else
        temp = "e " + b + " " + a + " " + to_string(mp[c]) + "\n";
      
      ofile << temp;
    }
  }

  ifile.close();
  ofile.close();
}

int main(int argc, char **argv)
{
  convToGspan(argv[1], argv[2]);
  string threshold = to_string(((float)(stoi(argv[3])))/100.0);
  string command = "./gSpan6/gSpan -s " + threshold + " -f output.txt -o -i";
  const char *cmd = command.c_str();
  system(cmd);

  return 0;
}
