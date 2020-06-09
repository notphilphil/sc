#include <iostream>
#include <netinet/in.h>
// For inet_ntoa and htonl
#include <arpa/inet.h>
#include <unistd.h>

// For getaddrinfo and freeaddrinfo
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/wait.h>

// Error handling
#include <errno.h>
#include <cstring>

#include <curl/curl.h>

// For reading and writing from/to our .conf file
#include <fstream>

#define KAKAO_PORT "20206"
#define BUF_SIZE 128

// Function declarations

/*
Function to be used in case of shutting down the application.
TODO add some sort of real functionality to this instead of just calling exit(EXIT_FAILURE) or exit(EXIT_SUCCESS)
*/
void safe_exit(bool failure, const char* msg);
void find_ipv6_addr(std::string* ip);
int spinup_server(const char* port, std::string ip, addrinfo* host_info);