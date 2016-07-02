 /*
		 Рассмотрим число k1. Произведением всех цифр из него получим число k2. Из k2 таким же образом получим k3,
		 и процесс остановится на kn, состоящем из одной цифры. Число n называется устойчивостью числа k1.

		 # вычисление методом распаралеливания используя интерфейс MPI 
 
		Для работы программы необходимо подключить в проект библиотеки и заголовочные файлы MPI:
		C:\Program Files (x86)\MPICH2\include и C:\Program Files (x86)\MPICH2\lib
		В окне настроек проекта  Properties -> Configuration Properties -> Linker -> Input,
		необходимо добавить mpi.lib в поле Additional Dependencies
 */

#include <iostream>
#include <stdlib.h>
#include "mpi.h"

using namespace std;
int * IncreasingArray(int* m, int &n);

int main(int acgc, char* argv[])
{

	const unsigned int factor = 100; // разрядность диапазона чисел устойчивость которых проверяется (например 100 - проверяет числа от 100 до 999)

	unsigned int N = factor * 10 - 1;
	int* rcTemp = NULL;			//буфер получения данных от других процессов
	unsigned int k;				//размер блока
	int ProcNum, ProcRank;		//количество процессов, ранг процесса
	int ibeg, iend;				//границы элементов заполняемого массива

	MPI_Status status;		//информация о процессе от которого было принято сообщение
	MPI_Init(&acgc, &argv); //начало MPI
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); //количество процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);//текущий ранг процесса
	
	//распределение обробатываемых данных между процессами. Определения границ
	k = (N - 1) / ProcNum + 1;
	ibeg = ProcRank * k + 1;
	iend = (ProcRank + 1) * k;
	if (ibeg >= N) iend = ibeg;
	else if (iend >= N) iend = N;

	//инициализация элементов
	unsigned int temp, digit_t;
	int sizeArr = 256, weight = 0, weight_max = 0, j = 0;
	int* pArr = new int[sizeArr];
	memset(pArr, 0, sizeof(pArr)*sizeArr);

	while (ibeg <= iend) //перебор всех чисел в заданом диапазоне range
	{
		weight = 1;
		temp = ibeg;

		while (true) //проходы, уровень устойчивости
		{
			digit_t = 1;
			while (temp&&digit_t) //умножение всех цифр в числе
			{
				digit_t *= temp % 10;
				temp /= 10;
			}

			if (digit_t / 10) //если после умножения в числе больше одной цифры
			{
				temp = digit_t;
				weight++;
			}
			else
			{
				if (weight<weight_max) break;//если глубина меньше максимальной
				else if (weight>weight_max) //если глубина больше максимальной
				{
					weight_max = weight;
					memset(pArr, 0, sizeof(pArr)*sizeArr);
					j = 0;
				}
				//заполняем масив числами одинакового, наибольшего, уровня устойчивости
				pArr[j] = ibeg;
				j++;
				//при необходимости вызываем функцию динамического расширения выделенной памяти
				if (j == sizeArr - 1)
					pArr = IncreasingArray(pArr, sizeArr);
				break;
			}
		}
		ibeg++;
	}
	if (ProcRank == 0) //действия для 0 процесса
	{
		for (int i = 1; i < ProcNum; i++)
		{	//получение результатов от других процессов
			MPI_Recv(rcTemp, sizeArr, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			if (status.MPI_TAG == weight_max) //сравнение уровней устойчивости и заполнение массива наибольшими
			{
				for (int i = 0; rcTemp[i]; i++, j++)
				{
					if (j == sizeArr - 1)
						pArr = IncreasingArray(pArr, sizeArr);
					pArr[j] = rcTemp[i];
				}
			}
			else if (status.MPI_TAG > weight_max)
			{
				weight_max = status.MPI_TAG;
				memset(pArr, 0, sizeof(pArr)*sizeArr);
				for (j = 0; rcTemp[j]; j++)
					pArr[j] = rcTemp[j];
			}
		}

		for (int i = 0; i < sizeArr && pArr[i]; i++)	 //вывод на экран всех чисел наибольшего уровня устойчивости
		{
			cout << pArr[i] << " ";
		}
		cout << endl;
		cout << "level of stability : " << weight_max << endl;
		system("pause");
	}
	else
	{
		//процессы (ProcRang != 0) отправляют полученный результат процессу с рангом 0
		MPI_Send(pArr, sizeArr, MPI_INT, 0, weight_max, MPI_COMM_WORLD);
	}

	delete[] pArr;
	MPI_Finalize(); //завершение MPI
	return 0;
}
//функция динамического расширения выделенной памяти
int * IncreasingArray(int * m, int &n)
{
	int *m_t;
	n += 256;
	m_t = new int[n];
	memset(m_t, 0, sizeof(m_t)*n);
	memcpy(m_t, m, sizeof(m_t)*(n - 256));
	delete[] m;
	return m_t;
}
