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
    uint16_t grpGetDirentry(int pih, const char *name)
    {
        soProbe(201, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        /* replace the following line with your code */
        //return binGetDirentry(pih, name);

        SOInode* inode = soGetInodePointer(pih);

        if(!S_ISDIR(inode->mode)){  // pih must be valid node handler of a directory
            throw SOException(ENOTDIR, __FUNCTION__);
        }

        if(name && !name[0]){   // Name cannot be empty
            throw SOException(EINVAL, __FUNCTION__);
        }

        if(strchr(name, '/')!=NULL){   // Name cannot contain '/'
            throw SOException(EINVAL, __FUNCTION__);
        }

        for(uint32_t i = 0; i < inode->blkcnt; i++){
           
            SODirentry dirEntries[DPB];
            soReadFileBlock(pih, i, dirEntries);
            
            for(uint32_t j = 0; j < DPB; j++){
                
                if(strcmp(dirEntries[j].name, name) == 0){
                    return dirEntries[j].in;
                }
            }
        }
        return InodeNullReference;
    }
};

