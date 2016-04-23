// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBOVRWRAPPER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBOVRWRAPPER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBOVRWRAPPER_EXPORTS
#define LIBOVRWRAPPER_API __declspec(dllexport)
#else
#define LIBOVRWRAPPER_API __declspec(dllimport)
#endif

// This class is exported from the LibOVRWrapper.dll
class LIBOVRWRAPPER_API CLibOVRWrapper {
public:
	CLibOVRWrapper(void);
	// TODO: add your methods here.
};

extern LIBOVRWRAPPER_API int nLibOVRWrapper;

LIBOVRWRAPPER_API int fnLibOVRWrapper(void);
