#include "stdafx.h"
#include "SEAL.h"

u32 SEAL::f1(u32 B, u32 C, u32 D)
{
	               //возможно нужно ~B
	return (B & C) | (B & D);
}

u32 SEAL::f2(u32 B, u32 C, u32 D)
{
	return B ^ C ^ D;
}

u32 SEAL::f3(u32 B, u32 C, u32 D)
{
	return (B & C) | (B & D) | (C & D);
}

u32 SEAL::f4(u32 B, u32 C, u32 D)
{
	return B ^ C ^ D;
}

u32 SEAL::word(string h,int i)
{
	u32 n = 0;
	for (int j = 0; j < 4; j++, i++)
	{
		n <<= 8;
		n |= h[i] & 0x000000FF;
	}
	return n;
}

//циклический свдиг
u32 SEAL::rol(u32 number, int shift)
{
	return (number >> shift) | (number << (32 - shift));
}

SEAL::SEAL()
{
	//
}
void SEAL::init(string key)
{
	u32 h[5];
	for (int i = 0; i < 510; i += 5)
		G(key, i / 5, &T[i]);

	G(key, 510, h);
	for (int i = 510; i < 512; i++)
		T[i] = h[i - 510];
	        
	G(key, (-1 + 0x1000), h);

	for (int i = 0; i < 4; i++)
		S[i] = h[i + 1];

	for (int i = 4; i < 254; i += 4)
		G(key, (i + 0x1000) / 5, h);
	
	G(key, (254 + 0x1000), h);
	for (int i = 254; i < 256; i++)
		S[i] = h[i - 254];

	G(key, (-2 + 0x2000), h);
	for (int i = 0; i < 3; i++)
		R[i] = h[i + 2];

	for (int i = 3; i < 13; i += 5)
		G(key, (i + 0x2000) / 5, &R[i]);

	G(key, (13 + 0x2000) / 5, h);
	for (int i = 13; i < 16; i++)
		R[i] = h[i - 13];



}
//string заменить на char
u32 SEAL::gamma(string key, int i)
{
	u32 h[5];
	G(key, i / 5, h);
	return h[i % 5];
}
void SEAL::G(string key,int i, u32* h)
{
	u32 h0, h1, h2, h3, h4;
	u32 a, b, c, d, e;
	u32 w[80];
	u32 tmp;

	h0 = word(key, 0);
	h1 = word(key, 4);
	h2 = word(key, 8);
	h3 = word(key, 12);
	h4 = word(key, 16);

	w[0] = i;
	for (i = 1; i < 16; i++)
		w[i] = 0;

	for (i = 16; i < 80; i++)
		w[i] = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];

	a = h0;
	b = h1;
	c = h2;
	d = h3;
	e = h4;

	for (i = 0; i < 20; i++)
	{
		tmp = rol(a, 27) + f1(b, c, d) + e + w[i] + K1;
		e = d;
		d = c;
		c = rol(b, 2);
		b = a;
		a = tmp;
	}

	for (i = 20; i < 40; i++)
	{
		tmp = rol(a, 27) + f2(b, c, d) + e + w[i] + K2;
		e = d;
		d = c;
		c = rol(b, 2);
		b = a;
		a = tmp;
	}

	for (i = 40; i < 60; i++)
	{
		tmp = rol(a, 27) + f3(b, c, d) + e + w[i] + K3;
		e = d;
		d = c;
		c = rol(b, 2);
		b = a;
		a = tmp;
	}

	for (i = 60; i < 80; i++)
	{
		tmp = rol(a, 27) + f3(b, c, d) + e + w[i] + K3;
		e = d;
		d = c;
		c = rol(b, 2);
		b = a;
		a = tmp;
	}
	h[0] = h0 + a;
	h[1] = h1 + b;
	h[2] = h2 + c;
	h[3] = h3 + d;
	h[4] = h4 + e;

}

int SEAL::seal(u32 in, u32* out)
{
	u32 a, b, c, d;
	u32 n1, n2, n3, n4;
	u32* wp = out;
	//константа нужная для вычислений
	const u32 FC = 0x7fc;
	//сделать p unsigned short
	u32 p, q;
	for (int l = 0; l < 4; l++)
	{
		a = in ^ R[4 * l];
		b = rol(in, 8) ^ R[4 * l + 1];
		c = rol(in, 16) ^ R[4 * l + 2];
		d = rol(in, 24) ^ R[4 * l + 3];

		for (int j = 0; j < 2; j++)
		{
			p = a & FC;
			b += T[p / 4];
			a = rol(a, 9);

			p = b & FC;
			c += T[p / 4];
			b = rol(b, 9);

			p = c & FC;
			d += T[p / 4];
			c = rol(c, 9);

			p = d & FC;
			a += T[p / 4];
			d = rol(d, 9);
		}

		n1 = a;
		n2 = b;
		n3 = c;
		n4 = d;

		p = a & FC;
		b += T[p / 4];
		a = rol(a, 9);

		p = b & FC;
		c += T[p / 4];
		b = rol(b, 9);

		p = c & FC;
		d += T[p / 4];
		c = rol(c, 9);

		p = d & FC;
		a += T[p / 4];
		d = rol(d, 9);

		for (int i = 0; i < 64; i++)
		{
			p = a & FC;
			b += T[p / 4];
			a = rol(a, 9);
			b ^= a;

			p = b & FC;
			c += T[p / 4];
			b = rol(b, 9);
			c ^= b;

			p = (p + c) & FC;
			d += T[p / 4];
			c = rol(c, 9);
			d ^= c;

			q = (q + d) & FC;
			a += T[p / 4];
			d = rol(d, 9);
			a ^= d;

			p = (p + a) & FC;
			b += T[p / 4];
			a = rol(a, 9);

			q = (q + b) & FC;
			c += T[p / 4];
			b = rol(b, 9);
			
			p = (p + c) & FC;
			d += T[p / 4];
			c = rol(c, 9);

			q = (q + d) & FC;
			a += T[p / 4];
			d = rol(d, 9);
			
			*wp = b + S[4 * i];
			wp++;

			*wp = c & S[4 * i + 1];
			wp++;

			*wp = d + S[4 * i + 2];
			wp++;

			*wp = a & S[4 * i + 1];
			wp++;
			if (i & 1)
			{
				a += n3;
				c += n4;
			}
			else
			{
				a += n1;
				c += n2;
			}
		}
	}

	return 0;
}
//обнуление 
void SEAL::refill()
{
	seal(counter, sequence);
	counter++;
	sq_pos = 0;
}

void SEAL::encrypt(u32 * data, int size)
{
	for (int i = 0; i < size; i++, sq_pos++)
	{
		if (sq_pos >= sq_size) refill();
		data[i] ^= sequence[sq_pos];
	}
}

void SEAL::key(string key)
{
	init(key);
	counter = 0;
	sq_pos = sq_size;
}

void SEAL::decrypt(u32 * data, int size)
{
	encrypt(data, size);
}

SEAL::~SEAL()
{
}
