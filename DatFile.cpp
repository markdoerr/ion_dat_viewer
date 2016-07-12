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

#include "DatFile.h"
#include <iostream>
#ifndef WIN32
#include <pthread.h>
#include <unistd.h>  // for sysconf ()
#include <sys/mman.h>
#endif

#include "byteswaputils.h"

#define DEAD_BEEF 0xdeadbeef

DatFile::DatFile( FNAME fname )
{
  MY_FILE_HANDLE rc;

#ifndef WIN32
  rc = open ( fname, O_RDONLY );
  if ( rc<0 )
  {
    std::cout << "failed to open file " << fname << std::endl;
    exit(66);
  }
#else

  rc = CreateFile( fname, GENERIC_READ, 0, NULL,
                    OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
  if ( rc == INVALID_HANDLE_VALUE )
  {
    std::cout << "failed to open file " << fname << std::endl;
    exit(66);
  }
#endif
  else
  {
      this->PageSize = 4096;
      this->hFile = rc;

    #ifdef WIN32
      int dwFileSize;
      SYSTEM_INFO SysInfo; // system information; used to get granularity

      // Get the system allocation granularity.
      GetSystemInfo ( &SysInfo );
      this->PageSize = SysInfo.dwAllocationGranularity;
      this->fileLen = dwFileSize = GetFileSize ( this->hFile, NULL );
      // Create a file mapping object for the file
      // Note that it is a good idea to ensure the file size is not zero
      this->mFile = CreateFileMapping ( this->hFile, // current file handle
                                      NULL, // default security
                                      PAGE_READONLY, // read/write permission
                                      0, // size of mapping object, high
                                      dwFileSize, // size of mapping object, low
                                      NULL ); // name of mapping object
    #else
      struct stat statbuf;
      fstat ( rc,&statbuf );
      this->fileLen = statbuf.st_size;
    #endif
  }
}

DatFile::~DatFile()
{
    this->FreeFileData();
#ifndef WIN32
  close ( this->hFile );
#else
  CloseHandle ( this->mFile );
  CloseHandle ( this->hFile );
#endif
  std::cout << "Dat file closed" << std::endl;
}

char * DatFile::GetFileData (int offset, int len)
{
  /*if(this->CurrentAllocLen && this->CurrentOffset <= offset && this->CurrentOffset + this->CurrentAllocLen >= offset + len) {
      std::cout << "Reusing pre-allocated memory" << std::endl;
      return this->CurrentAllocPtr + ( this->CurrentOffset % this->PageSize ) +
              (offset - this->CurrentOffset); // new offset
  }*/
  this->FreeFileData();

  if ( ( offset + len ) > this->fileLen ) {
    std::cout << "File length error" << std::endl;
    return NULL;
  }

  this->CurrentAllocLen = len + ( offset % this->PageSize );
  if(this->CurrentAllocLen < this->PageSize && offset + this->PageSize <= this->fileLen)
      this->CurrentAllocLen = this->PageSize;

#ifndef WIN32
  this->CurrentAllocPtr = ( char * ) mmap ( 0,this->CurrentAllocLen,PROT_READ,MAP_PRIVATE,this->hFile, ( offset - ( offset % 4096 ) ) );
#else
  this->CurrentAllocPtr = ( char * ) MapViewOfFile ( this->mFile, FILE_MAP_READ, 0, ( offset
                        - ( offset % this->PageSize ) ), this->CurrentAllocLen );
#endif
  if ( this->CurrentAllocPtr == NULL ) {
    this->FreeFileData();
    std::cout << "Pointer was NULL" << std::endl;
    return NULL;
  }

  this->CurrentOffset = offset;
  return this->CurrentAllocPtr + ( offset % this->PageSize );
}

void DatFile::FreeFileData()
{
    if(this->CurrentAllocLen)
    {
      std::cout << "Freeing allocated memory" << std::endl;
    #ifndef WIN32
      munmap ( this->CurrentAllocPtr,this->CurrentAllocLen );
    #else
      UnmapViewOfFile ( this->CurrentAllocPtr );
    #endif
      this->CurrentAllocPtr = NULL;
      this->CurrentAllocLen = 0;
      this->CurrentOffset = 0;
    }
}

void DatFile::GetHeader(struct dat_file_header &hdr)
{
    char* f = this->GetFileData(0, sizeof(struct dat_file_header));
    memcpy (&hdr, f, sizeof ( struct dat_file_header ) );
    ByteSwap4 ( hdr.signature );
    ByteSwap4 ( hdr.struct_version );
    ByteSwap4 ( hdr.header_size );
    ByteSwap4 ( hdr.data_size );
    if(hdr.signature != DEAD_BEEF) {
        std::cout << "Bad dat file header signature" << std::endl;
        exit(55);
    }
    if(hdr.struct_version != 4) {
        std::cout << "This program currently only works with version 4 of the dat file" << std::endl;
        exit(55);
    }

}

void DatFile::GetExperimentHeader(struct experiment_header &hdr)
{
    char* f = this->GetFileData(sizeof(struct dat_file_header), sizeof(struct experiment_header));
    memcpy ( &hdr, f, sizeof ( hdr ) );
    ByteSwap4(hdr.first_frame_time);
    ByteSwap2( hdr.rows );
    ByteSwap2 ( hdr.cols );
    ByteSwap2( hdr.x_region_size );
    ByteSwap2( hdr.y_region_size );
    ByteSwap2( hdr.frames_in_file );
    ByteSwap2( hdr.uncomp_frames_in_file );
    ByteSwap2( hdr.interlaceType );

    if(hdr.uncomp_frames_in_file < hdr.frames_in_file || hdr.uncomp_frames_in_file >= hdr.frames_in_file * 4) {
        std::cout << "Unknown compression" << std::endl;
        exit(55);
    }
}

void DatFile::GetFrameHeader(int offset, struct frame_header &hdr)
{
    char* f = this->GetFileData(offset, sizeof(struct frame_header));
    memcpy (&hdr, f, sizeof ( struct frame_header ) );
    ByteSwap4 ( hdr.timestamp );
    ByteSwap4 ( hdr.Compressed );

    if(hdr.Compressed == 1) {

        ByteSwap4 ( hdr.len );
        ByteSwap4 ( hdr.sentinel );
        ByteSwap4 ( hdr.total );
        ByteSwap4 ( hdr.Transitions );
        if(hdr.sentinel != DEAD_BEEF) {
            std::cout << "Bad frame sentinel" << std::endl;
            exit(55);
        }

    } else if(hdr.Compressed > 1) {
        std::cout << "Compression level not implemented" << std::endl;
        exit(55);
    }
}
