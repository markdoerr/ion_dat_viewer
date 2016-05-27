/****************************************************************************
 *
 * * ion_dat_viewer - an Ion Torrent Dat file viewer based on Qt
 *
 * * author: ben caller (bcaller {at} gmail)
 *
 * * date: 160525
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 ***************************************************************************/

#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <memory.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "dat_headers.h"

#ifndef WIN32
#define MY_FILE_HANDLE  int
#define FNAME char*
#else
#include <windows.h>
#define MY_FILE_HANDLE HANDLE
#define FNAME LPWSTR
#endif

class DatFile
{
public:
    DatFile(FNAME fname);
    char *GetFileData (int offset, int len);
    void GetHeader(struct dat_file_header &hdr);
    void GetExperimentHeader(struct experiment_header &hdr);
    void GetFrameHeader(int offset, struct frame_header &hdr);
    ~DatFile();

private:
    char *CurrentAllocPtr = NULL;
    int CurrentAllocLen = 0;
    int PageSize;
    int CurrentOffset = 0;
    int fileLen;
    MY_FILE_HANDLE hFile;
  #ifdef WIN32
    MY_FILE_HANDLE mFile;
  #endif
    void FreeFileData();
};



#endif // FILE_HANDLING_H
