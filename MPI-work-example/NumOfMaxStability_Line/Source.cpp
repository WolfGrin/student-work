/*
	Рассмотрим число k1. Произведением всех цифр из него получим число k2. Из k2 таким же образом получим k3, 
	и процесс остановится на kn, состоящем из одной цифры. Число n называется устойчивостью числа k1.
	
	# вычисление методом линейного алгоритма
*/
#include <iostream>
#include <cstdio>
#include <stdlib.h>
using namespace std;

int * IncreasingArray(int* m, int &n);

void main()
{
	const unsigned int factor = 100;	// разрядность диапазона чисел устойчивость которых проверяется (например 100 - проверяет числа от 100 до 999)
	unsigned int beginDigit = factor;
	unsigned int temp, digit_t;

	int sizeArr = 256, weight = 0, weight_max = 0, i = 0;
	int *pArr;
	pArr = new int[sizeArr];	//массив чисел с максимальной усточивостью
	memset(pArr, 0, sizeof(pArr)*sizeArr);

	while (beginDigit <= (factor * 10) - 1) //перебор всех чисел в заданом диапазоне range
	{
		weight = 0;
		temp = beginDigit;

		while (true) //проходы, уровень устойчивости
		{
			digit_t = 1;
			while (temp && digit_t) //умножение всех цифр в числе
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
				if (weight < weight_max) break;//если глубина меньше максимальной


				else if (weight > weight_max) //если глубина больше максимальной
				{
					weight_max = weight;
					memset(pArr, 0, sizeof(pArr)*sizeArr);	
					i = 0;
				}
				//заполняем масив числами одинакового, наибольшего, уровня устойчивости
				pArr[i] = beginDigit;
				i++;
				//при необходимости вызываем функцию динамического расширения выделенной памяти
				if (i == sizeArr - 1)
					pArr = IncreasingArray(pArr, sizeArr);
				break;
			}
		}

		beginDigit++;
	}

	for (int i = 0; i < sizeArr && pArr[i]; i++) //вывод на экран всех чисел наибольшего уровня устойчивости
	{
		cout << pArr[i] << " ";
	}
	cout << endl;
	cout << "level of stability : " << weight_max << endl;
	delete[] pArr;
	system("pause");
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

