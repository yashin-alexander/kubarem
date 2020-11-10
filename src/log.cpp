#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <sstream>
#include <thread>

#include "log.h"


static const char *
log_level_description (KubaremLogLevel level)
{
    switch (level) {
    case KUBAREM_LOG_LEVEL_DEBUG:
        return "DEBUG";
    case KUBAREM_LOG_LEVEL_WARN:
        return "WARNING";
    case KUBAREM_LOG_LEVEL_INFO:
        return "INFO";
    case KUBAREM_LOG_LEVEL_ERR:
        return "ERROR";
    }
    return "__LOG";
}


static void build_log_format(char * log_format, size_t length, KubaremLogLevel level, const char * requested_format)
{
    const char * log_level_str;
    size_t log_level_str_size = 0;

    std::stringstream thread_id;
    thread_id << std::this_thread::get_id();

    log_level_str = log_level_description(level);
    sprintf(log_format, "[%s: %s]: ", thread_id.str().c_str(), log_level_str);

    log_level_str_size = sizeof (log_format);
    strncat(log_format, requested_format, length - log_level_str_size);

    log_level_str_size = sizeof (log_format);
    strncat(log_format, "\n", length - log_level_str_size);
}


void _log (KubaremLogLevel level, const char * requested_format, ...)
{
    if(level > MAX_LOG_LEVEL)
        return;

    va_list args;
    char log_format[MAX_LOG_STR_LENGTH];

    build_log_format(log_format, MAX_LOG_STR_LENGTH, level, requested_format);

    va_start(args, requested_format);
    vfprintf(stdout, log_format, args);
    va_end(args);
}
