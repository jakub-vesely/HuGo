#!/bin/bash
find ./devel/ -iname "*.py" -and -not -path "./__init__.py" | xargs pylint --disable=R,C,I
if [ $? -ne 0 ]; then
  exit 1
fi

python -m unittest discover -v -s test -p test_*.py
if [ $? -ne 0 ]; then
  exit 1
fi
