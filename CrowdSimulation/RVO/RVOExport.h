#ifdef RVO_EXPORTS
#define RVO__DLL_EXPORT __declspec(dllexport)
#else
#define RVO__DLL_EXPORT __declspec(dllimport)
#endif