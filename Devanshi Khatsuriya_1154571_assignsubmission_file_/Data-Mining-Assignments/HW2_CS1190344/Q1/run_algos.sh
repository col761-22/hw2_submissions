#!/usr/bin/bash

# minSup = 5%, 10%, 25%, 50% and 95%. Plot the
# plot running times and explain the trend observed in the running times

g++ data_format_converter.cpp -o data_format_converter
./data_format_converter "Yeast/167.txt_graph"
rm data_format_converter

# clear previously generated logs from the timing files
echo -n "" > "gspan_timings.txt"
echo -n "" > "fsg_timings.txt"
echo -n "" > "gaston_timings.txt"
# for minSup in 0.95 # 0.05 0.10 0.25 0.5
for minSup in 0.95 0.5 0.25 0.10 0.05
do
    minSupOutOf100=$( echo "100 * $minSup" | bc)
    absSup=$( echo $( printf %.$2f $( echo "64110 * $minSup" | bc))) # 64110 is the number of graphs in yeast
    echo "minSup = $minSup"
    # run the algorithms
    echo "Support Threshold $minSupOutOf100" >> "gspan_timings.txt"
    timeout 4h /usr/bin/time -v ../gSpan6/gSpan-64 -f yeast_for_gspan.txt -s $minSup -o 2>> "gspan_timings.txt"
    echo "Support Threshold $minSupOutOf100" >> "fsg_timings.txt"
    timeout 4h /usr/bin/time -v ../pafi-1.0.1/Linux/fsg -s $minSupOutOf100 yeast_for_fsg.txt 2>> "fsg_timings.txt"
    echo "Support Threshold $minSupOutOf100" >> "gaston_timings.txt"
    timeout 4h /usr/bin/time -v ../gaston-1.1/gaston $absSup yeast_for_gaston.txt 2>> "gaston_timings.txt"
    # generate a plot after each threshold
    python plot_script.py "timings_on_yeast"
done