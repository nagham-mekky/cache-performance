#include <iostream>
#include  <iomanip>
#include <vector>
#include <cmath>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)
#define		CACHE_LINE_SIZE (32) //Variable 16-128
#define		NUM_WAYS		(4) //1-32

int address_bits = log2(DRAM_SIZE);
int offset_bits = log2(CACHE_LINE_SIZE);
int num_lines = CACHE_SIZE / CACHE_LINE_SIZE;
int num_sets = num_lines / NUM_WAYS;
int set_bits = log2(num_sets);
int tag_bits = address_bits - (set_bits + offset_bits);

struct line {
	int tag;
	int set;
	bool valid;
};

vector<int> counter(num_sets, 0); //set elements counter
vector<line> r(NUM_WAYS);
vector<vector<line>> cache(num_sets, r);

enum cacheResType { MISS = 0, HIT = 1 };

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

	int mask1 = pow(2, set_bits) - 1;
	temp.set = ((addr >> offset_bits) & mask1);
	temp.tag = (addr >> (offset_bits + set_bits));
	temp.valid = 1;

	int ii = 0;
	for (auto i = cache[temp.set].begin(); i != cache[temp.set].end(); i++) {
		if (temp.set == cache[temp.set][ii].set && temp.tag == cache[temp.set][ii].tag && temp.valid) {
			return HIT;
		}
		ii++;
	}

	if (counter[temp.set] < NUM_WAYS) {
		cache[temp.set].push_back(temp);
		counter[temp.set]++;
	}
	else
	{
		auto del = cache[temp.set].begin() + (rand() % NUM_WAYS);
		del = cache[temp.set].erase(del);
		cache[temp.set].push_back(temp);
	}
	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}


char* msg[2] = { (char*)"Miss",(char*)"Hit" };

#define		NO_OF_Iterations	100		// CHange to 1,000,000
int main()
{
	unsigned int hit = 0;
	cacheResType r;

	unsigned int addr;
	cout << "Cache Simulator\n";

	for (int inst = 0; inst < NO_OF_Iterations; inst++)
	{
		addr = memGenF();
		r = cacheSimDM(addr);
		if (r == HIT) hit++;
		cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "Hit ratio = " << (100 * hit / NO_OF_Iterations) << endl;
}
//{"mode":"full", "isActive" : false}
