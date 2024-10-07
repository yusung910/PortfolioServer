#pragma once
class Packet;

class DispatcherInterface
{
public:
	virtual ~DispatcherInterface() = default;
	virtual bool DispatchPacket(const Packet& _packet) = 0;
};