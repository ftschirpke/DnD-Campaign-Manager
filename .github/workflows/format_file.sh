#!/bin/bash

clang-format -style=file -output-replacements-xml $1 | grep -c "<replacement " >/dev/null

if [ $? -eq 0 ]; then
    echo "formatted incorrectly: $1"
    exit
fi
echo "formatted correctly: $1"
