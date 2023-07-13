#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs20
{
    bool grpCheckDirEmpty(int ih)
    {
        soProbe(205, "%s(%d)\n", __FUNCTION__, ih);

        SOInode* in = soGetInodePointer(ih);
        
        for(uint32_t i = 0; i <= in->size/BlockSize; i++)
        {
            SODirentry dir[DPB];
            soReadFileBlock(ih,i,dir);
            if(strcmp(dir[0].name, ".") != 0){   return false; }
            if(strcmp(dir[1].name, "..") != 0){    return false;}
            for(uint32_t p = 2; p< DPB; p++)
            {
                if(strcmp(dir[p].name, "") != 0){ return false;}
            }
        }
        return true;
        /* replace the following line with your code 
        return binCheckDirEmpty(ih);*/
    }
};

