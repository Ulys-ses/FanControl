// ������ �������� ���������� ���������� �������������
//----------------------------------------------------

#if !defined (_FC_parametes_h_)
#define _FC_parametes_h_

#include <ArduinoJson.h>

#include "FCConst.h"

// ���� ����������
enum EFCParameterCodes
{
    c_FCPCTimeGetParams,      // - ���������� ������� ����� ��������� ����������
    c_FCPCTimeSaveProbValues, // - ���������� ������� ����� ������������ ��������� ��������
    c_FCPCRestTimeWorkHard,   // - ����� ������������� �������
    c_FCPCRestTimeShine,      // - ����� ������� ����� ��� ��������� �������������
    c_FCPCRestShineLevel,     // - ������� ������������ � �������
    c_FCPCRestLowFreq,        // - ������� ����������� ������ � �������
    c_FCPCRestHighFreq,       // - ������� ������������ ������ � �������
    c_FCPCBathLowFreq,        // - ������� ����������� ������ � ������
    c_FCPCBathHighFreq,       // - ������� ������������ ������ � ������
    c_FCPCBathHudimOn,        // - ��������� �������� ��������� ��� ��������� ������������� � ������
    c_FCPCBathHudimOff,       // - ��������� �������� ��������� ��� ���������� ������������� � ������

    c_FCPCParametersQuant     // - ������������ ����� ��� ����������� ���������� ����������
};

class CFCParameters
{
    // ���������
    int m_Parameters[c_FCPCParametersQuant];

public:
    CFCParameters(void);

    // ������ ���������� �� ������
    int Load(const String &strJSON);

    // ��������� �������� ����������
    int GetParameterValue(EFCParameterCodes nParamCode)
    {
        return m_Parameters[nParamCode];
    }

    // ��������� ���� ����������
    const String GetParameterName(EFCParameterCodes nParamCode);
};

#endif // _FC_parametes_h_