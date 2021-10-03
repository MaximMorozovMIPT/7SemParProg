#!/bin/bash

while (( "$#" )); do
  case "$1" in
    -t|--threads)
      if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
        export OMP_NUM_THREADS=$2
        shift 2
      else
        echo "Error: Argument for $1 is missing" >&2
        exit 1
      fi
      ;;
    -n|--number)
      if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
        N=$2
        shift 2
      else
        echo "Error: Argument for $1 is missing" >&2
        exit 1
      fi
      ;;
    -*|--*=) # unsupported flags
      echo "Error: Unsupported flag $1" >&2
      exit 1
      ;;
  esac
done

if [[ -z ${OMP_NUM_THREADS} ]] ; then
    echo 'Pass number of threads in script as -t ... or setup the env OMP_NUM_THREADS'
    exit 1
fi

if [[ -z ${N} ]] ; then
    echo 'Pass number of iterations as -n ...'
    exit 1
fi

# echo "num of threads = ${OMP_NUM_THREADS}"

./asin ${N}