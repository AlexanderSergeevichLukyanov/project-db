#include "head.h"
#include "mytest.h"
TEST_CASE("Create without compare"){
	Head<int> h(80);
}

struct compa{
	bool operator()(int a1, int a2){
		return (std::abs(a1)<std::abs(a2));
	}
};

struct compa2{
	int x;
	compa2(int x_): x(x_){
	}
	
	bool operator()(int a1, int a2){
		return (std::abs(a1-x)<std::abs(a2-x));
	}
};

TEST_CASE("Create with compare"){
	Head<int, compa> h(100);
	compa2 t = compa2(10);
	//HeadCompare<compa2> tt(t);
	Head<int, compa2> h2(100, t);
}

TEST_CASE("add & size"){
	Head<int> h(5);
	CHECK(h.empty());
	CHECK(h.size()==0);
	h.add(1);
	CHECK(!h.empty());
	CHECK(h.size()==1);
	CHECK(h[0]==1);
	h.add(-1);
	CHECK(h[0]==-1);
	CHECK(h.size()==2);
	CHECK(h[1]==1);
	h.add(10);
	CHECK(h.size()==3);
	CHECK(h[2]==10);
	CHECK(h[0]==-1);
	CHECK(h[1]==1);
}

TEST_CASE("extract"){
	Head<int> h(10);
	h.add(1);
	h.add(2);
	h.extract();
	CHECK(h.size()==1);
	CHECK(h[0]==2);
	h.add(-1);
	CHECK(h[0]==-1);
	h.add(0);
	h.extract();
	CHECK(h[0]==0);
}