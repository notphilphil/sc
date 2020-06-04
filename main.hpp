#include <iostream>
#include <netinet/in.h> 
#include <arpa/inet.h>

// For getaddrinfo and freeaddrinfo
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

// Error handling
#include <errno.h>
#include <cstring>

// Function declarations

/*
Function to be used in case of shutting down the application.
TODO add some sort of real functionality to this instead of just calling exit(EXIT_FAILURE) or exit(EXIT_SUCCESS)
*/
void safe_exit(bool failure);