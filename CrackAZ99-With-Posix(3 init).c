#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include<time.h>


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

    ./CrackAZ99-With-Posix > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$IoVjGxl2H9u6lVL.iYHTxkNVmyEWEKR2Di5oN3m4sWOPBVQznxqMkIOvZ9fHH92BwAO5TE6yPH94XPRGrx66O0",

"$6$KB$R8cWvremVmo5HrjSlJBRMPPIYGLzDcpc.WnWo.Cx5wq/OWvjS1CKauasWrEWAyrsaDmBcJeCj2JaAMI74Nl7x.",

"$6$KB$Ppu3.HLZyTWpFYTH3.FUHYySUqn7GjhDkrNSO8WL29d2/6yXlPXPRE9V4xCxlvWN6PO5LLV.SzjPbrFClIymY0",

"$6$KB$g7gR5TkllL7dypUrDx1fY5BTgg7XCUCzhOm68/rismXzta3X.m6oJW9RYt2K1T7e9cMl8sUUUf6xEopjaETg01"
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

void crack(char *salt_and_encrypted){
  int a, x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
 for(a='A'; a<='Z'; a++){
  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%c%02d", a,x, y, z); 
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
}
  printf("%d solutions explored\n", count);
}



//copy
int time_difference(struct timespec *start, struct timespec *finish, 
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
 
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
   for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  	}

  clock_gettime(CLOCK_MONOTONIC, &finish);

  time_difference(&start, &finish, &time_elapsed);

  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}
