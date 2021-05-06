#include <iostream>
#include <algorithm>
#include <queue>
#include <time.h>
#include "funnel_heap.h"
#include "pairing_heap.h"
#include "soft_heap.h"
#include "list_heap.h"
#include "buffer_heap.h"
#define priority_queue_
float lh = -1.0, pq = -1.0, sh = -1.0, bh = -1.0, fh = -1.0, ph = -1.0;
void min_max(){
	float min, max;
	min=10000.0;
	max=-1.0;
	if(lh!=-1.0){ if(min>lh){ min=lh; } if(max<lh){ max=lh; } }
	if(pq!=-1.0){ if(min>pq){ min=pq; } if(max<pq){ max=pq; } }
	if(sh!=-1.0){ if(min>sh){ min=sh; } if(max<sh){ max=sh; } }
	if(bh!=-1.0){ if(min>bh){ min=bh; } if(max<bh){ max=bh; } }
	if(fh!=-1.0){ if(min>fh){ min=fh; } if(max<fh){ max=fh; } }
	if(ph!=-1.0){ if(min>ph){ min=ph; } if(max<ph){ max=ph; } }
	
	if(lh!=-1.0){ if(lh==min){ std::cerr<<"\x1b[32m"; } else if(lh==max){ std::cerr<<"\x1b[31m"; } std::cerr<<"    list-heap:      "<<lh<<"s\n\x1b[0m"; }
	if(pq!=-1.0){ if(pq==min){ std::cerr<<"\x1b[32m"; } else if(pq==max){ std::cerr<<"\x1b[31m"; } std::cerr<<"    priority-queue: "<<pq<<"s\n\x1b[0m"; }
	if(sh!=-1.0){ if(sh==min){ std::cerr<<"\x1b[32m"; } else if(sh==max){ std::cerr<<"\x1b[31m"; } std::cerr<<"    soft-heap:      "<<sh<<"s\n\x1b[0m"; }
	if(bh!=-1.0){ if(bh==min){ std::cerr<<"\x1b[32m"; } else if(bh==max){ std::cerr<<"\x1b[31m"; } std::cerr<<"    buffer-heap:    "<<bh<<"s\n\x1b[0m"; }
	if(fh!=-1.0){ if(fh==min){ std::cerr<<"\x1b[32m"; } else if(fh==max){ std::cerr<<"\x1b[31m"; } std::cerr<<"    funnel-heap:    "<<fh<<"s\n\x1b[0m"; }
	if(ph!=-1.0){ if(ph==min){ std::cerr<<"\x1b[32m"; } else if(ph==max){ std::cerr<<"\x1b[31m"; } std::cerr<<"    pairing-heap:   "<<ph<<"s\n\x1b[0m"; }
}

template<class T> 
void inserts(int count, float &time_, T &test_str){
	int time_start = clock();
		for(int i=0; i<count; ++i){
			test_str.insert(rand());
		}
	time_=(float(clock() - time_start)) / CLOCKS_PER_SEC;
}

template<class T> 
void extract_mins(int count, float &time_, T &test_str){
		for(int i=0; i<count+10; ++i){
			test_str.insert(rand());
		}
	int time_start = clock();
		for(int i=0; i<count; ++i){
			test_str.extractMin();
		}
	time_=(float(clock() - time_start)) / CLOCKS_PER_SEC;
}


int main(){
	std::cerr<<"\x1b[33mAfter 1'000'000 insert():\n\x1b[0m";
	const int insert_count=1'000'000;
	{
	#ifdef list_hedsap_
	list_fdheap<int> h;
	inserts(insert_count, lh, h); 
	#endif
	}
	{
	#ifdef pairing_heap_
	pairing_heap<int> h;
	inserts(insert_count, ph, h); 
	#endif
    }
	{
	#ifdef funnel_heap_
	funnel_heap<int> h;
	inserts(insert_count, fh, h); 
	#endif
	}
	{
	#ifdef priority_queue_
	std::priority_queue<int> h;
	int time_start = clock();
		for(int i=0; i<insert_count; ++i){
			h.push(rand());
		}
	pq=(float(clock() - time_start)) / CLOCKS_PER_SEC;
	#endif
	}
	{
	#ifdef buffer_heap_
	buffer_heap<int> h;
	inserts(insert_count, bh, h); 
	#endif
	}
	{
	#ifdef soft_heap_
	soft_heap<int> h;
	inserts(insert_count, sh, h); 
	#endif
	}
	
	min_max();
	
	
	std::cerr<<"\x1b[33mAfter 100'000 extractMin():\n\x1b[0m";
	const int extractMin_count=100'000;
/*	{
	#ifdef list_ffkf_heap_
	list_heap<int> h;
	extract_mins(extractMin_count, lh, h); 
	#endif
	} */
	{
	#ifdef pairing_heap_
	pairing_heap<int> h;
	extract_mins(extractMin_count, ph, h); 
	#endif
    }
	{
	#ifdef funnel_heap_
	funnel_heap<int> h;
	extract_mins(extractMin_count, fh, h); 
	#endif
	}
	{
	#ifdef priority_queue_
	std::priority_queue<int> h;
	
		for(int i=0; i<extractMin_count; ++i){
			h.push(rand());
		}
	int time_start = clock();
		for(int i=0; i<extractMin_count; ++i){
			h.pop();
		}
	
	pq=(float(clock() - time_start)) / CLOCKS_PER_SEC;
	#endif
	}
	{
	#ifdef buffer_heap_
	buffer_heap<int> h;
	extract_mins(extractMin_count, bh, h); 
	#endif
	}
	{
	#ifdef soft_heap_
	soft_heap<int> h;
	extract_mins(extractMin_count, sh, h); 
	#endif
	}
	
	min_max();
}