#!/bin/bash

all_correct=true

cd $(dirname $0)/../..

files=$(find src tests -type f -not -path "src/lib/*" \( -name "*.cpp" -o -name "*.hpp" \))

for file in $files
do
    clang-format-21 -style=file -output-replacements-xml $file | grep -c "<replacement " >/dev/null

    if [ $? -eq 0 ]; then
        echo "formatted incorrectly: $file"
        all_correct=false
    fi
done

if $all_correct
then
    echo "All files formatted correctly."
else
    exit 1
fi
