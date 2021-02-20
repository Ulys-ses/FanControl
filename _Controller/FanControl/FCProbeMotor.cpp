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
    , m_nFinishWork(c_nMotorTimeInf)
{
}

void CFCProbeMotor::sInitMotor(void)
{
    // wait motor shield ready.
    while (m_Motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR)
        m_Motor.getInfo();

    //Change A & B 's Frequency to 1000Hz.
    m_Motor.changeFreq(MOTOR_CH_BOTH, c_nMotorFrequency);
}

// Установка нового уровня
void CFCProbeMotor::SetLevel(int nLevel, int nTimeout)
{
    if (m_nValue != nLevel)
    {
        m_Motor.changeDuty(m_nMotorCode, g_FCParameters.GetParameterValue(m_nBaseLevelCode+nLevel));
        
        m_nValue = nLevel;
    }

    // Если таймер задан - пересчитаем его во время финиша
    if (c_nMotorTimeInf != nTimeout)
        nTimeout = millis() + nTimeout * 1000;
    
    // Фиксируем время финиша
    m_nFinishWork = nTimeout;

    // Отправляем на сервер
    SendToServer();
}

void CFCProbeMotor::CheckTimer(int nLevel)
{
    if (c_nMotorTimeInf != m_nFinishWork)    // Если таймер установлен
        if (millis() > m_nFinishWork)        // Если время пришло
            SetLevel(nLevel);                // Включаем  бессрочно

    // Отправляем на сервер
    SendToServer();
}
