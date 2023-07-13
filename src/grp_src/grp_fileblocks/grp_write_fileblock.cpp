#include "fileblocks.h"

#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <string.h>
#include <inttypes.h>

namespace sofs20
{
    void grpWriteFileBlock(int ih, uint32_t fbn, void *buf)
    {
        soProbe(332, "%s(%d, %u, %p)\n", __FUNCTION__, ih, fbn, buf);

        /* replace the following line with your code */
        //binWriteFileBlock(ih, fbn, buf);
        //write a file block
        //precondition: ih is a valid inode handler
        //data is written into specific inode
        //if the reference block hasn't been allocated, alloc it and write data

        uint32_t block =  soGetFileBlock(ih, fbn);
        if(block == BlockNullReference){
            soAllocFileBlock(ih,fbn);
        }
        soWriteDataBlock(block, buf);
    }
};

