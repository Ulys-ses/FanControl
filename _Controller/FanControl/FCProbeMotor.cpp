#include "FCProbeMotor.h"

#include <Wire.h>
#include <LOLIN_I2C_MOTOR.h>

#include "FCParameters.h"

// Объект шилда - общий для обоих моторов
LOLIN_I2C_MOTOR CFCProbeMotor::m_Motor; //I2C address 0x30

CFCProbeMotor::CFCProbeMotor(const char *szProbeCode, EFCMotorCodes nMotorCode, int nBaseLevelCode)
    : CFCProbeBase(szProbeCode)
    , m_nMotorCode(nMotorCode)
    , m_nBaseLevelCode(nBaseLevelCode)
{
    // wait motor shield ready.
    while (m_Motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR)
        m_Motor.getInfo();

    //Change A & B 's Frequency to 1000Hz.
    m_Motor.changeFreq(MOTOR_CH_BOTH, c_nMotorFrequency);

    // Ставим минимальный уровень
    SetLevel(c_FCMLBaseLevel);
}

// Установка нового уровня
void CFCProbeMotor::SetLevel(int nLevel)
{
    if (m_nValue != nLevel)
    {
        m_Motor.changeDuty(m_nMotorCode, g_FCParameters.GetParameterValue(m_nBaseLevelCode+nLevel));
        
        m_nValue = nLevel;
    }

    // Отправляем на сервер
    SendToServer();
}
