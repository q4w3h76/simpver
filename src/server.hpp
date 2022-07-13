#ifndef SERVER_HPP
#define SERVER_HPP

#include <thread>

class Listener;
class IOWorker;

class Server
{
    Listener* m_listener;
    const unsigned int m_nproc;
    IOWorker** m_io_workers;
public:
    Server(const unsigned short, char*);
    ~Server();

    void run();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
};

#endif // SERVER_HPP
