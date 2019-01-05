#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <crypt.h>
 

/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code. 


  Compile with:
   cc -o CrackAZ99-With-Posix CrackAZ99-With-Posix.c -lcrypt

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./CrackAZ99-With-Data > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;
pthread_t t1, t2;
// Encrypted passwd are kept here 
char *encrypted_passwords[] = {
"$6$KB$IoVjGxl2H9u6lVL.iYHTxkNVmyEWEKR2Di5oN3m4sWOPBVQznxqMkIOvZ9fHH92BwAO5TE6yPH94XPRGrx66O0",

"$6$KB$WHGPdZpea02k5HC4Xe3b.hPfVKNmhB1N1PH2dipVWPaiIjfeXXw2/CKM/96w3KXJpy/bVmSKvPVEbxRcPbxKp/",

"$6$KB$pcrtsVHQbNSnqZfaDNSeCkN0VnLgGjOrGe4cDM2LAvEA3jKu/Dravsf4byluUiplsSxOlWSaJFFTsxqz1If2V.",

"$6$KB$BUzFS.GZFgoa2DvC3MgE55eaBLtoiZPe/eXYi9NA20gZm/gUHKlqqejqawlUjq5PJp0D1NpUOfAzT2CH5DmEU1"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}




/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the 
 start of the line. Note that one of the most time consuming operations
that 
 it performs is the output of intermediate results, so performance
experiments 
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void *firstRun(void *salt_and_encrypted){
  int a, b, c;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='M'; a++){
    for(b='A'; b<='Z'; b++){
      for(c=0; c<=99; c++){
        sprintf(plain, "%c%c%02d", a, b, c); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}



void *secondRun(void *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='N'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
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

int main(int argc, char *argv[]){
  int i;
  //for(i=0;i<n_passwords;i<i++) {
    //crack(encrypted_passwords[i]);
  //}

  struct timespec start, finish;   
  long long int time_elapsed;
	

  clock_gettime(CLOCK_MONOTONIC, &start);
 for(i=0;i<n_passwords;i<i++) {
   pthread_create(&t1, NULL, firstRun, encrypted_passwords[i]);
   pthread_create(&t2, NULL, secondRun, encrypted_passwords[i]);
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);		

  }
 

  clock_gettime(CLOCK_MONOTONIC, &finish);
   time_difference(&start, &finish, &time_elapsed);


  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}
