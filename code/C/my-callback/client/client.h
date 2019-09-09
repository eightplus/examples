#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int checkCallbackLibrary(void);
int startCallbackDaemon(void);
void stopCallbackDaemon(void);

#ifdef __cplusplus
}
#endif


