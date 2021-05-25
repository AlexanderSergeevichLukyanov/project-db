#include "mytest.h"
#include "pairing_heap.h"
#include "head.h"
#include <queue>

TEST_CASE("#1 -- without-comp"){
	pairing_heap<Head<int>, HeadCompare<std::less<int>>> ph;
	for(int j=0; j<7; ++j){
		Head<int> h;
		for(int i=0; i<3; ++i){
			h.add(j*3+i);
		}
		ph.insert(h);
	}
	for(int j=0; j<7; ++j){
		CHECK(ph.getMin().data[0]==3*j);
		ph.extractMin();
	}
}
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
	Head<int, compa2> h(compa2(0));
	h.add(5), h.add(3);
	ph.insert(h);
	Head<int, compa2> h2(compa2(0));
	h2.add(0), h2.add(8);
	ph.insert(h2);
	Head<int, compa2> h3(compa2(0));
	h3.add(1), h3.add(3), h3.add(7);
	ph.insert(h3);
	CHECK_MESSAGE(ph.getMin().data[0]==0, "not "+std::to_string(ph.getMin().data[0]));
	ph.extractMin();
	CHECK_MESSAGE(ph.getMin().data[0]==1, "not "+std::to_string(ph.getMin().data[0]));
	ph.extractMin();
	CHECK_MESSAGE(ph.getMin().data[0]==3, "not "+std::to_string(ph.getMin().data[0]));
	ph.extractMin();
}

void add(std::priority_queue<int> &pq, pairing_heap<Head<int, std::less<int>>, HeadCompare<std::less<int>>> &ph){
	int x = rand();
	int y = rand();
	int z = rand();
	pq.push(x);
	pq.push(y);
	pq.push(z);
	Head<int, std::less<int>> h;
	h.add(x), h.add(y), h.add(z);
	ph.insert(h);
}

bool check_get(std::priority_queue<int> &pq, pairing_heap<Head<int, std::less<int>>, HeadCompare<std::less<int>>> &ph){
	int x = pq.top();
	int y = ph.getMin().data[0];
	return (x==y);
}

TEST_CASE("stress test with priority_queue: 10^6 operations"){
	std::priority_queue<int> pq;
	pairing_heap<Head<int, std::less<int>>, HeadCompare<std::less<int>>> ph;
	for(int i = 0; i<1'000'000; ++i){
		int r = rand() % 3;
		if(i==0 or r>0 or !pq.empty()){ //с большей вероятностью push
			add(pq, ph);
			REQUIRE(pq.size()/3==ph.size());
		} else{
			REQUIRE_MESSAGE(check_get(pq, ph), "#"+std::to_string(i)+": minimums are not equal!");
			REQUIRE(pq.size()/3==ph.size());
		}
	}
}