#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ADynamicLibrary ADynamicLibrary;

ADynamicLibrary *adynamicLibrary_open( const char *libraryName, char **pError);
void *adynamicLibrary_findSymbol(ADynamicLibrary *lib, const char *symbolName, char **pError);
void adynamicLibrary_close(ADynamicLibrary *lib);

#ifdef __cplusplus
}
#endif
