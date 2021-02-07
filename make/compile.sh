#!/bin/bash
g++ src/tests/$1.cpp mytest/src/mytest.cpp  mytest/src/mytest_main.cpp -I include -I mytest/include -o dbms
