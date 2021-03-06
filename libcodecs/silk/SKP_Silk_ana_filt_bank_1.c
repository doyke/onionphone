/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:noexpandtab */

/***********************************************************************
Copyright (c) 2006-2010, Skype Limited. All rights reserved. 
Redistribution and use in source and binary forms, with or without 
modification, (subject to the limitations in the disclaimer below) 
are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright 
notice, this list of conditions and the following disclaimer in the 
documentation and/or other materials provided with the distribution.
- Neither the name of Skype Limited, nor the names of specific 
contributors, may be used to endorse or promote products derived from 
this software without specific prior written permission.
NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED 
BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
CONTRIBUTORS ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF 
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/

/*                                                                          *
 * SKP_ana_filt_bank_1.c                                                    *
 *                                                                          *
 * Split signal into two decimated bands using first-order allpass filters  *
 *                                                                          *
 * Copyright 2006 (c), Skype Limited                                        *
 * Date: 060221                                                             *
 *                                                                          */
#include "SKP_Silk_SigProc_FIX.h"

/* Coefficients for 2-band filter bank based on first-order allpass filters */
static int16_t A_fb1_20[1] = { 5394 };
static int16_t A_fb1_21[1] = { 20623 };

/* Split signal into two decimated bands using first-order allpass filters */
void SKP_Silk_ana_filt_bank_1(const int16_t * in,	/* I:    Input signal [N]       */
			      int32_t * S,	/* I/O: State vector [2]        */
			      int16_t * outL,	/* O:    Low band [N/2]         */
			      int16_t * outH,	/* O:    High band [N/2]        */
			      int32_t * scratch,	/* I:    Scratch memory [3*N/2] */
			      const int32_t N	/* I:   Number of input samples */
    )
{
	int k, N2 = SKP_RSHIFT(N, 1);
	int32_t out_tmp;

	/* De-interleave three allpass inputs, and convert Q15 -> Q25 */
	for (k = 0; k < N2; k++) {
		scratch[k + N] = SKP_LSHIFT((int32_t) in[2 * k], 10);
		scratch[k + N2] = SKP_LSHIFT((int32_t) in[2 * k + 1], 10);
	}

	/* Allpass filters */
	SKP_Silk_allpass_int(scratch + N2, S + 0, A_fb1_20[0], scratch, N2);
	SKP_Silk_allpass_int(scratch + N, S + 1, A_fb1_21[0], scratch + N2, N2);

	/* Add and subtract two allpass outputs to create bands */
	for (k = 0; k < N2; k++) {
		out_tmp = scratch[k] + scratch[k + N2];
		outL[k] = (int16_t) SKP_SAT16(SKP_RSHIFT_ROUND(out_tmp, 11));

		out_tmp = scratch[k] - scratch[k + N2];
		outH[k] = (int16_t) SKP_SAT16(SKP_RSHIFT_ROUND(out_tmp, 11));
	}
}
