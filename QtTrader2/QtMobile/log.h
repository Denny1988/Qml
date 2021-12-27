#ifndef LOG_H
#define LOG_H
#include <QLoggingCategory>

class Log
{
public:
    Log();
    static void e(const char * msg);
    static void i(const char * msg);
    static void d(const char * msg);

};

#endif // LOG_H
