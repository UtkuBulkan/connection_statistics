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

#include <string.h>
#include "connect_manager_curl.h"
#include "curl.h"

static CURL *curl = NULL;

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	(void)buffer;
	(void)userp;
	return size * nmemb;
}

int init_with_curl(void)
{
	curl_version_info_data *data;
    curl = curl_easy_init();
    data = curl_version_info(CURLVERSION_NOW);
    printf("Curl Version : %s", data->version);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    return 0;
}

int uninit_with_curl(void)
{
    /* always cleanup */
	curl_easy_cleanup(curl);

	return CURLE_OK;
}

int set_url_with_curl(const char* url)
{
	CURLcode res;

	res = curl_easy_setopt(curl, CURLOPT_URL, url);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
	}

	return CURLE_OK;
}

int get_ip_address_with_curl(char **ip)
{
	CURLcode res;
	char *ip_local;

	res = curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip_local);
	(*ip) = strdup(ip_local);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
	}

	return CURLE_OK;
}

int get_http_response_code_with_curl(long *http_response_code)
{
	CURLcode res;

	res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_response_code);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
	}

    return CURLE_OK;
}

int change_http_header_with_curl(const char *header)
{
	CURLcode res;

	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_setopt() failed: %s\n", curl_easy_strerror(res));
	}

    return CURLE_OK;
}

int change_number_of_max_requests_with_curl(int n)
{
	CURLcode res = CURLE_OK;

	res = curl_easy_setopt(curl, CURLOPT_MAXCONNECTS, n);
    if(res != CURLE_OK) {
    	fprintf(stderr, "curl_easy_setopt() failed: %s\n", curl_easy_strerror(res));
	}

    return CURLE_OK;
}

int perform_connection_request_with_curl()
{
	CURLcode res;

	res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
    	fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    return 0;
}

int collect_statistics_with_curl(connection_statistics_t *connection_statistics)
{
	CURLcode res;

	res = curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &connection_statistics->namelookup_time);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
	}
	res = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connection_statistics->connect_time);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
	}
	res = curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &connection_statistics->starttransfer_time);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
	}
	res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &connection_statistics->total_time);
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
	}

	return CURLE_OK;
}

