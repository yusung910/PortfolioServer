#pragma once

#ifdef CORE_EXPORT
#define SCoreAPI __declspec(dllexport)
#define SCoreAPITemplateAPI
#else
#define SCoreAPI __declspec(dllimport)
#define SCoreAPITemplateAPI extern
#endif // CORE_EXPORT

// ���� �Ǿ��� �� Git rev ��ȣ�� �����´�
SCoreAPI int GetLibraryRev() noexcept;