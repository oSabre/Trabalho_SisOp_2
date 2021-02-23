#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// número de trapézios
int n = 1;

// número de threads a serem criadas
int t = 2;

pthread_t *threads;
int status, i;
void* thread_return;

// limites função 1
float a1 = 0.0;
float b1 = 10.0;

// função 1
float f1(float x){
	return 5.0;
} 

float calculo(float a, float b, int m){
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

void* funcThread(void *tid){
	float local_a, local_b, retorno;
	int local_n;
	if((int)(size_t)tid > 0){
		pthread_join(threads[(size_t)tid-1], &thread_return);
		printf("Esta é a thread %d. A thread %d finalizou. \n", (int)(size_t)tid, (int)(size_t)tid-1);
	}else{
		printf("Esta é a primeira thread\n");
	}

	//retorno = calculo(local_a, local_b, local_n);
	pthread_exit(NULL);
}

// argv[1] = número de threads
// argv[2] = número de trapézios 
int main(int argc, char const *argv[])
{
	t = atoi(argv[1]);
	n = atoi(argv[2]);

	threads = (pthread_t *) malloc(t * sizeof(pthread_t));

	for(i = 0; i < t; ++i){
		printf("Processo principal criando thread #%d\n", i);
		status = pthread_create(&threads[i], NULL, funcThread, (void *)(size_t) i);

		if(status != 0){
			printf("Erro na criação da thread. Código:%d\n", status);
			return 1;
		}
	}

	printf("Esperando thread %d finalizar\n", i-1);
	pthread_join(threads[i-1], &thread_return);
	printf("thread %d finalizou\n", i-1);
	printf("Processo vai finalizar.\n");
	return 0;
}