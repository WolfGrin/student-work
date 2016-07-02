/*
	программа для вычисления значения функции y(x)
	y(x) = a integral b * sin(x) + cos(x) * dx
	Реализованно паралельное выполнение задачи используя интерфейс MPICH
*/

#define _USE_MATH_DEFINES
#include "mpi.h"
#include "stdio.h"
#include <iostream>
#include <math.h>

const double a = 0.0;		//Нижний предел
const double b = M_PI;		//Верхний предел
const double h = 0.00001;	//Шаг дискритезации

using namespace std;
int main ( int argc, char *argv[ ] )
{
	int numprocs, myrank;
	
	MPI_Status status;
	MPI_Init(&argc, &argv);	//Инициализация MPI
	
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);	//Определяем свой номер в группе
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);		//Определяем размер группы

	//интеграл sin(x)*cos(x) вычисляем следующим образом - h* (sin(x0)*cos(x0)+sin(x1)*cos(x1) + ...)
	int xMin = 0;
	double res, tStart, range, cur_a, cur_b;
	double sbuf [2], sum = 0;
	

	if(!myrank)
	{
		tStart = MPI_Wtime();		//отсчет времени
		range = (b - a) / numprocs;	//диапазон для процессов
			
		cur_a = a + range;
		for(int i = 1; i < numprocs; i++)
		{
			cur_b = cur_a + range - h;
			sbuf[0] = cur_a;
			sbuf[1] = cur_b;
			MPI_Send(sbuf, 2, MPI_DOUBLE, i, 101,  MPI_COMM_WORLD);
			cur_a += range;
		}
		cur_a = a;
		cur_b = a + range - h;
	}
	else
	{
		MPI_Recv(sbuf, 2, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD, &status);
		cur_a = sbuf[0];
		cur_b = sbuf[1];
	}
	
	cout<<"Process : "<<myrank<<" | Lower limit: "<<cur_a<<" | Upper limit: "<<cur_b<<endl;
	for(float x = cur_a; x <= cur_b; x += h)
		sum += sin(x) + cos(x);

	//сбор результатов
	MPI_Reduce(&sum, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(!myrank)
	{
		double tEnd = MPI_Wtime();
		res *= h;
		printf("Elapsed time: %.4f\n", tEnd-tStart);
		printf("Result: %f\n", res);
		cin>>res;
	}
	
	MPI_Finalize();	//Завершение работы с MPI
	return 0;
}