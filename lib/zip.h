#ifndef _HAD_ZIP_H
#define _HAD_ZIP_H

/*
  $NiH: zip.h,v 1.35.4.5 2004/04/08 16:53:07 dillo Exp $

  zip.h -- exported declarations.
  Copyright (C) 1999, 2003 Dieter Baron and Thomas Klausner

  This file is part of libzip, a library to manipulate ZIP archives.
  The authors can be contacted at <nih@giga.or.at>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
  3. The names of the authors may not be used to endorse or promote
     products derived from this software without specific prior
     written permission.
 
  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#include <sys/types.h>
#include <stdio.h>
#include <zlib.h>
#include <time.h>

/* flags for zip_open */

#define ZIP_CREATE           1
#define ZIP_EXCL             2
#define ZIP_CHECKCONS        4


/* flags for zip_name_locate, zip_fopen, zip_stat, ... */

#define ZIP_FL_NOCASE		1 /* ignore case on name lookup */
#define ZIP_FL_NODIR		2 /* ignore directory component */
#define ZIP_FL_COMPRESSED	4 /* read compressed data (XXX: rename) */
#define ZIP_FL_UNCHANGED	8 /* use original data, ignoring changes */

/* flags for zip_add and zip_replace */

#define ZIP_CH_ISCOMP	1	/* data returned by ch_func is compressed */

/* libzip error codes */

#define ZERR_OK               0  /* N No error */
#define ZERR_MULTIDISK        1  /* N Multi-disk zip archives not supported */
#define ZERR_RENAME           2  /* S Renaming temporary file failed */
#define ZERR_CLOSE            3  /* S Closing zip archive failed */
#define ZERR_SEEK             4  /* S Seek error */
#define ZERR_READ             5  /* S Read error */
#define ZERR_WRITE            6  /* S Write error */
#define ZERR_CRC              7  /* N CRC error */
#define ZERR_ZIPCLOSED        8  /* N Containing zip archive was closed */
#define ZERR_NOENT            9  /* N No such file */
#define ZERR_EXISTS          10  /* N File already exists */
#define ZERR_OPEN            11  /* S Can't open file */
#define ZERR_TMPOPEN         12  /* S Failure to create temporary file */
#define ZERR_ZLIB            13  /* Z Zlib error */
#define ZERR_MEMORY          14  /* N Malloc failure */
#define ZERR_CHANGED         15  /* N Entry has been changed */
#define ZERR_COMPNOTSUPP     16  /* N Compression method not supported */
#define ZERR_EOF             17  /* N Premature EOF */
#define ZERR_INVAL           18  /* N Invalid argument */
#define ZERR_NOZIP           19  /* N Not a zip archive */
#define ZERR_INTERNAL        20  /* N Internal error */
#define ZERR_INCONS	     21  /* N Zip archive inconsistent */


/* type of system error value */

#define ZIP_ET_NONE	      0  /* sys_err unused */
#define ZIP_ET_SYS	      1  /* sys_err is errno */
#define ZIP_ET_ZIP	      2  /* sys_err is zlib error code */

/* compression methods */

#define ZIP_CM_DEFAULT	      -1  /* better of deflate or store */
#define ZIP_CM_STORE	       0  /* stored (uncompressed) */
#define ZIP_CM_SHRINK	       1  /* shrunk */
#define ZIP_CM_REDUCE_1	       2  /* reduced with factor 1 */
#define ZIP_CM_REDUCE_2	       3  /* reduced with factor 2 */
#define ZIP_CM_REDUCE_3	       4  /* reduced with factor 3 */
#define ZIP_CM_REDUCE_4	       5  /* reduced with factor 4 */
#define ZIP_CM_IMPLODE	       6  /* imploded */
#define ZIP_CM_DEFLATE	       8  /* deflated */
#define ZIP_CM_DEFLATE64       9  /* deflate64 */
#define ZIP_CM_PKWARE_IMPLODE 10  /* PKWARE imploding */



enum zip_cmd {
    ZIP_CMD_INIT,	/* prepare for reading */
    ZIP_CMD_READ, 	/* read data */
    ZIP_CMD_CLOSE,	/* close and cleanup */
    ZIP_CMD_STAT	/* for compressed data */
};

typedef ssize_t (*zip_read_func)(void *state, void *data,
				 size_t len, enum zip_cmd cmd);

struct zip_stat {
    const char *name;			/* name of the file */
    int index;				/* index within archive */
    unsigned int crc;			/* crc of file data */
    time_t mtime;			/* modification time */
    off_t size;				/* size of file (uncompressed) */
    off_t comp_size;			/* size of file (compressed) */
    unsigned short comp_method;		/* compression method used */
};

struct zip;
struct zip_file;



int zip_add(struct zip *, const char *, zip_read_func, void *, int);
int zip_add_data(struct zip *, const char *, const void *, off_t, int);
int zip_add_file(struct zip *, const char *, const char *, off_t, off_t);
int zip_add_filep(struct zip *, const char *, FILE *, off_t, off_t);
int zip_add_zip(struct zip *, const char *, struct zip *, int, int,
		off_t, off_t);
int zip_close(struct zip *);
int zip_delete(struct zip *, int);
int zip_error_str(char *, size_t, int, int);
int zip_error_sys_type(int);
int zip_fclose(struct zip_file *);
void zip_file_get_error(struct zip_file *, int *, int *);
const char *zip_file_strerror(struct zip_file *);
struct zip_file *zip_fopen(struct zip *, const char *, int);
struct zip_file *zip_fopen_index(struct zip *, int, int);
ssize_t zip_fread(struct zip_file *, void *, size_t);
void zip_get_error(struct zip *, int *, int *);
const char *zip_get_name(struct zip *, int);
int zip_get_num_files(struct zip *);
int zip_name_locate(struct zip *, const char *, int);
struct zip *zip_open(const char *, int, int *);
int zip_rename(struct zip *, int, const char *);
int zip_replace(struct zip *, int, zip_read_func, void *, int);
int zip_replace_data(struct zip *, int, const void *, off_t, int);
int zip_replace_file(struct zip *, int, const char *, off_t, off_t);
int zip_replace_filep(struct zip *, int, FILE *, off_t, off_t);
int zip_replace_zip(struct zip *, int, struct zip *, int, int, off_t, off_t);
int zip_stat(struct zip *, const char *, int, struct zip_stat *);
int zip_stat_index(struct zip *, int, int, struct zip_stat *);
const char *zip_strerror(struct zip *);
int zip_unchange(struct zip *, int);
int zip_unchange_all(struct zip *);

#endif /* _HAD_ZIP_H */
