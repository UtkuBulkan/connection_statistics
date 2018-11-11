/*
 * Copyright 2018 Utku Bulkan. All rights reserved.
 *
 * MIT License
 *
 * Copyright (c) 2018 SU Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "median.h"

int median_init(median_t **median, int size)
{
	*median = malloc(sizeof(median_t));
	(*median)->data = (MEDIAN_DATA_TYPE*) calloc(sizeof(MEDIAN_DATA_TYPE) * size, sizeof(MEDIAN_DATA_TYPE));
	(*median)->size = size;
	(*median)->index = 0;
	return 0;
}

int median_uninit(median_t *median)
{
	free(median->data);
	free(median);
	return 0;
}

int median_add_number(median_t *median, MEDIAN_DATA_TYPE number)
{
	if (median->index >= median->size) {
		return -1;
	}
	median->data[median->index] = number;
	median->index = median->index + 1;

	printf("median->index : %d, median->size : %d , number : %lf\n", median->index, median->size, number);
	for(int i = 0;i<median->index;i++) printf(" %lf ",median->data[i]);
	printf("\n");
	return 0;
}

static int median_compare (const void * a, const void * b)
{
	MEDIAN_DATA_TYPE arg1 = *(MEDIAN_DATA_TYPE*)a;
	MEDIAN_DATA_TYPE arg2 = *(MEDIAN_DATA_TYPE*)b;

	if (arg1 < arg2) return -1;
	if (arg1 > arg2) return 1;
	return 0;
}

double median_find_median(median_t *median)
{
	qsort(median->data, median->index, sizeof(MEDIAN_DATA_TYPE), median_compare);
	if(median->index == 0) {
		return 0;
	} else if(median->index == 1) {
		return median->data[0];
	} else {
		return (median->index % 2) == 0 ?
				(median->data[(median->index-1)/2]+median->data[((median->index-1)/2)+1])/2.0 :
				median->data[median->index/2];
	}
	return 0;
}


