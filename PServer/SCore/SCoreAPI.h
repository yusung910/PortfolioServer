#pragma once

#ifdef CORE_EXPORT
#define CoreAPI __declspec(dllexport)
#define CoreAPITemplateAPI
#else
#define CoreAPI __declspec(dllimport)
#define CoreAPITemplateAPI extern
#endif // CORE_EXPORT

// ���� �Ǿ��� �� Git rev ��ȣ�� �����´�
CoreAPI int GetLibraryRev() noexcept;
