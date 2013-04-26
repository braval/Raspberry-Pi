#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <fcntl.h>
#include <malloc.h>
#include <inttypes.h>
int start_capturing();
int read_data();
void change_size();

int fd ;
int a=0x0000,j=0,k=0,count=0; //Read Starting address
char MH,ML;
char LH,LL;
unsigned int chunk_size = 32;

int main ()
{

  if ((fd = serialOpen ("/dev/ttyAMA0", 38400)) < 0)
    {
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
      return 1 ;
    }

  if (wiringPiSetup () == -1)
    {
      fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
      return 1 ;
    }

  int file = open("test.jpg",O_RDWR | O_CREAT,0644);
  printf ("\nTesting Camera");
  fflush (stdout);

  delay (3) ;
  char h;
  printf("Press ENTER to continue\n");
  scanf("%c",&h);

  //Waiting for camera data which is ignored
  while (serialDataAvail (fd))
    {
      serialGetchar(fd);
    }

  delay (100) ;
  int check;
  char t;
  printf ("Starting to capture \n");
  fflush (stdout);
  delay(100);
  check = start_capturing();
  if(check == -1)
    {
      printf("Error with take picture command\n");
      return 1;
    }
  delay(100);
  printf("\nSending read data command\n");
  char *data = malloc(sizeof(char)*32);
  int l = 0;
  for(l=0;l<32;l++)
    data[l] = 0;

  int End = 0;
  int b = 1;
  while(!End)
    {
      delay(200);
      read_data();
      int j = 0;
      int k = 0;
      count = 0;
      delay(100);
      while (serialDataAvail (fd))
	{
	  printf("In\n");
	  k++;
	  t = serialGetchar(fd);
	  //Ignoring the response
	  if((k>5) && (j<32) && (!End))
	    {
	      data[j] = t;
	      //Checking for the end of image
	      if((data[j-1] == 0xFF) && (data[j] == 0xD9))
		End = 1;
	      j++;
	      count++;
	    }
	}
      //Writing the image to the output file
      write(file,(void *)data,32);
      b++;
    }
  close(file);

  return 0 ;
}

int start_capturing()
{
  serialPutchar(fd,0x56);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x36);
  serialPutchar(fd,0x01);
  serialPutchar(fd,0x00);

	int i = 0;
	char temp[5];
	char return_val[] = {0x76,0x0,0x36,0x0,0x0};
	while(i<5)
	{
	  temp[i] = serialGetchar(fd);
	  if(temp[i] != return_val[i])
	    return -1;
		i++;
	}
	return 1;
}


void get_size()
{

  serialPutchar(fd,0x56);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x34);
  serialPutchar(fd,0x01);
  serialPutchar(fd,0x00);
}


int read_data()
{
  MH= a/0x100;
  ML= a%0x100;
  LH = chunk_size >> 8;
  LL = chunk_size;

  serialPutchar(fd,0x56);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x32);
  serialPutchar(fd,0x0c);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x0a);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x00);
  serialPutchar(fd,MH);
  serialPutchar(fd,ML);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x20);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x0a);
  a+=0x20; //address increases 32Â£Â¬set according to buffer size
  return 1;
}

void change_size()
{
  serialPutchar(fd,0x56);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x31);
  serialPutchar(fd,0x05);
  serialPutchar(fd,0x04);
  serialPutchar(fd,0x01);
  serialPutchar(fd,0x00);
  serialPutchar(fd,0x19);
  serialPutchar(fd,0x00);
}
