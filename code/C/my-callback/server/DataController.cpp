#include <stdint.h>
#include <stdio.h>

#include "DataController.h"

DataController *DataController::m_pInstance = NULL;

DataController *DataController::getInstance()
{
    if (NULL == m_pInstance) {
        if(NULL == m_pInstance) {
            m_pInstance = new DataController;
        }
    }

    return m_pInstance;
}

DataController::DataController()
{
    postData1 = NULL;
    postData2 = NULL;
    getStrData = NULL;
    getIntData = NULL;
}

DataController::~DataController()
{

}

size_t DataController::parseData()
{
    uint32_t var_id = 2019;
    int32_t var_age = 30;
    if (postData1) {
        this->postData1(var_id, var_age);
    }

    const void *mBuff = "lixiang";
    var_id = 2020;
    var_age = 31;
    if (postData2) {
        this->postData2((const char*)mBuff, var_id, var_age);
    }

    if (getStrData) {
        char const *t = reinterpret_cast<char*>(static_cast<unsigned char*>(this->getStrData()));
        printf("getStrData: %s\n", t);
    }

    if (getIntData) {
        int32_t value = this->getIntData();
        printf("getIntData value=%d\n", value);
    }

    return 0;
}
