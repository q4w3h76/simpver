#include "logging.hpp"
#include <fcntl.h>
#include <cstring>
#include <ctime>
#include <unistd.h>

Logging::Logging(const char* path)
{
    fd = open(path, O_WRONLY | O_CREAT);
    if(fd < 0)
        return;
}

Logging::~Logging()
{
    close(fd);
}

void Logging::write_log(const char* log)
{
    struct tm* u;
    char time_str[40], full_log[strlen(log)+45];
    const time_t timer = time(nullptr);
    u = localtime(&timer);
    strftime(time_str, 40, "%d.%m.%Y %H:%M:%S ", u);
    strcpy(full_log, time_str);
    strcat(full_log, log);
    strcat(full_log, "\n");
    write(fd, full_log, strlen(full_log));
}
