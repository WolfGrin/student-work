/*	—ерверна¤ часть программы с реализацией
	поочередного доступа к ресурсам сервера
	дл¤ корректной работы проекта, необходимо подключить библиотеку ws2_32.lib
	(Configuration Properties -> Linker -> Input -> Additional Dependencies -> ws2_32.lib)
*/

#include <stdio.h>
#include <winsock2.h>
#include <time.h>

#define PORT 777    // порт сервера

int main(int argc, char* argv[])
{
   const int n = 10;				//максимальное число клиентов
   sockaddr_in clients [n] = {0};	//массив клиентов
   int NumOfClients=0, LastActivClient=0;	//тек. кол. клиентов, активный клиент
   time_t ActivStart, ActivEnd;	// начало, конец активности
   char buff[1024]; //буффер дл¤ различных нужд

   printf("DEMONSTRATION OF ACCESS STRUCTURES: Server\n\n");
	// инициирование использовани¤ библиотеки DLL Winsock
  if (WSAStartup(0x202,(WSADATA *) &buff[0]))
  {
    printf("WSAStartup error: %d\n", WSAGetLastError());
	return -1;
  }
  
    // создание сокета
  SOCKET my_sock;
  my_sock = socket(AF_INET,SOCK_DGRAM,0);
  if (my_sock == INVALID_SOCKET)
  {
	  printf("Socket() error: %d\n",WSAGetLastError());
      WSACleanup();
      return -1;
  }

    // св¤зывание сокета с локальным адресом 
   sockaddr_in local_addr;
   local_addr.sin_family = AF_INET;
   local_addr.sin_addr.s_addr = INADDR_ANY;
   local_addr.sin_port = htons(PORT);

   if (bind(my_sock, (sockaddr *) &local_addr, sizeof(local_addr)))
   {
	   printf("bind error: %d\n", WSAGetLastError());
	   closesocket(my_sock);
       WSACleanup();
       return -1;
   }

    // обработка пакетов, присланных клиентами
   while(true)
   {
	   sockaddr_in client_addr;
       int client_addr_size = sizeof(client_addr);
       int bsize = recvfrom(my_sock, &buff[0], sizeof(buff)-1, 0, (sockaddr *) &client_addr, &client_addr_size);
	   if (bsize == SOCKET_ERROR)
		   printf("recvfrom() error: %d\n", WSAGetLastError());

      // ќпредел¤ем IP-адрес клиента и прочие атрибуты
	   HOSTENT *hst;
	   hst = gethostbyaddr((char *) &client_addr.sin_addr, 4, AF_INET);
	   printf("new DATAGRAM from +%s [%s:%d]\n",
		   (hst) ? hst->h_name : "Unknown host",
		   inet_ntoa(client_addr.sin_addr),
		   ntohs(client_addr.sin_port));

      // добавление завершающего нул¤
	   buff[bsize] = 0;

//-------определение приоритетности и реализаци¤ обмена данными----------
	   bool old_client = 0;
	   int CurentClient = 0;
		   
	   double ActivTime;
	   NumOfClients == 0 ? time(&ActivStart) : 0;
	    
	   //страый или новый клиент	 
	   for(int i = NumOfClients; i >= 0; i--)
		   if (client_addr.sin_addr.S_un.S_addr == clients[i-1].sin_addr.S_un.S_addr)
		   {
			   old_client = true;
			   CurentClient = i-1;
			   break;
		   }

	   //если не найден, добавл¤ем в масив
	   if(!old_client) 
	   {
		   clients[NumOfClients] = client_addr;
		   printf("New User, ID:%d IP:%s\n", NumOfClients+1, inet_ntoa(clients[NumOfClients].sin_addr));
		   NumOfClients++;
	   }

	   time(&ActivEnd);
	   ActivTime = difftime(ActivEnd, ActivStart);

		//распредиление доступа
	   if(ActivTime < 5 && client_addr.sin_addr.S_un.S_addr != clients[LastActivClient].sin_addr.S_un.S_addr)
	   {	
		   // посылка датаграммы не активному клиенту
		   //--------------------------------------------
		   // оповещение ожидающие клиента о необходимости
		   // дождатьс¤ своей очереди
		   //--------------------------------------------
		   sendto(my_sock, "Not now!\n", 9, 0, (sockaddr *)&client_addr, sizeof(client_addr));
	   }
	   else
	   {
		   // посылка датаграммы активному клиенту
		   //--------------------------------------------
		   // тут описываем работу с активным пользователем
		   //--------------------------------------------
		   sendto(my_sock, &buff[0], bsize, 0, (sockaddr *)&client_addr, sizeof(client_addr));

		   time(&ActivStart);
		   LastActivClient = CurentClient;
	   }
//---------------------------
	   // ¬ывод в консоль сервера информацию о активности
	   printf("current client: %d\n active client: %d, omission time: %g\n", CurentClient+1, LastActivClient+1, ActivTime);
       printf("C=>S:%s\n", &buff[0]);
   }

    //выход
   closesocket(my_sock);
   WSACleanup();
   return 0;
}