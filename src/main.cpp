#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>
#include "logging.hpp"
#include <cstring>
#include <string>
#include <iostream>
#include "server.hpp"

void start_daemon();

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cerr << "Too few arguments\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    openlog(argv[0], LOG_ODELAY | LOG_PID, LOG_DAEMON);

    char pid_str[10], path[] = "/var/run/simpver.pid";
    pid_t pid;
    int fd;

    if(!strcmp(argv[1], "start"))
    {
        start_daemon();
        fd = open(path, O_WRONLY | O_CREAT | O_EXCL);
        if(fd < 0)
        {
            perror("Simpver is already running: ");
            exit(EXIT_FAILURE);
        }
        pid = getpid();
        sprintf(pid_str, "%d", pid);
        write(fd, pid_str, strlen(pid_str));
        close(fd);
        syslog(LOG_INFO, "Simpver is running");
        closelog();

        try
        {
            Server server(8888, argv[2]);
            server.run();
        }
        catch(std::runtime_error const& error)
        {
            Logging log;
            std::string tmp("Error:");
            tmp += error.what();
            log.write_log(tmp.c_str());
            exit(EXIT_FAILURE);
        }
    }
    if(!strcmp(argv[1], "stop"))
    {
        fd = open(path, O_RDONLY);
        if(fd < 0)
        {
            perror("Simpver isn't running: ");
            exit(-3);
        }
        size_t size = read(fd, pid_str, 10);
        pid_str[size] = '\0';
        pid = atoi(pid_str);
        kill(pid, SIGTERM);
        close(fd);
        remove(path);
        syslog(LOG_INFO, "Simpver stoped");
        closelog();
    }
    exit(EXIT_SUCCESS);
}

void start_daemon()
{
    pid_t pid = fork();
    if(pid < 0)
        exit(EXIT_FAILURE);
    if(pid > 0)
        exit(EXIT_SUCCESS);
    if(setsid() < 0)
        exit(EXIT_FAILURE);
    
    pid = fork();
    if(pid < 0)
        exit(EXIT_FAILURE);
    if(pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    if(chdir("/") < 0)
        exit(EXIT_FAILURE);
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
