/****************************************************************************
 *
 * * ion_dat_viewer - an Ion Torrent Dat file viewer based on Qt
 *
 * * version: v0.1.1
 *
 * * author: mark doerr (mark.doerr@uni-greifswald.de), ben caller (bcaller {at} gmail)
 *
 * * date: 160521
 *
 * * Inspired by chip Qt4 example from Trolltech A/S.
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

#include "dat_loader.h"
#include "DatFile.h"
#include "dat_headers.h"
#include <iostream>
#include "byteswaputils.h"
#ifdef GNUC
#include <inttypes.h>
#define UINT32 uint32_t
#else
typedef unsigned int UINT32;
typedef unsigned int uint32_t;
#endif

#define SKIP 0xffffffff
#define CEILDIV(x,y) (x/y + (x % y != 0))
#define KEY_16_1 0xBB
#define KEY_STATE_CHANGE 0x7F

DatLoader::DatLoader(std::vector<double>& raw_data )
{    

    DatFile dat = DatFile("/path/to/acq_0010.dat");
    dat.GetHeader(this->df_header);
    std::cout << this->df_header.data_size << std::endl;
    std::cout << this->df_header.signature << std::endl;
    std::cout << this->df_header.struct_version << std::endl;
    dat.GetExperimentHeader(this->expt_header);
    std::cout << this->expt_header.x_region_size << std::endl;

    int offset = sizeof(struct dat_file_header) + sizeof(struct experiment_header);
    int wells = this->expt_header.rows * this->expt_header.cols;
    raw_data.reserve(wells * num_data_points);

    int x_regions = CEILDIV(this->expt_header.cols, this->expt_header.x_region_size);
    int y_regions = CEILDIV(this->expt_header.rows, this->expt_header.y_region_size);
    int n_regions = x_regions * y_regions;

    for(int frame_num = 0; frame_num < num_data_points; frame_num++) {
        std::cout << "Frame " << frame_num << std::endl;

        struct frame_header frame;
        dat.GetFrameHeader(offset, frame);
        std::cout << "Timestamp " << frame.timestamp << std::endl;

        if(frame_num == 0 && frame.Compressed == 0) {
            offset += 8;
            char * f = dat.GetFileData(offset, wells * 2);
            for(int i=0; i<wells; i++) {
                unsigned short val = * ( unsigned short * ) f;
                short unmasked = BYTE_SWAP_2 ( val ) & 0x3fff;
                double actual_point = (( double ) unmasked);
                raw_data.push_back(actual_point);
                for(int j = 1; j < numDataPoints(); j++) {
                    //Fill in array
                    raw_data.push_back(actual_point);
                }
                if(i < 5) {
                    std::cout << i << ": " << unmasked << std::endl;
                }
                f+=2;
            }
            offset += 2 * wells;
        } else if(frame_num > 0 && frame.Compressed) {
            offset += 6*4;
            if(frame.len > n_regions * 4) {
                //Read region offsets
                char * f = dat.GetFileData(offset, n_regions * 4);
                UINT32 max_region_offset = frame.len - n_regions * 4;
                UINT32 *reg_offsets = ( UINT32 * ) malloc ( n_regions*4 );
                memcpy( &reg_offsets[0], f, n_regions*4);
                UINT32 delta = 0;
                for(int region_id = 0; region_id<n_regions; region_id++) {

                    ByteSwap4(reg_offsets[region_id]);
                    if(delta==0 && reg_offsets[region_id] != SKIP)
                        delta = reg_offsets[region_id];

                    if(reg_offsets[region_id] != SKIP) {
                        if(reg_offsets[region_id] - delta > max_region_offset) {
                            std::cout << "Region diff beyond end of frame. Should mean skip!" << std::endl;
                            exit(44);
                        } else {
                            if(reg_offsets[region_id] < delta) {
                                std::cout << "negative region offset < 0 AFTER SUBTRACTING DELTA" << std::endl;
                                exit(44);
                            }
                            reg_offsets[region_id] -= delta;
                        }
                    }
                }
                // Go through each region
                f = dat.GetFileData(offset + n_regions*4, max_region_offset);
                int transitions_seen = 0;
                for(int yr=0; yr<y_regions; yr++) {
                    for(int xr=0; xr<x_regions; xr++) {
                        int region_id = yr * x_regions + xr;
                        uint32_t region_offset = reg_offsets[region_id];
                        if(region_offset != SKIP) {
                            char * signal = f+region_offset;
                            //std::cout << "region" << region_id << ": " << region_offset << std::endl;
                            int state = 0;
                            int x_size = ((xr + 1) * expt_header.x_region_size <= expt_header.cols)
                                    ? expt_header.x_region_size
                                    : expt_header.cols % expt_header.x_region_size;
                            int y_size = ((yr + 1) * expt_header.y_region_size <= expt_header.rows)
                                    ? expt_header.y_region_size
                                    : expt_header.rows % expt_header.y_region_size;
                            int x_start = xr * expt_header.x_region_size;
                            int y_start = yr * expt_header.y_region_size;
                            int x_end = x_start + x_size;
                            int y_end = y_start + y_size;
                            int x = x_start;
                            int y = y_start;
                            //                            std::cout << "reg" << x_start << y_start << " " << x_size << y_size << " " <<x_end<<std::endl;
                            // We read this region 8 wells at a time
                            int wells_in_region = CEILDIV(x_size*y_size, 8)*8;
                            for(int w=0;w<wells_in_region;w+=8) {
                                // State is how many data values combine for these 8 wells
                                if(*signal == KEY_STATE_CHANGE) {
                                    if((*(signal + 1) & 0x0f) == KEY_16_1)
                                        state = 16;
                                    else {
                                        state = *(signal + 1) & 0xf;
                                        state = state > 8 ? 16 : state;
                                    }
                                    transitions_seen++;
                                    signal += 2;
                                }
                                //Now de-interlace 8 values
                                short eight_values[8] = {0};

                                if(state == 3){
                                    eight_values[0] = (signal[0] >> 5) & 0x7;
                                    eight_values[1] = (signal[0] >> 2) & 0x7;
                                    eight_values[2] = ((signal[0] << 1) & 0x6) | ((signal[1] >> 7) & 1);
                                    eight_values[3] = ((signal[1] >> 4) & 0x7);
                                    eight_values[4] = ((signal[1] >> 1) & 0x7);
                                    eight_values[5] = ((signal[1] << 2) & 0x4) | ((signal[2] >> 6) & 3);
                                    eight_values[6] = ((signal[2] >> 3) & 0x7);
                                    eight_values[7] = ((signal[2]) & 0x7);
                                } else if(state == 4){
                                    eight_values[0] = (signal[0] >> 4) & 0xf;
                                    eight_values[1] = (signal[0]) & 0xf;
                                    eight_values[2] = (signal[1] >> 4) & 0xf;
                                    eight_values[3] = (signal[1]) & 0xf;
                                    eight_values[4] = (signal[2] >> 4) & 0xf;
                                    eight_values[5] = (signal[2]) & 0xf;
                                    eight_values[6] = (signal[3] >> 4) & 0xf;
                                    eight_values[7] = (signal[3]) & 0xf;
                                } else if(state == 5){
                                    eight_values[0] = (signal[0] >> 3) & 0x1f;
                                    eight_values[1] = ((signal[0] << 2) & 0x1c) | ((signal[1] >> 6) & 0x3);
                                    eight_values[2] = (signal[1] >> 1) & 0x1f;
                                    eight_values[3] = ((signal[1] << 4) & 0x10) | ((signal[2] >> 4) & 0xf);
                                    eight_values[4] = ((signal[2] << 1) & 0x1e) | ((signal[3] >> 7) & 0x1);
                                    eight_values[5] = (signal[3] >> 2) & 0x1f;
                                    eight_values[6] = ((signal[3] << 3) & 0x18) | ((signal[4] >> 5) & 0x7);
                                    eight_values[7] = (signal[4]) & 0x1f;
                                } else if(state == 6){
                                    eight_values[0] = (signal[0] >> 2) & 0x3f;
                                    eight_values[1] = ((signal[0] << 4) & 0x30) | ((signal[1] >> 4) & 0xf);
                                    eight_values[2] = ((signal[1] << 2) & 0x3c) | ((signal[2] >> 6) & 0x3);
                                    eight_values[3] = (signal[2] & 0x3f);
                                    eight_values[4] = (signal[3] >> 2) & 0x3f;
                                    eight_values[5] = ((signal[3] << 4) & 0x30) | ((signal[4] >> 4) & 0xf);
                                    eight_values[6] = ((signal[4] << 2) & 0x3c) | ((signal[5] >> 6) & 0x3);
                                    eight_values[7] = (signal[5] & 0x3f);
                                } else if(state == 7){
                                    eight_values[0] = (signal[0] >> 1) & 0x7f;
                                    eight_values[1] = ((signal[0] << 6) & 0x40) | ((signal[1] >> 2) & 0x3f);
                                    eight_values[2] = ((signal[1] << 5) & 0x60) | ((signal[2] >> 3) & 0x1f);
                                    eight_values[3] = ((signal[2] << 4) & 0x70) | ((signal[3] >> 4) & 0x0f);
                                    eight_values[4] = ((signal[3] << 3) & 0x78) | ((signal[4] >> 5) & 0x07);
                                    eight_values[5] = ((signal[4] << 2) & 0x7c) | ((signal[5] >> 6) & 0x3);
                                    eight_values[6] = ((signal[5] << 1) & 0x7e) | ((signal[6] >> 7) & 0x1);
                                    eight_values[7] = (signal[6] & 0x7f);
                                } else if(state == 8){
                                    //Signal has 8 values
                                    for(int v=0;v<8;v++)
                                        eight_values[v]=signal[v];
                                } else if(state == 16){
                                    eight_values[0] = (signal[0] << 8) | signal[1];
                                    eight_values[1] = (signal[2] << 8) | signal[3];
                                    eight_values[2] = (signal[4] << 8) | signal[5];
                                    eight_values[3] = (signal[6] << 8) | signal[7];
                                    eight_values[4] = (signal[8] << 8) | signal[9];
                                    eight_values[5] = (signal[10] << 8) | signal[11];
                                    eight_values[6] = (signal[12] << 8) | signal[13];
                                    eight_values[7] = (signal[14] << 8) | signal[15];
                                }
                                signal += state;

                                if(state != 16) {
                                    for (int v=0;v<8;v++ ) {
                                        eight_values[v] -= 1 << ( state-1 );
                                    }
                                }

                                for(int e=0; e<8; e++) {
                                    if(x >= x_end) {
                                        x = x_start;
                                        y++;
                                    }

                                    int index_for_frames = (y * expt_header.cols + x) * numDataPoints();

                                    //This is where we actually update the signal value
                                    raw_data[index_for_frames + frame_num] =
                                            eight_values[e] + raw_data[index_for_frames + frame_num - 1];

                                    //Propagate to later frames in case the well is ignored in some frames
                                    for(int fnum=frame_num+1; fnum<numDataPoints(); fnum++) {
                                        raw_data[index_for_frames + fnum] = raw_data[index_for_frames + fnum - 1];
                                    }

                                    x++;
                                }
                            }
                        }
                    }
                }
                free ( reg_offsets );
                if(transitions_seen!=frame.Transitions) {
                    std::cout << "Wrong number of transitions!! " << frame.Transitions << " but got " << transitions_seen << std::endl;
                    exit(44);
                }
            }

            offset += frame.len - 16;

        } else {
            std::cout << "Cannot read this frame (First frame should be uncompressed, rest should not be)" << std::endl;
            exit(44);
        }
    }

    // Just to test
    for(int x=0;x<numDataPoints();x++) {
        std::cout << raw_data[x] << std::endl;
    }
    int off = (722 * expt_header.cols + 720) * numDataPoints();
    for(int x=0;x<numDataPoints();x++) {
        std::cout << raw_data[off+x] << std::endl;
    }
    off = (616 * expt_header.cols + 616) * numDataPoints();
    for(int x=0;x<numDataPoints();x++) {
        std::cout << raw_data[off+x] << std::endl;
    }
    off = (1 * expt_header.cols + 2) * numDataPoints();
    for(int x=0;x<numDataPoints();x++) {
        std::cout << raw_data[off+x] << std::endl;
    }
}
