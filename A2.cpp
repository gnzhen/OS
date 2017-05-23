#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h> 
#include <semaphore.h> 

void *workers(void *arg);
void *pile_supplier(void *arg);
void *fuel_supplier(void *arg);

sem_t fuel_empty, pile_empty, pile_fill, fuel_fill;
int pile = 0, fuel = 0, found = 0;

#define PILES_DELIVERED 12
#define FUELS_DELIVERED 24
#define PILES_CONSUMED 2
#define FUELS_CONSUMED 4

int main() {
	int res;
	pthread_t worker_thread, pile_supplier_thread, fuel_supplier_thread;
	void *thread_result;

	res = sem_init(&fuel_empty, 0, 0);     
	if (res != 0) {         
		perror("Semaphore fuel_empty initialization failed");         
		exit(EXIT_FAILURE);     
	}  
 
	res = sem_init(&pile_empty, 0, 0);     
	if (res != 0) {         
		perror("Semaphore pile_empty initialization failed");         
		exit(EXIT_FAILURE);     
	}

	res = sem_init(&fuel_fill, 0, 0);     
	if (res != 0) {         
		perror("Semaphore fuel_fill initialization failed");         
		exit(EXIT_FAILURE);     
	}

	res = sem_init(&pile_fill, 0, 0);     
	if (res != 0) {         
		perror("Semaphore pile_fill initialization failed");         
		exit(EXIT_FAILURE);     
	}

	printf("Semaphores initialization complete.\n");

	res = pthread_create(&worker_thread, NULL, workers, NULL);     
	if (res != 0) {         
		perror("worker_thread creation failed");         
		exit(EXIT_FAILURE);     
	}  
 
	res = pthread_create(&pile_supplier_thread, NULL, pile_supplier, NULL);     
	if (res != 0) {         
		perror("pile_supplier_thread creation failed");         
		exit(EXIT_FAILURE);     
	} 

	res = pthread_create(&fuel_supplier_thread, NULL, fuel_supplier, NULL);     
	if (res != 0) {         
		perror("fuel_supplier_thread creation failed");         
		exit(EXIT_FAILURE);     
	} 

	printf("Threads creation complete.\n");

	res = pthread_join(worker_thread, &thread_result);     
	if (res != 0) {         
		perror("worker_thread join failed");         
		exit(EXIT_FAILURE);     
	}

	res = pthread_join(pile_supplier_thread, &thread_result);     
	if (res != 0) {         
		perror("pile_supplier_thread join failed");         
		exit(EXIT_FAILURE);     
	}

	res = pthread_join(fuel_supplier_thread, &thread_result);     
	if (res != 0) {         
		perror("fuel_supplier_thread join failed");         
		exit(EXIT_FAILURE);     
	}

	printf("Threads join complete.\n");

	sem_destroy(&pile_empty);  
	sem_destroy(&fuel_empty);
	sem_destroy(&pile_fill);
	sem_destroy(&fuel_fill);

	exit(EXIT_SUCCESS);	
}

int isFound(void) {   
	srand(time(NULL));   
	return (rand()%8+1); 
} 

void *workers(void *arg) {

	do {
		if(pile < 2)
			sem_post(&pile_empty);
		if(fuel < 4)
			sem_post(&fuel_empty);
	
		sem_wait(&pile_fill);
		sem_wait(&fuel_fill);
	
		do {
			printf("Searching...\n");
			for(int i = 0; i < PILES_CONSUMED; i++)
				pile--;
			for(int j = 0; j < FUELS_CONSUMED; j++)
				fuel--;

			found = isFound();
			printf("found = %d\n", found);

			if(found == 8)
				printf("Mineral found.\n");
			else
				printf("Mineral not found.\n");
	
			if(found != 8)
				sleep(rand()%3 + 1);
			
		} while(found != 8 && pile >= 2 && fuel >= 4);

	} while(found != 8);

	pthread_exit(NULL);
}

void *pile_supplier(void *arg) {
	
	do {
		sem_wait(&pile_empty);

		printf("Delivering piles.\n");
		for(int i = 0; i < PILES_DELIVERED; i++)
			pile++;

		sem_post(&pile_fill);

	} while(found != 8);

	pthread_exit(NULL);
}

void *fuel_supplier(void *arg) {

	do {
		sem_wait(&fuel_empty);

		printf("Delivering fuels.\n");
		for(int i = 0; i < FUELS_DELIVERED; i++)
			fuel++;

		sem_post(&fuel_fill);

	} while(found != 8);

	pthread_exit(NULL);
}






