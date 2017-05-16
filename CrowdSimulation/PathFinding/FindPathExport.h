#ifdef FIND_PATH_EXPORTS
#define FIND_PATH_DLL_EXPORT __declspec(dllexport)
#else
#define FIND_PATH_DLL_EXPORT __declspec(dllimport)
#endif