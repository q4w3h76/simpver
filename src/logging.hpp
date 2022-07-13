#ifndef LOGGING_HPP
#define LOGGING_HPP

class Logging
{
    int fd;
public:
        Logging(const char* = "/var/log/simpver.log");
        ~Logging();
        void write_log(const char*);
};

#endif // LOGGING_HPP
