#include "stdafx.h"
#include "Mailbox.h"

int _tmain(int argc, _TCHAR* argv[]) {

	//Mailbox MB = Mailbox(_T("db.txt"), 32);
	Mailbox MB = Mailbox::Load(_T("db.txt"));

	Message M = Message(256);
	M += _T("OleĞ");

	Message M2 = Message(256);
	M2 += _T("Cesar18103");
	
	MB += M;
	MB += M2;

	MB.Save();

	return 0;
}

