#include "ResponseParser.h"


#include <iostream> 
#include <sstream> 
#include <unistd.h> 
#include <cstdlib> 
#include <sys/types.h>
#include <sys/socket.h>

static bool readChar(int sockfd, char &c) { 
    ssize_t r = recv(sockfd, &c, 1, 0); 
    return (r==1); 
}


static std::string readLine(int sockfd) { 
    std::string line; 
    char c; 
    while(readChar(sockfd, c)) { 
        if(c == '\r') { 
            // Expect '\n next; read and breal 

            readChar(sockfd, c); 
            break;  
        }
        line.push_back(c); 
    }
    return line; 
}
std::string ResponseParser::parseResponse(int sockfd) { 
    char prefix; 
    if(!readChar(sockfd, prefix)) { 
        return ("(Error) No response or connection closed. "); 
    }

    switch(prefix) { 
        case '+' : return parseSimpleString(sockfd); 
        case '-' : return parseSimpleErrors(sockfd); 
        case ':' : return parseIntegers(sockfd); 
        case '$' : return parseBulkString(sockfd); 
        case '*' : return parseArrays(sockfd); 
        default: 
            return "(Error) Unknown reply type."; 
        

    }
}

std::string ResponseParser::parseSimpleString(int sockfd) { 
    return readLine(sockfd);  
}
std::string ResponseParser::parseSimpleErrors(int sockfd) { 
    return readLine(sockfd);  
}
std::string ResponseParser::parseIntegers(int sockfd) { 
    return readLine(sockfd);  
}
std::string ResponseParser::parseBulkString(int sockfd) { 
    return readLine(sockfd);  
}
std::string ResponseParser::parseArrays(int sockfd) { 
    return readLine(sockfd);  
}