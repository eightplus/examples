#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "dll.h"
#include "client.h"
#include "../server/my_callback_api.h"

#define CALLBACK_LIB_NAME  "libMyCallbackApi"

static ADynamicLibrary *m_lib;
static int m_libDaemonStarted;

int checkCallbackLibrary(void)
{
    char *error = NULL;

    if (m_lib != NULL)
        return 0;

    m_lib = adynamicLibrary_open(CALLBACK_LIB_NAME, &error);
    if (m_lib == NULL) {
        printf("Could not load display control library [%s]: %s\n", CALLBACK_LIB_NAME, error);
        return -1;
    }

    /* Resolve the functions */
    void*  symbol;
    symbol = adynamicLibrary_findSymbol(m_lib, "initCallbackDaemon", &error);
    if (symbol != NULL) {
	printf("Found required symbol (initCallbackDaemon)\n");
    }
    else {
        printf("Could not find required symbol (initCallbackDaemon): %s\n", error);
        free(error);
        goto out;
    }
    symbol = adynamicLibrary_findSymbol(m_lib, "destroyCallbackDaemon", &error);
    if (symbol != NULL) {
	printf("Found required symbol (destroyCallbackDaemon)\n");
    }
    else {
        printf("display control: Could not find required symbol (destroyCallbackDaemon): %s\n", error);
        free(error);
        goto out;
    }

    adynamicLibrary_close(m_lib);
    m_lib = NULL;

    return 0;

out:
    printf("library [%s] could not be initialized!\n", CALLBACK_LIB_NAME);
    adynamicLibrary_close(m_lib);
    m_lib = NULL;
    return -1;

}

int startCallbackDaemon(void)
{
    if (m_libDaemonStarted) {
        return 0;
    }

    if (!initCallbackDaemon("lixiang")) {//my_callback_api.h
        printf("Can't start display control?\n");
        return -1;
    }

    m_libDaemonStarted = 1;

    return 0;
}

void stopCallbackDaemon(void)
{
    if (m_libDaemonStarted) {
        destroyCallbackDaemon();//my_callback_api.h

        m_libDaemonStarted = 0;
    }
}
