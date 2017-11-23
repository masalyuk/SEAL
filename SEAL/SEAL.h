#pragma once
#include <string>
using namespace std;
typedef unsigned long u32;
class SEAL
{
private:

	const int  K1 = 0x5a827999; // 0-19 
	const int  K2 = 0x6ed9eba1; // 20-39
	const int  K3 = 0x8f1bbcdc; // 40-59
	const int  K4 = 0xca62c1d6; // 60-79

	const static int sq_size = 1024;
	//последовательность с псевдослучайныеми числами
	u32 sequence[sq_size];
	int counter = 0;
	int sq_pos = 0;

	u32 T[520];
	u32 S[265];
	u32 R[20];

	u32 f1(u32 B, u32 C, u32 D); // 0-19
	u32 f2(u32 B, u32 C, u32 D); // 20-39
	u32 f3(u32 B, u32 C, u32 D); // 40-59
	u32 f4(u32 B, u32 C, u32 D); // 60-79

	u32 rol(u32 number, int shift);
	u32 word(string h,int i);
	void init(string key);
	u32 gamma(string key,int i);
	void G(string key, int i, u32* h);
	int seal(u32 in, u32* out);
	void refill();

public:
	SEAL();
	//взять из blowfish функцию преобразовансие 8chat v u32
	void encrypt(u32* data, int size);
	void key(string key);
	void decrypt(u32* data, int size);
	~SEAL();
};

