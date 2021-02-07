#include "pairing_heap.h"
#include "mytest.h"
//#define construct
//#define insert
//#define get_min
//#define make
//#define extract_min
//#define merge
//#define solyanka
TEST_CASE("Pairing-heap: demo-test"){
	CHECK(2*2==5);
}
#ifdef construct
TEST_CASE("Pairing-heap: constructors without Compare"){
	pairing_heap<int> h1;
	pairing_heap<double> h2;
	pairing_heap<std::string> h3;
}

TEST_CASE("Pairing-heap: constructors with Compare"){
	
}
#endif

#ifdef insert
TEST_CASE("Pairing-heap: 30000 insert"){
	pairing_heap<int> h1;
	for(int i=0; i<30000; ++i){
		h1.insert(rand());
	}
	CHECK(h1.size()==30000);
}

TEST_CASE("Pairing-heap: 300000 insert"){
	pairing_heap<int> h1;
	for(int i=0; i<300000; ++i){
		h1.insert(rand());
	}
	CHECK(h1.size()==300000);
}
#endif

#ifdef get_min
TEST_CASE("Pairing-heap: GetMin()"){
	pairing_heap<int> h1;
	int min=300000;
	for(int i=0; i<300000; ++i){
		int x = rand()%300000;
		if(x<min) min=x;
		h1.insert(x);
	}
	CHECK_MESSAGE(h1.getMin()==min, "сгенерированый минимум другой");
}

TEST_CASE("Pairing-heap: 300000 GetMin()"){
	pairing_heap<int> h1;
	int min=300000;
	for(int i=0; i<300000; ++i){
		int x = rand()%300000;
		if(x<min) min=x;
		h1.insert(x);
		CHECK_MESSAGE(h1.getMin()==min, "сгенерированый минимум другой");
		if(h1.getMin()!=min) break;
	}
}
#endif

