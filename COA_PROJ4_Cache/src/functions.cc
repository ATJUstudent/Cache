#include "class_cache.h"


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
        break;
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
    int   s =  log(S) /  log(2);
    int   b = log(block_size) / log(2);
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

    //LRU:  0               LFU:    1
    if(replacement_policy == 0)
        replace_func = &LRU;
    else
    {
        replace_func = &LFU;
    }
    //WBWA: 0           WTNA:   1
    if(write_policy == 0){
        write_func = &WBWA;
    }else
    {
        write_func = &WTNA;
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
    return true;   
}

bool Cache::writeToAddress(unsigned int add)
{
    return true;
}

bool Cache::LRU()
{
    return true;
}

bool Cache::LFU()
{
    return true;
}

bool Cache::WBWA()
{
    return true;
}

bool Cache::WTNA()
{
    return true;
}

void Cache::printcontaint()
{
    return;
}