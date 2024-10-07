#pragma once
#include <cstdint>
#include <cstring>

#ifdef _MSC_VER
#include <WinSock2.h>
#else
// clang, swift, gcc
#include <arpa/inet.h>
#endif

#include "Object.h"

constexpr size_t MAX_PACKET_BINARY_SIZE = 8192;				// ��Ŷ ���̳ʸ� �ִ� ũ�� (Payload)
constexpr size_t PACKET_HEADER_SIZE = 8;					// size 4 + protocol 4
constexpr size_t MAX_PACKET_DATA_SIZE = MAX_PACKET_BINARY_SIZE - PACKET_HEADER_SIZE;	// ��Ŷ�� ���� �ִ� �ִ� ������ ũ��
constexpr unsigned int PACKET_COMPRESS_MASK = 0x80000000;	// �ֻ��� 1��Ʈ, ���࿩��
constexpr int DEFAULT_PACKET_COMPRESS_START_SIZE = 60;		// 60 byte �̻� ������ �� �� ����.

// ��Ŷ ���� ��뿩��
#define USE_PACKET_COMPRESS true	// ��Ŷ ���� ��� ����


/* *********************************************************************************
* [ 4 byte ]     [ 4 byte ] [ n bytes (max MAX_PACKET_DATA_SIZE) ]
*    size         messageid    binaryData (���� �� ����)
* (payload size)
*
*  ** Optional **
*  size �ֻ��� ��Ʈ = compress mask [1000 0000 0000 0000 0000 0000 0000 0000]
********************************************************************************* */

class Packet : public Object<Packet>
{
public:
	int HostID = 0;
	char BinaryData[MAX_PACKET_BINARY_SIZE] = { 0, };	// ��Ŷ ���̳ʸ�. Message Payload

public:
	Packet() : Packet(ObjectTag()) {};
	Packet(ObjectTag) {};

	/// <summary>
	/// Ŭ���� �� �ʱ�ȭ
	/// </summary>
	virtual void Reset()
	{
		HostID = 0;
		memset(BinaryData, 0x00, MAX_PACKET_BINARY_SIZE);
	}

	/// <summary>
	/// � ��Ŷ���� Message ID�� �����Ѵ�.
	/// </summary>
	/// <param name="messageid">Message ID</param>
	void SetMessageID(const int& messageid)
	{
		*((int*)(BinaryData + sizeof(int))) = htonl(messageid);
		SetPacketSize(PACKET_HEADER_SIZE);
	}

	/// <summary>
	/// ��Ŷ�� �Ǿ� ���� �����͸� �����Ѵ�.
	/// </summary>
	/// <param name="pData">���� ������</param>
	/// <param name="dataSize">���� �������� ũ��</param>
	/// <returns>������ ���� ���� ����</returns>
	bool SetData(const void* pData, const size_t& dataSize)
	{
		if (nullptr == pData)
			return false;

		if (dataSize > MAX_PACKET_DATA_SIZE
			|| dataSize == 0)
			return false;

#ifdef _MSC_VER
		memcpy_s(BinaryData + PACKET_HEADER_SIZE, MAX_PACKET_DATA_SIZE, pData, dataSize);
#else
		memcpy(BinaryData + PACKET_HEADER_SIZE, pData, dataSize);
#endif

		SetPacketSize(PACKET_HEADER_SIZE + dataSize);
		return true;
	}

	/// <summary>
	/// �ѹ濡 ��Ŷ �����͸� �����Ѵ�.
	/// pData�� nullptr�� ���, Header-only ��Ŷ�� ���������.
	/// </summary>
	/// <param name="messageid">Message ID</param>
	/// <param name="pData">���� ������</param>
	/// <param name="dataSize">���� ������ ũ��</param>
	/// <returns>��Ŷ ������ ���� ���� ����</returns>
	bool SetPacketData(const int& messageid, const void* pData = nullptr, const size_t& dataSize = 0)
	{
		SetMessageID(messageid);
		if (nullptr != pData)
			return SetData(pData, dataSize);
		return true;
	}

	/// <summary>
	/// � ��Ŷ �������� Message ID�� ���´�.
	/// </summary>
	/// <returns>Message ID</returns>
	int GetMessageID() const
	{
		return ntohl(*((int*)(BinaryData + sizeof(int))));
	}

	/// <summary>
	/// ��Ŷ�� ��ܿ�, ����� ������ ���� ������ ���̳ʸ��� ũ��.
	/// </summary>
	/// <returns>������ ���̳ʸ� ũ��</returns>
	int GetMessageSize() const
	{
		return GetPacketSize() - PACKET_HEADER_SIZE;
	}

	/// <summary>
	/// ����� ������, ��ü ������ ��Ŷ�� ũ��. 
	/// ( Payload Length )
	/// </summary>
	/// <returns>��Ŷ ��ü ũ��</returns>
	int GetPacketSize() const
	{
		return (int)(*((unsigned int*)(BinaryData)) & ~PACKET_COMPRESS_MASK);
	}

	/// <summary>
	/// ����� ������ ���� ������ ���̳ʸ��� ���� ������.
	/// </summary>
	/// <returns>������ ���̳ʸ� ������</returns>
	void* GetDataPtr() const
	{
		return (void*)(BinaryData + PACKET_HEADER_SIZE);
	}

	/// <summary>
	/// �ش� ��Ŷ�� ����� ��Ŷ���� ����
	/// </summary>
	/// <returns>��Ŷ �������� ���� ����</returns>
	bool IsCompressed() const
	{
		return (*((unsigned int*)(BinaryData)) & PACKET_COMPRESS_MASK) > 0u;
	}

	/// <summary>
	/// ��Ŷ �������� ���� ���� FLAG�� �����Ѵ�.
	/// </summary>
	/// <param name="onoff">������ ��Ŷ ���� ���� ����</param>
	void SetCompressed(const bool& onoff)
	{
		if (true == onoff)
			*((unsigned int*)(BinaryData)) |= PACKET_COMPRESS_MASK;
		else
			*((unsigned int*)(BinaryData)) &= ~PACKET_COMPRESS_MASK;
	}

	/// <summary>
	/// ��Ʈ��ũ�� ���� ��Ŷ�� ��ü ũ�⸦ �����Ѵ�.
	/// ( Payload Length )
	/// 
	/// ������ �ܺο����� �� �ʿ��� ��� �����ϰ�� ������� ����.
	/// (��Ŷ ���� �� ���� ����� ���۵Ǵ� ��� ���)
	/// </summary>
	/// <param name="size">������ ��Ŷ�� ũ��. PACKET_HEADER_SIZE ���� ������ �ȵ�.</param>
	void SetPacketSize(const size_t& size)
	{
		if (size < PACKET_HEADER_SIZE)
			return;
		bool isCompress = IsCompressed();

		*((int*)(BinaryData)) = (static_cast<int>(size));

		if (true == isCompress)
			SetCompressed(isCompress);
	}


};




