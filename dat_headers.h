#ifndef DAT_HEADERS_H
#define DAT_HEADERS_H

#ifdef GNUC
#include <inttypes.h>
#define UINT32 uint32_t
#define UINT16 uint16_t
#else
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
#endif

struct dat_file_header
{
    UINT32 signature;
    UINT32 struct_version;
    UINT32 header_size;
    UINT32 data_size;
};

struct experiment_header
{
    UINT32 first_frame_time;  /* "seconds since 1970" format - walltime */
    UINT16 rows;
    UINT16 cols;
    UINT16 x_region_size;
    UINT16 y_region_size;
    UINT16 frames_in_file;
    UINT16 uncomp_frames_in_file;
    UINT32 sample_rate;
#define DEVICE_MAX_CHANS 4
    UINT16 channel_offset[DEVICE_MAX_CHANS];
    UINT16 hw_interlace_type; // for un-doing deinterlace if needed
    UINT16 interlaceType;  // set to 5 for now
};

struct frame_header
{
    UINT32 timestamp;
    UINT32 Compressed;
    UINT32 len;
    UINT32 Transitions;
    UINT32 total;
    UINT32 sentinel;
};

#undef UINT32
#undef UINT16

#endif // DAT_HEADERS_H
