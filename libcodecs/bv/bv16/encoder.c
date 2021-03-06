/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:noexpandtab */

/*****************************************************************************/
/* BroadVoice(R)16 (BV16) Fixed-Point ANSI-C Source Code                     */
/* Revision Date: November 13, 2009                                          */
/* Version 1.1                                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Copyright 2000-2009 Broadcom Corporation                                  */
/*                                                                           */
/* This software is provided under the GNU Lesser General Public License,    */
/* version 2.1, as published by the Free Software Foundation ("LGPL").       */
/* This program is distributed in the hope that it will be useful, but       */
/* WITHOUT ANY SUPPORT OR WARRANTY; without even the implied warranty of     */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the LGPL for     */
/* more details.  A copy of the LGPL is available at                         */
/* http://www.broadcom.com/licenses/LGPLv2.1.php,                            */
/* or by writing to the Free Software Foundation, Inc.,                      */
/* 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 */
/*****************************************************************************/

/*****************************************************************************
  encoder.c : BV16 Fixed-Point Encoder Main Subroutines

  $Log$
******************************************************************************/

#include <stdint.h>
#include "bvcommon.h"
#include "bv16cnst.h"
#include "bv16strct.h"
#include "bv16externs.h"
#include "basop32.h"
#include "utility.h"
#include "../itug729ilib/oper_32b.h"

void Reset_BV16_Encoder(struct BV16_Encoder_State *c)
{
	W16zero((int16_t *) c,
		sizeof(struct BV16_Encoder_State) / sizeof(int16_t));
	c->lsplast[0] = 3641;
	c->lsplast[1] = 7282;
	c->lsplast[2] = 10923;
	c->lsplast[3] = 14564;
	c->lsplast[4] = 18204;
	c->lsplast[5] = 21845;
	c->lsplast[6] = 25486;
	c->lsplast[7] = 29127;
	c->xwd_exp = 32;
	c->cpplast = 12 * DECF;
	c->lmax = MIN_32;
	c->lmin = MAX_32;
	c->lmean = 419430400;	/* 12.5 Q25 */
	c->x1 = 570425344;	/* 17.0 Q25 */
	c->level = 570425344;	/* 17.0 Q25 */
	c->old_A[0] = 4096;
}

void BV16_Encode(struct BV16_Bit_Stream *bs,
		 struct BV16_Encoder_State *cs, int16_t * inx)
{
	int32_t r[NSTORDER + 1];	/* DPF representation of autocorrelation lags */
	int16_t ltnfm[MAXPP1 + FRSZ];
	int16_t a[LPCO + 1];	/* LPC coefficients */
	int16_t aw[LPCO + 1];	/* Weighted LPC coefficients */
	int16_t fsz[NSTORDER + 1];
	int16_t fsp[NSTORDER + 1];
	int16_t x[LX];		/* Signal buffer */
	int16_t dq[LX];		/* quantized short term pred error */
	int16_t sdq[LX];
	int16_t xw[FRSZ];	/* Q0 perceptually weighted version of x() */
	int16_t lsp[LPCO], lspq[LPCO];	/* Q15 */
	int16_t cbs[VDIM * CBSZ];
	int16_t bq[3];		/* Q15 */
	int16_t beta;		/* Q13 */
	int16_t ppt;		/* Q9 */
	int16_t gainq;		/* Q2 */
	int32_t ee;		/* Q1 */
	int32_t a0;
	int16_t gain_exp;
	int16_t pp, cpp, i;
	int16_t eehi, eelo;
	int16_t a0hi, a0lo;
	int16_t dummy;

	/* copy state memory to local memory buffers */
	W16copy(x, cs->x, XOFF);
	W16copy(ltnfm, cs->ltnfm, MAXPP1);

	/* 150 Hz HighPass filtering */
	preprocess(cs, x + XOFF, inx, FRSZ);

	/* update highpass filtered signal buffer */
	W16copy(cs->x, x + FRSZ, XOFF);

	/* perform lpc analysis with asymmetrical window */
	Autocorr(r, x + LX - WINSZ, winl, WINSZ, NSTORDER);
	Spectral_Smoothing(NSTORDER, r, sstwinl_h, sstwinl_l);
	Levinson(r, a, cs->old_A, LPCO);

