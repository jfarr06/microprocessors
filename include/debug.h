#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef DEBUG
#define DBG_INIT()
#define DBG_TRACE(...)
#else
#include <nucleo_f031k6/serial.h>

#define DBG_INIT() do { init_serial(); } while (0);
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define DBG_TRACE(fmt, ...) do { s_printf("[INFO] " fmt " (%s:%d)\n", ##__VA_ARGS__, __FILENAME__, __LINE__); } while (0)
#endif

#endif
