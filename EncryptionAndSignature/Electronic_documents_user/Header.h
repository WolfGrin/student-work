#include <iostream>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <math.h>

#define SIZE_FILE 40960
#define SHA256_HASH_SIZE 32	//Byte for CALG_SHA_256
#define CRYPT_BLOC_NUM SHA256_HASH_SIZE/2

void WFile(const LPWSTR szFileName, const LPVOID buf, const DWORD sizeBuf);
bool RFile(const LPWSTR szFileName, LPVOID buf, const DWORD sizeBuf);
void EncodingGronsfeld(const char *openText, const char *key, char *codeText, bool encoding = true);
void GetingHeshSHA256Short(const CHAR *pchData, WORD *pbHash, DWORD dwLenHash, BOOL bShow = false);
bool Miller(unsigned long p);
int gcd(int a, int b);
unsigned powmod(unsigned base, unsigned exp, unsigned modulo);
void RSAKeysGenerate(unsigned &uN, unsigned short &uE, unsigned &uD);
void RSAEncoding(unsigned uED, unsigned uN, unsigned short *upHash, unsigned short *upHashCrypt, unsigned short uHashSize);
void RSADecoding(unsigned uED, unsigned uN, unsigned *upHashCrypt, unsigned short *upHash, unsigned short uHashSize);

//������� ��������� ��������
const char *czAlphabet = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz�����Ũ������������������������������������������������������������������ \"\'!@#$%^&*()_+-=\\/,.`;:?0123456789";

// ����������/������� RSA
void RSAEncoding(unsigned uED, unsigned uN, unsigned short *upHash, unsigned *upHashCrypt, unsigned short uHashSize)
{
	for (int i = 0; i < uHashSize; i++)
		upHashCrypt[i] = powmod(upHash[i], uED, uN);
}

// ����������/������� RSA
void RSADecoding(unsigned uED, unsigned uN, unsigned *upHashCrypt, unsigned short *upHash, unsigned short uHashSize)
{
	for (int i = 0; i < uHashSize; i++)
		upHash[i] = powmod(upHashCrypt[i], uED, uN);
}

// ���������� � ������� �� ������
unsigned powmod(unsigned base, unsigned exp, unsigned modulo)
{
    unsigned res = 1;

    while (exp != 0) 
    {
        if ((exp & 1) != 0)
        {
            res = (1ll * res * base) % modulo;
        }
		unsigned long long a = 1;
        base = (a * base * base) % modulo;
        exp >>= 1;
    }
    
    return res;
}

// ��������� ������ RSA ������������ 4 �����
void RSAKeysGenerate(unsigned &uN, unsigned short &uE, unsigned &uD)
{
	unsigned p, q, f;
	// �������� ��� ������� ����� p � q � ��������� 10 007 - 65521
	// ��� ��� ������������ ����� n , ��� n = p * q (MAX 4�293�001�441)
	// �� ��������� ������������� �������� unsigned int - 4 294 967 295
	srand(time(NULL));
	p = (rand() % 27757) * 2 + 10007;
	while(!(Miller(p)))
		p++;
	q = (rand() % 27757) * 2 + 10007;
	while(!(Miller(q) || p==q))
		q++;
	// �������� ������ - n
	uN = p * q;
	// ��������� �������� ������� ������ �� ����� n 
	f = (p-1) * (q-1);
	// �������� ����� e (�������� ����������) �����, ��� 1<e<f(n) � e ������� ������� � f(n)
	uE = 5;
	while(gcd(uE, f) != 1)
		uE += 2;
	// ��������� ����� d (��������� ����������) ���������������� �������� � ����� e �� ������ f(n)
	unsigned long long k = 2;
	while ((k * f + 1) % uE)
		k++;
	uD = (k * f + 1) / uE;
}

