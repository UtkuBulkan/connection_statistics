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

typedef int (*init_func)(void);
typedef int (*uninit_func)(void);
typedef int (*enable_http_logs_func)(int enable);
typedef int (*set_url_func)(const char *url);
typedef int (*get_ip_address_func)(char **ip);
typedef int (*get_http_response_code_func)(long *http_response_code);
typedef int (*change_http_header_func)(const char* header);
typedef int (*change_number_of_max_requests_func)(int n);
typedef int (*perform_connection_request_func)(void);
typedef int (*collect_statistics_func)(connection_statistics_t *connection_statistics);

typedef struct connection_manager_s
{
	init_func init;
	uninit_func uninit;
	enable_http_logs_func enable_http_logs;
	set_url_func set_url;
	get_ip_address_func get_ip_address;
	get_http_response_code_func get_http_response_code;
	change_http_header_func change_http_header;
	change_number_of_max_requests_func change_number_of_max_requests;
	perform_connection_request_func perform_connection_request;
	collect_statistics_func collect_statistics;
} connection_manager_t;

int set_connection_library_api(connection_manager_t *connection_manager);

