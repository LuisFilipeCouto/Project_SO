#!/bin/bash

if [ $# != 2 ]; then
	echo "$0 diskfile numblocks"
	exit 1
fi

dd if=/dev/urandom of=$1 bs=1024 count=$2
