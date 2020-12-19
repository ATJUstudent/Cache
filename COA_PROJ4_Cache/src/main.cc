
#include "world.h"
#include "class_cache.h"

int main(int argc, char *argv[]) {
	if(argc != 7)
		cout<<"too less arguments"<<endl;

	int		block_size = atoi(argv[1]);
	int		size = atoi(argv[2]);
	int 	assoc = atoi(argv[3]);
	int 	replacement_policy  =atoi(argv[4]);
	int 	write_policy = atoi(argv[5]);
	string		trace_file = argv[6];
	Cache  *L1  = new  Cache(block_size,size,assoc,replacement_policy,write_policy);

	L1->deal_file(trace_file);
	/*
	下面需要调用打印函数打印结果
	*/
	printf("  ===== Simulator configuration =====\n");
	printf("  %-22s","L1_BLOCKSIZE:");
	printf("%13d\n", block_size);
	printf("  %-22s","L1_SIZE:");
	printf("%13d\n", size);
	printf("  %-22s","L1_ASSOC:");
	printf("%13d\n", assoc);
	printf("  %-22s","L1_REPLACEMENT_POLICY:");
	printf("%13d\n", replacement_policy);
	printf("  %-22s","L1_WRITE_POLICY:");
	printf("%13d\n", write_policy);
	printf("  %-22s","trace_file:");
	printf("%13s\n", trace_file.c_str());
	printf("  ===================================\n\n");
	L1->printcontaint();
	int a = L1->read_num;
	int b = L1->read_miss;
	int c = L1->write_num;
	int d = L1->write_miss;
	float e = (b + d)*1.0 / (a + c);
	int f = L1->to_memory_num;
	int g = write_policy?b+c:b+d+f;

	float ht = 0.25 + 0.25*(size*1.0 / (512*1024)) + 0.025*(block_size / 16) + 0.025*(assoc);
	float mp = 20 + 0.5*(block_size / 16);

	float aat = ht + e*mp;
	printf("\n  ====== Simulation results (raw) ======\n");
	printf("  a. number of L1 reads:          %6d\n", a);
	printf("  b. number of L1 read misses:    %6d\n", b);
	printf("  c. number of L1 writes:         %6d\n", c);
	printf("  d. number of L1 write misses:   %6d\n", d);
	printf("  e. L1 miss rate:                %.4f\n", e);
	printf("  f. number of writebacks from L1:%6d\n", f);
	printf("  g. total memory traffic:        %6d\n", g);

	printf("\n  ==== Simulation results (performance) ====\n");
	printf("  1. average access time:        %2.4f ns",aat);
	printf("%f %f \n", ht,mp);
	return 0;
}