// ������ � ����
void WFile(const LPWSTR szFileName, const LPVOID buf, const DWORD sizeBuf)
{
	HANDLE hFile;
	hFile = CreateFile(szFileName, FILE_ALL_ACCESS, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //CREATE_ALWAYS - ������� ��� �������������� ����
	if(hFile == INVALID_HANDLE_VALUE)
	{
		printf("Could not open file (error %d)\n", GetLastError());
		return;
	}

	DWORD dwRead;
	if(!WriteFile(hFile, buf, sizeBuf, &dwRead, NULL))
	{
		printf("Could not write file (error %d)\n", GetLastError());
		CloseHandle(hFile);
		return;
	}

	CloseHandle(hFile);
}

// ������ �� �����
bool RFile(const LPWSTR szFileName, LPVOID buf, const DWORD sizeBuf)
{
	HANDLE hFile;
	hFile = CreateFile(szFileName, FILE_GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //CREATE_ALWAYS - ������� ��� �������������� ����
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwRead;
	BYTE *ReadBuffer = new BYTE[sizeBuf];
	if(!ReadFile(hFile, ReadBuffer, sizeBuf, &dwRead, NULL))
	{
		printf("Could not read file (error %d)\n", GetLastError());
		CloseHandle(hFile);
		return false;
	}
	memcpy(buf, ReadBuffer, dwRead);
	
	CloseHandle(hFile);
	delete [] ReadBuffer;
	return true;
}

// ����������� ������� �����������
void EncodingGronsfeld(const char *inputText, const char *key, char *outputText, bool encoding)
{
	int sizeBuff = strlen(inputText)+1;	//������ ��������� �� ����� ������
	char *buff = new char[sizeBuff];	//��������� ������ ��� �������������� ������
	
	char *positionInText;	// ������� ������� � �������� ������
	int positionInAlph,		// ������� ������� ��������� ������ � ��������
		offsetKey;			// �������� ������������ ���������� ������� � �������� � ����������� � ������� ������
	
	for (int i = 0, p; i < sizeBuff-1; i++)	// ���� ���������� ������� �����������
	{
		positionInText = (char *)strchr(czAlphabet, inputText[i]);
		positionInAlph = (positionInText - czAlphabet);	// ������� ������� ������� ��������� ������ � ��������
		if(positionInText == NULL)	// ���� ������ �� ������ - ������ �������� ���
		{
			buff[i] = inputText[i];			
			continue;
		}
			
		offsetKey = ((key[i % strlen(key)]-48) % 10);	// �������� � �������� �� N �������� � ������������ � ������� ������ - key
		if(encoding)
			buff[i] = czAlphabet[ (positionInAlph + offsetKey) % strlen(czAlphabet) ];	// ������ �������������� �������� � ����� ������
		else
		{
			p = positionInAlph - offsetKey;
			buff[i] = czAlphabet[ p<0? (strlen(czAlphabet)+p) : p ];	// ������ �������� �������� � ����� ������
		}
	}
	buff[sizeBuff-1] = '\0';
	strcpy(outputText, buff);

	delete [] buff;
}

// ��������� ��� ����� ������������ 32 ����� � ������� �� 4 ����� (unsigned int)
void GetingHeshSHA256Short(const CHAR *pchData, WORD *puHash, DWORD dwLenHash, BOOL bShow)
{
	BYTE pbBuff[SHA256_HASH_SIZE];
	HCRYPTPROV hProv;
	if(!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		std::cout<< "Failed to acquire cryptographic context\n";
		return;
	}
	HCRYPTHASH hHash;
	if(!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
	{
		std::cout<< "Error during CryptCreateHash.\n";
		return;
	}
	if(!CryptHashData(hHash, (BYTE *)pchData, strlen(pchData)+1, 0))
	{
		std::cout<< "Error during CryptHashData.\n";
		return;
	}
	if(!CryptGetHashParam(hHash, HP_HASHVAL, pbBuff, &dwLenHash, 0))
	{
		std::cout<< "Error during reading hash value.";
		return;
	}
	memcpy(puHash, pbBuff, SHA256_HASH_SIZE);

	if(bShow)
	{
		std::cout<< std::endl;
		for (int i = 0; i < CRYPT_BLOC_NUM; i++)
			std::cout<< puHash[i] << "\t";
		std::cout<< std::endl;
	}

	if(hHash) CryptDestroyHash(hHash);
	if(hProv) CryptReleaseContext(hProv,0);
}

// ���� ������� � ������ (��������� �������� �����)
long mulmod(long a, long b, long mod)
{
    unsigned long x = 0,y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {    
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}
long modulo(long base, long exponent, long mod)
{
    unsigned long x = 1;
    unsigned long y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}
bool Miller(unsigned long p)
{
	int iteration = 5;
    if (p < 2)
    {
        return false;
    }
    if (p != 2 && p % 2==0)
    {
        return false;
    }
    unsigned long s = p - 1;
    while (s % 2 == 0)
    {
        s /= 2;
    }
    for (int i = 0; i < iteration; i++)
    {
        unsigned long a = rand() % (p - 1) + 1, temp = s;
        unsigned long mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
        {
            return false;
        }
    }
	for (int i = (p / 2); i > 1; i--)
	{
		if(!(p % i))
			return false;
	}
    return true;
}

// �������� ������� (���������� ����������� ������ �������� ���� ����� �����)
// �������� ����� �� ��������������.
int gcd(int a, int b)
{
	int c;
	while (b)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return abs(a);
}
