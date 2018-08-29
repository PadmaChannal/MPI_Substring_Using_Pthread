// Mostly a circular queue Meaning If start of queue is same as size, then bring the pointer to first place
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// Please enter Queue Size here
#define QSIZE 2
char MyQ [QSIZE] ; pthread_mutex_t lock ; 
int frst = 0 , lst = 0 , ptr = 0 ,signalFlag = 1 ; 
pthread_cond_t cond1, cond2 ; // Condition variables. Reference: ftp://sourceware.org/pub/pthreads-win32/sources/pthreads-w32-2-9-1-release/manual/pthread_cond_init.html

void * toQueue()
{	
	FILE *fp = fopen("message.txt","r"); 
	char letter = getc(fp); 
	while (letter != EOF) { 
		pthread_mutex_lock (&lock) ; // Declare a lock. As discussed in class, a lock is inside while loop.
			while ( ptr >= QSIZE )  
				pthread_cond_wait ( &cond2 , &lock ) ; 
			MyQ [frst++] = letter ; // put the character into queue
			if(frst==QSIZE) frst = 0 ;ptr ++ ; // it head of queue has reached the size, then let it point to first element
			// printf ("PRODUCER is writing to Queue : %c \n" , letter ) ; //For DEBUGGING purpose only
			pthread_cond_signal(&cond1); 
			pthread_mutex_unlock (&lock) ; // unlock the variable 

		letter = getc(fp);		
	}
	pthread_mutex_lock (&lock) ;
	signalFlag = 0 ; // to indicate the consumer about the availibility
	pthread_cond_signal(&cond1);
	pthread_mutex_unlock (&lock) ; // unlock 
	
	fclose (fp); 
	 pthread_exit (0); 
}

void * fromQueue()
{	
	while ( signalFlag != 0 ) { 
		pthread_mutex_lock (&lock) ; // acquire lock
		while ( ptr <= 0 && signalFlag!= 0 )  
			pthread_cond_wait ( &cond1 , &lock ) ;
		char letter = MyQ [lst++] ;// read the character and increase the last pointer 
		if (lst==QSIZE) lst = 0 ; ptr -- ; // decrease the pointer and make last as 0 if it reaches queue size
		printf ("%c" , letter ) ;
		pthread_cond_signal(&cond2); 
		pthread_mutex_unlock (&lock) ;	 // release lock
	}

	pthread_exit (0);
}
void main(int argc, char *argv[])
{// main function to create and join consumer and producer threads
	pthread_t thrd1 , thrd2;
	pthread_create ( &thrd1 , NULL, toQueue, NULL);
	pthread_cond_init (&cond1, NULL) ;
	pthread_create ( &thrd2 , NULL, fromQueue, NULL);
	pthread_cond_init (&cond2, NULL) ;
	pthread_mutex_init (&lock, NULL) ;
	pthread_join ( thrd1 , NULL );
	pthread_join ( thrd2 , NULL );	
	
}