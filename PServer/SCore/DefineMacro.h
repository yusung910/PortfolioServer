#pragma once

#define SafeDelete(x)		if (x != nullptr) { delete x; x = nullptr; }
#define SafeDeleteArray(x)	if (x != nullptr) { delete[] x; x = nullptr; }
#define SafeFree(x)			if (x != nullptr) { free(x); x = nullptr; }
#define SafeRelease(x)		if (x != NULL) { x->Release(); x = NULL; }

// for #pragma message()
#define TO_STRING(x) _TO_STRING(x)
#define _TO_STRING(x) #x

#ifdef _DEBUG
#	define NOTICE(x) __pragma(message(__FILE__ " ("  TO_STRING(__LINE__) "): " x))
#else
#	define NOTICE(x)
#endif


// Need to include <mutex>
#define AutoLock(x) std::scoped_lock __AUTO_LOCK__##x(x)
#define AutoLock2(x, y) std::scoped_lock __AUTO_LOCK__##x##y(x,y)
