#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    void grpResetFreeDataBlocks(uint32_t ntotal, uint32_t itotal, uint32_t dbtotal)
    {
        soProbe(607, "%s(%u, %u, %u)\n", __FUNCTION__, ntotal, itotal, dbtotal);
        //binResetFreeDataBlocks(ntotal,itotal,dbtotal);
        uint32_t fill[BlockSize];

        for(unsigned int i = 0; i<BlockSize ; i++){    
            fill[i]=0x0;
        }
        for(uint32_t i=2+itotal/16; i<(dbtotal+1+itotal/16) ; i++){
            soWriteRawBlock(i,fill);
        }
    }
};

