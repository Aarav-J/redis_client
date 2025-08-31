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
    return "(Error) " + readLine(sockfd); 
}
std::string ResponseParser::parseIntegers(int sockfd) { 
    return readLine(sockfd);  
}
std::string ResponseParser::parseBulkString(int sockfd) { 
    std::string lenStr = readLine(sockfd); 
    int length = std::stoi(lenStr); 
    if(length == -1) { 
        return "(nil)"; 
    }  
    std::string bulk; 
    bulk.resize(length); 
    int totalRead = 0; 

    while(totalRead < length) { 
        ssize_t r  = recv(sockfd, &bulk[totalRead], length- totalRead, 0); 
        if(r <= 0) { 
            return "(error) incomplete bulk data."; 
        }
        totalRead += r; 
    }

    // Consume trailing CRLF


    char dummy; 

    readChar(sockfd, dummy); 
    readChar(sockfd, dummy); 

    return bulk;
}
std::string ResponseParser::parseArrays(int sockfd) { 
    std::string countStr = readLine(sockfd);
    int count = std::stoi(countStr);  
    if(count == -1) { 
        return "(nil)"; 
    }
    std::ostringstream oss; 
    for (int i = 0; i < count; ++i) { 
        oss << parseResponse(sockfd); 
        if (i != count -1) { 
            oss << "\n"; 
        }
    }

    return oss.str(); 

}