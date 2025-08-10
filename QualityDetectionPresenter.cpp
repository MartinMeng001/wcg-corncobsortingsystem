//---------------------------------------------------------------------------

#pragma hdrstop

#include "TQualityDetectionPresenter.h"
#include <iostream>
#include <sstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)

TQualityDetectionPresenter::TQualityDetectionPresenter()
    : m_ModbusContext(nullptr), m_bConnected(false)
{
}
//---------------------------------------------------------------------------

TQualityDetectionPresenter::~TQualityDetectionPresenter()
{
    Disconnect();
}
//---------------------------------------------------------------------------

bool TQualityDetectionPresenter::Connect(const char* ip, int port)
{
    try {
        // ����Modbus TCP������
        m_ModbusContext = modbus_new_tcp(ip, port);
        if (m_ModbusContext == nullptr) {
            SetLastError("�޷�����Modbus������");
            return false;
        }

        // ���ôӻ�ID
        modbus_set_slave(m_ModbusContext, 1);

        // ���ó�ʱʱ��
        modbus_set_response_timeout(m_ModbusContext, 1, 0); // 1�볬ʱ

        // ��������
        if (modbus_connect(m_ModbusContext) == -1) {
            std::string error = "����ʧ��: ";
            error += modbus_strerror(errno);
            SetLastError(error);
            modbus_free(m_ModbusContext);
            m_ModbusContext = nullptr;
            return false;
        }

        m_bConnected = true;
        SetLastError("");
        return true;
    }
    catch (...) {
        SetLastError("���ӹ����з���δ֪�쳣");
        return false;
    }
}
//---------------------------------------------------------------------------

void TQualityDetectionPresenter::Disconnect()
{
    if (m_ModbusContext != nullptr) {
        modbus_close(m_ModbusContext);
        modbus_free(m_ModbusContext);
        m_ModbusContext = nullptr;
    }
    m_bConnected = false;
}
//---------------------------------------------------------------------------

bool TQualityDetectionPresenter::StartReference()
{
    return WriteRegister(ADDR_REFERENCE_CONTROL, 1);
}
//---------------------------------------------------------------------------

bool TQualityDetectionPresenter::StopReference()
{
    return WriteRegister(ADDR_REFERENCE_CONTROL, 0);
}
//---------------------------------------------------------------------------

int TQualityDetectionPresenter::GetReferenceStatus()
{
    return ReadRegister(ADDR_REFERENCE_CONTROL);
}
//---------------------------------------------------------------------------

bool TQualityDetectionPresenter::StartDetection()
{
    return WriteRegister(ADDR_DETECTION_CONTROL, 1);
}
//---------------------------------------------------------------------------

bool TQualityDetectionPresenter::StopDetection()
{
    return WriteRegister(ADDR_DETECTION_CONTROL, 0);
}
//---------------------------------------------------------------------------

int TQualityDetectionPresenter::GetDetectionStatus()
{
    return ReadRegister(ADDR_DETECTION_CONTROL);
}
//---------------------------------------------------------------------------

std::vector<float> TQualityDetectionPresenter::ReadDetectionResults()
{
    std::vector<float> results;

    try {
        std::vector<uint16_t> rawData = ReadRegisters(ADDR_RESULTS_START, RESULTS_COUNT);

        // ��ԭʼ����ת��Ϊ������������100��
        for (uint16_t value : rawData) {
            float result = static_cast<float>(value) / 100.0f;
            results.push_back(result);
        }
    }
    catch (...) {
        SetLastError("��ȡ�����ʱ�����쳣");
    }

    return results;
}
//---------------------------------------------------------------------------

int TQualityDetectionPresenter::GetSerialNumber()
{
    return ReadRegister(ADDR_SERIAL_NUMBER);
}
//---------------------------------------------------------------------------

int TQualityDetectionPresenter::GetErrorCode()
{
    return ReadRegister(ADDR_ERROR_CODE);
}
//---------------------------------------------------------------------------

bool TQualityDetectionPresenter::WriteRegister(int address, int value)
{
    if (!m_bConnected || m_ModbusContext == nullptr) {
        SetLastError("�豸δ����");
        return false;
    }

    try {
        int result = modbus_write_register(m_ModbusContext, address, value);
        if (result == -1) {
            std::string error = "д�Ĵ���ʧ��(��ַ:";
            error += std::to_string(address);
            error += "): ";
            error += modbus_strerror(errno);
            SetLastError(error);
            return false;
        }
        return true;
    }
    catch (...) {
        SetLastError("д�Ĵ���ʱ����δ֪�쳣");
        return false;
    }
}
//---------------------------------------------------------------------------

int TQualityDetectionPresenter::ReadRegister(int address)
{
    if (!m_bConnected || m_ModbusContext == nullptr) {
        SetLastError("�豸δ����");
        return -1;
    }

    try {
        uint16_t data;
        int result = modbus_read_registers(m_ModbusContext, address, 1, &data);
        if (result == -1) {
            std::string error = "���Ĵ���ʧ��(��ַ:";
            error += std::to_string(address);
            error += "): ";
            error += modbus_strerror(errno);
            SetLastError(error);
            return -1;
        }
        return static_cast<int>(data);
    }
    catch (...) {
        SetLastError("���Ĵ���ʱ����δ֪�쳣");
        return -1;
    }
}
//---------------------------------------------------------------------------

std::vector<uint16_t> TQualityDetectionPresenter::ReadRegisters(int startAddress, int count)
{
    std::vector<uint16_t> results;

    if (!m_bConnected || m_ModbusContext == nullptr) {
        SetLastError("�豸δ����");
        return results;
    }

    try {
        std::vector<uint16_t> data(count);
        int result = modbus_read_registers(m_ModbusContext, startAddress, count, data.data());
        if (result == -1) {
            std::string error = "�������Ĵ���ʧ��(��ʼ��ַ:";
            error += std::to_string(startAddress);
            error += ", ����:";
            error += std::to_string(count);
            error += "): ";
            error += modbus_strerror(errno);
            SetLastError(error);
            return results;
        }

        results = data;
        return results;
    }
    catch (...) {
        SetLastError("�������Ĵ���ʱ����δ֪�쳣");
        return results;
    }
}
//---------------------------------------------------------------------------

void TQualityDetectionPresenter::SetLastError(const std::string& error)
{
    m_LastError = error;
}
