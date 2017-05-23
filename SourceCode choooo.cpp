#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h> 
#include <semaphore.h> 

void *Worker(void *arg);
void *PileSupplier(void *arg);
void *FuelSupplier(void *arg);
int isFound(void);


sem_t drillingPiles, Fuels;
sem_t emptyPiles,fillPiles,emptyFuels,fillFuels;

bool found=false;

int main()
{
	int res;
	pthread_t workerThread, pileSupplierThread, fuelSupplierThread;
	void *thread_result;
	
	//sem for piles
	res = sem_init(&drillingPiles, 0, 0);
	if(res != 0)
	{
		perror("Semaphore Drilling Piles initialization failed");
		exit(EXIT_FAILURE);
	}

	//sem for  filled piles
	res = sem_init(&fillPiles, 0, 0);
	if(res != 0)
	{
		perror("Semaphore Filled Piles initialization failed");
		exit(EXIT_FAILURE);
	}

	//sem for  filled fuels
	res = sem_init(&fillFuels, 0, 0);
	if(res != 0)
	{
		perror("Semaphore Filled Fuels initialization failed");
		exit(EXIT_FAILURE);
	}

	//sem for empty piles
	res= sem_init(&emptyPiles,0,0);
	if(res != 0)
	{
		perror("Semaphore Empty Piles initialization failed");
		exit(EXIT_FAILURE);
	}

	//sem for empty fuels
	res= sem_init(&emptyFuels,0,0);
	if(res != 0)
	{
		perror("Semaphore Empty Fuels initialization failed");
		exit(EXIT_FAILURE);
	}

	//sem for fuels
	res = sem_init(&Fuels, 0, 0);
	if(res != 0)
	{
		perror("Semaphore Fuels initialization failed");
		exit(EXIT_FAILURE);
	}

	printf("The semaphores are all initialized\n");



	//worker thread
 	res = pthread_create(&workerThread, NULL, Worker, NULL);
 	if (res != 0) {
 		perror("Thread creation failed");
 		exit(EXIT_FAILURE);
 	}

	
	//pile supplier thread
	res = pthread_create(&pileSupplierThread, NULL, PileSupplier, NULL);
 	if (res != 0) {
 		perror("Thread creation failed");
 		exit(EXIT_FAILURE);
 	}

	//fuel supplier thread
	res = pthread_create(&fuelSupplierThread, NULL, FuelSupplier, NULL);
 	if (res != 0) {
 		perror("Thread creation failed");
 		exit(EXIT_FAILURE);
 	}


	printf("Threads are all initialized\n");
	
	printf("Workers came to work!\n");
	res = pthread_join(workerThread, &thread_result);
	 if (res != 0) {
 		perror("Thread join failed");
 		exit(EXIT_FAILURE);
 	}
	printf("Workers went home with the mineral\n");
	pthread_cancel(fuelSupplierThread);
	pthread_cancel(pileSupplierThread);

	res = pthread_join(fuelSupplierThread, &thread_result);
	 if (res != 0) {
 		perror("Thread join failed");
 		exit(EXIT_FAILURE);
 	}
	printf("Employee in the fuel department went home\n");

	res = pthread_join(pileSupplierThread, &thread_result);
	 if (res != 0) {
 		perror("Thread join failed");
 		exit(EXIT_FAILURE);
 	}
	printf("Employee in the pile department went home\n");
	
	sem_destroy(&Fuels);
	sem_destroy(&drillingPiles);
	sem_destroy(&emptyPiles);
	sem_destroy(&emptyFuels);
	sem_destroy(&fillPiles);
	sem_destroy(&fillFuels);

	exit(EXIT_SUCCESS);
}

void *Worker(void *arg)
{
	int x;
	do{
		
	    
		printf("Resources empty, requested for refill\n");
		sleep(rand()%3 + 1);  
		sem_post(&emptyPiles);
		sem_post(&emptyFuels);
	     


		sem_wait(&fillPiles);
		sem_wait(&fillFuels);
		printf("Resources ready for drilling\n");
	
		x=1;
		do{
			for(int y = 0; y < 2; y++)
				sem_wait(&drillingPiles);
			for(int z = 0; z < 4; z++)
				sem_wait(&Fuels);
			printf("Drilling Started\n");
			sleep(rand()%3 + 1);  
			found = (isFound() == 5);	
			if(!found)
				printf("Mineral not found\n");
			else
				printf("Mineral found!\n");
			x++;
			
		}while(!found && x <= 6);
			
	
	}while(!found);
	

	pthread_exit(NULL);
}


void *PileSupplier(void *arg)
{
	do
	{
		int i=0;
		sem_wait(&emptyPiles);
		printf("Filling Piles....\n");
		//making 12 piles
		while(i<12)
		{
			sem_post(&drillingPiles);
			i++;
		}
		printf("12 Piles are ready\n");
		sem_post(&fillPiles);
	
	}while(!found);
	pthread_exit(NULL);

}

void *FuelSupplier(void *arg)
{

   do
     {
	int i=0;
	sem_wait(&emptyFuels);
	printf("Filling Fuel....\n");
	//making 24 units of fuel
	while(i<24)
	{
		sem_post(&Fuels);
		i++;
	}
	printf("24 Fuel units are ready\n");
	sem_post(&fillFuels);
	
      }while(!found);

	pthread_exit(NULL);

}

int isFound(void)
{
	srand(time(NULL));
	return (rand()%8+1);
}

	

