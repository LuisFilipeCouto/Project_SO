#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    /*
       filling in the contents of the root directory:
       the first 2 entries are filled in with "." and ".." references
       the other entries are empty.
       If rdsize is 2, a second block exists and should be filled as well.
       */
    void grpFillRootDir(uint32_t itotal)
    {
        soProbe(606, "%s(%u)\n", __FUNCTION__, itotal);
        //binFillRootDir(itotal);
        SODirentry dir[DPB]; 

        //first entry
        strcpy(dir[0].name,".");
        dir[0].in=0;    //pointing to inode 0

        //second entry
        dir[1].in=0;    //pointing to inode 0
        strcpy(dir[1].name,"..");

        //all the others
        for(unsigned int i=2;i<DPB; i++){
            dir[i].in = 0x0;  //filled with pattern 0x0
            strcpy(dir[i].name,"");
        }

        soWriteRawBlock(1+(itotal/16),&dir);
    }
};

