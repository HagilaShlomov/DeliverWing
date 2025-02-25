// 3_tier_model.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "3_tier_model.h"


// This is an example of an exported variable
MY3TIERMODEL_API int nMy3tiermodel=0;

// This is an example of an exported function.
MY3TIERMODEL_API int fnMy3tiermodel(void)
{
    return 0;
    
}

// This is the constructor of a class that has been exported.
CMy3tiermodel::CMy3tiermodel()
{
    return;
}
