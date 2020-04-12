#ifndef LOG_HPP
#define LOG_HPP


static const int MAX_LOG_STR_LENGTH = 512;


/* Log levels */
typedef enum {
    KUBAREM_LOG_LEVEL_ERR   = 0x00000001,
    KUBAREM_LOG_LEVEL_WARN  = 0x00000002,
    KUBAREM_LOG_LEVEL_INFO  = 0x00000004,
    KUBAREM_LOG_LEVEL_DEBUG = 0x00000008
} KubaremLogLevel;


void _log (KubaremLogLevel level, const char * requested_format, ...);


#define log_err(...) \
    _log (KUBAREM_LOG_LEVEL_ERR, __VA_ARGS__)

#define log_warn(...) \
    _log (KUBAREM_LOG_LEVEL_WARN, __VA_ARGS__)

#define log_info(...) \
    _log (KUBAREM_LOG_LEVEL_INFO, __VA_ARGS__)

#define log_dbg(...) \
    _log (KUBAREM_LOG_LEVEL_DEBUG, __VA_ARGS__)


#endif // LOG_HPP
