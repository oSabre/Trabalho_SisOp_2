#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define PI acos(-1.0)

// número de trapézios
int n = 1;

// número de threads a serem criadas
int t = 2;

pthread_t *threads;
float *resultados1, *resultados2;
int status, i;
void* thread_return;

// limites função 1
float a1 = 0.0;
float b1 = 10.0;

// limites função 2
float a2 = 0;
float b2 = 2*PI;

// função 1
float f1(float x){
	return 5.0;
}

// função 2
float f2(float x){
	float y;
	y = sin(2*x) + cos(5*x);
	return y;
}

float calculo1(float a, float b, int m){
	float h = (b-a)/m;
	float area_total = (f1(a)+f1(b))/2;
	float x_i;
	for (int i = 1; i < m; ++i){
		x_i = a+i*h;
		area_total += f1(x_i);
	}
	area_total = area_total*h;
	return area_total;
}

float calculo2(float a, float b, int m){
	float h = (b-a)/m;
	float area_total = (f2(a)+f2(b))/2;
	float x_i;
	for (int i = 1; i < m; ++i){
		x_i = a+i*h;
		area_total += f2(x_i);
	}
	area_total = area_total*h;
	return area_total;
}



void* funcThread(void *tid){
	float local_a1, local_b1, retorno1;
	float local_a2, local_b2, retorno2;
	int local_n1, local_n2;
	if((int)(size_t)tid > 0 && (int)(size_t)tid < t-1){
		local_a1 = ((b1 - a1)/t)*((int)(size_t)tid);
		local_n1 = n/t;
		local_b1 = ((b1 - a1)/t)*((int)(size_t)tid+1);

		local_a2 = ((b2 - a2)/t)*((int)(size_t)tid);
		local_n2 = n/t;
		local_b2 = ((b2 - a2)/t)*((int)(size_t)tid+1);
	}else if((int)(size_t)tid == 0){
		local_a1 = a1;
		local_n1 = n/t;
		local_b1 = ((b1 - a1)/t)*((int)(size_t)tid+1);

		local_a2 = a2;
		local_n2 = n/t;
		local_b2 = ((b2 - a2)/t)*((int)(size_t)tid+1);
	}else if((int)(size_t)tid == t-1){
		local_a1 = ((b1-a1)/t)*((int)(size_t)tid);
		local_b1 = b1;

		local_a2 = ((b2-a2)/t)*((int)(size_t)tid);
		local_b2 = b2;
		if(n%t != 0){
			local_n1 = (n/t)+(n%t);
			local_n2 = (n/t)+(n%t);
		}else{
			local_n1 = n/t;
			local_n2 = n/t;
		}
	}

	retorno1 = calculo1(local_a1, local_b1, local_n1);
	retorno2 = calculo2(local_a2, local_b2, local_n2);

	resultados1[(int)(size_t)tid] = retorno1;
	resultados2[(int)(size_t)tid] = retorno2;

	pthread_exit(NULL);
}

// argv[1] = número de threads
// argv[2] = número de trapézios 
int main(int argc, char const *argv[])
{
	t = atoi(argv[1]);
	n = atoi(argv[2]);

	threads = (pthread_t *) malloc(t * sizeof(pthread_t));
	resultados1 = (float *) malloc(t * sizeof(float));
	resultados2 = (float *) malloc(t * sizeof(float));

	for(i = 0; i < t; ++i){
		status = pthread_create(&threads[i], NULL, funcThread, (void *)(size_t) i);

		if(status != 0){
			printf("Erro na criação da thread. Código:%d\n", status);
			return 1;
		}
	}

	
	for (int i = 0; i < t; ++i){
		pthread_join(threads[i], &thread_return);
	}
	
	float somaTotal1, somaTotal2;

	for (int i = 0; i < t; ++i){
		somaTotal1 += resultados1[i];
		somaTotal2 += resultados2[i];
	}

	printf("Area total função 1: %f\nArea total função 2: %f\n", somaTotal1, somaTotal2);

	return 0;
}