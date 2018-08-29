#include "omp.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// Reference : https://stackoverflow.com/questions/4287531/how-to-generate-random-numbers-in-parallel
// Reference : https://www.appentra.com/parallel-computation-pi/
int main(){
	int sum, num_threads, npoints,i, seed ;
	double rand_no_x, rand_no_y;
	npoints = 1000000;
	

	#pragma omp parallel default(private) shared(npoints) \ reduction(+:sum) num_threads(8)
		{

			sum=0;
			seed = rand();
			#pragma omp for
			for(i=1; i<=npoints; i++){
				rand_no_x=(double)(rand_r(&seed))/(double)((2<<14)-1);
				rand_no_y=(double)(rand_r(&seed))/(double)((2<<14)-1);
				if(((rand_no_x-0.5)*(rand_no_x-0.5)+(rand_no_y-0.5)*(rand_no_y-0.5))<0.25)
					sum++;
			}
		}


	double computed_pi= 4.0*(double)sum/((double)npoints);
	printf("Computed PI = %lf\n", computed_pi);
	
	return 0;
}