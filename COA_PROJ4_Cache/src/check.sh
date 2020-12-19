#! /bin/bash

clear

#rm results output* trace

#ls

read -p "ReMake? (y/n)(default: n):" ReMake
ReMake=${ReMake:=n}
if [ $ReMake = "y" ]; then
    echo "Remake..."
    make
else
    echo "Dont remake."
fi

read -p "Check for validation(default: all): " choice
choice=${choice:=all}

read -p "Set to trace(default: -1): " ToTrace
ToTrace=${ToTrace:=-1}


if [ ! -e "sim_cache" ]; then
    echo "Plz make first!!!"
    exit 1
fi

case $choice in
    "all")
    #all test
    echo "test all ..."

    : > results
    ./sim_cache 16 16384 1 0 0 gcc_trace.txt $ToTrace 1>output1 2>trace
    diff output1 ../validation/ValidationRun1.txt >> results

    ./sim_cache 128 2048 8 0 1 go_trace.txt $ToTrace 1>output2 2>trace
    diff output2 ../validation/ValidationRun2.txt >> results
    
    ./sim_cache 32 4096 4 0 1 perl_trace.txt $ToTrace 1>output3 2>trace
    diff output3 ../validation/ValidationRun3.txt >> results
    
    ./sim_cache 64 8192 2 1 0 gcc_trace.txt $ToTrace 1>output4 2>trace
    diff output4 ../validation/ValidationRun4.txt >> results
    
    ./sim_cache 32 1024 4 1 1 go_trace.txt $ToTrace 1>output5 2>trace
    diff output5 ../validation/ValidationRun5.txt >> results

    if [ ! -s "results" ]; then
        echo "All tests passed!"
	rm output* results trace
    else
	echo "Failed some tests, plz check the results."
    fi

    exit 0

    ;;

    1)
    # validation1
    echo '
    L1_BLOCKSIZE:                    16
    L1_SIZE:                      16384
    L1_ASSOC:                         1
    L1_REPLACEMENT_POLICY:	    LRU
    L1_WRITE_POLICY:               WBWA
    trace_file:           gcc_trace.txt
    '
    ./sim_cache 16 16384 1 0 0 gcc_trace.txt $ToTrace 1>output 2>trace
    ;;

    2)
    # validation2
    echo '
    L1_BLOCKSIZE:                   128
    L1_SIZE:                       2048
    L1_ASSOC:                         8
    L1_REPLACEMENT_POLICY:          LRU
    L1_WRITE_POLICY:               WTNA
    trace_file:            go_trace.txt
    '
    ./sim_cache 128 2048 8 0 1 go_trace.txt $ToTrace 1>output 2>trace
    ;;

    3)
    # validation3
    echo '
    L1_BLOCKSIZE:                    32
    L1_SIZE:                       4096
    L1_ASSOC:                         4
    L1_REPLACEMENT_POLICY:          LRU
    L1_WRITE_POLICY:               WTNA
    trace_file:          perl_trace.txt
    '
    ./sim_cache 32 4096 4 0 1 perl_trace.txt $ToTrace 1>output 2>trace
    ;;

    4)
    # validation4
    echo '
    L1_BLOCKSIZE:                    64
    L1_SIZE:                       8192
    L1_ASSOC:                         2
    L1_REPLACEMENT_POLICY:          LFU
    L1_WRITE_POLICY:               WBWA
    trace_file:           gcc_trace.txt
    '
    ./sim_cache 64 8192 2 1 0 gcc_trace.txt $ToTrace 1>output 2>trace
    ;;

    5)
    # validation5
    echo '
    L1_BLOCKSIZE:                    32
    L1_SIZE:                       1024
    L1_ASSOC:                         4
    L1_REPLACEMENT_POLICY:          LFU
    L1_WRITE_POLICY:               WTNA
    trace_file:            go_trace.txt
    '
    ./sim_cache 32 1024 4 1 1 go_trace.txt $ToTrace 1>output 2>trace
    ;;

    *)
    echo "Out of range"
    exit -1
    ;;
esac

diff output ../validation/ValidationRun$choice.txt > results

if [ ! -s "results" ]; then
    echo "Test$choice passed!"
    rm output results trace
else
    echo "Failed the test, plz check the results."
fi

exit 0
