// ������ �������� ���������� ���������� �������������
//----------------------------------------------------

#include <ArduinoJson.h>

#include "FCParameters.h"

const String g_strParamNames[] = 
{
    "TimeGetParams",      // - ���������� ������� ����� ��������� ����������
    "TimeSaveProbValues", // - ���������� ������� ����� ������������ ��������� ��������
    "RestTimeWorkHard",   // - ����� ������������� �������
    "RestTimeShine",      // - ����� ������� ����� ��� ��������� �������������
    "RestShineLevel",     // - ������� ������������ � �������
    "RestLowFreq",        // - ������� ����������� ������ � �������
    "RestHighFreq",       // - ������� ������������ ������ � �������
    "BathLowFreq",        // - ������� ����������� ������ � ������
    "BathHighFreq",       // - ������� ������������ ������ � ������
    "BathHudimOn",        // - ��������� �������� ��������� ��� ��������� ������������� � ������
    "BathHudimOff"       // - ��������� �������� ��������� ��� ���������� ������������� � ������
};

CFCParameters::CFCParameters(void)
    : m_Parameters({5,60,300,300,10,20,100,20,100,60,40})       
{}

// ������ ���������� �� ������
int CFCParameters::Load(const String &strJSON)
{
    // ��������� ������
    StaticJsonDocument<1000> jsonParams;
    DeserializationError nError = deserializeJson(jsonParams, strJSON);
    if (nError)
        return c_FCEIncorrectParameterJSON;

    // ������� � ����������
    for (int i = 0; i < jsonParams.size(); ++i)
    {
        String strParamName = jsonParams[i]["Name"];
        for (int j = 0; j < c_FCPCParametersQuant; ++j)
            if (g_strParamNames[j] == strParamName)
            {
                m_Parameters[j] = jsonParams[j]["Value"];
                break;
            }
    }

    return c_FCEOK;
}

const String CFCParameters::GetParameterName(EFCParameterCodes nParamCode)
{
    return g_strParamNames[nParamCode];
}
