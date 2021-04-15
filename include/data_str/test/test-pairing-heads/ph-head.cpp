#include "mytest.h"
#include "pairing_heap.h"
#include "head.h"

TEST_CASE("#1 -- without-comp"){
	pairing_heap<Head<int>> ph;
	for(int j=0; j<7; ++j){
		Head<int> h;
		for(int i=0; i<3; ++i){
			h.add(j*3+i);
		}
		ph.insert(h);
	}
	for(int j=0; j<7; ++j){
		REQUIRE(ph.getMin().data[0]==3*j);
		ph.extractMin();
	}
}/*
struct compa2{
	int x;
	compa2(int x_): x(x_){
	}
	
	bool operator()(int a1, int a2){
		return (std::abs(a1-x)<std::abs(a2-x));
	}
};

TEST_CASE("#2 -- with-comp"){
	pairing_heap<Head<int, compa2>, HeadCompare<compa2>> ph(HeadCompare<compa2>(0));
	Head<int, compa2> h(compa2(10));
	h.add(5), h.add(3);
	ph.add(h);
	Head<int, compa2> h2(compa2(10));
	h2.add(0), h2.add(8);
	ph.add(h2);
	Head<int, compa2> h3(compa2(10));
	h3.add(1), h3.add(3), h3.add(7);
	ph.add(h3);
	CHECK(ph.getMin().data[0]==0);
	ph.extractMin();
	CHECK(ph.getMin().data[0]==1);
	ph.extractMin();
	CHECK(ph.getMin().data[0]==3);
	ph.extractMin();
}*/