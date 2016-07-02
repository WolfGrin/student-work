/*
	пример работы с реестром (создание и удаление разделов, изменение параметров...)
*/

#include "iostream"		//подключение библиотек "iostream"
#include "windows.h"	//подключение библиотек "windows.h"

using namespace std;	//пространства имен std
void Menu(INT*);		//прототип функции вызова меню пользователя
void main()				//основная функиця программы
{
	HKEY hKey;								//дескриптор указанного раздела реестра
	DWORD dwDisposition;					//_Out_opt_ функции RegCreateKeyEx()
	BYTE szStr[2];							//значение записываемого параметра в параметр
	szStr[0] = '1'; szStr[1] = '\0';		//инициализация значения "1" и конец строки "\0"

	TCHAR lpData[1024] = {0};				//строка для хранения значения указаного параметра
	DWORD dwBufferSize = wcslen(lpData);	//получение размера строки lpData
	TCHAR pVal[] = L"http://google.com";	//новое значение параметра
	DWORD dwSize = wcslen(pVal);			//получение размера строки pVal

	INT iKey=0;								//ключ выбора пользователем действий
/*-----------------------------создание подраздела по умолчанию-------------------------------------*/
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,		//«куст» реестра к которому обращаемся
		TEXT("Software\\Test"),				//адрес имени раздела
		0,									//зарезервировано "0"
		NULL,								//адрес строки класса
		REG_OPTION_VOLATILE,				//флаг особых опций
		KEY_ALL_ACCESS,						//желаемый доступ безопасности
		NULL,								//адрес структуры ключа безопасности
		&hKey,								//адрес буфера для открытого раздела
		&dwDisposition);					//адрес буфера значениия результата действий над разделом
	//проверка значениия ключа буфера
	if (dwDisposition != REG_CREATED_NEW_KEY && dwDisposition != REG_OPENED_EXISTING_KEY) 
				//раздел небыл создан/открыт
		cout<<"\nError creating the desired subkey (permissions?).\n";
	else
	{
		cout<<"\nThe subkey \"HKEY_LOCAL_MACHINE\\Software\\Test\" was successfully created.\n";
		iKey=1; //если раздел удачно создан/открыт - выполнить цикл программы
	}
	//освобождение дескриптора указанного раздела
	RegCloseKey(hKey);
