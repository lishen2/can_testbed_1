#ifndef _DEBUG_H_
#define _DEBUG_H_

#if defined(DBG_LEVEL_VERBOSE)
#define DbgInit()             _DebugInit()
#define DbgPrintfV(...)       _DebugPrintf(__VA_ARGS__)
#define DbgPrintfD(...)       _DebugPrintf(__VA_ARGS__)
#define DbgPrintfW(...)       _DebugPrintf(__VA_ARGS__)
#define DbgPrintfE(...)       _DebugPrintf(__VA_ARGS__)
#define DbgPrintHex(buf, len) _DebugPrintHex(buf, len)
#elif defined(DBG_LEVEL_DEBUG)
#define DbgInit()       _DebugInit()
#define DbgPrintfV(...)
#define DbgPrintfD(...) _DebugPrintf(__VA_ARGS__)
#define DbgPrintfW(...) _DebugPrintf(__VA_ARGS__)
#define DbgPrintfE(...) _DebugPrintf(__VA_ARGS__)
#define DbgPrintHex(buf, len)
#elif defined(DBG_LEVEL_WARN)
#define DbgInit()       _DebugInit()
#define DbgPrintfV(...)
#define DbgPrintfD(...)
#define DbgPrintfW(...) _DebugPrintf(__VA_ARGS__)
#define DbgPrintfE(...) _DebugPrintf(__VA_ARGS__)
#define DbgPrintHex(buf, len)
#elif defined(DBG_LEVEL_ERROR)
#define DbgInit()       _DebugInit()
#define DbgPrintfV(...)
#define DbgPrintfD(...)
#define DbgPrintfW(...)
#define DbgPrintfE(...) _DebugPrintf(__VA_ARGS__)
#define DbgPrintHex(buf, len)
#else
#define DbgInit()
#define DbgPrintfV(...)
#define DbgPrintfD(...)
#define DbgPrintfW(...)
#define DbgPrintfE(...)
#define DbgPrintHex(buf, len)
#endif

#if defined(DBG_LEVEL_VERBOSE) || defined(DBG_LEVEL_DEBUG)
#define ASSERT(exp)\
    if (!(exp)){\
        _DebugPrintf("ASSERT: %s:%d\r\n", __FUNCTION__, __LINE__);\
    }

#define TEST_ASSERT(exp, ...)\
    if (!(exp)){\
        _DebugPrintf("TEST FAULT, %s:%d | ", __FILE__, __LINE__);\
        _DebugPrintf(__VA_ARGS__);\
        _DebugPrintf("\r\n");\
    }
#else
#define ASSERT(exp)
#define TEST_ASSERT(exp, ...)
#endif

void _DebugInit(void);
void _DebugPrintf(const char* fmt, ...);
void _DebugPrintHex(const unsigned char* buf, int buflen);

#endif

