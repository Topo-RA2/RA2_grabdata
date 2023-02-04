#include "http_server.h"

httplib::MultipartFormData &get_file_value(httplib::MultipartFormDataItems &files, const char *key)
{
    auto it = std::find_if(files.begin(), files.end(),
        [&](const httplib::MultipartFormData &file)
        {
            return file.name == key;
        }
    );
#ifdef CPPHTTPLIB_NO_EXCEPTIONS
    return *it;
#else
    if (it != files.end()) { return *it; }
    throw std::runtime_error("invalid mulitpart form data name error");
#endif
}

int start_http_server()
{
    httplib::Server svr;
    if(!svr.is_valid())
    {
        BaseLog::Output("[Main] Http Init Error");
        return -1;
    }
    svr.Get(
        "/info", 
        [](const httplib::Request &, httplib::Response &res)
        {
            res.set_content("Hello World!", "text/plain");
        }
    );
    svr.set_error_handler(
        [](const auto& req, auto& res)
        {
            auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
            char buf[BUFSIZ];
            snprintf(buf, sizeof(buf), fmt, res.status);
            BaseLog::Output(
                "[Main] Http Error! status: ", res.status,
                "\nversion: " , res.version,
                "\nreason: ", res.reason,
                "\nbody: ", res.body,
                "\nlocation: ", res.location);
            res.set_content(buf, "text/html");
        }
    );
    svr.set_exception_handler(
        [](const auto& req, auto& res, std::exception_ptr ep)
        {
            auto fmt = "<h1>Error 500</h1><p>%s</p>";
            char buf[BUFSIZ];
            try
            {
                std::rethrow_exception(ep);
            } 
            catch (const std::exception &e)
            {
                snprintf(buf, sizeof(buf), fmt, e.what());
                BaseLog::Output("[Main] Http Exception: ", e.what());
            } 
            catch (...)
            {
                snprintf(buf, sizeof(buf), fmt, "Unknown Exception");
                BaseLog::Output("[Main] Http Unknown Exception");
            }
            res.set_content(buf, "text/html");
            res.status = 500;
        }
    );

    svr.listen("0.0.0.0", 8080);

    return 0;
}