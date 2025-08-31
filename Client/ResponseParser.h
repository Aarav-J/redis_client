#ifndef RESPONSEPARSER_H

#define RESPONSEPARSER_H 

#include <string> 

class ResponseParser { 
    public: 
        //read from the given socket and return parsed response s a string, return "" in failure. 
        static std::string parseResponse(int sockfd); 
    private: 
// Redis serialization protocool 2
        static std::string parseSimpleString(int sockfd); 
        static std::string parseSimpleErrors(int sockfd); 
        static std::string parseIntegers(int sockfd); 
        static std::string parseBulkString(int sockfd); 
        static std::string parseArrays(int sockfd); 
            



}; 



#endif 