#!/bin/bash

RED='\033[0;33m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
NC='\033[0m' # No Color

make run

fswatch -xr . |
while read filename event ftype; do
  if [[ "$filename" =~ (\.cpp|\.hpp|Makefile) ]] ; then
    echo -e "\n${ORANGE}Changed $filename${GREEN}\n"
    make
    make_result=$?
    echo -e "${NC}"
    if [[ $make_result == 0 ]] ; then 
      _build/groundfloor
    fi
  fi
done