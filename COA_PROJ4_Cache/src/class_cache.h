#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>

using namespace std;

class Cache
{
private:
    int block_size;
    int  size;
    int  assoc;
    int  replacement_policy;
    int  write_policy;
    uint32_t** containt;
    Cache *next;

    
public:
    int     read_num;
    int     write_num;
    int     read_miss;
    int     write_miss;
    int     to_memory_num;

    bool   (*replace_func)();
    bool   (*write_func)();

    Cache(int  in_block_size,int in_size,int in_assoc, int in_replacement_policy,int in_write_policy);
    ~Cache();

    bool    readFromAddress(unsigned int add);
    bool    writeToAddress(unsigned int add);
    void     deal_file(string   file_name);
    void     printcontaint();
    
    static bool    LRU();
    static bool    LFU();
    static bool    WBWA();
    static bool    WTNA();
};



