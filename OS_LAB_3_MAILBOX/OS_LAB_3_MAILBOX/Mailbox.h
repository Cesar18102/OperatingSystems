#pragma once 

#include "stdafx.h"

class Message {

	public:
		Message(size_t MaxSize = 0);
		Message& operator += (TCHAR *Msg);
		void GetBytes(PBYTE Buffer);
		void Read(TCHAR *Buffer);
		DWORD GetMessageSize();
		DWORD GetDataSize();

	private:
		size_t MAX_SIZE;
		DWORD MessageSize;
		TCHAR *MessageText;
};

class Mailbox {

	public : 
		size_t MAX_SIZE;
		Mailbox(TCHAR *Filename, size_t MaxSize = 0); 
		Mailbox& operator += (Message Msg);
		Mailbox& operator -= (DWORD Id);
		void ReadMessage(TCHAR *Buffer, DWORD Id); 
		DWORD GetMessagesCount();
		DWORD GetDataSize();
		DWORD GetMessageSize(DWORD id);
		void GetBytes(PBYTE Buffer);
		void Clear(); 
		void Save();
		static Mailbox Load(TCHAR *Filename);

	private : TCHAR *Filename;
			  DWORD MessageCount, TotalMessagesDataSize;
			  Message *Messages;
}; 