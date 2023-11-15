/*

#include "http.h"

void HTTPRequestPayload::print(std::ostream& o) const {
    switch (method) {
        case GET: {
            o << "GET";
            break;
        }
        case POST: {
            o << "POST";
            break;
        }
        case DELETE: {
            o << "DELETE";
            break;
        }
        case PUT: {
            o << "PUT";
            break;
        }
        case PATCH: {
            o << "PATCH";
            break;
        }
    }
    o << " " << url << " HTTP/1.1\n";
    o << "Host: " << host << "\n";
    o << "User-Agent: " << userAgent << "\n";
    o << "Content-Type: " << contentType << "\n";
    o << "Content-Length: " << body.length() << "\n";
    o << body << "\n";
}

HTTPRequestPayload::HTTPRequestPayload(Method method, std::string path, 
std::string destination, std::string agent, 
std::string contentType, std::string body):
method(method), url(std::move(path)), host(std::move(destination)), 
userAgent(std::move(agent)), contentType(std::move(contentType)), body(std::move(body)) {

}

HTTPRequestPayload::HTTPRequestPayload(const HTTPRequestPayload& other):
method(other.method), url(other.url), host(other.host), userAgent(other.userAgent),
contentType(other.contentType), body(other.body) {

}

HTTPRequestPayload::Method HTTPRequestPayload::getMethod() const {
    return method;
}

const std::string& HTTPRequestPayload::getUrl() const {
    return url;
}

const std::string& HTTPRequestPayload::getUserAgent() const {
    return userAgent;
}

const std::string& HTTPRequestPayload::getContentType() const {
    return contentType;
}

const std::string& HTTPRequestPayload::getBody() const {
    return body;
}
*/