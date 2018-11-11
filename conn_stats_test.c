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

#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "connect_manager_curl.h"
#include "connect_manager.h"

static int print_statistics(connection_manager_t *connection_manager, connection_statistics_median_t *connection_statistics_median)
{
    char *ip;
    long code;

    connection_manager->get_ip_address(&ip);
    connection_manager->get_http_response_code(&code);

    printf("SKTEST;"
    	"<IP address of HTTP server: %s>;"
    	"<HTTP response code: %3ld>;"
		"<median of CURLINFO_NAMELOOKUP_TIME: %lf>;"
		"<median of CURLINFO_CONNECT_TIME: %lf>;"
		"<median of CURLINFO_STARTTRANSFER_TIME: %lf>;"
		"<median of CURLINFO_TOTAL_TIME: %lf>\n",
		ip,
		code,
		median_find_median(connection_statistics_median->namelookup_time),
		median_find_median(connection_statistics_median->connect_time),
		median_find_median(connection_statistics_median->starttransfer_time),
		median_find_median(connection_statistics_median->total_time));
    free(ip);
    return 0;
}

int main(int argc, char *argv[]) {
	connection_statistics_median_t connection_statistics_median;
	connection_statistics_t connection_statistics;
    connection_manager_t connection_manager;
    int max_requests = 1;
    int option = 0;

    while ((option = getopt(argc, argv,"H:n:")) != -1) {
        switch (option) {
             case 'H' :
            	 connection_manager.change_http_header(optarg);
                 break;
             case 'n' :
            	 max_requests = atoi(optarg);
				 //connection_manager.change_number_of_max_requests(max_requests);
                 break;
             default:
                 return -1;
        }
    }

    memset(&(connection_statistics), 0, sizeof(connection_statistics_t));
	connection_statistics_median_init(&connection_statistics_median, max_requests);

	/* Assigning function pointers of connection manager with curl library functions (default is curl) */
	set_connection_library_api(&connection_manager);

	/* Initializing connection manager */
	connection_manager.init();
    connection_manager.set_url("http://www.google.com/");

    /* Triggering n number of requests and collection statistics while calculating medians */
    for(int i=0;i<max_requests;i++) {
		connection_manager.perform_connection_request();
		connection_manager.collect_statistics(&connection_statistics);
		accumulate_statistics(&connection_statistics_median, &connection_statistics);
		print_statistics(&connection_manager, &connection_statistics_median);
    }
    /* Uninitializing connection manager */
    connection_manager.uninit();
    connection_statistics_median_uninit(&connection_statistics_median);
    return 0;
}
