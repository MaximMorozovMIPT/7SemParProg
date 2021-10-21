#!/bin/bash

while (( "$#" )); do
  case "$1" in
    -n)
      if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
        export NUM_THREADS=$2
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

if [[ -z ${NUM_THREADS} ]] ; then
    echo 'Pass number of threads in script -n ...'
    exit 1
fi

mpirun -n ${NUM_THREADS} common