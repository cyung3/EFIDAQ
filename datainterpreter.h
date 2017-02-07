#ifndef DATAINTERPRETER_H
#define DATAINTERPRETER_H

#include <stdint.h>
#include <QString>

#define RAW_DATA_LENGTH 40

class DataInterpreter
{
public:
    QString getString();
    double getValue(int index);
    
    void setBytes(void* rawBytesAddress);
    void* getBytes();
    
    int getNumBytes();
    int getNumFields();

    uint32_t getEnd();

private:
    union RawData
    {
        char data[RAW_DATA_LENGTH];
        struct ByteInterpreter
        {
            uint32_t time;
            uint32_t totalRevolutions;
            float ECT;
            float IAT;
            float MAP;
            float TPS;
            float OIN;
            float totalPulseTime;
            int32_t RPM;
            uint32_t end;
            //char nullTerminator[1];
        } bi;
    } *rd;
};
#endif // DATAINTERPRETER_H
