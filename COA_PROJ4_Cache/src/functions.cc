#include "class_cache.h"

using namespace std;

uint32_t char_to_hex(char s)
{
    uint32_t res ;
    switch (s)
    {
    case '0':   res=0;  break;
    case '1':   res=1;  break; 
    case '2':   res=2;  break;  
    case '3':   res=3;  break;  
    case '4':   res=4;  break;  
    case '5':   res=5;  break;  
    case '6':   res=6;  break;  
    case '7':   res=7;  break;  
    case '8':   res=8;  break;  
    case '9':   res=9;  break;  
    case 'a':   res=10;  break;  
    case 'b':   res=11;  break;  
    case 'c':   res=12;  break;  
    case 'd':   res=13;  break;  
    case 'e':   res=14;  break;  
    case 'f':   res=15;  break;     
    default:
        res = 0xf << 28;
        break;
    }
    return res;
}
char hex_to_char(uint32_t num)
{
    char res;
    switch (num)
    {
    case 0: res = '0';break;
    case 1: res = '1';break;
    case 2: res = '2';break;
    case 3: res = '3';break;
    case 4: res = '4';break;
    case 5: res = '5';break;
    case 6: res = '6';break;
    case 7: res = '7';break;
    case 8: res = '8';break;
    case 9: res = '9';break;
    case 10: res = 'a';break;
    case 11: res = 'b';break;
    case 12: res = 'c';break;
    case 13: res = 'd';break;
    case 14: res = 'e';break;
    case 15: res = 'f';break;
    default: res = '@';
        break;
    }
    return res;
}
string tag_to_string(uint32_t tag,int s,int b)
{
    string res = "";
    int rest = 32 - s- b;
    int len;
    if (rest % 4 == 0)
        len = rest / 4;
    else 
        len = (rest / 4) + 1;
    uint32_t t = 0xf;
    for(int i=0;i<len;i++)
    {
        uint32_t tmp = t & tag;
        tmp = tmp >> (4* i);

        char now = hex_to_char(tmp);
        res = now + res;

        t = t<<4;
    }
    return res;
}
Cache::Cache(int  in_block_size,int in_size,int in_assoc, int in_replacement_policy,int in_write_policy)
{
    block_size =  in_block_size;                                                      
    size               = in_size;
    assoc           = in_assoc;
    replacement_policy =  in_replacement_policy;
    write_policy  = in_write_policy;
    next = nullptr;

    int  S = size / (block_size * assoc);
    s =  log(S) /  log(2);
    b = log(block_size) / log(2);
   //int   t  =  32 - s - b;
    //int  single_long = 1 + t ;

    read_num = 0;
    read_miss = 0;
    write_num = 0;
    write_miss = 0;
    to_memory_num  = 0;

    containt = new  uint32_t*[size/block_size];
    for(int i=0;    i<(size/block_size);    i++){
        containt[i] = new uint32_t[4];                          // invild   tag     dirty   reference_num 
    }
	
	count_set = new uint32_t[S];
	for(int i = 0;i < S;i++){
		count_set[i] = 0;
	}

    //LRU:  0               LFU:    1
    if(replacement_policy == 0)
        read_func = &Cache::LRU;
    else
    {
        read_func = &Cache::LFU;
    }
    //WBWA: 0           WTNA:   1
    if(write_policy == 0){
        write_func = &Cache::WBWA;
    }else
    {
        write_func = &Cache::WTNA;
    }
}

Cache::~Cache()
{
    /*
        for(int i=0;    i<size / block_size ;   i++){
                delete  []  containt[i];
        }
        delete  []  containt;
    */
}
void Cache::deal_file(string    file_name)
{
    fstream  trace_file;
    trace_file.open(file_name);
    if(!trace_file.is_open()){
         cout<<"file doesn't open"<<endl;
         return;
    }

    string  request;
    while (getline(trace_file,request))
    {
        int len = request.length();
        uint32_t address=0;
        for(int i=2;i<len;i++){
            address+= char_to_hex(request[i]);
            if(i!=len-1)
                address = address<<4;
            else
                break;
        }
        if(request[0]=='r')
            readFromAddress(address);
        else
            writeToAddress(address);
    }
    trace_file.close();   
    return;
}

bool Cache::readFromAddress(unsigned int add)
{
    uint32_t  tag_containt  =   add>>(s+b);
    uint32_t  index_containt    =   (add<<(32-s-b))>>(32-s);
    return  (this->*read_func)(index_containt,tag_containt,false);
}

bool Cache::writeToAddress(unsigned int add)
{
    uint32_t  tag_containt  =   add>>(s+b);
    uint32_t  index_containt    =   (add<<(32-s-b))>>(32-s);
    return  (this->*write_func)(index_containt,tag_containt);
}

