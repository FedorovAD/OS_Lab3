#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

double calc_square(double r){
	return r * r * 3.14;
}

void* calculate_dots(void* args){
	double dots_per_thread = *((double*) args);
	double r = *((double*) args + 1);
	srand(time(NULL));
	rand();

	int* dots = malloc(sizeof(int));
	*dots = 0;
	double rMax = r;

	for (int i = 0; i < dots_per_thread; ++i) {
		double x = (double)rand() / RAND_MAX * rMax;
		double y = (double)rand() / RAND_MAX * rMax;
		printf("%lf %lf\n", x , y);
		if((x * x + y * y) <= r * r){
			++(*dots);
		}
	}
	return (void*)dots; 
}

int main(int argc, char const *argv[]) {
	if(argc > 2){
		printf("Usage: %s + <number_of_threads>\n", argv[0]);
		return -1;
	}

	int n = 1;
	if(argc == 2){
		n = atoi(argv[1]);
		printf("%d\n", n);
		if(n == 0){
			printf("Usage: %s + <number_of_threads>\n", argv[0]);
			return -1;
		}
	}

	const int dots_cnt = 1000000;
	double dots_per_thread = dots_cnt / n;
	printf("%lf\n", dots_per_thread);

	double r;
	scanf("%lf", &r);

	double args[2] = {dots_per_thread, r};

	pthread_t threads[n];

	for (int i = 0; i < n; ++i) {
		pthread_create(&threads[i], NULL, calculate_dots, &args);
	}

	int counted_dots = 0;

	for (int i = 0; i < n; ++i)	{
		int* cur;
		pthread_join(threads[i], (void**)&cur);
		counted_dots += *cur;
		printf("!%d\n", *cur);
		free(cur);
	}

	printf("Calculated square: %lf\n", r * r * counted_dots / (dots_per_thread * n) * 4.0);
	printf("Estimated square: %lf\n", calc_square(r));

	return 0;
}