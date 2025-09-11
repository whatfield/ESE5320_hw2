#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "stopwatch.h"

#define WIN_SIZE 16
#define PRIME 3
#define MODULUS 256
#define TARGET 0

stopwatch cdc_time;
stopwatch cdc_time_revised;

uint64_t hash_func(unsigned char *input, unsigned int pos)
{
	uint64_t hash = 0;
	for(int i = 0; i < WIN_SIZE; i++)
	{
		hash += (uint64_t)(input[pos+WIN_SIZE-1-i]) * (u_int64_t)(pow(PRIME, i+1));
	}
	return hash;
}

void cdc(unsigned char *buff, unsigned int buff_size)
{
	for (int i = WIN_SIZE; i < buff_size - WIN_SIZE; i++){
		uint64_t hash = hash_func(buff, i);
		if(hash % MODULUS == TARGET){
			printf("%d\n", i);
		}
	}
}

void cdc_revised(unsigned char *buff, unsigned int buff_size){
	uint64_t hash = hash_func(buff, WIN_SIZE);
	uint64_t hash_next;
	for (int i = WIN_SIZE; i < buff_size - WIN_SIZE; i++){
		if(hash % MODULUS == TARGET){
			printf("%d\n", i);
		}
		hash_next = hash*PRIME - (uint64_t)buff[i]*pow(PRIME, WIN_SIZE+1)+(uint64_t)buff[i+WIN_SIZE]*PRIME;
		hash = hash_next;
	}
}

void test_cdc( const char* file )
{
	FILE* fp = fopen(file,"r" );
	if(fp == NULL ){
		perror("fopen error");
		return;
	}

	fseek(fp, 0, SEEK_END); // seek to end of file
	int file_size = ftell(fp); // get current file pointer
	fseek(fp, 0, SEEK_SET); // seek back to beginning of file

	unsigned char* buff = (unsigned char *)malloc((sizeof(unsigned char) * file_size ));	
	if(buff == NULL)
	{
		perror("not enough space");
		fclose(fp);
		return;
	}

	int bytes_read = fread(&buff[0],sizeof(unsigned char),file_size,fp);

	cdc_time.start();
	cdc(buff, file_size);
	cdc_time.stop();
	

    free(buff);
    return;
}

void test_cdc_revised( const char* file )
{
	FILE* fp = fopen(file,"r" );
	if(fp == NULL ){
		perror("fopen error");
		return;
	}

	fseek(fp, 0, SEEK_END); // seek to end of file
	int file_size = ftell(fp); // get current file pointer
	fseek(fp, 0, SEEK_SET); // seek back to beginning of file

	unsigned char* buff = (unsigned char *)malloc((sizeof(unsigned char) * file_size ));	
	if(buff == NULL)
	{
		perror("not enough space");
		fclose(fp);
		return;
	}

	int bytes_read = fread(&buff[0],sizeof(unsigned char),file_size,fp);

	cdc_time_revised.start();
	cdc_revised(buff, file_size);
	cdc_time_revised.stop();
	
    free(buff);
    return;
}


int main()
{
	test_cdc("prince.txt");
	test_cdc_revised("prince.txt");
	printf("Average latency of cdc is: %lf ns.\n", cdc_time.avg_latency());
	printf("Average latency of cdc_revised is: %lf ns.\n", cdc_time_revised.avg_latency());
	return 0;
}