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
    -l|--length)
      if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
        LEN=$2
        shift 2
      else
        echo "Error: Argument for $1 is missing" >&2
        exit 1
      fi
      ;;
    -r|--random)
      if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
        RANDOM_SEED=$2
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

if [[ -z ${LEN} ]] ; then
    export LEN=50
    # echo "Applied default size settings for array length"
    # echo "Length array = ${LEN}"
fi

if [[ -z ${RANDOM_SEED} ]] ; then
    export RANDOM_SEED=RANDOM
fi


# echo "num of threads = ${OMP_NUM_THREADS}"

./sort ${LEN} ${RANDOM_SEED}