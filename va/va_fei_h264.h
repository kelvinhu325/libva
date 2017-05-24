/*
 * Copyright (c) 2007-2011 Intel Corporation. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL INTEL AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * \file va_fei_h264.h
 * \brief The FEI encoding H264 special API
 */

#ifndef VA_FEI_H264_H
#define VA_FEI_H264_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "va_fei.h"

/** \brief FEI frame level control buffer for H.264 */
typedef struct _VAEncMiscParameterFEIFrameControlH264 
{
    uint32_t      function; /* one of the VAConfigAttribFEIFunctionType values */   
    /** \brief MB (16x16) control input buffer. It is valid only when (mb_input | mb_size_ctrl) 
     * is set to 1. The data in this buffer correspond to the input source. 16x16 MB is in raster scan order, 
     * each MB control data structure is defined by VAEncFEIMBControlH264. 
     * Buffer size shall not be less than the number of 16x16 blocks multiplied by 
     * sizeof(VAEncFEIMBControlH264). 
     * Note: if mb_qp is set, VAEncQpBufferH264 is expected.
     */
    VABufferID        mb_ctrl;
    /** \brief distortion output of MB ENC or ENC_PAK.
     * Each 16x16 block has one distortion data with VAEncFEIDistortionH264 layout
     * Buffer size shall not be less than the number of 16x16 blocks multiplied by 
     * sizeof(VAEncFEIDistortionH264). 
     */
    VABufferID        distortion;
    /** \brief MVs data output of MB ENC.
     * Each 16x16 block has one MVs data with layout VAMotionVector 
     * Buffer size shall not be less than the number of 16x16 blocks multiplied by 
     * sizeof(VAMotionVector) * 16. 
     */
    VABufferID        mv_data;
    /** \brief MBCode data output of MB ENC.
     * Each 16x16 block has one MB Code data with layout VAEncFEIMBCodeH264
     * Buffer size shall not be less than the number of 16x16 blocks multiplied by 
     * sizeof(VAEncFEIMBCodeH264). 
     */
    VABufferID        mb_code_data;
    /** \brief Qp input buffer with layout VAEncQpBufferH264. It is valid only when mb_qp is set to 1. 
     * The data in this buffer correspond to the input source. 
     * One Qp per 16x16 block in raster scan order, each Qp is a signed char (8-bit) value. 
     **/
    VABufferID        qp;
    /** \brief MV predictor. It is valid only when mv_predictor_enable is set to 1. 
     * Each 16x16 block has one or more pair of motion vectors and the corresponding 
     * reference indexes as defined by VAEncFEIMVPredictorH264. 16x16 block is in raster scan order. 
     * Buffer size shall not be less than the number of 16x16 blocks multiplied by 
     * sizeof(VAEncFEIMVPredictorH264). */
    VABufferID        mv_predictor;

    /** \brief number of MV predictors. It must not be greater than maximum supported MV predictor. */
    uint32_t      num_mv_predictors_l0      : 16;
    uint32_t      num_mv_predictors_l1      : 16;

    /** \brief control parameters */
    uint32_t      search_path               : 8;     
    uint32_t      len_sp                    : 8;     
    uint32_t      reserved0	                : 16;    

    uint32_t      sub_mb_part_mask          : 7;     
    uint32_t      intra_part_mask           : 5;     
    uint32_t      multi_pred_l0             : 1;     
    uint32_t      multi_pred_l1             : 1;     
    uint32_t      sub_pel_mode              : 2;     
    uint32_t      inter_sad 	            : 2;
    uint32_t      intra_sad                 : 2;     
    uint32_t      distortion_type           : 1;     
    uint32_t      repartition_check_enable  : 1;     
    uint32_t      adaptive_search           : 1;     
    uint32_t      mv_predictor_enable       : 1;     
    uint32_t      mb_qp                     : 1;     
    uint32_t      mb_input                  : 1;     
    uint32_t      mb_size_ctrl              : 1;
    uint32_t      colocated_mb_distortion   : 1;
    uint32_t      reserved1	                : 4;    

    uint32_t      ref_width                 : 8;     
    uint32_t      ref_height                : 8;
    uint32_t      search_window             : 4;     
    uint32_t      reserved2                 : 12;
    
    uint32_t      max_frame_size; 
    uint32_t      num_passes;     //number of QPs
    uint8_t       *delta_qp;      //list of detla QPs
    uint32_t      reserved3;      //add reserved , the structure size will be 72, 8byte alignment
} VAEncMiscParameterFEIFrameControlH264;

