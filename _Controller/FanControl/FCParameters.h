// ћодуль хранени€ параметров управлени€ вентил€торами
//----------------------------------------------------

#if !defined (_FC_parametes_h_)
#define _FC_parametes_h_

#include <ArduinoJson.h>

#include "FCConst.h"

//  оды параметров
enum EFCParameterCodes
{
    c_FCPCTimeGetParams,      // - промежуток времени между запросами параметров
    c_FCPCTimeSaveProbValues, // - промежуток времени между сохранени€ми показаний датчиков
    c_FCPCRestTimeWorkHard,   // - врем€ проветривани€ туалета
    c_FCPCRestTimeShine,      // - врем€ горени€ лампы дл€ включени€ проветривани€
    c_FCPCRestShineLevel,     // - уровень освещенности в туалете
    c_FCPCRestLowFreq,        // - частота посто€нного режима в туалете
    c_FCPCRestHighFreq,       // - частота интенсивного режима в туалете
    c_FCPCBathLowFreq,        // - частота посто€нного режима в ванной
    c_FCPCBathHighFreq,       // - частота интенсивного режима в ванной
    c_FCPCBathHudimOn,        // - пороговое значение влажности дл€ включени€ проветривани€ в ванной
    c_FCPCBathHudimOff,       // - пороговое значение влажности дл€ вџключени€ проветривани€ в ванной

    c_FCPCParametersQuant     // - ћаксимальный номер дл€ определени€ количества параметров
};

class CFCParameters
{
    // ѕараметры
    int m_Parameters[c_FCPCParametersQuant];

public:
    CFCParameters(void);

    // „тение параметорв из строки
    int Load(const String &strJSON);

    // ѕолучение значений параметров
    int GetParameterValue(EFCParameterCodes nParamCode)
    {
        return m_Parameters[nParamCode];
    }

    // ѕолучение имен параметров
    const String GetParameterName(EFCParameterCodes nParamCode);
};

#endif // _FC_parametes_h_