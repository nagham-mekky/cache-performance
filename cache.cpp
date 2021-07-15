#include <iostream>
#include  <iomanip>
#include <vector>
#include <cmath>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024) //*1024 TC- 64*1024
#define		CACHE_SIZE		(64*1024) //64*1024 --> Test case: 64 TC- 64
#define		CACHE_LINE_SIZE (16) //Variable 16-128 TC- 16
#define		NUM_WAYS		(2) //1-32 TC- 2

int address_bits = log2(DRAM_SIZE); //number of address bits
int offset_bits = log2(CACHE_LINE_SIZE); //number of offset bits
int num_lines = CACHE_SIZE / CACHE_LINE_SIZE; //number of total lines
int num_sets = num_lines / NUM_WAYS; // number of sets in the cache
int set_bits = log2(num_sets); //number of bits representing the sets
int tag_bits = address_bits - (set_bits + offset_bits); //tag bits

struct line { //initialize all lines with -1
	int tag = -1; 
	int set = -1;
	bool valid = -1;
};

vector<int> counter(num_sets, 0); //counts number of elements per set
vector<line> r(NUM_WAYS);
vector<vector<line>> cache(num_sets, r); //2D vector representation of sets

int arrTest[15] = { 25, 408, 412, 12, 29, 123, 789, 75, 65, 47, 99, 98, 119, 413, 30 }; //iteration size

enum cacheResType { MISS = 0, HIT = 1 }; //same as a T/F in boolean

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

unsigned int memGenTestCase()
{
	static unsigned int addr = 0;
	return arrTest[addr++];
}

// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
	// This function accepts the memory address for the memory transaction and 
	// returns whether it caused a cache miss or a cache hit
	line temp; //save address here 

	int mask1 = pow(2, set_bits) - 1; //generates a mask that has set_bits amount of 1's in binary
	temp.set = ((addr >> offset_bits) & mask1); //shifts to get set number from the address using mask
	temp.tag = (addr >> (offset_bits + set_bits)); //shifts set and offset. rest of address is tag
	temp.valid = 1; //validity bit

	int ii = 0; //iterator counter cant be used for vector access
	for (auto i = cache[temp.set].begin(); i != cache[temp.set].end(); i++) {
		if (temp.set == cache[temp.set][ii].set && temp.tag == cache[temp.set][ii].tag && temp.valid) {
			return HIT;
		}
		ii++;
	}

	if (counter[temp.set] < NUM_WAYS) { //counter cant exceed number of elements per set 
		cache[temp.set].push_back(temp);
		counter[temp.set]++;
	}
	else
	{
		int random = rand() % NUM_WAYS;
		cache[temp.set][random] = temp; //overwrite random cache lines when set is full
	}

	return MISS; //if code reached all the way here then it is a miss
}


char* msg[2] = { (char*)"Miss",(char*)"Hit" }; //output message

#define		NO_OF_Iterations	100		// Change to 1,000,000 TC- 15
int main()
{
	double hit = 0; //initialize hit counter 
	cacheResType r; //miss or hit

	unsigned int addr; //input from generator
	cout << "Cache Simulator\n";

	for (int inst = 0; inst < NO_OF_Iterations; inst++)
	{ //TC- MEMGENTEST
		addr = memGenC(); //generates as long as the loop loops
		//leaving the function does not cause counter to repeat "static"
		r = cacheSimDM(addr); //calls this function to figure miss or hit
		if (r == HIT) hit++; //counter increments
		cout << "0x" << setfill('0') << setw(8) << hex << addr << dec << " (" << msg[r] << ")\n"; //output address in hex and ratio in dec
	}
	cout << "Hit ratio = " << (100 * hit / NO_OF_Iterations) << endl; //output hit ratio
}
//{"mode":"full", "isActive" : false}