bool Cache::LRU(uint32_t index, uint32_t tag, bool fromWrite)       // 0
{
    bool hit = 0;// hit = 1 待写入地址数据在cache中
    int indexOfMaxRef = 0;
    int available = -1;// 欲使用的记录序号[0~assoc];available = -1 无可用空闲块
    uint32_t set = index * assoc;
    for(int i = 0;i < assoc;i++)
    {
	if(containt[set + i][0] == 1 && containt[set + i][1] == tag)
	{
	    if(containt[set + i][1] == tag)
	    {
		hit = 1;
		available = i;
		break;
	    }
	    if(containt[set + i][3] > indexOfMaxRef)
	    {
		indexOfMaxRef = i;
	    }
	}
	else
	{
	    available = i;
	}
    }
    
    if(hit == 1)
    {// 命中
	for(int i = 0;i < assoc;i++)
	{
	    if(containt[set + i][3] < containt[set + available][3]) containt[set + i][3] += 1;
	}
	containt[set + available][3] = 0;
	return true;
    }
    else if(available != -1)
    {// 不命中,但有空闲块
	for(int i = 0;i < assoc;i++)
	{
	    containt[set + i][3] += 1;
	}

	containt[set + available][0] = 1;// invalid
	containt[set + available][1] = tag;// tag
	containt[set + available][2] = 0;// dirty
	containt[set + available][3] = 0;// reference_num

	read_miss += 1;

	return true;
    }
    else
    {// 不命中,且无空闲块
	containt[set + indexOfMaxRef][1] = tag;
	containt[set + available][2] = 0;// dirty
	containt[set + available][3] = 0;// reference_num

	if(!fromWrite) read_miss += 1;
	return true;
    }
    return true;
}

bool Cache::LFU(uint32_t index, uint32_t tag, bool fromWrite)           //1
{
	bool hit = 0;
	int available = -1;
	uint32_t set = index*assoc;
	for(int i = 0;i < assoc;i++){
		if(containt[set + i][0] == 1 && containt[set + i][1] == tag){
			hit = 1;
			available = i;
			break;
		}else if(containt[set + i][0] == 0){
			available = i;
		}
	}
	
	if(hit == 1){
		containt[set + available][3] += 1;
	}else{
		if(available >= 0){
			containt[set + available][0] = 1;
			containt[set + available][1] = tag;
			containt[set + available][2] = 0;
			containt[set + available][3] = count_set[index];
		}else{
			int mini = 0;
			for(int i = 1;i < assoc;i++){
				if(containt[set + i][3] < containt[set + mini][3]){
					mini = i;
				}
			}
			count_set[index] = containt[set + mini][3];
			containt[set + mini][0] = 1;
			containt[set + mini][1] = tag;
			containt[set + mini][2] = 0;
			containt[set + mini][3] = count_set[index];
		}
		if(!fromWrite)
			read_miss += 1;
		else 
			write_miss += 1;
	}
    return true;
}

bool Cache::WBWA(uint32_t index, uint32_t tag)                  //0
{
    bool hit = 0;// hit = 1 待写入地址数据在cache中
    int available = -1;// 欲使用的记录序号[0~assoc];available = -1 无可用空闲块
    uint32_t set = index * assoc;
    for(int i = 0;i < assoc;i++)
    {
	if(containt[set + i][0] == 1 && containt[set + i][1] == tag)
	{
	    if(containt[set + i][1] == tag)
	    {
		hit = 1;
		available = i;
		break;
	    }
	}
	else
	{
	    available = i;
	}
    }

    if(hit == 1)
    {// 命中
	    containt[set + available][2] = 1;// dirty
	for(int i = 0;i < assoc;i++)
	{
	    if(containt[set + i][3] < containt[set + available][3]) containt[set + i][3] += 1;
	}
	containt[set + available][3] = 0;
	return true;
    }
    else if(available != -1)
    {// 不命中,但有空闲块
	for(int i = 0;i < assoc;i++)
	{
        containt[set + i][3] += 1;
	}

	write_miss += 1;

	containt[set + available][0] = 1;// invalid
	containt[set + available][1] = tag;// tag
	containt[set + available][2] = 1;// dirty
	containt[set + available][3] = 0;// reference_num
	return true;
    }
    else
    {// 不命中,且无空闲块
	write_miss += 1;
	bool flag  =    (this->*read_func)(index,tag,true);
	return WBWA(index, tag);
    }
}

bool Cache::WTNA(uint32_t index, uint32_t tag)                  //1
{
    return true;
}

void Cache::printcontaint()
{
    cout<<endl;
    for(int i=0;i<pow(2,s);i++){
        cout<<"set   "<<i<<":";
        string res_tag = "";
        for(int j=0;j<assoc;j++){
            res_tag += "    ";
            int index = i*assoc+j;
            string tmp = tag_to_string(containt[index][1],s,b);
            res_tag += tmp;

            if (containt[index][2]==1)
                res_tag += " D";
        }
        cout<<res_tag<<endl;
    }
    return;
}
