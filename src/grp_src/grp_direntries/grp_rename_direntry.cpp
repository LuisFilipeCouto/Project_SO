#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <string.h>
#include <errno.h>
#include <sys/stat.h>

namespace sofs20
{
    void grpRenameDirentry(int pih, const char *name, const char *newName)
    {
        soProbe(204, "%s(%d, %s, %s)\n", __FUNCTION__, pih, name, newName);

        /* replace the following line with your code */
        //binRenameDirentry(pih, name, newName);

        SOInode* inode = soGetInodePointer(pih);

        if(!S_ISDIR(inode->mode)){  // pih must be valid node handler of a directory
            throw SOException(ENOTDIR, __FUNCTION__);
        }

        if(name && !name[0]){   // Name cannot be empty
            throw SOException(EINVAL, __FUNCTION__);
        }

        if(newName && !newName[0]){   // New name cannot be empty
            throw SOException(EINVAL, __FUNCTION__);
        }

        SODirentry dirEntries[DPB];
        bool entryExists = false;
        uint32_t i = 0;
        uint32_t b_index = BlockNullReference;
        
        
        while (i < inode->blkcnt) { 

            soReadFileBlock(pih, i, dirEntries);

            for (uint32_t k = 0; k < DPB; k++) {

                if (strcmp(newName, dirEntries[k].name)==0) { // If New Name already exists, we cannot rename another entry with the same New Name
                    throw SOException(EEXIST, __FUNCTION__);
                }

                if (strcmp(name, dirEntries[k].name)==0) { // If name exists
                    entryExists = true;
                    b_index = i;
                    strncpy(dirEntries[k].name, newName, SOFS20_FILENAME_LEN); // Rename entry using the max allowed length for a name (SOFS_FILENAME_LEN)
                    break;
                }
            }
            i++;
        }
        if (!entryExists){ // If name does not exist
            throw SOException(ENOENT, __FUNCTION__);
        }
        soWriteFileBlock(pih, b_index, dirEntries);
    }
};