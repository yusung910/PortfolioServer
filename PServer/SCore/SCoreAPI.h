#pragma once

#ifdef CORE_EXPORT
#define SCoreAPI __declspec(dllexport)
#define SCoreAPITemplateAPI
#else
#define SCoreAPI __declspec(dllimport)
#define SCoreAPITemplateAPI extern
#endif // CORE_EXPORT

// 빌드 되었을 때 Git rev 번호를 가져온다
SCoreAPI int GetLibraryRev() noexcept;
