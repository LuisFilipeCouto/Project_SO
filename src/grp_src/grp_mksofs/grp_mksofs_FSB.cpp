#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    void grpFillSuperblock(const char *name, uint32_t ntotal, uint32_t itotal, uint32_t dbtotal)
    {
        soProbe(602, "%s(%s, %u, %u, %u)\n", __FUNCTION__, name, ntotal, itotal, dbtotal);

        /* replace the following line with your code */
        //binFillSuperblock(name, ntotal, itotal, dbtotal);

        SOSuperblock super_block;

        super_block.magic = 0xFFFF;

        super_block.version = VERSION_NUMBER;

        super_block.mntstat = 0;

        strncpy(super_block.name, name, PARTITION_NAME_LEN); // Copies the first PARTITION_NAME_LEN characters of name to super_block.name

        super_block.ntotal = ntotal;

        super_block.itotal = itotal;

        super_block.ifree = (itotal - 1); 

        super_block.iidx = 0;


        uint32_t *map = super_block.ibitmap;

        for(unsigned int i = 0; i < (MAX_INODES/32); i++){

            memset((map+i), 0, 4); // Set all bits of all words in ibitmap to 0

        }
        super_block.ibitmap[0] |= (1 << 0); // Change bit 0 of ibitmap[0] to 1


        super_block.dbp_start = (itotal/IPB) + 1; 

        super_block.dbtotal = dbtotal;

        super_block.dbfree = super_block.dbtotal - 1;

        super_block.rt_start = super_block.dbp_start + dbtotal;

        super_block.rt_size = super_block.ntotal - super_block.rt_start;

        if(super_block.dbfree > REF_CACHE_SIZE){
            super_block.reftable.blk_idx = 0;
            super_block.reftable.ref_idx = 0;
            super_block.reftable.count = super_block.dbtotal - (REF_CACHE_SIZE % RPB) - 1;
        }
        else{
            super_block.reftable.blk_idx = 0;
            super_block.reftable.ref_idx = 0;
            super_block.reftable.count = 0;
        }
        
        if(super_block.dbfree > REF_CACHE_SIZE){
            super_block.retrieval_cache.idx = 0;
        } 
        else {
            super_block.retrieval_cache.idx = REF_CACHE_SIZE - super_block.dbfree;
        }

        super_block.insertion_cache.idx = 0;
        uint32_t val = 1;

        for(unsigned int i = 0; i < REF_CACHE_SIZE; i++){

            super_block.insertion_cache.ref[i] = BlockNullReference; // Insertion cache is left empty

            if(i < super_block.retrieval_cache.idx){
                super_block.retrieval_cache.ref[i] = BlockNullReference;
            } 

            else {
                super_block.retrieval_cache.ref[i] = val;
                val++;
            }
        }
        soWriteRawBlock(0, &super_block);
    }
};
