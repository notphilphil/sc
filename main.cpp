#include "main.hpp"
using namespace std;
 
int main(int argc, char** argv) {
    /*
    The purpose of the main method is to obtain both the public IP address (ipv6) and an open port (check header) in order to call
    spinup_server method. In the quest to find our public IP address I found that C++ has no innate networking layer, and as such
    I am required to resort to some other solution to find the IP address of the current machine. There are a couple different
    ways that this can go.
    1.  lookup ip address over https with curl/wget using a site like icanhazip.com or whatismyip.com. the problem with this is that
        even if I use a server such as checkip.amazonaws.com that is hosted by amazon, these https servers tend to not last for very long
        (see https://unix.stackexchange.com/questions/22615/how-can-i-get-my-external-ip-address-in-a-shell-script). also, we must either
        have curl installed or use some sort of system() call in order to call wget (maybe this isn't so bad?? internet doesn't like
        system() calls i think)
    2.  lookup ip address using dns server. this seems to be safer (and faster) than using https because we can use the command `dig` in
        order to simply make a query to dns servers. this answer is explored more in the link in option #1. this is probably the best option
    3.  have the user look up and input their own public ip address into the command line. the pro of this is that I don't have to write
        any code to handle finding the IP address, but the con is that it opens up the application to human error. 
    4.  ???
    */


    addrinfo* hints = (addrinfo*)calloc(1, sizeof(addrinfo));
    addrinfo* host_info;
    hints->ai_flags = AI_PASSIVE;
    hints->ai_family = AF_INET6;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_addrlen = 0;
    hints->ai_addr = NULL;
    hints->ai_canonname = NULL;
    hints->ai_next = NULL;

    // char host_name[BUF_SIZE];
    // memset(&host_name, 0, sizeof(host_name));
    string host_ip;

    // Check if initial calloc() for hints did not error out
    if (!hints) {
        free(hints);
        safe_exit(true, "calloc()'ing failed");
    }

    // Fill host_info with information about host machine to be used when connecting to friends
    getaddrinfo(NULL, KAKAO_PORT, hints, &host_info);
    if (errno < 0)
        safe_exit(true, "initial getaddrinfo ran into an error");

    // hints no longer needed, can free
    free(hints);
    
    cout << "Welcome to Simple Chat!" << endl;
    cout << "Here is information to connect with friends" << endl << endl;

    // Get public ipv6 address, store in host_ip
    find_ipv6_addr(&host_ip);

    cout << "Port:\t\t" << KAKAO_PORT << endl;
    cout << "IPv6 Address:\t" << host_ip << endl;

    return spinup_server(KAKAO_PORT, host_ip, host_info);
};

int spinup_server(const char* port, std::string ip, addrinfo* host_info) {
    
    
    freeaddrinfo(host_info);
    return 0;
};

void find_ipv6_addr(std::string* ip) {
    /*
        Check if there is an available shell to use. From system() man page:
        `If command is NULL, then a nonzero value if a shell is available, or 0 if no shell is available.`
        Then, if so, call system() and evaluate return value using macros from sys/wait.h
        If IP address has changed, let user know.
    */
    if (system(NULL)) {
        const char* system_call = "dig @resolver1.opendns.com ANY myip.opendns.com +short > temp.conf";
        int ret = system(system_call);
        if (errno < 0)
            safe_exit(true, "I'm not sure what happened :(");

        if (!WIFEXITED(ret))
            safe_exit(true, "system() call returned something weird");

        // cout << "We good! System call exited normally I think..." << endl;
        /* Only need space for two lines as we will have possibly one line from an existing .conf file and one line from our new DNS lookup */
        string lines[2];

        ifstream temp_conf;
        temp_conf.open("temp.conf");
        ifstream conf_file(".conf");
        if (!conf_file.is_open()) {
            /* Configuration file does not exist, make one by specifying truncate in mode (check
            http://www.cplusplus.com/reference/fstream/ifstream/open/ and https://stackoverflow.com/a/8836041/8161432 for more details) */
            conf_file.open(".conf", fstream::trunc | fstream::out);
        }
        
        if (conf_file.is_open() && temp_conf.is_open()) {
            getline(conf_file, lines[0]);
            getline(temp_conf, lines[1]);
            if (lines[0].compare(lines[1]) != 0) {
                // DNS lookup returned a different address than we have on file
                // TODO confirm that the stuff we are reading from our .conf file are valid ipv6 addresses
                cout << "New IP address! Make sure to tell your friends :)" << endl;
                cout << "Your new IP address: \t" << lines[1] << endl;
            }
            // Always set ip to return to whatever is returned by dns lookup.
            // TODO add timestamps to when we get our dns query answer so that we do not do a DNS lookup everytime the program is run.
            *ip = lines[1];

            conf_file.close();
            temp_conf.close();
            remove(".conf");
            if (errno < 0)
                safe_exit(true, "removing .conf file fail");

            rename("temp.conf", ".conf");
            if (errno < 0)
                safe_exit(true, "renaming temp.conf to .conf fail");
        } else {
            cout << "Couldn't open " << (!conf_file.is_open() ? "conf_file " : " ") << (!temp_conf.is_open() ? "temp_conf" : "") << endl;
            cout << "Error: " << strerror(errno) << endl;
        }
    } else {
        cout << "No shell was available, try another option to update our ip address?" << endl;
        safe_exit(false, "");
    }
};

void safe_exit(bool failure, const char* msg) {
    if (failure)
        fprintf(stderr, "Dev error:\t\t%s\nError string:\t\t%s", msg, strerror(errno));

    cout << "Exiting due to " << (failure ? "failure :(" : "success :)") << endl;
    exit(failure ? EXIT_FAILURE : EXIT_SUCCESS);
};