/*-------------------------------основной цикл программы-------------------------------------------*/
	while(iKey!=0)		
	{
		
		Menu(&iKey);	//вызов меню
		system("cls");	//очистка экрана

		switch (iKey)	//проверка ключа свича
		{
		case 1:		//открытие указанного раздела
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,//«куст» реестра к которому обращаемся
				TEXT("Software\\Test"),			//адрес имени открываемого подраздела
				0,								//зарезервировано "0"
				KEY_ALL_ACCESS,					//маска доступа безопасности
				&hKey)							//адрес дескриптора открытого раздела
				!=ERROR_SUCCESS)				//проверка на успешность открытия раздела
					//если не получилось открыть
				cout<<"\nError opening the desired subkey (doesn't exist?).\n";
			else	//усли успешно открыто
			{		//создание параметра "String Value" 
				if (RegSetValueEx(hKey,		//дескриптор открытого раздела
					TEXT("String Value"),	//адрес имени установливаемого параметра
					NULL,					//зарезервировано "0"
					REG_SZ,					//тип данных
					szStr,					//адрес данных для установки
					sizeof(szStr))			//размер данных 
					==ERROR_SUCCESS)		//проверка на успешность задания значения параметру
						//значение установленно успешно
					cout<<"\nThe value of the key was set successfully.\n";
				else	//ошибка задания значения параметра
					cout<<"\nError setting the value of the key.\n";
			}
			//освобождение дескриптора указанного раздела
			RegCloseKey(hKey);
			break;

		case 2: //создание указанного раздела
			RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Test\\Another SubKey"), 0, NULL, REG_OPTION_NON_VOLATILE,				
				KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
			//проверка значениия ключа буфера
			if (dwDisposition != REG_CREATED_NEW_KEY && dwDisposition != REG_OPENED_EXISTING_KEY)
				cout<<"\nError creating the desired subkey (permissions?).\n";
			else
				cout<<"\nThe subkey was successfully created.\n";
			//освобождение дескриптора указанного раздела
			RegCloseKey(hKey);
			break;

		case 3: //открытие указанного раздела
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Test"), 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
			{	//если удалось открыть, удаляем выбраный параметр из указанного раздела
				if (RegDeleteValue(hKey,	//дескриптор открытого раздела
					TEXT("String Value"))	//удрес имени удаляемого параметра
					==ERROR_SUCCESS)		//проверка на успешность удаления параметра
						//если параметр успешно удален
					cout<<"\nString Value value successfully removed.\n";
				else	//ошибка удаления параметра
					cout<<"\nError removing the specified value (permissions?).\n";
			}
			else//не удалось открыть указанный раздел
				cout<<"\nError opening the specified subkey path (doesn't exist?).\n";
			//освобождение дескриптора указанного раздела
			RegCloseKey(hKey);
			break;
			
		case 4: //открытие указанного раздела
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Test"), 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
			{	//если удалось открыть, удаляем выбраный подраздел из указанного раздела
				if (RegDeleteKey(hKey,		//дескриптор открытого раздела
					TEXT("Another SubKey"))	//удрес имени удаляемого подраздела
					==ERROR_SUCCESS)		//проверка на успешность удаления подраздела
						//если подраздел успешно удален
					cout<<"\nAnother SubKey key successfully removed.\n";
				else	//ошибка удаления подраздела
					cout<<"\nError removing the specified key (permissions?).\n";
			}
			else//не удалось открыть указанный раздел
				cout<<"\nError opening the specified subkey path (doesn't exist?).\n";
			//освобождение дескриптора указанного раздела
			RegCloseKey(hKey);
			break;

		case 5: //открытие указанного раздела
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software"), 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
			{	//если удалось открыть, удаляем выбраный подраздел из указанного раздела
				if (RegDeleteKey(hKey, TEXT("Test"))==ERROR_SUCCESS)
					//если подраздел успешно удален
					cout<<"\nTest key successfully removed.\n";
				else //ошибка удаления подраздела
					cout<<"\nError removing the specified key (permissions?).\n";
			}
			else//не удалось открыть указанный раздел
				cout<<"\nError opening the specified subkey path (doesn't exist?).\n";
			//освобождение дескриптора указанного раздела
			RegCloseKey(hKey);
			break;

		case 6: //открытие указанного раздела
			if (RegOpenKeyEx (HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Internet Explorer\\Main"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
			{	//получаем значение указанного параметра в открытом разделе
				if (RegQueryValueEx(hKey,	//дескриптор открытого раздела
					TEXT("Start Page"),		//адерс имени параметра
					0,						//зарезервировано "0"
					NULL,					//адрес переменной для типа параметра
					(LPBYTE)lpData,			//адрес буфера для значений
					&dwBufferSize)			//адрес переменной для размер буфера значений
					== ERROR_SUCCESS)		//проверка на успешность выполнения функции
				{	//если функция отработала корректно
					cout<<"\nYour current Internet start page is ";
					wcout<<lpData<<endl; //показ значения выбранного параметра
				}
				else//ошибка выполнения функции
					cout<<"\nError getting the specified value.\n";
			}
			else//не удалось открыть указанный раздел
				cout<<"\nError opening the specified subkey path (doesn't exist?).\n";
			//освобождение дескриптора указанного раздела
			RegCloseKey (hKey);
			break;

		case 7: //открытие указанного раздела
			if (RegOpenKeyEx (HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Internet Explorer\\Main"),NULL,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
			{	//установка указанному параметру значения "pVal"
				if (RegSetValueEx(hKey, TEXT("Start Page"),0, REG_SZ, (BYTE*)pVal, 2*dwSize)==ERROR_SUCCESS)
				{	//если функция отработала корректно
					cout<<"\nNow your Internet start page is ";
					wcout<<pVal<<endl; //показ нового значения выбранного параметра
				}
				else//ошибка выполнения функции
					cout<<"\nError setting the value of the key.\n";
			}
			else//не удалось открыть указанный раздел
				cout<<"\nError opening the specified subkey path (doesn't exist?).\n";
			//освобождение дескриптора указанного раздела
			RegCloseKey (hKey);
			break;

		default: //действия по умолчанию (выбор пользователем не корректен, повторный запрос)
			cout<<"\nThe choice is not correct. Try again...\n";
			break;
		}
	}
/*----------------------очистка реестра от созданных ключей и каталогов-------------------------------*/
	//открытие указанного раздела
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Test"), 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
	{
		RegDeleteValue(hKey, TEXT("String Value"));	//удаление параметра "String Value"
		RegDeleteKey(hKey, TEXT("Another SubKey"));	//удаление раздела "Another SubKey"
	}
	RegCloseKey(hKey);//освобождение дескриптора указанного раздела
	//открытие указанного раздела
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software"), 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
		RegDeleteKey(hKey, TEXT("Test"));			//
	RegCloseKey(hKey);//освобождение дескриптора указанного раздела
}
/*-----------------------------функция меню пользователя-----------------------------------------------*/
void Menu(INT *Key) //принимаемые значения
{	//визуальная часть программы
	cout<<"\n1 - RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(\"Software\\Test\"), ...\n"
		"\tRegSetValueEx(hKey, TEXT(\"String Value\"),..."<<endl; 
	cout<<"2 - RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT(\"Software\\Test\\Another SubKey\"), ..."<<endl;
	cout<<"3 - RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(\"Software\\Test\"), ...\n"
		"\tRegDeleteValue(hKey, TEXT(\"String Value\")"<<endl;
	cout<<"4 - RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(\"Software\\Test\"), ...\n"
		"\tRegDeleteKey(hKey, TEXT(\"Another SubKey\")"<<endl;
	cout<<"5 - RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(\"Software\"), ...\n"
		"\tRegDeleteKey(hKey, TEXT(\"Test\")"<<endl;
	cout<<"6 - RegOpenKeyEx (HKEY_CURRENT_USER, TEXT(\"Software\\Microsoft\\Internet Explorer\\Main\"), ...\n"
		"\tRegQueryValueEx(hKey, TEXT(\"Start Page\")"<<endl;
	cout<<"7 - RegOpenKeyEx (HKEY_CURRENT_USER, TEXT(\"Software\\Microsoft\\Internet Explorer\\Main\"), ...\n"
		"\tRegSetValueEx(hKey, TEXT(\"Start Page\"), 0, REG_SZ, (BYTE*)Val, 2*size)"<<endl;
	cout<<"0 - exit"<<endl;
	wcin>>*Key; //запрос на выполнение действий
}