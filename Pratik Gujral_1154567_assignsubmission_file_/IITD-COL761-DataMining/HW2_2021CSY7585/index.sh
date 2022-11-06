g++ conv_input.cpp -o convertor
./convertor "$1" "output.txt" 50
g++ helper.cpp -o myhelper
./myhelper "$1" "output.txt.fp"
