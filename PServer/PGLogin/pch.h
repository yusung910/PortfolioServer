// pch.h: �̸� �����ϵ� ��� �����Դϴ�.
// �Ʒ� ������ ������ �� ���� �����ϵǾ�����, ���� ���忡 ���� ���� ������ ����մϴ�.
// �ڵ� ������ �� ���� �ڵ� �˻� ����� �����Ͽ� IntelliSense ���ɿ��� ������ ��Ĩ�ϴ�.
// �׷��� ���⿡ ������ ������ ���� �� ������Ʈ�Ǵ� ��� ��� �ٽ� �����ϵ˴ϴ�.
// ���⿡ ���� ������Ʈ�� ������ �߰����� ������. �׷��� ������ ���ϵ˴ϴ�.

#ifndef PCH_H
#define PCH_H

// ���⿡ �̸� �������Ϸ��� ��� �߰�
#include <memory>
#include <LoggingMacro.h>
#include <Packet.h>

#pragma comment(lib, "ws2_32.lib")

#ifndef POCO_STATIC
#define POCO_STATIC 1
#endif

#include <StringUtil.h>
#include <DefineMacro.h>
//#include "ServiceHelper.h"

#pragma warning(push)
#pragma warning(disable : 26812)
#include <Protocol_generated.h>
#pragma warning(pop)

#include <PGPDefine.h>

#endif //PCH_H
