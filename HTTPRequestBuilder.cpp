#include "HTTPRequestBuilder.h"

HTTPRequestBuilder::HTTPRequestBuilder(std::string host_name, std::string path) {
    this->parameter_list = std::unique_ptr<std::vector<
            std::unique_ptr < QueryParameter>>>(
            new std::vector<std::unique_ptr < QueryParameter >> ());
    this->set_host_name(host_name);
    this->set_path(path);
}

void HTTPRequestBuilder::set_host_name(std::string host_name) {
    this->host_name = host_name;
}

void HTTPRequestBuilder::set_path(std::string path) {
    this->path = path;
}

void HTTPRequestBuilder::add_parameter(std::string name, std::string value) {
    this->parameter_list->push_back(std::unique_ptr<QueryParameter>(new QueryParameter{name, value}));
}

std::string HTTPRequestBuilder::get_request() {
    std::string request = std::string("GET ") + this->path;
    std::vector<std::unique_ptr < QueryParameter>>::iterator it;
    if (this->parameter_list->size() > 0) {
        request += "?";
        for (it = this->parameter_list->begin();
                it != this->parameter_list->end(); it++) {
            request += (*it)->name + "=" + (*it)->value;
            if (it != this->parameter_list->end() - 1) {
                request += "&";
            }
        }
    }
    // HTTP 1.0 prevents chunked encoding
    request += std::string(" HTTP/1.0\r\nHost: ")
            + this->host_name + std::string("\r\nConnection: close\r\n\r\n");
    return request;
}

bool HTTPRequestBuilder::is_secure() {
    return this->secure;
}

int HTTPRequestBuilder::get_port() {
    if (this->port == -1) {
        if (this->secure == true) {
            return 443;
        } else {
            return 80;
        }
    } else {
        return this->port;
    }
}

std::string HTTPRequestBuilder::get_host_name() {
    return this->host_name;
}

HTTPRequestBuilder::~HTTPRequestBuilder() {
    this->host_name = "";
    this->path = "";
}

