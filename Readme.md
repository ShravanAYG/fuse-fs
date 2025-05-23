# FUSE filesystem test
This is a test project for my upcomming project on fuse file system

# Building
``` bash
mkdir build
cd build
cmake ..
make
```

# Running
Create a mount point directory and mount the fs there
``` bash
mkdir mount
./my_fs mount
```

## Unmounting

``` bash
fusermount -u /full/path/to/mount
```

