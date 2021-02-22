#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// número de trapézios
int n = 1;

// número de threads a serem criadas
int t = 2;

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

void* funcThread(){
	float local_a, local_b, retorno;
	int local_n;
	//retorno = calculo(local_a, local_b, local_n);
	pthread_exit(NULL);
}

// argv[1] = número de threads
// argv[2] = número de trapézios 
int main(int argc, char const *argv[])
{
	t = atoi(argv[1]);
	n = atoi(argv[2]);

	pthread_t threads[t];
	int status, i;
	void* thread_return;

	for(i = 0; i < t; ++i){
		printf("Processo principal criando thread #%d\n", i);
		status = pthread_create(&threads[i], NULL, funcThread, (void *)(size_t) i);

		if(status != 0){
			printf("Erro na criação da thread. Código:%d\n", status);
			return 1;
		}
	}

	for(i = 0; i < t; i++){
		printf("Esperando Thread %d finalizar \n", i);
		pthread_join(threads[i], &thread_return);
		printf("Thread %d finalizada\n", i);
	}

	printf("Processo vai finalizar.\n");
	return 0;
}