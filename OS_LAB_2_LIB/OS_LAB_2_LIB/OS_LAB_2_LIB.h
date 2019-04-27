#ifdef OS_LAB_2_LIB_EXPORTS
#define EIAPI __declspec(dllexport)
#else
#define EIAPI __declspec(dllimport)
#endif

EIAPI unsigned GetEncryptAndDecryptKeys(unsigned* E, unsigned* D);
EIAPI unsigned Encrypt(TCHAR* Message, unsigned E, unsigned n, unsigned* EncryptedMessage);
EIAPI void Decrypt(unsigned* Message, unsigned L, unsigned D, unsigned n, TCHAR* DecryptedMessage);