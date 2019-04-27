#include "stdafx.h"
#include "Auxilary.h"

#include <math.h>
#include <random>
#include <ctime>

bool IsPrime(unsigned n) {

	if(n == 2)
		return true;

	if(n == 1 || n % 2 == 0)
		return false;

	unsigned Sqrt = (unsigned)sqrt((double)n);

	for(unsigned i = 3; i <= Sqrt; i += 2)
		if(n % i == 0)
			return false;

	return true;
}

unsigned GreatestCommonDevider(unsigned n, unsigned q) {

	while(n && q) {

		if(n > q)
			n %= q;
		else
			q %= n;
	}

	return n + q;
}

bool AreInterPrime(unsigned n, unsigned q) {

	return GreatestCommonDevider(n, q) == 1;
}

unsigned RandInt(unsigned min, unsigned max) {

	srand(time(0));
	return min + (rand() % (max - min));
}

unsigned NearestPrime(unsigned min, unsigned max, unsigned n) {

	for(int i = 1; ; i++)
		if(n - i >= min && IsPrime(n - i))
			return n - i;
		else if(n + i < max && IsPrime(n + i))
			return n + i;

	return 2;
}

unsigned NearestInterPrime(unsigned min, unsigned max, unsigned n, unsigned co) {

	for(unsigned i = 1; ; i++)
		if(n - i >= min && AreInterPrime(n - i, co))
			return n - i;
		else if(n + i < max && AreInterPrime(n + i, co))
			return n + i;

	return 1;
}

unsigned RandPrime(unsigned min, unsigned max) {

	return NearestPrime(min, max, RandInt(min, max));
}

unsigned RandPrime(unsigned min, unsigned max, unsigned neq) {

	unsigned n = NearestPrime(min, max, RandInt(min, max));

	while(n == neq)
		n = NearestPrime(min, max, RandInt(min, max));

	return n;
}

unsigned RandInterPrime(unsigned min, unsigned max, unsigned co) {

	return NearestInterPrime(min, max, RandInt(min, max), co);
}

unsigned EulerFunc(unsigned n, unsigned q) {

	return (n - 1) * (q - 1);
}

unsigned PowModEnc(TCHAR x, unsigned y, unsigned m) {

	m %= TCHARMAX;
	unsigned result = 1;

	for(unsigned i = 0; i < y; i++)
		result = (result * x) % m;

	return result;
}

TCHAR PowModDec(unsigned x, unsigned y, unsigned m) {

	m %= TCHARMAX;
	TCHAR result = 1;

	for(unsigned i = 0; i < y; i++)
		result = (result * x) % m;
	
	return result;
}