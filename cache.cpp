#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h> 

using namespace std;

#define		DBG				1 //DBG = num of ways
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)
#define		CACHE_LINE_SIZE (16)

int blocknum = CACHE_SIZE / CACHE_LINE_SIZE;
int offset_bits = log2(CACHE_LINE_SIZE); //check if log2 works correctly //should i be using cache line size or blocknum
int set_bits = log2((CACHE_SIZE / (CACHE_LINE_SIZE * DBG))); //DBG??? IS IT SET#
int set_num = (CACHE_SIZE / (CACHE_LINE_SIZE * DBG));
int tag_bits = CACHE_LINE_SIZE - offset_bits - set_bits;
enum cacheResType { MISS = 0, HIT = 1 };

struct line
{
	bool valid = 0;
	int setnum = -1;
	int tagnum = -1;
	//int offset = -1;
	//int address = -1;
};

//line* cache = new line[blocknum];
vector<line> cache(CACHE_SIZE);
vector<int> counter(set_num, 0);
//vector<line> cache;

//void readAddress(unsigned int addr) {
//	line temp;
//
//	int mask = pow(2, tag_bits) - 1;
//	temp.setnum = ((addr >> offset_bits) & 1);
//	temp.tagnum = ((addr >> (offset_bits + set_bits)) & mask);
//	temp.valid = 1;
//
//	cache.push_back(temp);
//}

/* The following implements a random number generator */
unsigned int m_w = 0xABCCAB99;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0xDEAD6902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGenA()
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE);
}

unsigned int memGenB()
{
	static unsigned int addr = 0;
	return  rand_() % (64 * 1024);
}

unsigned int memGenC()
{
	static unsigned int a1 = 0, a0 = 0;
	a0++;
	if (a0 == 512) { a1++; a0 = 0; }
	if (a1 == 128) a1 = 0;
	return(a1 + a0 * 128);
}

unsigned int memGenD()
{
	return  rand_() % (16 * 1024);
}

unsigned int memGenE()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 64);
}

unsigned int memGenF()
{
	static unsigned int addr = 0;
	return (addr += 64) % (64 * 4 * 1024);
}


// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
	// This function accepts the memory address for the memory transaction and 
	// returns whether it caused a cache miss or a cache hit
	line temp;

	int mask = pow(2, tag_bits) - 1;
	temp.setnum = ((addr >> offset_bits) & 1);
	temp.tagnum = ((addr >> (offset_bits + set_bits)) & mask);
	temp.valid = 1;

	if (counter[temp.setnum] < DBG)
		counter[temp.setnum]++;
	/*else
	{
		while ()
	}*/

	cache.push_back(temp);
	
	return MISS;
}


char msg[2] = { 'M','H' };

#define		NO_OF_Iterations	100		// CHange to 1,000,000
int main()
{
	unsigned int hit = 0;
	cacheResType r;

	unsigned int addr;
	cout << "Cache Simulator\n";

	for (int inst = 0; inst < NO_OF_Iterations; inst++)
	{
		addr = memGenB();
		//readAddress(addr);
		r = cacheSimDM(addr);
		if (r == HIT) hit++;
		cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "Hit ratio = " << (100 * hit / NO_OF_Iterations) << endl;
}
//{"mode":"full", "isActive" : false}
