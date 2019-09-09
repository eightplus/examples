#ifndef _DATA_CONTROLLER_H_
#define _DATA_CONTROLLER_H_

#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>

typedef int32_t (*postData1_proc_t) (uint32_t, int32_t);
typedef void (*postData2_proc_t) (const char*, uint32_t, int32_t);
typedef unsigned char * (*getStrData_proc_t) ();
typedef uint32_t (*getIntData_proc_t) ();

class DataController
{
public:
    DataController();
    ~DataController();

    static DataController *getInstance();

    postData1_proc_t postData1;
    postData2_proc_t postData2;
    getStrData_proc_t getStrData;
    getIntData_proc_t getIntData;

    size_t parseData();

private:
    static DataController *m_pInstance;
};

#endif // _DATA_CONTROLLER_H_
