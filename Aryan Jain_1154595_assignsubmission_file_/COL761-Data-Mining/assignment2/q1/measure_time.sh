python transformInput.py "$1" "fsg_input.fsg" "fsg"
python transformInput.py "$1" "gspan_gaston_input.gspan_gaston" "gspan/gaston"

/usr/bin/time --format=%e --output="gspan_times.txt" --append ../binaries/gSpan-64 -f "gspan_gaston_input.gspan_gaston" -s "0.05" 
/usr/bin/time --format=%e --output="gspan_times.txt" --append ../binaries/gSpan-64 -f "gspan_gaston_input.gspan_gaston" -s "0.10" 
/usr/bin/time --format=%e --output="gspan_times.txt" --append ../binaries/gSpan-64 -f "gspan_gaston_input.gspan_gaston" -s "0.25" 
/usr/bin/time --format=%e --output="gspan_times.txt" --append ../binaries/gSpan-64 -f "gspan_gaston_input.gspan_gaston" -s "0.50" 
/usr/bin/time --format=%e --output="gspan_times.txt" --append ../binaries/gSpan-64 -f "gspan_gaston_input.gspan_gaston" -s "0.95" 

/usr/bin/time --format=%e --output="fsg_times.txt" --append ../binaries/fsg -s "5" "fsg_input.fsg"
/usr/bin/time --format=%e --output="fsg_times.txt" --append ../binaries/fsg -s "10" "fsg_input.fsg"
/usr/bin/time --format=%e --output="fsg_times.txt" --append ../binaries/fsg -s "25" "fsg_input.fsg"
/usr/bin/time --format=%e --output="fsg_times.txt" --append ../binaries/fsg -s "50" "fsg_input.fsg"
/usr/bin/time --format=%e --output="fsg_times.txt" --append ../binaries/fsg -s "95" "fsg_input.fsg"

typeset -i n=$(cat numGraphs.numGraphs)
sup=$(echo `expr $n*0.05` | bc)
/usr/bin/time --format=%e --output="gaston_times.txt" --append ../binaries/gaston $sup "gspan_gaston_input.gspan_gaston" 
sup=$(echo `expr $n*0.10` | bc)
/usr/bin/time --format=%e --output="gaston_times.txt" --append ../binaries/gaston $sup "gspan_gaston_input.gspan_gaston" 
sup=$(echo `expr $n*0.25` | bc)
/usr/bin/time --format=%e --output="gaston_times.txt" --append ../binaries/gaston $sup "gspan_gaston_input.gspan_gaston" 
sup=$(echo `expr $n*0.50` | bc)
/usr/bin/time --format=%e --output="gaston_times.txt" --append ../binaries/gaston $sup "gspan_gaston_input.gspan_gaston" 
sup=$(echo `expr $n*0.95` | bc)
/usr/bin/time --format=%e --output="gaston_times.txt" --append ../binaries/gaston $sup "gspan_gaston_input.gspan_gaston" 

python plot.py