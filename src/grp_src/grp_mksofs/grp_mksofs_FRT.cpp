#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <string.h>

namespace sofs20
{
    void grpFillReferenceTable(uint32_t ntotal, uint32_t itotal, uint32_t dbtotal)
    {
        soProbe(605, "%s(%u, %u, %u)\n", __FUNCTION__, ntotal, itotal, dbtotal);

        /* replace the following line with your code */
        //binFillReferenceTable(ntotal, itotal, dbtotal);

        uint32_t rbtotal = ntotal - 1 - (itotal/16) - dbtotal;

        if(rbtotal > 0){
            uint32_t ref = REF_CACHE_SIZE + 1;

            uint32_t buf[RPB];

            for(uint32_t i = 0; i < rbtotal; i++){

                for(uint32_t j = 0; j < RPB; j++){
                     
                    if(ref > dbtotal-1){
                        buf[j] = BlockNullReference;
                    }
                    else{
                        buf[j] = ref;
                    }
                    ref++;
                }   

                soWriteRawBlock(1+itotal/IPB+dbtotal+i,buf);
            }
        }        
    }
};

