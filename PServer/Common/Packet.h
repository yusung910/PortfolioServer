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

constexpr size_t MAX_PACKET_BINARY_SIZE = 8192;				// 패킷 바이너리 최대 크기 (Payload)
constexpr size_t PACKET_HEADER_SIZE = 8;					// size 4 + protocol 4
constexpr size_t MAX_PACKET_DATA_SIZE = MAX_PACKET_BINARY_SIZE - PACKET_HEADER_SIZE;	// 패킷에 들어갈수 있는 최대 데이터 크기
constexpr unsigned int PACKET_COMPRESS_MASK = 0x80000000;	// 최상위 1비트, 압축여부
constexpr int DEFAULT_PACKET_COMPRESS_START_SIZE = 60;		// 60 byte 이상 보내야 할 때 압축.

// 패킷 압축 사용여부
#define USE_PACKET_COMPRESS true	// 패킷 압축 사용 여부


/* *********************************************************************************
* [ 4 byte ]     [ 4 byte ] [ n bytes (max MAX_PACKET_DATA_SIZE) ]
*    size         messageid    binaryData (없을 수 있음)
* (payload size)
*
*  ** Optional **
*  size 최상위 비트 = compress mask [1000 0000 0000 0000 0000 0000 0000 0000]
********************************************************************************* */

class Packet : public Object<Packet>
{
public:
	int HostID = 0;
	char BinaryData[MAX_PACKET_BINARY_SIZE] = { 0, };	// 패킷 바이너리. Message Payload

public:
	Packet() : Packet(ObjectTag()) {};
	Packet(ObjectTag) {};

	/// <summary>
	/// 클래스 값 초기화
	/// </summary>
	virtual void Reset()
	{
		HostID = 0;
		memset(BinaryData, 0x00, MAX_PACKET_BINARY_SIZE);
	}

	/// <summary>
	/// 어떤 패킷인지 Message ID를 설정한다.
	/// </summary>
	/// <param name="messageid">Message ID</param>
	void SetMessageID(const int& messageid)
	{
		*((int*)(BinaryData + sizeof(int))) = htonl(messageid);
		SetPacketSize(PACKET_HEADER_SIZE);
	}

	/// <summary>
	/// 패킷에 실어 보낼 데이터를 설정한다.
	/// </summary>
	/// <param name="pData">보낼 데이터</param>
	/// <param name="dataSize">보낼 데이터의 크기</param>
	/// <returns>데이터 설정 성공 여부</returns>
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
	/// 한방에 패킷 데이터를 설정한다.
	/// pData가 nullptr일 경우, Header-only 패킷이 만들어진다.
	/// </summary>
	/// <param name="messageid">Message ID</param>
	/// <param name="pData">보낼 데이터</param>
	/// <param name="dataSize">보낼 데이터 크기</param>
	/// <returns>패킷 데이터 설정 성공 여부</returns>
	bool SetPacketData(const int& messageid, const void* pData = nullptr, const size_t& dataSize = 0)
	{
		SetMessageID(messageid);
		if (nullptr != pData)
			return SetData(pData, dataSize);
		return true;
	}

	/// <summary>
	/// 어떤 패킷 종류인지 Message ID를 얻어온다.
	/// </summary>
	/// <returns>Message ID</returns>
	int GetMessageID() const
	{
		return ntohl(*((int*)(BinaryData + sizeof(int))));
	}

	/// <summary>
	/// 패킷에 담겨온, 헤더를 제외한 실제 데이터 바이너리의 크기.
	/// </summary>
	/// <returns>데이터 바이너리 크기</returns>
	int GetMessageSize() const
	{
		return GetPacketSize() - PACKET_HEADER_SIZE;
	}

	/// <summary>
	/// 헤더를 포함한, 전체 전송할 패킷의 크기. 
	/// ( Payload Length )
	/// </summary>
	/// <returns>패킷 전체 크기</returns>
	int GetPacketSize() const
	{
		return (int)(*((unsigned int*)(BinaryData)) & ~PACKET_COMPRESS_MASK);
	}

	/// <summary>
	/// 헤더를 제외한 실제 데이터 바이너리의 시작 포인터.
	/// </summary>
	/// <returns>데이터 바이너리 포인터</returns>
	void* GetDataPtr() const
	{
		return (void*)(BinaryData + PACKET_HEADER_SIZE);
	}

	/// <summary>
	/// 해당 패킷이 압축된 패킷인지 여부
	/// </summary>
	/// <returns>패킷 데이터의 압축 여부</returns>
	bool IsCompressed() const
	{
		return (*((unsigned int*)(BinaryData)) & PACKET_COMPRESS_MASK) > 0u;
	}

	/// <summary>
	/// 패킷 데이터의 압축 여부 FLAG를 설정한다.
	/// </summary>
	/// <param name="onoff">설정할 패킷 압축 여부 상태</param>
	void SetCompressed(const bool& onoff)
	{
		if (true == onoff)
			*((unsigned int*)(BinaryData)) |= PACKET_COMPRESS_MASK;
		else
			*((unsigned int*)(BinaryData)) &= ~PACKET_COMPRESS_MASK;
	}

	/// <summary>
	/// 네트워크로 보낼 패킷의 전체 크기를 설정한다.
	/// ( Payload Length )
	/// 
	/// 가급적 외부에서는 꼭 필요한 경우 제외하고는 사용하지 말것.
	/// (패킷 압축 등 내부 사이즈가 조작되는 경우 사용)
	/// </summary>
	/// <param name="size">전송할 패킷의 크기. PACKET_HEADER_SIZE 보다 작으면 안됨.</param>
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




