
#include "fnFS.h"

#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "compat_string.h"
#include "../../include/debug.h"


char * FileSystem::_make_fullpath(const char *path)
{
    if(path != nullptr)
    {
        // Build full path
        char *fullpath = (char *)malloc(MAX_PATHLEN);
        strlcpy(fullpath, _basepath, MAX_PATHLEN);
        // Make sure there's a '/' separating the base from the give path
        if(path[0] != '/')
        {
            int l = strlen(fullpath);
            fullpath[l] = '/';
            fullpath[l+1] = '\0';
        }
        strlcat(fullpath, path, MAX_PATHLEN);
        #ifdef DEBUG
        //Debug_printf("_make_fullpath \"%s\" -> \"%s\"\r\n", path, fullpath);
        #endif

        return fullpath;
    }
    return nullptr;
}


// Returns size of open file
long FileSystem::filesize(FILE *f)
{
    long curr = ftell(f);
    fseek(f, 0, SEEK_END);
    long end = ftell(f);
    fseek(f, curr, SEEK_SET);
    return end;
}


long FileSystem::filesize(FileHandler *fh)
{
    Debug_println("FileSystem::filesize from FileHandler");
    long curr = fh->tell();
    fh->seek(0, SEEK_END);
    long end = fh->tell();
    fh->seek(curr, SEEK_SET);
    return end;
}


// TODO - jk: implement per filesystem?
// Returns size of file given path
long FileSystem::filesize(const char *filepath)
{
    Debug_println("!!! TODO !!! FileSystem::filesize from filepath");
    struct stat fstat;
    if( 0 == stat(filepath, &fstat))
        return fstat.st_size;
    return -1;
}

const char * FileSystem::type_to_string(fsType type)
{
    switch(type)
    {
        case FSTYPE_SPIFFS:
            return "FS_SPIFFS";
        case FSTYPE_LITTLEFS:
            return "FS_LITTLEFS";
        case FSTYPE_SDFAT:
            return "FS_SDFAT";
        case FSTYPE_TNFS:
            return "FS_TNFS";
        case FSTYPE_SMB:
            return "FS_SMB";
        case FSTYPE_FTP:
            return "FS_FTP";
        default:
            return "UNKNOWN FS TYPE";
    }
}
