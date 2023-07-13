#include "grp_mksofs.h"

#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <iostream>
#include <math.h> 

namespace sofs20
{
    void grpComputeStructure(uint32_t ntotal, uint32_t & itotal, uint32_t & dbtotal)
    {
        soProbe(601, "%s(%u, %u, ...)\n", __FUNCTION__, ntotal, itotal);

        /* replace the following line with your code */
      // binComputeStructure(ntotal, itotal, dbtotal);
 
    
       if(itotal == 0){
           itotal = ntotal/16;
        }

        if(itotal > ceil(ntotal/8)){
           itotal = ntotal/16;
        }

        while(itotal < IPB){
            itotal++;
        }
        while(itotal < 32){
            itotal++;
        }

        while(itotal % IPB != 0 || itotal % 32 != 0){
           itotal++;
        }
        
        
        uint32_t inode_blocks = itotal / IPB;

        uint32_t numblocosprovisorio = ntotal - 1 - inode_blocks;
        
        uint32_t DBreferences = numblocosprovisorio - 1;

        int32_t RTreferences = DBreferences - REF_CACHE_SIZE;

        float reftable_blocks;

        if(RTreferences < 0){
            reftable_blocks = 0;
            dbtotal = numblocosprovisorio;
        }
        else{
            reftable_blocks = ceil((float)RTreferences / (float)RPB);
            dbtotal = ntotal - 1 - inode_blocks - reftable_blocks;
            RTreferences = dbtotal - 1 - REF_CACHE_SIZE;
            float divisao = ceil((float)RTreferences / (float)RPB);
            if(divisao < reftable_blocks){
                reftable_blocks--;
                dbtotal++;
            }
        }
        
 
    }
};

