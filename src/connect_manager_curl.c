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
#include "lib/logger/macrologger.h"
#include "helper.h"

static CURL *curl = NULL;
static struct curl_slist *header_list = NULL;

int init_with_curl(void)
{
	curl_version_info_data *data;
	(void)data;
    curl = curl_easy_init();
    data = curl_version_info(CURLVERSION_NOW);
    LOG_DEBUG("Curl Version : %s", data->version);

    return CONN_OK;
}

int uninit_with_curl(void)
{
    /* always cleanup */
	curl_slist_free_all(header_list); /* free the list again */
	curl_easy_cleanup(curl);

	return CONN_OK;
}

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	(void)buffer;
	(void)userp;
	return size * nmemb;
}

static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size)
{
	size_t i;
	size_t c;
	unsigned int width=0x10;

	fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n", text, (long)size, (long)size);

	for(i=0; i<size; i+= width) {
		fprintf(stream, "%4.4lx: ", (long)i);

		/* show hex to the left */
		for(c = 0; c < width; c++) {
			if(i+c < size)
				fprintf(stream, "%02x ", ptr[i+c]);
			else
				fputs("   ", stream);
		}

		/* show data on the right */
		for(c = 0; (c < width) && (i+c < size); c++) {
			char x = (ptr[i+c] >= 0x20 && ptr[i+c] < 0x80) ? ptr[i+c] : '.';
			fputc(x, stream);
		}

		fputc('\n', stream); /* newline */
	}
}

static int my_trace(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
{
	const char *text;
	FILE *file_out;
	(void)handle; /* prevent compiler warning */
	(void)userp;

	switch (type) {
		case CURLINFO_TEXT:
			fprintf(stderr, "== Info: %s", data);
		default: /* in case a new one is introduced to shock us */
			return 0;
		case CURLINFO_HEADER_OUT:
			text = "=> Send header";
			break;
		case CURLINFO_DATA_OUT:
			text = "=> Send data";
			break;
		case CURLINFO_SSL_DATA_OUT:
			text = "=> Send SSL data";
			break;
		case CURLINFO_HEADER_IN:
			text = "<= Recv header";
			break;
		case CURLINFO_DATA_IN:
			text = "<= Recv data";
			break;
		case CURLINFO_SSL_DATA_IN:
			text = "<= Recv SSL data";
			break;
	}
	file_out = fopen("./example_http_comm.txt", "a+");
	dump(text, file_out, (unsigned char *)data, size);
	fclose(file_out);
	return 0;
}

int enable_http_logs_with_curl(int enable)
{
	CURLcode res;

	if(enable == 0) {
		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		if (res != CURLE_OK) {
			LOG_DEBUG("curl_easy_setopt with option : CURLOPT_WRITEFUNCTION has failed.");
			return CONN_FAIL;
		}
	} else {
		/* ask libcurl to show us the verbose output */
		res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		if (res != CURLE_OK) {
			LOG_DEBUG("curl_easy_setopt with option : CURLOPT_VERBOSE has failed.");
			return CONN_FAIL;
		}
		res = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
		if (res != CURLE_OK) {
			LOG_DEBUG("curl_easy_setopt with option : CURLOPT_DEBUGFUNCTION has failed.");
			return CONN_FAIL;
		}
	}

	return CONN_OK;
}

int set_url_with_curl(const char* url)
{
	CURLcode res;

	res = curl_easy_setopt(curl, CURLOPT_URL, url);
	if(res != CURLE_OK) {
		LOG_DEBUG("curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		return CONN_FAIL;
	}

	return CONN_OK;
}

int get_ip_address_with_curl(char **ip)
{
	CURLcode res;
	char *ip_local;

	res = curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip_local);
	(*ip) = strdup(ip_local);
	if(res != CURLE_OK) {
		LOG_DEBUG("curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		return CONN_FAIL;
	}

	return CONN_OK;
}

int get_http_response_code_with_curl(long *http_response_code)
{
	CURLcode res;

	res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, http_response_code);
	if(res != CURLE_OK) {
		LOG_DEBUG("curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		return CONN_FAIL;
	}

    return CONN_OK;
}

int change_http_header_with_curl(const char *header)
{
	int ret = CONN_OK;
	CURLcode res;

	header_list = curl_slist_append(header_list, header);
	if (header_list == NULL) {
		return CONN_FAIL;
	}
	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	if(res != CURLE_OK) {
		LOG_DEBUG("curl_easy_setopt() failed: %s\n", curl_easy_strerror(res));
		ret = CONN_FAIL;
	}

	return ret;
}

int change_number_of_max_requests_with_curl(int n)
{
	CURLcode res = CURLE_OK;

	res = curl_easy_setopt(curl, CURLOPT_MAXCONNECTS, n);
    if(res != CURLE_OK) {
    	LOG_DEBUG("curl_easy_setopt() failed: %s\n", curl_easy_strerror(res));
    	return CONN_FAIL;
	}

    return CONN_OK;
}

int perform_connection_request_with_curl()
{
	CURLcode res;

	res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
    	LOG_DEBUG("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    	return CONN_FAIL;
    }

    return CONN_OK;
}

int collect_statistics_with_curl(connection_statistics_t *connection_statistics)
{
	CURLcode res;

	res = curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &connection_statistics->namelookup_time);
	if(res != CURLE_OK) {
		LOG_DEBUG("curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		return CONN_FAIL;
	}
	res = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connection_statistics->connect_time);
	if(res != CURLE_OK) {
		LOG_DEBUG("curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		return CONN_FAIL;
	}
	res = curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &connection_statistics->starttransfer_time);
	if(res != CURLE_OK) {
		LOG_DEBUG("curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		return CONN_FAIL;
	}
	res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &connection_statistics->total_time);
	if(res != CURLE_OK) {
		LOG_DEBUG("curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
		return CONN_FAIL;
	}

	return CONN_OK;
}
