#!/bin/bash
g++ -std=c++17 src/tests/$1*.cpp mytest/src/mytest.cpp  mytest/src/mytest_main.cpp -I include -I mytest/include -o dbms$1 -Wall -Wextra -Werror
