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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "connect_manager_curl.h"
#include "connect_manager.h"
#include "helper.h"

static int set_connection_library_as_curl(connection_manager_t *connection_manager)
{
	connection_manager->init = init_with_curl;
	connection_manager->uninit = uninit_with_curl;
	connection_manager->enable_http_logs = enable_http_logs_with_curl;
	connection_manager->set_url = set_url_with_curl;
	connection_manager->get_ip_address = get_ip_address_with_curl;
	connection_manager->get_http_response_code = get_http_response_code_with_curl;
	connection_manager->change_http_header = change_http_header_with_curl;
	connection_manager->change_number_of_max_requests = change_number_of_max_requests_with_curl;
	connection_manager->perform_connection_request = perform_connection_request_with_curl;
	connection_manager->collect_statistics = collect_statistics_with_curl;

	return CONN_OK;
}

int set_connection_library_api(connection_manager_t *connection_manager)
{
	set_connection_library_as_curl(connection_manager);

	return CONN_OK;
}


