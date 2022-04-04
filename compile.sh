#!/bin/bash
echo $0
echo $1
if [[ $1 == "readline" ]]; then
	make re RDLINE=1
else
	make re RDLINE=0
fi