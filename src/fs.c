#ifndef CCONSTRUCT_FS_C
#define CCONSTRUCT_FS_C

//! fs.c: Provides Filesystem related functions, such as cc_create_file and cc_dir_exists.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#  include <windows.h>
#  include <io.h>
#  include <direct.h>
#  define mkdir(PATH, _) _mkdir(PATH)
#  define F_OK 0
#  define access _access
#  define _CRT_INTERNAL_NONSTDC_NAMES 1
#  if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
#    define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#  endif
#  if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
#    define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#  endif
#else
#  include <unistd.h>
#endif

/*************************************************************************/
// Existence Checking Functions //

/// Returns: 1 if path exists, else 0
bool cc_file_exists(char* path) {
    struct stat s;
    int err = stat(path, &s);
    if (err == -1) return false;
    return S_ISREG(s.st_mode);
}
/// Returns: 0 if exists, else -1, on error: errno is returned
errno_t cc_file_exists_safe(char* path) {
    struct stat s;
    int err = stat(path, &s);
    if (err == -1) {
        if (errno == ENOENT) return -1;
        else return errno;
    }
    return S_ISREG(s.st_mode) - 1;
}

/// Returns: 1 if dir exists, else 0
bool cc_dir_exists(char* path) {
    struct stat s;
    int err = stat(path, &s);
    if (err == -1) return false; // ignore error (use cc_dir_exists_safe)
    return S_ISDIR(s.st_mode);
}
/// Returns: 0 if exists, -1 if not exists, else return errno
errno_t cc_dir_exists_safe(char* path) {
    struct stat s;
    int err = stat(path, &s);
    if (err == -1) {
        if (err == ENOENT) return -1;
        else return errno;
    }
    if (S_ISDIR(s.st_mode)) return 0;
    else return EISDIR;
}

/// Return whether the path points to a valid entity
bool cc_path_exists(char* path) {
    struct stat _s;
    return stat(path, &_s) != -1;
}

/*************************************************************************/
// File Creation Functions //

/// Create a file and return whether creating succeeded.
bool cc_file_create(char* path) {
    FILE* fp = fopen(path, "a");
    if (fp == NULL) return false;
    fclose(fp);
    return true;
}
/// Create a file and return 0 if succeeded, else errno
errno_t cc_file_create_safe(char* path) {
    if (cc_file_exists(path)) return EEXIST;
    if (cc_dir_exists(path)) return EISDIR;
    FILE* fp = fopen(path, "a");
    if (fp == NULL) return errno;
    fclose(fp);
    return 0;
}

/*************************************************************************/
// Dir Creation Functions //

#ifndef CCONSTRUCT_DEFAULT_MODE
#  define CCONSTRUCT_DEFAULT_MODE 0770
#endif 

/// Create a Directory and return whether it succeeded
bool cc_dir_create(char* path) {
    return mkdir(path, CCONSTRUCT_DEFAULT_MODE) == 0;
}
/// Create a Directory with permissions and return whether it succeeded
bool cc_dir_create_with_mode(char* path, int mode) {
    return mkdir(path, mode) == 0;
}
/// Create a Directory and return 0 if it succeeded, else errno
errno_t cc_dir_create_safe(char* path) {
    return mkdir(path, CCONSTRUCT_DEFAULT_MODE);
}
/// Create a Directory with permissions and return whether it succeeded
bool cc_dir_create_with_mode_safe(char* path, int mode) {
    return mkdir(path, mode);
}

/*************************************************************************/
// Reading Functions //

/// Read contents of file, return NULL if failed.
/// NOTE: result must be freed.
char* cc_file_read(char* path) {
    char* buf = NULL;
    long size;
    FILE* fp = fopen(path, "rb"); // Open the file in binary mode
    if (fp == NULL) return NULL;

    // Move to the end of the file to get the size
    if (fseek(fp, 0, SEEK_END) != 0) goto defer;
    size = ftell(fp);
    if (size < 0) goto defer; // Handle ftell error
    rewind(fp); // Move back to the start of the file

    buf = (char*)malloc(sizeof(char) * (size + 1));
    if (buf == NULL) goto defer;

    size_t read_bytes = fread(buf, 1, size, fp);
    buf[size] = '\0'; // Null-terminate the buffer

    if (read_bytes != size) {
        free(buf);
        buf = NULL;
    }

defer:
    fclose(fp);
    return buf;
}

/// Read contents of file, return NULL if failed, and set error
/// NOTE: result must be freed.
char* cc_file_read_safe(char* path, errno_t* error) {
    char* buf = NULL;
    long size;
    FILE* fp = fopen(path, "rb"); // Open the file in binary mode
    if (fp == NULL) goto error;

    // Move to the end of the file to get the size
    if (fseek(fp, 0, SEEK_END) != 0) goto error;
    size = ftell(fp);
    if (size < 0) goto error;
    rewind(fp); // Move back to the start of the file

    buf = (char*)malloc(sizeof(char) * (size + 1));
    if (buf == NULL) goto error;

    size_t read_bytes = fread(buf, 1, size, fp);
    buf[size] = '\0'; // Null-terminate the buffer

    if (read_bytes != size) goto error;

close:
    fclose(fp);
    *error = 0;
    return buf;

error:
    fclose(fp);
    *error = errno;
    if (buf) free(buf);
    return NULL;
}

#endif // CCONSTRUCT_FS_C