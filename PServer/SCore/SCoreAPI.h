#pragma once

#ifdef CORE_EXPORT
#define CoreAPI __declspec(dllexport)
#define CoreAPITemplateAPI
#else
#define CoreAPI __declspec(dllimport)
#define CoreAPITemplateAPI extern
#endif // CORE_EXPORT

// 빌드 되었을 때 Git rev 번호를 가져온다
CoreAPI int GetLibraryRev() noexcept;
