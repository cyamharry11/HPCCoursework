
#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code. 


  Compile with:
    nvcc -o CrackAZ99-With-Cuda CrackAZ99-With-Cuda.cu 

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./CrackAZ99-With-Cuda > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
__device__ int is_a_match(char *attempts) {	// Compares each password attempt.
  char plain_passwords1[] = "IT8312";
  char plain_passwords2[] = "RB3211";
  char plain_passwords3[] = "AV7213";
  char plain_passwords4[] = "ES2114";

  char *x = attempts;
  char *p = plain_passwords1;
  char *q = plain_passwords2;
  char *r = plain_passwords3;
  char *s = plain_passwords4;


  while(*x == *p) {
    if(*x== '\0') {
  printf("Found password successfully: %s\n",plain_passwords1);
      break;
    }
    x++;
    p++;
  }
  
while(*x == *q) {
    if(*x== '\0') {
  printf("Found password successfully: %s\n",plain_passwords2);
      break;
    }
    x++;
    q++;
  }
while(*x == *r) {
    if(*x== '\0') {
  
 printf("Found password successfully: %s\n",plain_passwords3); 
    break;
    }
    x++;
    r++;
  }
while(*x == *s) {
    if(*x== '\0') {
  printf("Found password successfully: %s\n",plain_passwords4);
      return 1;
    }
    x++;
    s++;
  }

  return 0;
}


__global__ void  kernel() {
char k,l,m,n;
  
  char password[7];
  password[6] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstValue = i; 
char secondValue = j; 
    
password[0] = firstValue;
password[1] = secondValue;
	for(k='0'; k<='9'; k++){
	  for(l='0'; l<='9'; l++){
	   for(m='0'; m<='9'; m++){
	     for(n='0'; n<='9'; n++){
	        password[2] = k;
	        password[3] = l;
	        password[4] = m;
	        password[5] = n; 
	      if(is_a_match(password)) {
		//printf("Success");
	      } 
             else {
	     //printf("tried: %s\n", password);		  
	         }
	      }
	   }
	}
    }
}

int time_difference(struct timespec *start, 
                    struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main() {
// starting kernel
  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

  kernel <<<26, 26>>>();

  cudaThreadSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9)); 

  return 0;
}
