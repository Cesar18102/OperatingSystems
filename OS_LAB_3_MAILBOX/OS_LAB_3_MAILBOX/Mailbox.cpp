#include "stdafx.h"
#include "Mailbox.h"

Message::Message(size_t MaxSize) {

	this->MessageSize = 0;
	this->MAX_SIZE = MaxSize;
	this->MessageText = new TCHAR[this->MAX_SIZE];
}

DWORD Message::GetMessageSize() { return this->MessageSize; } 
DWORD Message::GetDataSize() { return GetMessageSize() + sizeof(DWORD) * 2; } 

Message& Message::operator += (TCHAR *Msg) {

	size_t Len = Length(Msg);
	size_t Offset = this->GetMessageSize();

	if(Offset + Len <= MAX_SIZE) {

		Copy(this->MessageText + Offset, Msg);
		this->MessageSize += Len;
	}

	return *this;
}

void Message::Read(TCHAR* Buffer) {

	Copy(Buffer, this->MessageText);
}

void Message::GetBytes(PBYTE Buffer) {

	int SizeOffset = sizeof(DWORD);

	memcpy(Buffer, &MAX_SIZE, SizeOffset);
	memcpy(Buffer + SizeOffset, &MessageSize, SizeOffset);

	char* Data = NULL;
	if(sizeof(TCHAR) == sizeof(wchar_t)) {

		Data = new char[MessageSize];
		WideCharToMultiByte(CP_ACP, 0, (wchar_t*)MessageText, MessageSize, Data, MessageSize, NULL, NULL);
	} 
	else
		Data = (char*)MessageText;

	memcpy(Buffer + 2 * SizeOffset, Data, MessageSize);
}

Mailbox::Mailbox(TCHAR *Filename, size_t MaxSize) {

	this->Filename = Filename;
	this->MAX_SIZE = MaxSize;
	this->MessageCount = 0;
	this->TotalMessagesDataSize = 0;
	this->Messages = new Message[this->MAX_SIZE];
}

Mailbox& Mailbox::operator += (Message Msg) {

	this->Messages[this->MessageCount++] = Msg;
	this->TotalMessagesDataSize += Msg.GetDataSize();
	return *this;
}

Mailbox& Mailbox::operator -= (DWORD Id) {

	if(Id >= this->MessageCount)
		return *this;

	this->TotalMessagesDataSize -= Messages[Id].GetDataSize();
	for(DWORD i = Id; i < this->MessageCount; i++)
		this->Messages[i] = this->Messages[i + 1];

	this->MessageCount--;
	return *this;
}

void Mailbox::Clear() {  this->MessageCount = 0; }

DWORD Mailbox::GetMessageSize(DWORD id) {

	return Messages[id].GetMessageSize();
}

void Mailbox::ReadMessage(TCHAR *Buffer, DWORD Id) {

	if(Id >= this->MessageCount)
		return;

	Messages[Id].Read(Buffer);
}

DWORD Mailbox::GetMessagesCount() {

	return this->MessageCount;
}

DWORD Mailbox::GetDataSize() {

	return this->TotalMessagesDataSize + sizeof(DWORD) * 3;
}

void Mailbox::GetBytes(PBYTE Buffer) {

	memcpy(Buffer, &MAX_SIZE, sizeof(DWORD));
	memcpy(Buffer + sizeof(DWORD), &TotalMessagesDataSize, sizeof(DWORD));
	memcpy(Buffer + sizeof(DWORD) * 2, &MessageCount, sizeof(DWORD));

	size_t Offset = sizeof(DWORD) * 3;

	for(int i = 0; i < MessageCount; i++) {

		Messages[i].GetBytes(Buffer + Offset);
		Offset += Messages[i].GetDataSize();
	}
}

enum Results {

	FILE_NOT_EXIST,
	READING_ERROR,
	OK
};

