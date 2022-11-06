./convertTogaston "$1" "gastonInputFile.gaston"
n=`grep -o t gastonInputFile.gaston | wc -l`
if [ "$n" -gt 90000 ]
then
         minSup="0.22"
elif [ "$n" -gt 80000 ]
then
         minSup="0.15"
elif [ "$n" -gt 70000 ]
then
         minSup="0.12"
elif [ "$n" -gt 65000 ] 
then
         minSup="0.1"
elif [ "$n" -gt 55000 ]
then
        minSup="0.06"
elif [ "$n" -gt 30000 ]
then
        minSup="0.05"
elif [ "$n" -gt 18000 ]
then
        minSup="0.05"
else
        minSup="0.05"
fi
sup=$(echo "$n*$minSup" | bc)
maxSize="-1"
./runGaston "gastonInputFile.gaston" $sup $maxSize "gastonOutputFile.gaston"
rm -f graph-ids.txt feature-idxs.txt features.txt label-to-int.txt edge-idxs.txt
./indexer "$1" "gastonOutputFile.gaston"