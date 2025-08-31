/* 
1. command line argument parsing 
    -h <host> default: 127.0.0.1 -p <port> default: :6379
    if no args, launch interactive REPL mode

2. Object-Oriented Programming
    RedisClient, CommandHandler, ResponseParser, CLI
3. Establish TCP connection to Redis (RedisClient)
    Berkeley sockets to open TCP connection
    IPv4 and IPv6 'getaddrinfo' 
4. Parsing and Command formatting (Command Handler)
    Split user input
    Convert comands into RESP format 
    *3\r\n
    $3\r\nSET\r\n
    $31\r\nkey\r\n
    $5\r\nvalue\r\n
5. Handling Redis Response(Response Parser)
    Read server respones and parses RESP types
        +Ok, -Error, :100, 

*/


#include <iostream> 
#include "CLI.h"
#include <string> 
int main(int argc, char* argv[]) { 
    std::string host = "127.0.0.1"; 
    int port = 6379; 
    int i = 1; 

    // parse command-line args for -h and -p 
    while (i < argc) { 
        std::string arg = argv[i]; 
        if (arg=="-h" && i + 1 < argc) { 
            host = argv[i+1];
        }else if (arg == "-p" && i + 1 < argc) { 
            port = std::stoi(argv[++i]); 
        } else { 
            break;
        }
        i++; 
    }

    // Handle REPL and one-shot command modes
    CLI cli(host, port); 
    cli.run(); 

    return 0; 
}