#!/bin/bash

PARAMS=""
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
    -*|--*=) # unsupported flags
      echo "Error: Unsupported flag $1" >&2
      exit 1
      ;;
    *) # preserve positional arguments
      PARAMS="$PARAMS $1"
      shift
      ;;
  esac
done

if [[ -z ${OMP_NUM_THREADS} ]] ; then
    echo 'Pass number of threads in script or setup the env OMP_NUM_THREADS'
    exit 1
fi

# echo "num of threads = ${OMP_NUM_THREADS}"

./hello