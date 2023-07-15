# Project_SO

### About the project 
This project involves the development of a simple file system based on the ext2 file system <br>
The most important source code can be found inside [src/grp_src](src/grp_src) <p>
For a comprehensive project analysis, please consult the accompanying [documentation](doc/sofs20.pdf), which provides a complete detailed description of the file system developed

### Detailed description/usage 
On Ubuntu you need the following packages installed: 
_build-essential_, _cmake_, _doxygen_, _libfuse-dev_, and _git_

```
sudo apt install build-essential cmake doxygen libfuse-dev git
```

To generate and open doxygen documentation, assuming you are in the base directory:
```
cd doc
doxygen
firefox html/index.html &
```

To compile the source code, assuming you are in the base directory:
```
cd build
cmake ../src
make
```

To mount a **sofs** file system and test it, assuming you are in the base directory and have already compiled the code:
```
cd bin
./createDisk.sh /tmp/dsk 1000   # /tmp/dsk will be a disk with 1000 blocks
./mksofs /tmp/dsk               # format the disk as a sofs20 file system
mkdir /tmp/mnt                  # our mount point
./sofsmount /tmp/dsk /tmp/mnt   # mount the disk in the mount point
./showblock /tmp/dsk            # show details about the disk
```

There are also additional test scripts in the base directory that you can use
