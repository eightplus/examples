#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

int initCallbackDaemon(const char *addr);
int destroyCallbackDaemon(void);

#ifdef __cplusplus
}
#endif
