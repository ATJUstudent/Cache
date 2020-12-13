
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
	return 0;
}
