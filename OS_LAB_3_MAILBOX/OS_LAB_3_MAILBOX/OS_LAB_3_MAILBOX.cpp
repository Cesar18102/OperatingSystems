#include "stdafx.h"
#include "Mailbox.h"

bool MessageReadController(Mailbox *MB) { 

	TCHAR* MsgBuffer = new TCHAR[1024];
	GRead_(MsgBuffer);

	if(MsgBuffer[0] == _T('c') && MsgBuffer[1] == _T('s')) {

		MB->Save();
		return false;
	}

	if(MsgBuffer[0] == _T('c') && MsgBuffer[1] == _T('n') && MsgBuffer[2] == _T('s'))
		return false;

	Message Msg = Message(1024);
	Msg += MsgBuffer;
	*MB += Msg;
	CWrite_("%s", "> ");
}

void CreateMailBox() {

	CWrite_("%s\n", "Type sc to save and close mailbox");
	CWrite_("%s\n", "Type snc to close mailbox without saving");

	CWrite_("%s", "Input creating mailbox path: ");

	TCHAR* MailBoxPath = new TCHAR[256];
	Read_("%s", MailBoxPath);

	Mailbox MB = Mailbox(MailBoxPath, 1024);
	CWrite_("\n%s\n", "MESSAGES: ");

	while(MessageReadController(&MB));
}

void LoadMailBox() {

	CWrite_("%s\n", "Type sc to save and close mailbox");
	CWrite_("%s\n", "Type snc to close mailbox without saving");

	CWrite_("%s", "Input mailbox path: ");

	TCHAR* MailBoxPath = new TCHAR[256];
	Read_("%s", MailBoxPath);

	Mailbox MB = Mailbox::Load(MailBoxPath);
	CWrite_("\n%s\n", "MESSAGES: ");

	int count = MB.GetMessagesCount();
	for(int i = 0; i < count; i++) {

		TCHAR *Msg = new TCHAR[MB.GetMessageSize(i)];
		MB.ReadMessage(Msg, i);
		Write_("%s\n", Msg);
	}

	while(MessageReadController(&MB));
}

void DrawMainMenu() {

	system("cls");

	CWrite_("%s", "Input 1 to create new mailbox\n");
	CWrite_("%s", "Input 2 to load mailbox\n");

	int option = 0;
	Read_("%d", &option);

	if(option == 1)
		CreateMailBox();
	else if(option == 2)
		LoadMailBox();
	else
		CWrite_("%s", "Invalid option\n");
}

int _tmain(int argc, CHAR* argv[]) {

	//setlocale(LC_ALL, "Russian");

	DrawMainMenu();

	system("pause");
	return 0;
}