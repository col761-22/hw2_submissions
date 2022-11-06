#./gSpan-64 -f correct_gSpan -s {frequency/0.1} [-o] [-i] 
#./gaston 7 filename
#cd Sun
#./fsg

python3 format_gSpan.py "$1"
python3 format_gaston.py "$1"
python3 format_fsg.py "$1"
echo "converted"
python3 plot.py
