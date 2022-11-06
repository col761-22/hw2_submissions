#!/usr/bin/bash
case $1 in

    -apriori)
        ./apriori $2 $3 $4
        export LC_ALL=C
        sort -S 50% --parallel=1 $4 -o $4
        ;;

    -fptree)
        ./fptree $2 $3 $4
        export LC_ALL=C
        sort -S 50% --parallel=1 $4 -o $4
        ;;

    -plot)

        rm apriori_timing.dat fptree_timing.dat
        /usr/bin/time -o apriori_timing.dat -a -f %e ./apriori $2 5 output.dat
        /usr/bin/time -o apriori_timing.dat -a -f %e ./apriori $2 10 output.dat
        /usr/bin/time -o apriori_timing.dat -a -f %e ./apriori $2 25 output.dat
        /usr/bin/time -o apriori_timing.dat -a -f %e ./apriori $2 50 output.dat
        /usr/bin/time -o apriori_timing.dat -a -f %e ./apriori $2 90 output.dat

        /usr/bin/time -o fptree_timing.dat -a -f %e ./fptree $2 5 output.dat
        /usr/bin/time -o fptree_timing.dat -a -f %e ./fptree $2 10 output.dat
        /usr/bin/time -o fptree_timing.dat -a -f %e ./fptree $2 25 output.dat
        /usr/bin/time -o fptree_timing.dat -a -f %e ./fptree $2 50 output.dat
        /usr/bin/time -o fptree_timing.dat -a -f %e ./fptree $2 90 output.dat

        python3 plotting.py $3
        rm apriori_timing.dat fptree_timing.dat output.dat
        ;;

esac
