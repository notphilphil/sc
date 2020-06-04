#include "main.hpp"
using namespace std;

int main(int argc, char** argv) {
    struct addrinfo* hints = (addrinfo*)calloc(1, sizeof(addrinfo));
    struct addrinfo* host_info;
    hints->ai_flags = AI_PASSIVE;
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_addrlen = 0;
    hints->ai_addr = NULL;
    hints->ai_canonname = NULL;
    hints->ai_next = NULL;

    char host_name[BUF_SIZE];
    char host_ip[BUF_SIZE];

    // Check if initial calloc() for hints did not error out
    if (!hints) {
        free(hints);
        safe_exit(true, "calloc()'ing of hints failed");
    }
    
    // Get host name to display to user
    gethostname(host_name, sizeof(host_name));
    if (errno < 0)
        safe_exit(true, "initial gethostname ran into an error");

    // Fill host_info with information about host machine to be used when connecting to friends
    getaddrinfo(NULL, KAKAO_PORT, hints, &host_info);
    if (errno < 0)
        safe_exit(true, "initial getaddrinfo ran into an error");

    // Convert returned struct sockaddr to IP address to display to user
    inet_ntop(AF_INET, host_info->ai_addr, host_ip, sizeof(host_ip));
    if (errno < 0)
        safe_exit(true, "inet_ntop ran into an error");

    cout << "Welcome to Simple Chat!" << endl;
    cout << "Here is information to connect with friends" << endl;
    cout << "Host name:\t\t" << host_name << endl;
    cout << "Host port:\t\t" << KAKAO_PORT << endl;
    cout << "Host IPv4addr:\t\t" << host_ip << endl;

    freeaddrinfo(host_info);
    free(hints);

    

    return 0;
};

void safe_exit(bool failure, char* msg) {
    if (failure)
        fprintf(stderr, "Dev error:\t\t%s\nError string:\t\t%s", msg, strerror(errno));

    cout << "Exiting due to " << (failure ? "failure :(" : "success :)") << endl;
    exit(failure ? EXIT_FAILURE : EXIT_SUCCESS);
};
