#pragma once
#include <string>
using namespace std;
class SEAL
{
private:
	string key;

	int  K1 = 0x5a827999;
	int  K2 = 0x6ed9eba1;
	int  K3 = 0x8f1bbcdc;
	int  K4 = 0xca62c1d6;

	unsigned long T[520];
	unsigned long S[265];
	unsigned long R[20];
	

public:
	SEAL(string key);
	void init();
	~SEAL();
};

