#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define ENCODE 0
#define DECODE 1
#ifndef MODE
	#define MODE ENCODE
#endif

void encode(char *argv[],char data[],int keyLength);
void decode(char *argv[],char data[],int keyLength);

int main(int argc, char *argv[]) {

char *filepath = argv[1];
unsigned char buffer[5000];
char data[128]; 
FILE *INPUT;

if ((INPUT = fopen(filepath, "rb")) == NULL ) {
     printf("Problem opening key file '%s'; errno: %d/n", filepath, errno);
     exit(1);
}

fseek(INPUT,0L, SEEK_END);
int size = ftell(INPUT);
rewind(INPUT);

fread(buffer,1,size,INPUT);
if(size > 128) {
	for(int i = 0; i < 128; i++){
		data[i] = (char)buffer[i];
	}
   size = 128; 
}
else {
	for(int i = 0; i < size; i++){
		data[i] = (char)buffer[i];
	}
}

fclose(INPUT);

if (MODE == ENCODE) {
encode(argv, data, size);
}
else {
decode(argv, data, size);
}

return 0;

}


void encode(char *argv[],char data[],int keyLength) {

char *filepath = argv[2];
char *output = argv[3]; 


//open input file
FILE *INPUT;
if ((INPUT = fopen(filepath, "rb")) == NULL ) {
     printf("Problem opening encode file '%s'; errno: %d/n", filepath, errno);
     exit(1);
}

//open output file
FILE *OUTPUT;
if ((OUTPUT = fopen(output, "w+")) == NULL) {
    printf("Problem opening output file '%s'; errno: %d\n", output, errno);
    exit(1);
}


char* keyindex = &data[0];
int ptrcount = 0; 
char cipher;
unsigned char efile[10000];

//read through file and encrypt
int bytes; 
while((bytes = fread(&efile,1,1,INPUT)) > 0){
  for( int i = 0; i < bytes; i++){
   cipher = (char) (((char) efile[i] + *keyindex) % 256);
   fprintf(OUTPUT, "%c", cipher);
   keyindex++;
   ptrcount++;
	if(ptrcount == keyLength) {
	 keyindex = &data[0];
	 ptrcount =0;
		}
  	 }

 } 
fclose(INPUT);
fclose(OUTPUT);
}

void decode(char *argv[], char data[],int keyLength) {

char *filepath = argv[2];
char *output = argv[3]; 

//open input file
FILE *INPUT;
if ((INPUT = fopen(filepath, "rb")) == NULL ) {
     printf("Problem opening encode file '%s'; errno: %d/n", filepath, errno);
     exit(1);
}

//open output file
FILE *OUTPUT;
if ((OUTPUT = fopen(output, "w+")) == NULL) {
    printf("Problem opening output file '%s'; errno: %d\n", output, errno);
    exit(1);
}

char* keyindex = &data[0];
int ptrcount = 0; 
char cipher;
unsigned char dfile[10000];

int bytes; 
while((bytes = fread(&dfile,1,1,INPUT)) > 0){
  for( int i = 0; i < bytes; i++){
   cipher = (char) (((char) dfile[i] - *keyindex) % 256);
   fprintf(OUTPUT, "%c", cipher);
   keyindex++;
   ptrcount++;
	if(ptrcount == keyLength) {
	 keyindex = &data[0];
	 ptrcount =0;
		}
  	 }

 } 

}
