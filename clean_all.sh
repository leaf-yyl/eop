#!/bin/bash

if [ $# -gt 0 ]; then
    if [[ $* =~ "eol" ]]; then
        rm -rf build/eol
    fi
    if [[ $* =~ "test" ]]; then
        rm -rf build/test
    fi
else if [ -d build ]; then
    rm -rf build
fi
fi

