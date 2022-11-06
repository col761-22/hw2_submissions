g++ -std=c++17 q2/src/convertTogaston.cpp -o convertTogaston
g++ -std=c++17 q2/src/runGaston.cpp q2/lib/libGaston.so -I q2/include -o runGaston
g++ -std=c++17 q2/src/index.cpp q2/src/graph-utils.cpp -I q2/include -I /home/apps/LIBBOOST/1.64.0/gnu_ucs71/include/ -o indexer
g++ -std=c++17 q2/src/query.cpp q2/src/graph-utils.cpp -I q2/include -I /home/apps/LIBBOOST/1.64.0/gnu_ucs71/include/ -o queryEngine