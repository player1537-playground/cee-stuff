#!/bin/bash
whatdo=$1; shift
case $whatdo in
    simple) 
        # compiles ${1%-}-server.c -> server
        #          ${1%-}-client.c -> client
	for suffix in server client; do
	    gcc ${1%-}-$suffix.c -lzmq -o $suffix
	done
	;;
    all)
	for i; do
	    set -x
	    gcc $i -lzmq -o ${i%.c}
	    set +x
	done
	;;
esac
