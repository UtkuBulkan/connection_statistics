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
#include "helper.h"

static int print_statistics(connection_manager_t *connection_manager, connection_statistics_median_t *connection_statistics_median)
{
#if 0
	char *string_list[]={"IP address of HTTP server: ",
						"HTTP response code: ",
						"median of CURLINFO_NAMELOOKUP_TIME: ",
						"median of CURLINFO_CONNECT_TIME: ",
						"median of CURLINFO_STARTTRANSFER_TIME: ",
						"median of CURLINFO_TOTAL_TIME: "};
#endif
	char *string_list[]={"", "", "", "", "", ""};
    char *ip;
    long code;

    connection_manager->get_ip_address(&ip);
    connection_manager->get_http_response_code(&code);

    printf("SKTEST;"
    	"<%s%s>;"
    	"<%s%3ld>;"
		"<%s%lf>;"
		"<%s%lf>;"
		"<%s%lf>;"
		"<%s%lf>\n",
		string_list[0], ip,
		string_list[1], code,
		string_list[2], median_find_median(connection_statistics_median->namelookup_time),
		string_list[3], median_find_median(connection_statistics_median->connect_time),
		string_list[4], median_find_median(connection_statistics_median->starttransfer_time),
		string_list[5], median_find_median(connection_statistics_median->total_time));
    free(ip);

	return CONN_OK;
}

int main(int argc, char *argv[]) {
	connection_statistics_median_t connection_statistics_median;
	connection_statistics_t connection_statistics;
    connection_manager_t connection_manager;
    int max_requests = 1, enable_http_logs_flag = 0;
    int option = 0;

    /* Assigning functions for connection manager with curl library functions (default is curl) */
	set_connection_library_api(&connection_manager);

	/* Initializing connection manager */
	connection_manager.init();
    connection_manager.set_url("http://www.google.com/");

    /*Getting options from the command line */
    while ((option = getopt(argc, argv,"lH:n:")) != -1) {
        switch (option) {
             case 'H' :
            	 connection_manager.change_http_header(optarg);
                 break;
             case 'n' :
            	 max_requests = atoi(optarg);
                 break;
             case 'l' :
            	 enable_http_logs_flag = 1;
            	 break;
             default:
                 return -1;
        }
    }
    /* Enabling/Disabling detailed http communication logs into example_http_comms.txt file */
    connection_manager.enable_http_logs(enable_http_logs_flag);

    /* Triggering n number of requests and collection statistics while calculating medians */
    memset(&(connection_statistics), 0, sizeof(connection_statistics_t));
    connection_statistics_median_init(&connection_statistics_median, max_requests);
    for(int i=0;i<max_requests;i++) {
		connection_manager.perform_connection_request();
		connection_manager.collect_statistics(&connection_statistics);
		accumulate_statistics(&connection_statistics_median, &connection_statistics);
		print_statistics(&connection_manager, &connection_statistics_median);
    }
    /* Uninitializing connection manager */
    connection_statistics_median_uninit(&connection_statistics_median);
    connection_manager.uninit();

	return CONN_OK;
}
