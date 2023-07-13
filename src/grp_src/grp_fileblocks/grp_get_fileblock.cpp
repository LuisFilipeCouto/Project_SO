#include "fileblocks.h"

#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <errno.h>

namespace sofs20
{
    /* ********************************************************* */

    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t fbn);
    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t fbn);


    /* ********************************************************* */

    uint32_t grpGetFileBlock(int ih, uint32_t fbn)
    {
        soProbe(301, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        //soCheckInodeHandler(ih,__FUNCTION__);

        if(fbn<0 || fbn>(N_DIRECT+N_INDIRECT*RPB+N_DOUBLE_INDIRECT*RPB*RPB)){
            throw SOException(EINVAL,__FUNCTION__);
        }

        SOInode* ip = soGetInodePointer(ih);
        uint32_t res;

        if(fbn < N_DIRECT){
            res = ip->d[fbn];
        }else if( N_DIRECT<= fbn && fbn < N_DIRECT + N_INDIRECT * RPB ) {
            res = grpGetIndirectFileBlock(ip,fbn);
        }else{
            res = grpGetDoubleIndirectFileBlock(ip,fbn);
        }

        return res;
        /* replace the following line with your code 
        return binGetFileBlock(ih, fbn);*/
    }

    /* ********************************************************* */

    static uint32_t grpGetIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);
        
        uint32_t str[RPB];
        uint32_t i = (afbn-N_DIRECT)/RPB;
        soReadDataBlock(ip->i1[i], str);

        return str[afbn-N_DIRECT-i*RPB];
        /* replace the following two lines with your code */
        /* throw SOException(ENOSYS, __FUNCTION__); 
        return 0; */
    }


    /* ********************************************************* */

    static uint32_t grpGetDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(301, "%s(%d, ...)\n", __FUNCTION__, afbn);

        uint32_t str[RPB];
        uint32_t str2[RPB];
        uint32_t p = (afbn-N_INDIRECT*RPB-N_DIRECT)/(RPB*RPB);
        uint32_t i = (afbn-N_INDIRECT*RPB-N_DIRECT)/RPB;
        soReadDataBlock(ip->i2[p], str);
        soReadDataBlock(str[i], str2);

        return str2[afbn-N_INDIRECT*RPB-N_DIRECT-p*RPB*RPB-i*RPB];
        /* replace the following two lines with your code */
        /*throw SOException(ENOSYS, __FUNCTION__); 
        return 0; */
    }

};

