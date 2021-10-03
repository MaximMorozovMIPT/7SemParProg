#!/bin/bash

while (( "$#" )); do
  case "$1" in
    -s|--size)
      if [ -n "$2" ] && [ -n "$3" ] && [ -n "$4" ] \
         && [ ${2:0:1} != "-" ] && [ ${3:0:1} != "-" ] && [ ${4:0:1} != "-" ]; then
        export NUM_ROWS_ONE=$2
        export NUM_COLS_ONE=$3
        export NUM_COLS_TWO=$4
        shift 4
      else
        echo "Error: Argument for $1 is missing" >&2
        exit 1
      fi
      ;;
    -r|--random)
      if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
        export RANDOM_SEED=$2
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

if [[ -z ${NUM_ROWS_ONE} ]] || [[ -z ${NUM_COLS_ONE} ]] || [[ -z ${NUM_COLS_TWO} ]] ; then
    export NUM_ROWS_ONE=10
    export NUM_COLS_ONE=10
    export NUM_COLS_TWO=10
    echo "Applied default size settings"
    echo "NUM_ROWS_ONE = ${NUM_ROWS_ONE}"
    echo "NUM_COLS_ONE = ${NUM_COLS_ONE}"
    echo "NUM_COLS_TWO = ${NUM_COLS_TWO}"
fi

if [[ -z ${RANDOM_SEED} ]] ; then
    export RANDOM_SEED=42
    echo "Applied default settings for random seed"
    echo "RANDOM_SEED = ${RANDOM_SEED}"
fi

./singlematr ${NUM_ROWS_ONE} ${NUM_COLS_ONE} ${NUM_COLS_TWO} ${RANDOM_SEED}