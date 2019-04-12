#include "stdafx.h"
#include "BigInt.h"

BigInt BigInt::GetBigInt(TCHAR* ds) {

	int L = Length(ds);
	short* a = new short[L];

	for(int i = 0; i < L; i++)
		a[i] = ds[i] - 48;

	return BigInt(a, L);
}

BigInt BigInt::ReadBigInt() {

	TCHAR* a = new TCHAR[256];
	Read("%s", a);
	return GetBigInt(a);
}

BigInt::BigInt(short* digits, long long len) {

	this->digits = new short[len];
	this->digitsCount = len;

	for(int i = 0; i < len; i++)
		this->digits[i] = digits[i];
}

BigInt::~BigInt(void) {

}

BigInt BigInt::operator + (BigInt B) {

	int MaxDigitsCount = (digitsCount > B.digitsCount? digitsCount : B.digitsCount) + 1;
	short* sumDigits = new short[MaxDigitsCount];

	for(int i = 0; i < MaxDigitsCount; i++)
		sumDigits[i] = 0;

	for(int i = 0; i < MaxDigitsCount; i++) {

		int sum = sumDigits[i] + (i >= digitsCount? 0 : digits[i]) + (i >= B.digitsCount? 0 : B.digits[i]);
		int extra = sum / 10;

		sumDigits[i] = sum % 10;

		if(extra != 0)
			sumDigits[i + 1] = extra;
	}

	while(sumDigits[MaxDigitsCount - 1] == 0)
		MaxDigitsCount--;

	return BigInt(sumDigits, MaxDigitsCount);
}

BigInt BigInt::operator - (BigInt B) {

	return B;
}

BigInt BigInt::operator * (BigInt B) {

	return B;
}

BigInt BigInt::operator / (BigInt B) {

	return B;
}

std::ostream& operator << (std::ostream& os, const BigInt& B) {

	for(int i = B.digitsCount - 1; i >= 0; i--)
		os << B.digits[i];

	return os;
}

void BigInt::print() {

	for(int i = digitsCount - 1; i >= 0; i--)
		std::cout << digits[i];
}