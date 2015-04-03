#ifndef CONNECTOR_H
#define	CONNECTOR_H

#include <memory>
#include <string>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "HTTPRequestBuilder.h"
#include "Logger.h"

class Connector {
public:
    Connector(const std::shared_ptr<HTTPRequestBuilder>& http_request, 
            std::shared_ptr<Logger>& logger);
    void run();
    std::string get_response();
    std::string run_and_get_response();
    virtual ~Connector();
private:
    std::string response;
    int port;
    std::shared_ptr<HTTPRequestBuilder> http_request;
    std::shared_ptr<Logger> logger;
};

#endif	/* CONNECTOR_H */

