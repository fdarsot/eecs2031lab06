/*  */

#include <stdio.h>
#include <wiringPi.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "ifttt.h"
#include <time.h>


void tempRead(char *serialNum);

int main(int argc, char *argv[])
{
  wiringPiSetup () ;
  char *device = "28-021312d0c0aa";
  tempRead(device); /*call function, pass device serial number*/
  
  return 0; /*never returned*/
}



void tempRead(char *serialNum)
{
	
  FILE *fd; 
  int n;
  char buf[1001]; /* string buffer of size 1001, first 1000 characters will be read, last one is terminating character */
  char temperature[6];/* temp *1000 */
  char path[1000] = "/sys/bus/w1/devices/"; /* create array for path */
  strcat(path, serialNum); /*concatenate the device serial  */
  strcat(path, "/w1_slave"); /*concatenate the text file that contains the temp, now full path is done*/
  
  char maximum[500];
  char minimum[500];
  char current[500];
  
  int firstInstance = 1;
  
  float max,tempC;
  float tempCprev;
  tempCprev = tempC; /*prev temp*/  
  
  float min; 
  min = -5.00; /*picked arbitrary number to initlaize min for if statement*/
  
  /*
  const int RUNTIME = 1;   1 second
  time_t start, current; */
  

  if(serialNum == NULL)
  {
  	  perror("Please Provide a single argument to the tempRead function");
  	  (void) exit(1);
  }
  
  while(1){	    
  /* fprintf(stdout, "FIRST TIME '%s'\n", path); testing if the path works */
  fd = fopen(path, "r");/* make fd = to open path, read only  */
  if(fd == (FILE *)NULL)  /* if fd == NULL, then it means it cant opent the file, so exit*/
  {
  	 perror("open of w1_slave failed. Device not found or device Serial number is wrong \n");
  	 (void) exit(1);
  } 
  
  /*
  int fd_check = (int) fd; 
  if(fd_check == -1)    fd_check returns -1 if cannot find device
  {
   perror ("Couldn't open the w1 device. \n");
  	  exit(1); 
  }
  */
  
  
  n = fread(buf,sizeof(char),1000,fd); delay(900); /* fread returns total number of elements successfully read*/
  if(n==0) /*if no items have been read*/
  {
  	  perror("read failed"); /* then read fails */
  	  exit(1); 
  }
  
  buf[n] = '\0'; /*terminate array with null character*/
  
   /* fprintf(stdout, "Read '%s'\n", buf);  testing the reading of the file */
  
   strncpy(temperature, strstr(buf, "t=") + 2, 5);  /*skip 2 characters (t=) and count the next 5 (the temp) and copy into array temperature*/
   temperature[n]='\0'; /*terminate array with null character*/
   
   tempC = (float)atof(temperature); /*convert temperature from char array to float*/
   tempC = tempC /1000;
   
   if(min == -5.00)
   {
   	  min = tempC; /*min = tempC to start with*/  	        	   
 	  /*if this gets executed that means this is the first reading*/
 	  max = tempC;  	  
 	  tempCprev = tempC;
 	  
 	  sprintf(maximum, "Highest Temp: %.3f", max);
 	  sprintf(minimum, "Lowest Temp: %.3f", min);
 	  sprintf(current, "Current Temp: %.3f", tempC);
 	  printf("ifttt run for the first time \n");
  	 ifttt("https://maker.ifttt.com/trigger/Temperature_change/with/key/dOAsUOxosjh0MW7O3HSvFl", maximum, minimum, current);   	   
 	  
   }
   
    if(tempC > max){
   	   max = tempC;
   }
   
   
   if(tempC<min)
   {
   	   min = tempC;
   }
   
   int onesDigit =  (int) tempC % 10; /*get the ones Digit 2(4).54 or 2(2).25 and check it*/
   int onesDigitPrev = (int) tempCprev %10; /*get the ones Digit 2(4).54 or 2(2).25 and check it*/
   int onesDigitDiff = onesDigit-onesDigitPrev;
   
   int oneHigher = ++onesDigit;
   int oneLower = --onesDigit;

   	  printf("value of onesDigitDiff  before IF is %d \n", onesDigitDiff);      	     
   	   if((onesDigitDiff >= 1) || (onesDigitDiff <= -1))
   	   {
   	   	  sprintf(maximum, "Highest Temp: %.3f", max);
		  sprintf(minimum, "Lowest Temp: %.3f", min);
		  sprintf(current, "Current Temp: %.3f", tempC);
		  printf("ifttt called due to change in temp \n");  	   
		  ifttt("https://maker.ifttt.com/trigger/Temperature_change/with/key/dOAsUOxosjh0MW7O3HSvFl", maximum, minimum, current);
		  
   	   }
   
   
   	   float difference = tempC - tempCprev;
  /* if((tempC-tempCprev)>=1)
   {
	  sprintf(maximum, "Highest Temp: %.3f", max);
 	  sprintf(minimum, "Lowest Temp: %.3f", min);
 	  sprintf(current, "Current Temp: %.3f", tempC);
 	  printf("ifttt called due to change in temp \n");  	   
  	 ifttt("https://maker.ifttt.com/trigger/Temperature_change/with/key/dOAsUOxosjh0MW7O3HSvFl", maximum, minimum, current);   	   
   }*/
   	 printf("value of PREV at END is %f \n", tempCprev); 
   	  printf("value of CURRENT  at END is %f \n", tempC);     	  
   /*  printf("value of onesDigit  at END is %d \n", onesDigit);   */
   	  
   	  tempCprev = tempC;   
   printf("Current Temp: %.3f C, Max: %.3f C, Min: %.3f C  \n", tempC, max, min); /*print temp, divide by 1000*/
    printf("\n");    

   
 (void) fclose(fd); /* close the fd */
  
   } /*WHILE LOOP ENDS HERE*/
  
  
}
