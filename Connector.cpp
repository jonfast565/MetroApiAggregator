#include "Connector.h"

Connector::Connector(const std::shared_ptr<HTTPRequestBuilder>& http_request,
        std::shared_ptr<Logger>& logger) {
    this->http_request = http_request;
    this->logger = logger;
}

void Connector::run() {
    int socket_descriptor = 0, bytes_received = 0, address_value = 0;
    char receive_buffer[1024];

    struct addrinfo hints, *server_address_info, *server_iterator;
    struct sockaddr_in server_in, *serv_lookup;

    std::string result = "";

    if (this->http_request == NULL) {
        this->logger->write(
                std::string("HTTP request object in an invalid state."),
                Logger::LogType::LOG_ERROR);
        return;
    }

    memset(receive_buffer, 0, sizeof (receive_buffer));
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        this->logger->write(
                std::string("Could not create socket."),
                Logger::LogType::LOG_ERROR);
        return;
    }

    // get possible servers (hostname entered)
    memset(&server_in, 0, sizeof (server_in));
    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(this->http_request->get_port());
    memset(&hints, 0, sizeof (hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char ip_port_string[100];
    char ip_string[100];
    sprintf(ip_port_string, "%d", this->http_request->get_port());

    if ((address_value = getaddrinfo(this->http_request->get_host_name().c_str(),
            ip_port_string, &hints, &server_address_info)) != 0) {
        this->logger->write(
                std::string("Could not obtain IP address of hostname."),
                Logger::LogType::LOG_ERROR);
        return;
    }

    for (server_iterator = server_address_info; server_iterator != NULL;
            server_iterator = server_iterator->ai_next) {
        serv_lookup = (struct sockaddr_in *) server_iterator->ai_addr;
        strcpy(ip_string, inet_ntoa(serv_lookup->sin_addr));
    }

    server_in.sin_addr.s_addr = inet_addr(ip_string);
    this->logger->write(std::string("Resolved as: ") +
            std::string(ip_string) + ":" + std::string(ip_port_string),
            Logger::LogType::LOG_STATUS);

    if (connect(socket_descriptor,
            (struct sockaddr *) &server_in,
            sizeof (server_in)) < 0) {
        this->logger->write(std::string("Connection failed."),
                Logger::LogType::LOG_ERROR);
        return;
    }

    this->logger->write(std::string("Connection looks ok."),
            Logger::LogType::LOG_STATUS);

    if (send(socket_descriptor, this->http_request->get_request().c_str(),
            strlen(this->http_request->get_request().c_str()), 0) < 0) {
        this->logger->write(std::string("Could not send the packet."),
                Logger::LogType::LOG_ERROR);
        return;
    }

    this->logger->write(std::string("Data sent over wire."),
            Logger::LogType::LOG_STATUS);

    while ((bytes_received = recv(socket_descriptor,
            receive_buffer, sizeof (receive_buffer) - 1, 0)) > 0) {
        result += std::string(receive_buffer);
        memset(receive_buffer, 0, sizeof (receive_buffer));
    }

    if (bytes_received < 0) {
        this->logger->write(std::string("Read error in stream."),
                Logger::LogType::LOG_ERROR);
        return;
    } else {
        this->logger->write(std::string("Data received."),
                Logger::LogType::LOG_STATUS);
    }

    // free
    freeaddrinfo(server_address_info);
    close(socket_descriptor);
    this->response = result;
    return;
}

std::string Connector::get_response() {
    return this->response;
}

std::string Connector::run_and_get_response() {
    this->run();
    return this->response;
}

Connector::~Connector() {
    this->response = "";
}

