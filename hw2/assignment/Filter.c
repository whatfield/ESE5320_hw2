#include <stdlib.h>
#include <stdio.h>
#include "App.h"
#include "stopwatch.h"

#define INPUT_HEIGHT (4000)
#define INPUT_WIDTH (6000)

#define FILTER_LENGTH (7)

#define OUTPUT_HEIGHT (INPUT_HEIGHT - (FILTER_LENGTH - 1))
#define OUTPUT_WIDTH (INPUT_WIDTH - (FILTER_LENGTH - 1))

unsigned Coefficients[] = {2, 15, 62, 98, 62, 15, 2};

void Filter_horizontal(const unsigned char * Input, unsigned char * Output)
{
  LOOP1: for (int Y = 0; Y < INPUT_HEIGHT; Y++)
    LOOP2: for (int X = 0; X < OUTPUT_WIDTH; X++)
    {
      unsigned int Sum = 0;
      LOOP3: for (int i = 0; i < FILTER_LENGTH; i++)
        Sum += Coefficients[i] * Input[Y * INPUT_WIDTH + X + i];
      Output[Y * OUTPUT_WIDTH + X] = Sum >> 8;
    }
}

void Filter_vertical(const unsigned char * Input, unsigned char * Output)
{
  LOOP1: for (int Y = 0; Y < OUTPUT_HEIGHT; Y++)
    LOOP2: for (int X = 0; X < OUTPUT_WIDTH; X++)
    {
      unsigned int Sum = 0;
      LOOP3: for (int i = 0; i < FILTER_LENGTH; i++)
        Sum += Coefficients[i] * Input[(Y + i) * OUTPUT_WIDTH + X];
      Output[Y * OUTPUT_WIDTH + X] = Sum >> 8;
    }
}

stopwatch filter_horiz;
stopwatch filter_vert;

void Filter(const unsigned char * Input, unsigned char * Output)
{
  unsigned char * Temp = (unsigned char*)malloc(INPUT_HEIGHT * OUTPUT_WIDTH);
  filter_horiz.start();
  Filter_horizontal(Input, Temp);
  filter_horiz.stop();
  filter_vert.start();
  Filter_vertical(Temp, Output);
  filter_vert.stop();

  printf("Average latency of horizontal filter is: %lf ns.\n", filter_horiz.avg_latency());
  printf("Average latency of vertical filter is: %lf ns.\n", filter_vert.avg_latency());

  free(Temp);
}

