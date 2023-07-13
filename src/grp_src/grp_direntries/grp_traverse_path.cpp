#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "direntries.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

namespace sofs20
{
    uint16_t grpTraversePath(char *path)
    {
        soProbe(221, "%s(%s)\n", __FUNCTION__, path);

        /* replace the following line with your code */
        //return binTraversePath(path);

        // get the inode associated to a given path.
        //the directory hierarchy of the file system is traversed to find an entry
        //whose name is the rightmost component of the given path

        //precondition:
        //path is an absolute path
        //path does not contain symbolic links -> checked when we see if it's a directory

        //throws ENOTDIR if one of the path components (except rightmost) isn't a 
        //directory
        //throws EACCES if the process that calls the operation does not have
        //traverse(x) permission on all components of the path, with exception
        //of the rightmost one

        //returns: the corresponding inode number

        if (strcmp(strndupa(path,1),"/")){
            throw SOException(EINVAL, __FUNCTION__);  //checking if it's a path
        }

        char* drname = dirname(strdupa(path));
        char* bsname = basename(strdupa(path));
        

        if( !strcmp(drname, "/")){
            if(! strcmp(drname,bsname))   //drname == bsname == '/', which means it's the root dir
                return 0;
        }
        int ih = soOpenInode(grpTraversePath(drname));
        SOInode* ip = soGetInodePointer(ih);

        if(! S_ISDIR(ip->mode))
            throw SOException(ENOTDIR, __FUNCTION__);

        if(! soCheckInodeAccess(ih, X_OK)){
            throw SOException(EACCES,__FUNCTION__);      //not accessible
        }
        uint32_t in = soGetDirentry(ih, bsname);
        soCloseInode(ih);
        if(in == InodeNullReference){
            throw SOException(ENOENT, __FUNCTION__);    //inode does not exist-> not an entry
        }
        return in;

    }
    
};