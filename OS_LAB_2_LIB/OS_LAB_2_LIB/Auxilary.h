#include <tchar.h>

#ifdef UNICODE
#define Length lstrlenW
#define TCHARMAX WCHAR_MAX
#else
#define Length strlen
#define TCHARMAX CHAR_MAX
#endif

bool IsPrime(unsigned n);
bool AreInterPrime(unsigned n, unsigned q);

unsigned RandInt(unsigned min, unsigned max);
unsigned RandPrime(unsigned min, unsigned max);
unsigned RandPrime(unsigned min, unsigned max, unsigned neq);
unsigned RandInterPrime(unsigned min, unsigned max, unsigned co);

unsigned NearestPrime(unsigned min, unsigned max, unsigned n);
unsigned NearestInterPrime(unsigned min, unsigned max, unsigned n, unsigned co);

unsigned GreatestCommonDevider(unsigned n, unsigned q);
unsigned EulerFunc(unsigned n, unsigned q);

unsigned PowModEnc(TCHAR x, unsigned y, unsigned m);
TCHAR PowModDec(unsigned x, unsigned y, unsigned m);