/** \brief FEI MB level control data structure */
typedef struct _VAEncFEIMBControlH264 
{
    /** \brief when set, correposndent MB is coded as intra */
    uint32_t force_to_intra                : 1;     
    /** \brief when set, correposndent MB is coded as skip */
    uint32_t force_to_skip                 : 1;     
    uint32_t force_to_nonskip              : 1;     
    uint32_t enable_direct_bias_adjustment : 1;
    uint32_t enable_motion_bias_adjustment : 1;
    uint32_t ext_mv_cost_scaling_factor    : 3;
    uint32_t reserved0                     : 24;

    uint32_t reserved1;    

    uint32_t reserved2;    

    /** \brief when mb_size_ctrl is set, size here is used to budget accumulatively. Set to 0xFF if don't care. */
    uint32_t reserved3                     : 16;    
    uint32_t target_size_in_word           : 8;     
    uint32_t max_size_in_word              : 8;     
} VAEncFEIMBControlH264;


/** \brief Application can use this definition as reference to allocate the buffer 
 * based on MaxNumPredictor returned from attribute VAConfigAttribFEIMVPredictors query. 
 **/
typedef struct _VAEncFEIMVPredictorH264 
{
    /** \brief Reference index corresponding to the entry of RefPicList0 & RefPicList1 in VAEncSliceParameterBufferH264. 
     * Note that RefPicList0 & RefPicList1 needs to be the same for all slices. 
     * ref_idx_l0_x : index to RefPicList0; ref_idx_l1_x : index to RefPicList1; x : 0 - MaxNumPredictor. 
     **/
    struct {
        uint8_t   ref_idx_l0    : 4;
        uint8_t   ref_idx_l1    : 4;
    } ref_idx[4]; /* index is predictor number */
    uint32_t reserved;
    /** \brief MV. MaxNumPredictor must be the returned value from attribute VAConfigAttribFEIMVPredictors query. 
     * Even application doesn't use the maximum predictors, the VAFEIMVPredictorH264 structure size 
     * has to be defined as maximum so each MB can be at a fixed location. 
     * Note that 0x8000 must be used for correspondent intra block. 
     **/
    VAMotionVector mv[4]; /* MaxNumPredictor is 4 */
} VAEncFEIMVPredictorH264;

/** \brief FEI output */
/**
 * Motion vector output is per 4x4 block. For each 4x4 block there is a pair of MVs 
 * for RefPicList0 and RefPicList1 and each MV is 4 bytes including horizontal and vertical directions. 
 * Depending on Subblock partition, for the shape that is not 4x4, the MV is replicated 
 * so each 4x4 block has a pair of MVs. The 16x16 block has 32 MVs (128 bytes). 
 * 0x8000 is used for correspondent intra block. The 16x16 block is in raster scan order, 
 * within the 16x16 block, each 4x4 block MV is ordered as below in memory. 
 * The buffer size shall be greater than or equal to the number of 16x16 blocks multiplied by 128 bytes. 
 * Note that, when separate ENC and PAK is enabled, the exact layout of this buffer is needed for PAK input. 
 * App can reuse this buffer, or copy to a different buffer as PAK input. 
 * Layout is defined as Generic motion vector data structure VAMotionVector
 *                      16x16 Block        
 *        -----------------------------------------
 *        |    1    |    2    |    5    |    6    |
 *        -----------------------------------------
 *        |    3    |    4    |    7    |    8    |
 *        -----------------------------------------
 *        |    9    |    10   |    13   |    14   |
 *        -----------------------------------------
 *        |    11   |    12   |    15   |    16   |
 *        -----------------------------------------
 **/

/** \brief VAEncFEIMBCodeH264 defines the data structure for VAEncFEIMBCodeBufferType per 16x16 MB block. 
 * it is output buffer of ENC and ENC_PAK modes, it's also input buffer of PAK mode.
 * The 16x16 block is in raster scan order. Buffer size shall not be less than the number of 16x16 blocks 
 * multiplied by sizeof(VAEncFEIMBCodeH264). Note that, when separate ENC and PAK is enabled, 
 * the exact layout of this buffer is needed for PAK input. App can reuse this buffer, 
 * or copy to a different buffer as PAK input, reserved elements must not be modified when used as PAK input. 
 **/
