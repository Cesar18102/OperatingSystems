#include "stdafx.h"
#include "BigInt.h"

int _tmain(int argc, _TCHAR* argv[]) {

	BigInt A = BigInt::ReadBigInt();
	BigInt B = BigInt::ReadBigInt();

	std::cout << (A + B) << std::endl;
	
	system("pause");
	return 0;
}

