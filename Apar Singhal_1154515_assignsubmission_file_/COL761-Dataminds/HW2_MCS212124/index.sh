./gspanformatconverter "$1" "gspanInputFile.gspan"
c=`grep -o t gspan_input_file.gspan | wc -l`
sup="0.50"
if [ "$c" -gt 50000 ] 
then
    sup="0.60"
elif [ "$c" -gt 40000 ]
then
    sup="0.55"
elif [ "$c" -gt 30000 ]
then
    sup="0.50"
elif [ "$c" -gt 20000 ]
then
    sup="0.40"
elif [ "$c" -gt 10000 ]
then
    sup="0.30"
else
    sup="0.20"
fi

echo $sup > tmp
./binaries/gSpan6/gSpan -f "gspanInputFile.gspan" -s "$sup" -o -i
./index "$1" "gspanInputFile.gspan.fp"