	/* pole-zero noise feedback filter */

	fsz[0] = 0;
	for (i = 1; i <= NSTORDER; i++)
		fsz[i] = bv_bv_mult_r(a[i], gfsz[i]);
	fsp[0] = 4096;
	for (i = 1; i <= NSTORDER; i++)
		fsp[i] = bv_bv_mult_r(a[i], gfsp[i]);

	/* bandwidth expansion */
	for (i = 1; i <= LPCO; i++)
		a[i] = bv_bv_mult_r(bwel[i], a[i]);

	/* LPC -> LSP Conversion */
	a2lsp(a, lsp, cs->lsplast);
	W16copy(cs->lsplast, lsp, LPCO);

	/* Spectral Quantization */
	lspquan(lspq, bs->lspidx, lsp, cs->lsppm);
	lsp2a(lspq, a);

	/* calculate lpc prediction residual */
	W16copy(dq, cs->dq, XOFF);
	azfilter(a, LPCO, x + XOFF, dq + XOFF, FRSZ);

	/* weighted version of lpc filter to generate weighted speech */

	aw[0] = a[0];
	for (i = 1; i <= LPCO; i++)
		aw[i] = bv_bv_mult_r(STWAL[i], a[i]);

	/* get perceptually weighted speech signal */
	apfilter(aw, LPCO, dq + XOFF, xw, FRSZ, cs->stwpm, 1);

	/* get the coarse version of pitch period using 4:1 decimation */
	cpp = coarsepitch(xw, cs);
	cs->cpplast = cpp;

	/* refine the pitch period in the neighborhood of coarse pitch period
	   also calculate the pitch predictor tap for single-tap predictor */

	for (i = 0; i < LX; i++)
		sdq[i] = bv_shr(dq[i], 2);
	pp = refinepitch(sdq, cpp, &ppt);
	bs->ppidx = pp - MINPP;

	/* vector quantize 3 pitch predictor taps with minimum residual energy */
	bs->bqidx = pitchtapquan(dq, pp, bq, &ee);

	/* scale up pitch prediction residual energy by 1.5^2=2.25 (18432 Q13) */
	L_Extract(ee, &eehi, &eelo);
	ee = L_bv_shl(Mpy_32_16(eehi, eelo, 18432), 2);

	/* get coefficients of long-term noise feedback filter */
	if (ppt > 512)
		beta = LTWF;
	else if (ppt <= 0)
		beta = 0;
	else
		beta = bv_extract_h(L_bv_shl(L_bv_mult(LTWF, ppt), 6));

	/* gain quantization */
	bs->gidx = gainquan(&a0, &ee, cs->lgpm, cs->prevlg, cs->level);

	/* level estimation */
	dummy = estl_alpha;
	estlevel(cs->prevlg[0], &cs->level, &cs->lmax, &cs->lmin,
		 &cs->lmean, &cs->x1, LGPORDER + 1, Nfdm + 1, &dummy);

	/* find appropriate gain block floating point exponent */
	gain_exp = bv_sub(bv_norm_l(a0), 2);
	/* scale down quantized gain by 1.5, 1/1.5=2/3 (21845 Q15) */
	L_Extract(a0, &a0hi, &a0lo);
	a0 = Mpy_32_16(a0hi, a0lo, 21845);
	gainq = intround(L_bv_shl(a0, gain_exp));

	/* scale the scalar quantizer codebook */
	for (i = 0; i < (VDIM * CBSZ); i++)
		cbs[i] = bv_bv_mult_r(gainq, cccb[i]);

	/* perform noise feedback coding of the excitation signal */
	excquan(bs->qvidx, x + XOFF, a, fsz, fsp, bq, beta, cs->stsym,
		dq, ltnfm, cs->stnfz, cs->stnfp, cbs, pp, gain_exp);

	/* update state memory */
	W16copy(cs->dq, &dq[FRSZ], XOFF);
	W16copy(cs->ltnfm, &ltnfm[FRSZ], MAXPP1);

}
