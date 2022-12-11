#!/usr/bin/env bash

usage() {
  echo "Usage: $0 <year> <day>" 1>&2
}

if [ "$#" -ne 2 ]; then
  usage
  exit 1
fi

year="$1"
shift

day="$1"
shift

"./build/${year}/${year}-day${day}" < "./${year}/inputs/day${day}"
