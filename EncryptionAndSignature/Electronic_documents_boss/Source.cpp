/*
	программа зашифровывает текстовый файл Шифром Гронсфельда,
	генерирует открытый и закрытый ключи (RSA)
	с помощью закрытого ключа осуществялет цифровую подпись хеша зашифрованного файла
*/

#include "Header.h"

void main()
{
	char pcKey[50];
	std::cout<< "Enter the secret key: ";
	std::cin>> pcKey;

	// чтение текста из файла
	CHAR textGet[SIZE_FILE] = {0};								// буфер для чтения строки из файла
	if(!RFile(L"ORIGINAL_text.txt", textGet, sizeof(textGet)))	//	чтение текста из файла с открытым текстом
	{
		std::cout<< "File \"ORIGINAL_text.txt\" is missing!\n";
		system("PAUSE");
		return;
	}

	// шифрование текста и запись в файл
	char *pcBuff = new char[strlen(textGet)+1];	
	EncodingGronsfeld(textGet, pcKey, pcBuff);				// получение зашифрованного текста
	WFile(L"ENCODED_text.txt", pcBuff, strlen(textGet)+1);	// сохранение в файл зашифрованного текста
	delete [] pcBuff;

	// получаем хеш открытого файла
	WORD puOriginHash[CRYPT_BLOC_NUM];
	GetingHeshSHA256Short(textGet, puOriginHash, SHA256_HASH_SIZE);
	
	// RSA генерация открытого и закрытого ключей (ключ - 4 байта)
	unsigned d, n;
	unsigned short e = 0;
	unsigned puKey[2] = {0};
	bool bOpenKeyOk = RFile(L"KEY_OPEN.key", puKey, sizeof(puKey));	// проверяем наличие открытого ключа
	if(!(RFile(L"KEY_CLOSE.key", puKey, sizeof(puKey)) && bOpenKeyOk)) // извлекаем закрытый ключ
	{
		// если ключи отсутствуют - генерируем новые
		RSAKeysGenerate(n, e, d);
		puKey[0] = e;
		puKey[1] = n;
		// сохраняем ключи
		WFile(L"KEY_OPEN.key", puKey, sizeof(puKey));	// сохранение в файл зашифрованного текста
		puKey[0] = d;
		WFile(L"KEY_CLOSE.key", puKey, sizeof(puKey));	// сохранение в файл зашифрованного текста
		std::cout<< "Files created... Enjoy!\n";
	}

	// RSA подпись хеша
	UINT puHashSigned[CRYPT_BLOC_NUM];
	RSAEncoding(puKey[0], puKey[1], puOriginHash, puHashSigned, CRYPT_BLOC_NUM);
	
	// сохранение подписи в файл
	WFile(L"SIGN.sign", puHashSigned, sizeof(puHashSigned));	
	system("PAUSE");
}