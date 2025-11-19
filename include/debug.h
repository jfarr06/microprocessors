/**
 * @file: debug.h
 * @desc: Debug logging system with configurable log levels
 * @auth: James Farrelly (C24402114)
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/* Log level definitions */
#define LOGLEVEL_INFO  0    /* Info level logging */
#define LOGLEVEL_DEBUG 1    /* Debug level logging */

#ifndef DEBUG
/**
 * Initialize debug system (no-op in release builds)
 */
#define DBG_INIT()

/**
 * Trace-level debug output (no-op in release builds)
 */
#define DBG_TRACE(...)

/**
 * Info-level debug output (no-op in release builds)
 */
#define DBG_INFO(...)
#else
#include <nucleo_f031k6/serial.h>

/**
 * Initialize debug system by setting up serial communication
 */
#define DBG_INIT() do { init_nucleo_f031k6_serial(); } while (0);

/**
 * Extract filename from full path for logging
 */
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#if LOGLEVEL == LOGLEVEL_DEBUG
/**
 * Print trace-level debug message with file and line info
 */
#define DBG_TRACE(fmt, ...) do { nucleo_f031k6_printf("[DEBUG] " fmt " (%s:%d)\n", ##__VA_ARGS__, __FILENAME__, __LINE__); } while (0)
#else
#define DBG_TRACE(...)
#endif

/**
 * Print info-level debug message with file and line info
 */
#define DBG_INFO(fmt, ...) do { nucleo_f031k6_printf("[INFO] " fmt " (%s:%d)\n", ##__VA_ARGS__, __FILENAME__, __LINE__); } while (0)
#endif

#endif
