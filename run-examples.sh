#!/bin/bash
g++ $1 -I lib/include/heaps/pairing-heap -I lib/include/heaps/soft-heap -I lib/include/heaps/flower-heap -I lib/include/heaps/tower-heap -I lib/mytest/include -I lib/include/data_str -I lib -o $2 -std=c++17
