#ifndef DEBUG_H_
#define DEBUG_H_

#define LOGLEVEL_INFO  0
#define LOGLEVEL_DEBUG 1

#ifndef DEBUG
#define DBG_INIT()
#define DBG_TRACE(...)
#else
#include <nucleo_f031k6/serial.h>

#define DBG_INIT() do { init_serial(); } while (0);
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#if LOGLEVEL == LOGLEVEL_DEBUG
#define DBG_TRACE(fmt, ...) do { s_printf("[DEBUG] " fmt " (%s:%d)\n", ##__VA_ARGS__, __FILENAME__, __LINE__); } while (0)
#else
#define DBG_TRACE(...)
#endif
#define DBG_INFO(fmt, ...) do { s_printf("[INFO] " fmt " (%s:%d)\n", ##__VA_ARGS__, __FILENAME__, __LINE__); } while (0)
#endif

#endif
