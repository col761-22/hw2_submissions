set terminal png size 500,500
set output output_filename
set title 'Elbow Plot'
set xlabel "Number of clusters (K)"
set ylabel "Euclidean distance"
p './datasets/4.txt' u 1:2 w lp title 'k-means'
