#include "tcp_server.h"
#include "co/co.h"
#include "co/so.h"
#include "data_processing.h"

tcp::Server s;

void on_connection(tcp::Connection conn)
{
    LOG << "[tcp_thread] on_connection";
    char buf[2048] = { 0 };

    while (true) 
    {
        int r = conn.recv(buf, 2048);
        if (r == 0)
        {         
            conn.close();
            break;
        }
        else if (r < 0)
        {
            conn.reset(3000);
            break;
        }
        else
        {
            Json x = json::parse(fastring(buf, r));
            LOG << "server recv:" << x.pretty();
            insert_new_data(x);
        }
    }
}

void tcp_thread()
{
    go(
        [&]() {
            s.on_connection(on_connection);
            s.start("0.0.0.0", 7788);
        }
    );
}

void start_tcp_thread()
{
    LOG << "[start_tcp_thread] Server Start!";
    thread t(tcp_thread);
    t.detach();
}
