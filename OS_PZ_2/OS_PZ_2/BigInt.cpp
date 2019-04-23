#include "stdafx.h"
#include "BigInt.h"

BigInt BigInt::GetBigInt(TCHAR* ds) {

	int L = Length(ds);
	bool minus = ds[0] == _T('-');
	int S = minus? 1 : 0;

	short* a = new short[L - S];

	for(int i = S; i < L; i++)
		a[L - i - 1] = ds[i] - 48;

	return BigInt(a, L - S, minus);
}

BigInt BigInt::ReadBigInt() {

	TCHAR* a = new TCHAR[256];
	Read("%s", a);
	return GetBigInt(a);
}

BigInt::BigInt() { }

BigInt::BigInt(short* digits, long long len, bool minus) {

	this->digits = new short[len];
	this->digitsCount = len;
	this->minus = minus;

	for(int i = 0; i < len; i++)
		this->digits[i] = digits[i];
}

BigInt::~BigInt() {

}

BigInt BigInt::operator + (BigInt B) {

	if(this->minus)
		return -(-*this + -B);

	int MaxDigitsCount = (digitsCount > B.digitsCount? digitsCount : B.digitsCount) + 1;
	short* sumDigits = new short[MaxDigitsCount];
	bool Greater = B.digitsCount > digitsCount;

	if(B.digitsCount == digitsCount)
		for(int i = MaxDigitsCount - 1; i >= 0; i--) {

			short DB = i >= B.digitsCount? 0 : B.digits[i];
			short DT = i >= digitsCount? 0 : digits[i];
			if(DB > DT) {

				Greater = true;
				break;

			} else if(DB < DT) 
				break;
		}

	if(Greater)
		return B + *this;

	for(int i = 0; i < MaxDigitsCount; i++)
		sumDigits[i] = i < digitsCount? digits[i] : 0;

	for(int i = 0; i < MaxDigitsCount; i++) {

		int sum = sumDigits[i] + (i >= B.digitsCount? 0 : (B.minus? -1 : 1) * B.digits[i]);

		if(sum >= 0) {

			sumDigits[i] = sum % 10;
			sumDigits[i + 1] += sum / 10;

		} else {

			int j = i + 1;
			for( ; j < MaxDigitsCount && sumDigits[j] == 0; j++);

			if(sumDigits[j] == 0) {

				sumDigits[i] = abs(sum % 10);
				break;

			} else {

				for(int k = j; k > i; k--) {

					sumDigits[k]--;
					sumDigits[k - 1] += 10;
				}

				sumDigits[i] += -B.digits[i] % 10;
			}
		}
	}

	while(sumDigits[MaxDigitsCount - 1] == 0 && MaxDigitsCount > 1)
		MaxDigitsCount--;

	return BigInt(sumDigits, MaxDigitsCount, false);
}



BigInt BigInt::operator - () {

	BigInt S = BigInt(this->digits, this->digitsCount, !this->minus);
	return S;
}

BigInt BigInt::operator - (BigInt B) {

	return *this + -B;
}

BigInt BigInt::operator * (BigInt B) {

	bool NewMinus = B.minus ^ minus;
	BigInt* Multis = new BigInt[B.digitsCount];

	for(int i = 0; i < B.digitsCount; i++) {

		int L = digitsCount + i + 1;
		short* Multi = new short[L];

		for(int j = 0; j < L; Multi[j++] = 0);

		for(int j = 0; j < digitsCount; j++) {
			
			short M = B.digits[i] * digits[j];
			Multi[j + i] += M % 10;
			Multi[j + i + 1] += M / 10;
		}

		while(L > 1 && Multi[L - 1] == 0)
			L--;

		Multis[i] = BigInt(Multi, L, false);
	}

	BigInt Result = Multis[0];
	for(int i = 1; i < B.digitsCount; i++)
		Result = Result + Multis[i];

	Result.minus = NewMinus;
	return Result;
}

BigInt BigInt::operator / (BigInt B) {

	return B;
}

std::ostream& operator << (std::ostream& os, const BigInt& B) {

	if(B.digitsCount == 0)
		return os;

	if(B.minus && B.digits[B.digitsCount - 1] != 0)
		os << '-';

	for(int i = B.digitsCount - 1; i >= 0; i--)
		os << B.digits[i];

	return os;
}

void BigInt::print() {

	for(int i = digitsCount - 1; i >= 0; i--)
		std::cout << digits[i];
}