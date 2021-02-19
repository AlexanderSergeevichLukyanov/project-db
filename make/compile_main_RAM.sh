#!/bin/bash
g++ -std=c++17 src/tests-RAM/*.cpp mytest/src/mytest.cpp  mytest/src/mytest_main.cpp -I include -I mytest/include -o dbms -Wall -Wextra