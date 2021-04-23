#include "mytest.h"
#include "soft_heap.h"
#include "head.h"
#include <queue>

TEST_CASE("#1 -- without-comp"){
	soft_heap<Head<int>, HeadCompare<std::less<int>>> sh;
	for(int j=0; j<7; ++j){
		Head<int> h;
		for(int i=0; i<3; ++i){
			h.add(j*3+i);
		}
		sh.insert(h);
	}
	for(int j=0; j<7; ++j){
		CHECK(sh.getMin().data[0]==3*j);
		sh.extractMin();
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
	soft_heap<Head<int, compa2>, HeadCompare<compa2>> sh(HeadCompare<compa2>(0));
	Head<int, compa2> h(compa2(0));
	h.add(5), h.add(3);
	sh.insert(h);
	Head<int, compa2> h2(compa2(0));
	h2.add(0), h2.add(8);
	sh.insert(h2);
	Head<int, compa2> h3(compa2(0));
	h3.add(1), h3.add(3), h3.add(7);
	sh.insert(h3);
	CHECK_MESSAGE(sh.getMin().data[0]==0, "not "+std::to_string(sh.getMin().data[0]));
	sh.extractMin();
	CHECK_MESSAGE(sh.getMin().data[0]==1, "not "+std::to_string(sh.getMin().data[0]));
	sh.extractMin();
	CHECK_MESSAGE(sh.getMin().data[0]==3, "not "+std::to_string(sh.getMin().data[0]));
	sh.extractMin();
}

void add(std::priority_queue<int> &pq, soft_heap<Head<int, std::less<int>>, HeadCompare<std::less<int>>> &sh){
	int x = rand();
	int y = rand();
	int z = rand();
	pq.push(x);
	pq.push(y);
	pq.push(z);
	Head<int, std::less<int>> h;
	h.add(x), h.add(y), h.add(z);
	sh.insert(h);
}

bool check_get(std::priority_queue<int> &pq, soft_heap<Head<int, std::less<int>>, HeadCompare<std::less<int>>> &sh){
	int x = pq.top();
	int y = sh.getMin().data[0];
	return (x==y);
}

TEST_CASE("stress test with priority_queue: 10^6 operations"){
	std::priority_queue<int> pq;
	soft_heap<Head<int, std::less<int>>, HeadCompare<std::less<int>>> sh;
	for(int i = 0; i<1'000'000; ++i){
		int r = rand() % 3;
		if(i==0 or r>0 or !pq.empty()){ //с большей вероятностью push
			add(pq, sh);
			REQUIRE(pq.size()/3==sh.size());
		} else{
			REQUIRE_MESSAGE(check_get(pq, sh), "#"+std::to_string(i)+": minimums are not equal!");
			REQUIRE(pq.size()/3==sh.size());
		}
	}
}