typedef struct _VAEncFEIMBCodeH264 
{
    //DWORD  0~2
    uint32_t    reserved0[3];

    //DWORD  3
    uint32_t    inter_mb_mode            : 2;
    uint32_t    mb_skip_flag             : 1;
    uint32_t    reserved1                : 1; 
    uint32_t    intra_mb_mode            : 2;
    uint32_t    reserved2                : 1; 
    uint32_t    field_mb_polarity_flag   : 1;
    uint32_t    mb_type                  : 5;
    uint32_t    intra_mb_flag	         : 1;
    uint32_t    field_mb_flag            : 1;
    uint32_t    transform8x8_flag        : 1;
    uint32_t    reserved3                : 1; 
    uint32_t    dc_block_coded_cr_flag   : 1;
    uint32_t    dc_block_coded_cb_flag   : 1;
    uint32_t    dc_block_coded_y_flag    : 1;
    uint32_t    reserved4                : 12; 

    //DWORD 4
    uint32_t    horz_origin              : 8;
    uint32_t    vert_origin              : 8;
    uint32_t    cbp_y                    : 16;

    //DWORD 5
    uint32_t    cbp_cb                   : 16;
    uint32_t    cbp_cr                   : 16;

    //DWORD 6
    uint32_t    qp_prime_y               : 8;
    uint32_t    reserved5                : 17; 
    uint32_t    mb_skip_conv_disable     : 1;
    uint32_t    is_last_mb               : 1;
    uint32_t    enable_coefficient_clamp : 1; 
    uint32_t    direct8x8_pattern        : 4;

    //DWORD 7 8 and 9
    union 
    {
        /* Intra MBs */
        struct 
        {
            uint32_t   luma_intra_pred_modes0 : 16;
            uint32_t   luma_intra_pred_modes1 : 16;

            uint32_t   luma_intra_pred_modes2 : 16;
            uint32_t   luma_intra_pred_modes3 : 16;

            uint32_t   chroma_intra_pred_mode : 2;
            uint32_t   intra_pred_avail_flag  : 5;
            uint32_t   intra_pred_avail_flagF : 1;
            uint32_t   reserved6              : 24; 
        } intra_mb;

        /* Inter MBs */
        struct 
        {
            uint32_t   sub_mb_shapes          : 8; 
            uint32_t   sub_mb_pred_modes      : 8;
            uint32_t   reserved7              : 16; 
 
            uint32_t   ref_idx_l0_0           : 8; 
            uint32_t   ref_idx_l0_1           : 8; 
            uint32_t   ref_idx_l0_2           : 8; 
            uint32_t   ref_idx_l0_3           : 8; 

            uint32_t   ref_idx_l1_0           : 8; 
            uint32_t   ref_idx_l1_1           : 8; 
            uint32_t   ref_idx_l1_2           : 8; 
            uint32_t   ref_idx_l1_3           : 8; 
        } inter_mb;
    } mb_mode;

    //DWORD 10
    uint32_t   reserved8                 : 16; 
    uint32_t   target_size_in_word       : 8;
    uint32_t   max_size_in_word          : 8; 

    //DWORD 11~14
    uint32_t   reserved9[4];
    
    //DWORD 15
    uint32_t   batchbuffer_end;   /* it is: is_last_mb ? BATCHBUFFER_END : 0 in cmodel */
} VAEncFEIMBCodeH264;        // 64 bytes

/** \brief VAEncFEIDistortionH264 defines the data structure for VAEncFEIDistortionBufferType per 16x16 MB block.
 * It is output buffer of ENC and ENC_PAK modes, The 16x16 block is in raster scan order. 
 * Buffer size shall not be less than the number of 16x16 blocks multiple by sizeof(VAEncFEIDistortionH264). 
 **/
