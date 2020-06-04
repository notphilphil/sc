#include "main.hpp"
using namespace std;

int main(int argc, char** argv) {
    struct addrinfo* hints = (addrinfo*)calloc(1, sizeof(addrinfo));
    struct addrinfo** host_info = (addrinfo**)calloc(1, sizeof(addrinfo*));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;

    // Check if initial calloc() for hints and host_info did not error out
    if (!hints | !host_info) {
        cout << "calloc()'ing of hints/host_info failed" << endl;
        if (hints != NULL) free(hints);
        if (host_info != NULL) free(host_info);
        safe_exit(true);
    }
    
    // Fill host_info with information about host machine to be used when connecting to friends
    getaddrinfo(NULL, NULL, hints, host_info);
    if (errno < 0) {
        cout << "initial getaddrinfo ran into an error -> " << strerror(errno) << endl;
        safe_exit(true);
    }

    cout << "Welcome to Simple Chat!" << endl;
    return 0;
};

void safe_exit(bool failure) {
    cout << "Exiting due to " << (failure ? "failure :(" : "success :)") << endl;
    exit(failure ? EXIT_FAILURE : EXIT_SUCCESS);
}