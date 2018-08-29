#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
int main(){
    
    int total=0, sum=0;
    double index,rand_no_x = 0, rand_no_y = 0, pi;
    #pragma omp parallel 
    {
      #pragma omp for
      for(index = 0;index < 9999999;index++){
        rand_no_x=(rand() % (6000)) -3000;
        rand_no_y=(rand() % (6000)) -3000;
        #pragma omp critical
        {
          total++;
        }
          if (rand_no_x*rand_no_x + rand_no_y*rand_no_y <= 3000*3000)
            #pragma omp critial
            {
              sum++;
            }
      }
    }
   
    pi = 4*sum/(double)total;
    printf("Value of pi is %f\n",pi);
   

}