typedef struct _VAEncFEIDistortionH264 {
    /** \brief Inter-prediction-distortion associated with motion vector i (co-located with subblock_4x4_i). 
     * Its meaning is determined by sub-shape. It must be zero if the corresponding sub-shape is not chosen. 
     **/
    uint16_t    inter_distortion[16];
    uint32_t    best_inter_distortion     : 16;
    uint32_t    best_intra_distortion     : 16;
    uint32_t    colocated_mb_distortion   : 16;
    uint32_t    reserved0                 : 16;
    uint32_t    reserved1[2];
} VAEncFEIDistortionH264;    // 48 bytes

/** \brief Motion Vector and Statistics frame level controls. 
 * VAStatsStatisticsParameterBufferType for H264 16x16 block
 **/
typedef struct _VAStatsStatisticsParameterH264 
{
   VAStatsStatisticsParameter stats_params;

    uint32_t    frame_qp                    : 8;     
    uint32_t    len_sp                      : 8;     
    uint32_t    search_path                 : 8;     
    uint32_t    reserved0                   : 8;

    uint32_t    sub_mb_part_mask            : 7;     
    uint32_t    sub_pel_mode                : 2;     
    uint32_t    inter_sad                   : 2;     
    uint32_t    intra_sad                   : 2;     
    uint32_t    adaptive_search	            : 1;     
    /** \brief indicate if future or/and past MV in mv_predictor buffer is valid. 
     * 0: MV predictor disabled
     * 1: MV predictor enabled for past reference
     * 2: MV predictor enabled for future reference
     * 3: MV predictor enabled for both past and future references
     **/
    uint32_t    mv_predictor_ctrl           : 3;     
    uint32_t    mb_qp                       : 1;     
    uint32_t    ft_enable                   : 1;
    /** \brief luma intra mode partition mask
     * xxxx1: luma_intra_16x16 disabled
     * xxx1x: luma_intra_8x8 disabled
     * xx1xx: luma_intra_4x4 disabled
     * xx111: intra prediction is disabled
     **/
    uint32_t    intra_part_mask             : 5;
    uint32_t    reserved1                   : 8;    

    uint32_t    ref_width                   : 8;     
    uint32_t    ref_height                  : 8;
    uint32_t    search_window               : 4;     
    uint32_t    reserved2                   : 12;    

    /** \brief MVOutput. When set to 1, MV output is NOT provided */
    uint32_t	disable_mv_output           : 1;    
    /** \brief StatisticsOutput. When set to 1, Statistics output is NOT provided. */
    uint32_t    disable_statistics_output   : 1;    
    uint32_t    enable_8x8_statistics       : 1;
    uint32_t    reserved3                   : 29;
} VAStatsStatisticsParameterH264;

/** \brief VAStatsStatisticsH264. H264 Statistics buffer layout for VAStatsStatisticsBufferType 
 * and VAStatsStatisticsBottomFieldBufferType(for interlaced only). 
 * Statistics output is per 16x16 block. Data structure per 16x16 block is defined below. 
 * The 16x16 block is in raster scan order. The buffer size shall be greater than or equal to 
 * the number of 16x16 blocks multiplied by sizeof(VAStatsStatisticsH264). 
 **/
typedef struct _VAStatsStatisticsH264 
{
    /** \brief past reference  */
    uint32_t    best_inter_distortion0 : 16;     
    uint32_t    inter_mode0            : 16;     

    /** \brief future reference  */
    uint32_t    best_inter_distortion1 : 16;     
    uint32_t    inter_mode1            : 16;     

    uint32_t    best_intra_distortion  : 16;     
    uint32_t    intra_mode             : 16;     

    uint32_t    num_non_zero_coef      : 16;     
    uint32_t    reserved0              : 16;     

    uint32_t    sum_coef; 

    /** \brief DWORD 5 flat info **/
    uint32_t    mb_is_flat             : 1;     
    uint32_t    reserved1              : 31;     

    /** \brief DWORD 6 variance for block16x16**/
    uint32_t    variance_16x16;
    /** \brief DWORD 7 ~ 10, variance for block8x8 **/
    uint32_t    variance_8x8[4];
    
    /** \brief DWORD 11 pixel_average for block16x16 **/
    uint32_t    pixel_average_16x16;
    /** \brief DWORD 12 ~ 15, pixel_average for block8x8 **/
    uint32_t    pixel_average_8x8[4];
} VAStatsStatisticsH264;  // 64 bytes


#ifdef __cplusplus
}
#endif

#endif /* VA_FEI_H264_H */
