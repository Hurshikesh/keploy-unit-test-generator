#include <drogon/HttpResponse.h>
#include <string>

static drogon::HttpResponsePtr makeErrResp(std::string msg) {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k500InternalServerError);
    resp->setBody(msg);
    return resp;
}
