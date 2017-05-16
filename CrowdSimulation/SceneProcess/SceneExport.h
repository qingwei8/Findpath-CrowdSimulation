#ifdef SCENE_EXPORTS
#define SCENE_DLL_EXPORT __declspec(dllexport)
#else
#define SCENE_DLL_EXPORT __declspec(dllimport)
#endif