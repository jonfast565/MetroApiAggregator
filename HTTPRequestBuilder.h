#ifndef HTTPREQUESTBUILDER_H
#define	HTTPREQUESTBUILDER_H

#include <vector>
#include <string>
#include <memory>

class HTTPRequestBuilder {
private:
    typedef struct ParameterTuple {
        std::string name;
        std::string value;
    } QueryParameter;
    std::unique_ptr<std::vector<std::unique_ptr<QueryParameter>>> parameter_list;
    std::string host_name;
    std::string path;
    bool secure = false;
    int port = -1;
public:
    HTTPRequestBuilder(std::string host_name, std::string path);
    void add_parameter(std::string name, std::string value);
    std::string get_request();
    std::string get_host_name();
    void set_host_name(std::string host_name);
    void set_path(std::string path);
    int get_port();
    bool is_secure();
    virtual ~HTTPRequestBuilder();
};

#endif	/* HTTPREQUESTBUILDER_H */

