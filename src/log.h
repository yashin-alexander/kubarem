#pragma once

#include "glad/glad.h"

// Log levels
enum KubaremLogLevel : unsigned int
{
    kKubaremLogLevelErr = 0x00000001,
    kKubaremLogLevelWarn = 0x00000002,
    kKubaremLogLevelInfo = 0x00000004,
    kKubaremLogLevelDebug = 0x00000008
};


void _log (KubaremLogLevel level, const char * requested_format, ...);

void _glfwErrorCallback(int error, const char* description);

void GLAPIENTRY _glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                   GLsizei length, const GLchar* message, const void* userParam);

void _flush_log();


#define log_err(...) \
    _log (kKubaremLogLevelErr, __VA_ARGS__)

#define log_warn(...) \
    _log (kKubaremLogLevelWarn, __VA_ARGS__)

#define log_info(...) \
    _log (kKubaremLogLevelInfo, __VA_ARGS__)

#define log_dbg(...) \
    _log (kKubaremLogLevelDebug, __VA_ARGS__)
