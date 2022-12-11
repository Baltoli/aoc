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

src_file="${year}/src/day${day}.cpp"
input_file="${year}/inputs/day${day}"
cmake_file="${year}/CMakeLists.txt"

if [ -f "$src_file" ]; then
  echo "$src_file exists; not overwriting!" 1>&2
  exit 2
fi

if [ -f "$input_file" ]; then
  echo "$input_file exists; not overwriting!" 1>&2
  exit 3
fi

cp template.cpp "$src_file"

session=$(cat .session-secret)

input_tmp=$(mktemp)
curl --silent "https://adventofcode.com/$year/day/$day/input" \
     --header "cookie: session=${session}" > "$input_tmp"
curl_ret="$?"

if [ "$curl_ret" -eq 0 ]; then
  mv "$input_tmp" "$input_file"
else
  echo "Couldn't download input!" 1>&2
  exit 4
fi

{
  echo
  echo "add_executable(${year}-day${day} src/day${day}.cpp)"
  echo "target_link_libraries(${year}-day${day} utils)"
} >> "$cmake_file"
