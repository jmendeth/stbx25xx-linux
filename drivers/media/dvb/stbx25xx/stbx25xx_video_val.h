/*
 * This file is part of linux driver the digital TV devices equipped with IBM STBx25xx SoC
 *
 * stbx25xx_video_val.h - header file for video hardware.
 *
 * see stbx25xx.c for copyright information.
 */

#ifndef __STBx25xx_VIDEO_VAL_H__
#define __STBx25xx_VIDEO_VAL_H__

#if defined(__LITTLE_ENDIAN)
	#error Something went wrong, because STBx25xx is Big Endian only and endianess is set to Little Endian!
#elseif !defined(__BIG_ENDIAN)
	#error Something went wrong, because no endianess is defined!
#endif

typedef union {
	u32 raw;
	
	struct {
		unsigned	:8;
		unsigned key	:2;
		unsigned	:2;
		unsigned video	:1;
		unsigned	:1;
		unsigned vidosd	:1;
		unsigned	:16;
		unsigned viddis	:1;
	} cicvcr;
	
	struct {
		unsigned	:16;
		unsigned cv	:4;
		unsigned vcm	:1;
		unsigned	:4;
		unsigned amsm	:1;
		unsigned vmsm	:1;
		unsigned ds	:1;
		unsigned ev	:1;
		unsigned	:1;
		unsigned svp	:1;
		unsigned svd	:1;
	} video_cntl;
	
	struct {
		unsigned	:18;
		unsigned bpu	:1;
		unsigned	:2;
		unsigned lsm	:1;
		unsigned	:5;
		unsigned stb	:1;
		unsigned	:3;
		unsigned nbfm	:1;
	} video_mode;
	
	struct {
		unsigned chsfr	:9;
		unsigned	:1;
		unsigned edaf	:1;
		unsigned e32bco	:1;
		unsigned afdt	:2;
		unsigned vpafc	:2;
		unsigned	:1;
		unsigned ile	:1;
		unsigned ilbm	:1;
		unsigned cds	:1;
		unsigned cle	:1;
		unsigned clbm	:1;
		unsigned	:2;
		unsigned afvp	:1;
		unsigned gle	:1;
		unsigned glbm	:1;
		unsigned glblm	:1;
		unsigned anim	:1;
		unsigned animr	:3;
	} osd_mode;
	
	struct {
		unsigned bgY	:8;
		unsigned bgCb	:4;
		unsigned bgCr	:4;
		unsigned sm	:3;
		unsigned	:2;
		unsigned pal	:1;
		unsigned	:2;
		unsigned dyc	:2;
		#define SFM_NORMAL	0
		#define SFM_BOTTOM	1
		#define SFM_TOP		2
		#define SFM_FIRST	3
		unsigned sfm	:2;
		#define DF_PSCAN	0
		#define DF_LBOX		1
		#define DF_HALF		3
		#define DF_QUARTER	4
		#define DF_ZOOM2X	5
		#define DF_NORMAL	7
		unsigned df	:3;
		unsigned mon	:1;
	} disp_mode;
	
	struct {
		unsigned	:16;
		unsigned hd	:8;
		unsigned vd	:8;
	} disp_dly;
	
	struct {
		unsigned	:23;
		unsigned ttxi	:1;
		unsigned tbr	:1;
		unsigned	:2;
		unsigned lc	:5;
	} vbi_cntl;
	
	struct {
		unsigned	:7;
		unsigned lb	:9;
		unsigned	:8;
		unsigned tb	:8;
	} disp_border;
	
	struct {
		unsigned	:16;
		unsigned sign	:1;
		unsigned oor	:1;
		int diff	:14;
	} pts_delta;
	
	struct {
		unsigned bv	:1;
		unsigned eos	:1;
		unsigned sb	:1;
		unsigned	:8;
		unsigned vcblq	:21;
	} clip_len;
	
	struct {
		unsigned	:1;
		unsigned vbi0ba	:15;
		unsigned	:1;
		unsigned vbi1ba	:15;
	} vbi_base;
	
	struct {
		unsigned	:16;
		unsigned mbusy	:1;
		unsigned dready	:1;
		unsigned	:10;
		unsigned blockc	:1;
		unsigned vbip	:1;
		unsigned vbia	:1;
		unsigned mwrite	:1;
	} mem_cntl;
	
	struct {
		unsigned	:1;
		unsigned seg1s	:3;
		unsigned seg1a	:12;
		unsigned	:1;
		unsigned seg0s	:3;
		unsigned seg0a	:12;
	} mem_seg;
	
	struct {
		unsigned	:8;
		unsigned topb	:8;
		unsigned	:7;
		unsigned lftb	:9;
	} scale_border;
	
	struct {
		unsigned	:8;
		unsigned zho	:8;
		unsigned	:8;
		unsigned zvo	:8;
	} vid0_zoffs;
} stbx25xx_video_val;

#endif
