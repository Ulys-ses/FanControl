// Модуль хранения параметров управления вентиляторами
//----------------------------------------------------

#if !defined (_FC_parametes_h_)
#define _FC_parametes_h_

#include <ArduinoJson.h>

#include "FCConst.h"

// Коды параметров
enum EFCParameterCodes
{
    c_FCPCTimeGetParams,      // - промежуток времени между запросами параметров
    c_FCPCTimeSaveProbValues, // - промежуток времени между сохранениями показаний датчиков
    c_FCPCRestTimeWorkHard,   // - время проветривания туалета
    c_FCPCRestTimeShine,      // - время горения лампы для включения проветривания
    c_FCPCRestShineLevel,     // - уровень освещенности в туалете
    c_FCPCRestLowFreq,        // - частота постоянного режима в туалете
    c_FCPCRestHighFreq,       // - частота интенсивного режима в туалете
    c_FCPCBathLowFreq,        // - частота постоянного режима в ванной
    c_FCPCBathHighFreq,       // - частота интенсивного режима в ванной
    c_FCPCBathHudimOn,        // - пороговое значение влажности для включения проветривания в ванной
    c_FCPCBathHudimOff,       // - пороговое значение влажности для вЫключения проветривания в ванной

    c_FCPCParametersQuant     // - Максимальный номер для определения количества параметров
};

class CFCParameters
{
    // Параметры
    int m_Parameters[c_FCPCParametersQuant];

    // Время последнего обновления параметров
    long m_lLastUpdate;

public:
    CFCParameters(void);

    // Чтение параметорв из строки
    int Load(const String &strJSON);

    // Получение значений параметров
    int GetParameterValue(EFCParameterCodes nParamCode)
    {
        return m_Parameters[nParamCode];
    }

    // Получение имен параметров
    const String GetParameterName(EFCParameterCodes nParamCode);

    // Проверка необходимости обновления параметров
    bool NeedUpdate(void)
    {
        return  (m_lLastUpdate + m_Parameters[c_FCPCTimeGetParams] * 1000 <= millis());
    }
};

#endif // _FC_parametes_h_