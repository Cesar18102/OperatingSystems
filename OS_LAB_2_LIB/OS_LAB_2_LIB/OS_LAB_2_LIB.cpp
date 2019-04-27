// OS_LAB_2_LIB.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "OS_LAB_2_LIB.h"
#include "Auxilary.h"
#include <iostream>

EIAPI unsigned GetEncryptAndDecryptKeys(unsigned* E, unsigned* D) {

	const unsigned min = 2;
	const unsigned max = 256;

	unsigned p = RandPrime(min, max);
	unsigned q = RandPrime(min, max, p);

	unsigned n = p * q;
	unsigned eu = EulerFunc(p, q);

	*E = RandInterPrime(min, max * max, eu);
	for(*D = 3; (*D * *E) % eu != 1; *D += 1);

	return n;
}

EIAPI unsigned Encrypt(TCHAR* Message, unsigned E, unsigned n, unsigned* EncryptedMessage) {

	int L = Length(Message);

	for(int i = 0; i < L; i++)
		EncryptedMessage[i] = PowModEnc(Message[i], E, n);

	return L;
}

EIAPI void Decrypt(unsigned* Message, unsigned L, unsigned D, unsigned n, TCHAR* DecryptedMessage) {

	for(int i = 0; i < L; i++)
		DecryptedMessage[i] = PowModDec(Message[i], D, n);

	DecryptedMessage[L] = _T('\0');
}