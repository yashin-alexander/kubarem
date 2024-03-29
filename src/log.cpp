#include "log.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <thread>

static const unsigned kMaxLogStrLength = 512;
// Bit mask for filtering log messages
static const unsigned kLogLevelFilter = ~0u;

static const char* log_level_description(KubaremLogLevel level)
{
    switch (level) {
    case kKubaremLogLevelDebug:
        return "DEBUG";
    case kKubaremLogLevelWarn:
        return "WARNING";
    case kKubaremLogLevelInfo:
        return "INFO";
    case kKubaremLogLevelErr:
        return "ERROR";
    }
    return "__LOG";
}

static const char* gl_source_description(GLenum source)
{
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return "Shader Compiler";
    case GL_DEBUG_SOURCE_APPLICATION:
        return "Application";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return "Third Party";
    case GL_DEBUG_SOURCE_OTHER:
    default:
        return "Other";
    };
}

static const char* gl_type_description(GLenum type)
{
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return "Deprecated";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "Undefined Behaviour";
    case GL_DEBUG_TYPE_PORTABILITY:
        return "Portability Problem";
    case GL_DEBUG_TYPE_PERFORMANCE:
        return "Performance Problem";
    case GL_DEBUG_TYPE_OTHER:
    default:
        return "Other";
    }
}

static void build_log_format(char* log_format, size_t length, KubaremLogLevel level, const char* requested_format)
{
    const char* log_level_str;
    size_t log_level_str_size = 0;

    std::stringstream thread_id;
    thread_id << std::this_thread::get_id();

    log_level_str = log_level_description(level);
    sprintf(log_format, "[%s: %s]: ", thread_id.str().c_str(), log_level_str);

    log_level_str_size = sizeof(log_format);
    strncat(log_format, requested_format, length - log_level_str_size);

    log_level_str_size = sizeof(log_format);
    strncat(log_format, "\n", length - log_level_str_size);
}

void _log(KubaremLogLevel level, const char* requested_format, ...)
{
    if ((level & kLogLevelFilter) == 0)
        return;

    va_list args;
    char log_format[kMaxLogStrLength];

    build_log_format(log_format, kMaxLogStrLength, level, requested_format);

    va_start(args, requested_format);
    vfprintf(stdout, log_format, args);
    va_end(args);
}

void _glfwErrorCallback(int error, const char* description)
{
    log_err("GLFW Error %d: %s", error, description);
}

void GLAPIENTRY _glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
    const void* userParam)
{
    switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        log_info("GL Notification [%d]: %s (%s, %s)", id, message, gl_type_description(type), gl_source_description(source));
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
    case GL_DEBUG_SEVERITY_LOW:
        log_warn("GL Warning [%d]: %s (%s, %s)", id, message, gl_type_description(type), gl_source_description(source));
        break;
    case GL_DEBUG_SEVERITY_HIGH:
    default:
        log_err("GL Error [%d]: %s (%s, %s)", id, message, gl_type_description(type), gl_source_description(source));
        break;
    }
}

void _flush_log()
{
    fflush(stdout);
}
