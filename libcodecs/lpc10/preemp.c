/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:noexpandtab */

/*

$Log$
Revision 1.15  2004/06/26 03:50:14  markster
Merge source cleanups (bug #1911)

Revision 1.14  2003/02/12 13:59:15  matteo
mer feb 12 14:56:57 CET 2003

Revision 1.1.1.1  2003/02/12 13:59:15  matteo
mer feb 12 14:56:57 CET 2003

Revision 1.2  2000/01/05 08:20:39  markster
Some OSS fixes and a few lpc changes to make it actually work

 * Revision 1.1  1996/08/19  22:30:58  jaf
 * Initial revision
 *

*/

#include "lpc10.h"
#include "preemp.h"

/* ******************************************************************* */

/* 	PREEMP Version 55 */

/* $Log$
 * Revision 1.15  2004/06/26 03:50:14  markster
 * Merge source cleanups (bug #1911)
 *
 * Revision 1.14  2003/02/12 13:59:15  matteo
 * mer feb 12 14:56:57 CET 2003
 *
 * Revision 1.1.1.1  2003/02/12 13:59:15  matteo
 * mer feb 12 14:56:57 CET 2003
 *
 * Revision 1.2  2000/01/05 08:20:39  markster
 * Some OSS fixes and a few lpc changes to make it actually work
 *
 * Revision 1.1  1996/08/19  22:30:58  jaf
 * Initial revision
 * */
/* Revision 1.3  1996/03/14  23:16:29  jaf */
/* Just added a few comments about which array indices of the arguments */
/* are used, and mentioning that this subroutine has no local state. */

/* Revision 1.2  1996/03/11  23:23:34  jaf */
/* Added a bunch of comments to an otherwise simple subroutine. */

/* Revision 1.1  1996/02/07 14:48:48  jaf */
/* Initial revision */

/* ******************************************************************* */

/*   Preemphasize speech with a single-zero filter. */
/*  (When coef = .9375, preemphasis is as in LPC43.) */

/* Inputs: */
/*  NSAMP  - Number of samples to filter */
/*  INBUF  - Input speech buffer */
/*           Indices 1 through NSAMP are read. */
/*  COEF   - Preemphasis coefficient */
/* Input/Output: */
/*  Z      - Filter state */
/* Output: */
/*  PEBUF  - Preemphasized speech buffer (can be equal to INBUF) */
/*           Indices 1 through NSAMP are modified. */

/* This subroutine has no local state. */

int lpc10_preemp(float *inbuf, float *pebuf, int32_t * nsamp, float *coef,
		 float *z__)
{
	/* System generated locals */
	int32_t i__1;

	/* Local variables */
	float temp;
	int32_t i__;

/*       Arguments */
/*       Local variables */

/*       None of these need to have their values saved from one */
/*       invocation to the next. */

/*       Logically, this subroutine computes the output sequence */
/*       pebuf(1:nsamp) defined by: */

/*       pebuf(i) = inbuf(i) - coef * inbuf(i-1) */

/*       where inbuf(0) is defined by the value of z given as input to */
/*       this subroutine. */

/*       What is this filter's frequency response and phase response? */

/*       Why is this filter applied to the speech? */

/*       Could it be more efficient to apply multiple filters */
/*       simultaneously, by combining them into one equivalent filter? */

/*       Are there ever cases when "factoring" one high-order filter into 
*/
/*       multiple smaller-order filter actually reduces the number of */
/*       arithmetic operations needed to perform them? */
/*       When I first read this subroutine, I didn't understand why the */
/*       variable temp was used.  It seemed that the statements in the do 
*/
/*       loop could be replaced with the following: */

/*           pebuf(i) = inbuf(i) - coef * z */
/*           z = inbuf(i) */

/*       The reason for temp is so that even if pebuf and inbuf are the */
/*       same arrays in memory (i.e., they are aliased), then this */
/*       subroutine will still work correctly.  I didn't floatize this */
/*       until seeing the comment after PEBUF above that says "(can be */
/*       equal to INBUF)". */
	/* Parameter adjustments */
	--pebuf;
	--inbuf;

	/* Function Body */
	i__1 = *nsamp;
	for (i__ = 1; i__ <= i__1; ++i__) {
		temp = inbuf[i__] - *coef * *z__;
		*z__ = inbuf[i__];
		pebuf[i__] = temp;
/* L10: */
	}
	return 0;
}				/* lpc10_preemp */
