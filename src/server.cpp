#include "server.hpp"
#include "listener.hpp"
#include "IOWorker.hpp"

Server::Server(const unsigned short port, char* path_html)
    : m_listener(new Listener(port, path_html)), m_nproc(std::thread::hardware_concurrency()),
    m_io_workers(new IOWorker*[m_nproc])
{
    for(unsigned int i = 0; i < m_nproc; ++i)
        m_io_workers[i] = new IOWorker;
}

Server::~Server()
{
    delete m_listener;
    for(unsigned int i = 0; i < m_nproc; ++i)
        delete m_io_workers[i];
    delete [] m_io_workers;
}

void Server::run()
{
    while(true)
    {
        for(unsigned int i = 0; i < m_nproc; ++i)
            m_io_workers[i]->add_fd(m_listener->accept_client());
    }
}
