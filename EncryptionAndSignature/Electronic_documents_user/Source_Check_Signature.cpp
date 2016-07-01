/*
	программа декодирует зашифрованый файл Шифром Гронсфельда
	проверяет при помощи открытого ключа валидность цифровой подписи хеша и целостность файла
*/

#include "Header.h"

void main()
{
	char pcKey[50];
	std::cout<< "Enter the secret key: ";
	std::cin>> pcKey;

	// расшифровка файла
	CHAR textGet[SIZE_FILE] = {0};								// буфер для чтения строки из файла
	if(!RFile(L"ENCODED_text.txt", textGet, sizeof(textGet)))	//	чтение текста из файла с зашифрованым текстом
	{
		std::cout<< "File \"ENCODED_text.txt\" is missing!\n";
		system("PAUSE");
		return;
	}
	char *pcBuff = new char[strlen(textGet)+1];
	EncodingGronsfeld(textGet, pcKey, pcBuff, 0);			// получение открытого текста
	WFile(L"DECODED_text.txt", pcBuff, strlen(textGet)+1);	// сохранение в файл открытого текста
	
	// получение хеша расшифрованного файла и разделение на блоки по 4 байта
	WORD puFileDecHash[CRYPT_BLOC_NUM];
	GetingHeshSHA256Short(pcBuff, puFileDecHash, SHA256_HASH_SIZE);
	delete [] pcBuff;

	// RSA проверка подписи хеша расшифрованного файла
	UINT puGetHashSigned[CRYPT_BLOC_NUM];	// подписаный хеш из файла
	WORD puHashUnSigned[CRYPT_BLOC_NUM];	// расшифрованый хеш открытым ключем - е, n

	if(!RFile(L"SIGN.sign", puGetHashSigned, sizeof(puGetHashSigned)))	// получаем подпись из файла
	{
		std::cout<< "File \"SIGN_HASH.sign\" is missing!\n";
		system("PAUSE");
		return;
	}

	unsigned puKey[2];
	if(!RFile(L"KEY_OPEN.key", puKey, sizeof(puKey))) // извлекаем открытый ключ
	{
		std::cout<< "File \"KEY_OPEN.key\" is missing!\n";
		system("PAUSE");
		return;
	}
	RSADecoding(puKey[0], puKey[1], puGetHashSigned, puHashUnSigned, CRYPT_BLOC_NUM);

	// проверка корректности подписи
	if(memcmp(puFileDecHash, puHashUnSigned, sizeof(puHashUnSigned)))
	{
		std::cout<< "Secret key, file or signature is corupted!!!\n";
	}
	else
	{
		std::cout<< "File and signature is ok!\n";
	}
	system("PAUSE");
}