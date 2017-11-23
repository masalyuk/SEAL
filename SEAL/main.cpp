// SEAL.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "SEAL.h"
#include <iostream>
using namespace std;

int main()
{
	SEAL s;
	string key = "01234567890123456789";
	s.key(key);
	const int size = 1000;
	u32 buf[size];
	for (int i = 0; i < size; i++)
		buf[i] = 0;
	s.encrypt(buf, size);

	for (int i = 0; i < size; i++)
		cout << buf[i];

	s.key(key);
	s.decrypt(buf, size);

	for (int i = 0; i < size; i++)
		if (buf[i] != 0) cout << "SUKA BLYTA";

    return 0;
}

