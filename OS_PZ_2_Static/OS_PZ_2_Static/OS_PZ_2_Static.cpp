#include "stdafx.h"
#include "BigInt.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {
	
	BigInt B1 = BigInt::ReadBigInt();
	BigInt B2 = BigInt::ReadBigInt();

	cout << B1 << " + " << B2 << " = " << (B1 + B2) << endl;
	cout << B1 << " - " << B2 << " = " << (B1 - B2) << endl;
	cout << B1 << " * " << B2 << " = " << (B1 * B2) << endl;

	system("pause");
	return 0;
}

