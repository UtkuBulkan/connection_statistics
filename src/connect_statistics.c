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

#include "connect_statistics.h"
#include "helper.h"

int accumulate_statistics(connection_statistics_median_t *connection_statistics_median,
				    connection_statistics_t *connection_statistics)
{
	median_add_number(connection_statistics_median->namelookup_time, connection_statistics->namelookup_time);
	median_add_number(connection_statistics_median->connect_time, connection_statistics->connect_time);
	median_add_number(connection_statistics_median->starttransfer_time, connection_statistics->starttransfer_time);
	median_add_number(connection_statistics_median->total_time, connection_statistics->total_time);

	return CONN_OK;
}

int connection_statistics_median_init(connection_statistics_median_t *connection_statistics_median, int max_requests)
{
	median_init(&connection_statistics_median->namelookup_time, max_requests);
	median_init(&connection_statistics_median->connect_time, max_requests);
	median_init(&connection_statistics_median->starttransfer_time, max_requests);
	median_init(&connection_statistics_median->total_time, max_requests);

	return CONN_OK;
}

int connection_statistics_median_uninit(connection_statistics_median_t *connection_statistics_median)
{
	median_uninit(connection_statistics_median->namelookup_time);
	median_uninit(connection_statistics_median->connect_time);
	median_uninit(connection_statistics_median->starttransfer_time);
	median_uninit(connection_statistics_median->total_time);

	return CONN_OK;
}
