#! /bin/bash

clear

make

read -p "Check for validation: " choice

read -p "Set to trace: " ToTrace

case $choice in
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

diff output ../validation/ValidationRun$choice.txt

exit 0
