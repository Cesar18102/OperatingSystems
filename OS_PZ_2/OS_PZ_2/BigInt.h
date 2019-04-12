#pragma once
class BigInt
{
	public:
		BigInt(short* digits, long long len);
		~BigInt(void);
		void print();

		BigInt operator + (BigInt B);
		BigInt operator - (BigInt B);
		BigInt operator * (BigInt B);
		BigInt operator / (BigInt B);
		static BigInt ReadBigInt();
		static BigInt GetBigInt(TCHAR* ds);
		friend std::ostream& BigInt::operator << (std::ostream& os, const BigInt& B);

	private:
		short* digits;
		int digitsCount;
};