#!/bin/bash

while (( "$#" )); do
  case "$1" in
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

if [[ -z ${N} ]] ; then
    echo 'Pass maximum number to check -n ...'
    exit 1
fi

./prime ${N}