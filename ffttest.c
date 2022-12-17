/*
 * This file is part of pocketfft.
 * Licensed under a 3-clause BSD style license - see LICENSE.md
 */

/*
 *  Test codes for pocketfft.
 *
 *  Copyright (C) 2004-2018 Max-Planck-Society
 *  \author Martin Reinecke
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "pocketfft.h"

#define maxlen 8192

static void fill_random (float *data, size_t length)
  {
  for (size_t m=0; m<length; ++m)
    data[m] = rand()/(RAND_MAX+1.0)-0.5;
  }

static float errcalc (float *data, float *odata, size_t length)
  {
  float sum = 0, errsum = 0;
  for (size_t m=0; m<length; ++m)
    {
    errsum += (data[m]-odata[m])*(data[m]-odata[m]);
    sum += odata[m]*odata[m];
    }
  return sqrt(errsum/sum);
  }

static int test_real(void)
  {
  float data[maxlen], odata[maxlen];
  const float epsilon=2e-6;
  int ret = 0;
  fill_random (odata, maxlen);
  float errsum=0;
  for (int length=1; length<=maxlen; ++length)
    {
    memcpy (data,odata,length*sizeof(float));
    rfft_plan plan = make_rfft_plan (length);
    rfft_forward (plan, data, 1.);
    rfft_backward (plan, data, 1./length);
    destroy_rfft_plan (plan);
    float err = errcalc (data, odata, length);
    if (err>epsilon)
      {
      printf("problem at real length %i: %e\n",length,err);
      ret = 1;
      }
    errsum+=err;
    }
  printf("errsum: %e\n",errsum);
  return ret;
  }

static int test_complex(void)
  {
  float data[2*maxlen], odata[2*maxlen];
  fill_random (odata, 2*maxlen);
  const float epsilon=2e-6;
  int ret = 0;
  float errsum=0;
  for (int length=1; length<=maxlen; ++length)
    {
    memcpy (data,odata,2*length*sizeof(float));
    cfft_plan plan = make_cfft_plan (length);
    cfft_forward(plan, data, 1.);
    cfft_backward(plan, data, 1./length);
    destroy_cfft_plan (plan);
    float err = errcalc (data, odata, 2*length);
    if (err>epsilon)
      {
      printf("problem at complex length %i: %e\n",length,err);
      ret = 1;
      }
    errsum+=err;
    }
  printf("errsum: %e\n",errsum);
  return ret;
  }

int main(void)
  {
  int ret = 0;
  ret = test_real();
  ret += test_complex();
  return ret;
  }