DWORD GetControlSum(PBYTE Data, DWORD Size){ 

	DWORD controlSum = 0; 
	DWORD sizeCompressed4b = Size / 4; 
	PDWORD dataCompressed4b = (PDWORD)Data; 

	for (DWORD i = 0; i < sizeCompressed4b; ++i) 
		controlSum += dataCompressed4b[i];

	DWORD restSize = Size % 4; 

	if (restSize) { 

		DWORD dwNumber = 0; 
		memcpy(&dwNumber, Data, restSize); 
		controlSum += dwNumber; 
	}

	return controlSum; 
} 

DWORD WriteControlSum(TCHAR* filename) { 

	HANDLE h = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (h == INVALID_HANDLE_VALUE)
		return Results::FILE_NOT_EXIST;

	DWORD fileSize = GetFileSize(h, NULL);
	PBYTE fileContent = new BYTE[fileSize];
	DWORD readBytesCount, controlSum; 
	BOOL b = ReadFile(h, fileContent, fileSize, &readBytesCount, 0); 

	if (b) { 

		controlSum = GetControlSum(fileContent, fileSize); 
		b = WriteFile(h, &controlSum, sizeof(controlSum), &readBytesCount, 0); 
	}

	delete[] fileContent; 
	CloseHandle(h); 

	return b ? Results::OK : Results::READING_ERROR; 
}

void Mailbox::Save() {

	DWORD Size = GetDataSize();
	PBYTE Data = new BYTE[Size];
	GetBytes(Data);

	DWORD WriteOffset = 0;
	HANDLE h = CreateFile(Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0); 

	WriteFile(h, Data, Size, &WriteOffset, 0);
	CloseHandle(h);

	WriteControlSum(Filename);
}

Mailbox Mailbox::Load(TCHAR *Filename) {

	DWORD MAILBOX_MAX_SIZE, TOTAL_MESSAGES_DATA_SIZE, MESSAGES_COUNT, ReadOffset;

	HANDLE h = CreateFile(Filename, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	DWORD FileSize = GetFileSize(h, NULL);

	DWORD DataSize = FileSize - sizeof(DWORD);
	PBYTE Data = new BYTE[DataSize];
	ReadFile(h, Data, DataSize, &ReadOffset, NULL);

	DWORD CheckSumReal = GetControlSum(Data, DataSize);
	DWORD CheckSumWritten;

	ReadFile(h, &CheckSumWritten, sizeof(DWORD), &ReadOffset, NULL);

	if(CheckSumWritten != CheckSumReal)
		throw "Mailbox was corrupted";

	memcpy(&MAILBOX_MAX_SIZE, Data, sizeof(DWORD));
	memcpy(&TOTAL_MESSAGES_DATA_SIZE, Data + sizeof(DWORD), sizeof(DWORD));
	memcpy(&MESSAGES_COUNT, Data + sizeof(DWORD) * 2, sizeof(DWORD));

	Mailbox MB = Mailbox(Filename, MAILBOX_MAX_SIZE);

	for(int i = 0; i < MESSAGES_COUNT; i++) {

		DWORD MAX_MESSAGE_SIZE, MESSAGE_SIZE;

		memcpy(&MAX_MESSAGE_SIZE, Data + MB.GetDataSize(), sizeof(DWORD));
		memcpy(&MESSAGE_SIZE, Data + sizeof(DWORD) + MB.GetDataSize(), sizeof(DWORD));

		PBYTE MESSAGE_DATA = new BYTE[MESSAGE_SIZE];
		memcpy(MESSAGE_DATA, Data + sizeof(DWORD) * 2 + MB.GetDataSize(), MESSAGE_SIZE);

		TCHAR* MESSAGE_TEXT = new TCHAR[MESSAGE_SIZE];

		if(sizeof(TCHAR) == sizeof(wchar_t))
			MultiByteToWideChar(CP_ACP, 0, (char*)MESSAGE_DATA, MESSAGE_SIZE, (wchar_t*)MESSAGE_TEXT, MESSAGE_SIZE);
		else
			MESSAGE_TEXT = (TCHAR*)MESSAGE_DATA;

		MESSAGE_TEXT[MESSAGE_SIZE] = _T('\0');

		Message M = Message(MAX_MESSAGE_SIZE);
		M += MESSAGE_TEXT;

		MB += M;
	}

	CloseHandle(h);

	return MB;
}