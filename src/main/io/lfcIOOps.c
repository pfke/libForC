#include "lfcIOOps.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

int lfcIOOps_copyFile(
    const char *src,
    const char *dst
) {
    int input, output;
    if ((input = open(src, O_RDONLY)) == -1)
    {
        return -1;
    }
    if ((output = creat(dst, 0660)) == -1)
    {
        close(input);
        return -1;
    }

    //Here we use kernel-space copying for performance reasons
#if defined(__APPLE__) || defined(__FreeBSD__)
    //fcopyfile works on FreeBSD and OS X 10.5+
    int result = fcopyfile(input, output, 0, COPYFILE_ALL);
#else
    //sendfile will work with non-socket output (i.e. regular file) on Linux 2.6.33+
    off_t bytesCopied = 0;
    struct stat fileinfo = {0};
    fstat(input, &fileinfo);
    ssize_t result = sendfile(output, input, &bytesCopied, (size_t)fileinfo.st_size);
#endif

    close(input);
    close(output);

    return result == 0;
}

