#include "block.h"
#include "mytest.h"
#include <type_traits>
const std::string folder_name = "ph-data";//  заглушка
std::size_t block_counter = 0;

TEST_CASE("check fold ph-data!"){
	Block_t<int, 10> b;
	for(int i=0; i<10; ++i){
		b.add(i*2);
	}
	block_counter=3;
	b.WRITE(folder_name, block_counter);
}

TEST_CASE("small-auto"){
	Block_t<int, 10> b;
	for(int i=0; i<10; ++i){
		b.add(i*2);
	}
	block_counter=1;
	b.WRITE(folder_name, block_counter);
	Block_t<int, 10> b2;
	b2.READ(folder_name, block_counter);
	for(int i=0; i<10; ++i){
		CHECK(b2[i]==i*2);
	}
}

TEST_CASE("big-auto"){
	Block_t<int, 1000> b;
	for(int i=0; i<1000; ++i){
		b.add(i*2);
	}
	block_counter=2;
	b.WRITE(folder_name, block_counter);
	Block_t<int, 1000> b2;
	b2.READ(folder_name, block_counter);
	for(int i=0; i<1000; ++i){
		CHECK(b2[i]==i*2);
	}
}