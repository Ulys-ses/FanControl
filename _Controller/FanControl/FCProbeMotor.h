// Управление моторами
//--------------------

#if !defined (_FC_ProbeMotor_h_)
#define _FC_ProbeMotor_h_

#include "FCProbeBase.h"

#include <Wire.h>
#include <LOLIN_I2C_MOTOR.h>

class CFCProbeMotor
    : public CFCProbeBase
{
    // Объект шилда - общий для обоих моторов
    static LOLIN_I2C_MOTOR m_Motor; //I2C address 0x30
    // Код мотора
    EFCMotorCodes m_nMotorCode;
    // Номер базового уровня мотора
    int m_nBaseLevelCode;

protected:

public:
    CFCProbeMotor(const char *szProbeCode, EFCMotorCodes nMotorCode, int nBaseLevelCode);

    // Установка нового уровня
    void SetLevel(int nLevel);
};

#endif // _FC_ProbeMotor_h_