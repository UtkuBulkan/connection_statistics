# connection_statistics
* This is a wrapper c library for measuring connectivity statistics for a specific URL. It uses curl by default, yet it is designed to be used with any transfer library which fulfils the prerequisites of required api.

Usage :
```
git clone --recurse-submodules https://github.com/UtkuBulkan/connection_statistics.git conn_stats
cd conn_stats
make
make test
```

And below is the expected output :
```
LD_LIBRARY_PATH=. ./conn_stats_test -n 5
SKTEST;<108.177.126.105>;<200>;<0.014513>;<0.036288>;<0.128957>;<0.210864>
SKTEST;<108.177.126.105>;<200>;<0.007266>;<0.018154>;<0.119666>;<0.177787>
SKTEST;<108.177.126.105>;<200>;<0.000020>;<0.000020>;<0.128957>;<0.162521>
SKTEST;<108.177.126.105>;<200>;<0.000022>;<0.000023>;<0.122836>;<0.157922>
SKTEST;<108.177.126.105>;<200>;<0.000020>;<0.000020>;<0.128957>;<0.153323>
```
Detailed Information about the Library and Test Application

The library has declared following api :
```
init_func,
uninit_func uninit,
enable_http_logs_func,
set_url_func,
get_ip_address_func,
get_http_response_code_func,
change_http_header_func,
change_number_of_max_requests_func,
perform_connection_request_func,
collect_statistics_func
```

* As long as these functions are assigned with the functionality of a transfer library of any sort, the wrapper library will work as expected and accumulate the statistics of connectivity.

* Here below are the pseudo process of the library functionality :

```
/* Assigning functions for connection manager with curl library functions (default is curl) */
set_connection_library_api(&connection_manager);
/* Initializing connection manager */
connection_manager.init();
connection_manager.enable_http_logs();
connection_manager.set_url("http://www.google.com/");
/* Triggering n number of requests and collection statistics while calculating medians */
connection_statistics_median_init();
for(int i=0;i<max_requests;i++) {
	connection_manager.perform_connection_request();
	connection_manager.collect_statistics();
	accumulate_statistics();
	print_statistics();
}
/* Uninitializing connection manager */
connection_statistics_median_uninit();
connection_manager.uninit();
```

* The test file "conn_stats_test.c" has following command line option, "-H" for additional http headers ( this option supports to add many times with different headers, "-n" for the number of http get requests through the library and "-l" for enabling http communication logs.

* In order to check if headers are there, the detailed http communication logs are under "example_http_comms.txt" file.

* After each http communication activity the statistics are accumulated and at any time the median is calculated through simple median implementation. Details of median can be found on : https://en.wikipedia.org/wiki/Median

* The logger library  "simple header c logger" is used as a submodule from git repository : https://github.com/dmcrodrigues/macro-logger.git
 


