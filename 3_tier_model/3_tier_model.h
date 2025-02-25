// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MY3TIERMODEL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MY3TIERMODEL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MY3TIERMODEL_EXPORTS
#define MY3TIERMODEL_API __declspec(dllexport)
#else
#define MY3TIERMODEL_API __declspec(dllimport)
#endif

// This class is exported from the dll
class MY3TIERMODEL_API CMy3tiermodel {
public:
	CMy3tiermodel(void);
	// TODO: add your methods here.
};

extern MY3TIERMODEL_API int nMy3tiermodel;

MY3TIERMODEL_API int fnMy3tiermodel(void);
