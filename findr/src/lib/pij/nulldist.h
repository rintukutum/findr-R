/* Copyright 2016 Lingfei Wang
 * 
 * This file is part of Findr.
 * 
 * Findr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Findr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with Findr.  If not, see <http://www.gnu.org/licenses/>.
 */
/* This part contains analytical calculation of the histogram of log likelihood ratio from null hypothesis.
 * Each function is applicable to one or more stages, which are stated in the function name as pij_nulldistX_..., where X is the applicable stage.
 * For each stage, different methods to calculate histogram can coexist. The method is declared in the function name as suffix:
 * _cdf:	Calculate histogram as the difference of cdf.
 * 			This is applicable when distribution is single-variable integrable.
 * _pdf:	Calculate histogram as the pdf mean of points evenly split within the bin. This is applicable when distribution is single-variable non-integrable.
 * _sim:	Construct histogram by sampling. This is applicable when distribution is multi-variable non-integrable.
 */

#ifndef _HEADER_LIB_PIJ_NULLDIST_H_
#define _HEADER_LIB_PIJ_NULLDIST_H_
#include "../base/config.h"
#include "../base/types.h"
#ifdef __cplusplus
extern "C"
{
#endif


/*************************************************************
 * Generic functions for any step
 *************************************************************/

struct pij_nulldist_pdfs_param
{
	size_t		n1;
	size_t		n2;
};

/* Calculate the pdf p(x|n1,n2) for x=-0.5*log(1-z1/(z1+z2)),
 * where z1~chi2(n1), z2~chi2(n2).
 * p(x|n1,n2)=2*(1-exp(-2*x))^((i-2)/2)*exp(-n2*x)*Gamma((n1+n2)/2)
 * 			/(Gamma(n1/2)*Gamma(n2/2)).
 * loc:		(nd) Locations of x to calculate p(x|n1,n2).
 * ans:		(nd) Calculated p(x|n1,n2).
 * param:	Parameters. See struct pij_nulldist_pdfs_param.
 */
void pij_nulldist_pdfs(const VECTORD* loc,VECTORD* ans,const void* param);

/* Calculate the pdf p(x|i) for x=-0.5*log(1-z1_i/(z1_i+z2_i)),
 * where z1_i ~ chi2(i), z2_i~chi2(ntot-i), i=nsubmin,...,nsubmax-1.
 * p(x|i)=2*(1-exp(-2*x))^((i-2)/2)*exp((i-ntot)*x)*Gamma(ntot/2)
 * 			/(Gamma(i/2)*Gamma((ntot-i)/2)).
 * Buffer is provided.
 * nsubmin,
 * nsubmax,
 * ntot:		As indicated in equation.
 * loc:			(nd) Locations of x to calculate p(x|i).
 * ans:			(nsubmax-nsubmin,nd) Calculated p(x|i). ans[j,k]=p(loc[k]|i=nsubmin+j).
 * vb2:			(nd) Buffer. =log(1-exp(-2x))/2+x
 * nd:			loc->size
 */
void pij_nulldist_calcpdf_buffed(size_t nsubmin,size_t nsubmax,size_t ntot,const VECTORD* loc,MATRIXD* ans,VECTORD* vb2);

/* Calculate density histogram of null distribution based on pdf function.
 * This uses interpolation within each bin, similarly with pij_nulldist_nullhist_pdf.
 * Null distribution is for x=-0.5*log(1-z1/(z1+z2)), z1~chi2(n1), z2~chi2(n2)
 * nbin:	Number of bins for histogram
 * range:	(nbin+1) Histogram range
 * hist:	(nbin) Output of histogram bins
 * n1,
 * n2:		Parameters of null distribution
 * n:		Log_2 Number of points for interpolation within each bin.
 */
int pij_nulldist_hist_pdf(const double* restrict range,size_t nbin,double* restrict hist,size_t n1,size_t n2,size_t n);

/* pij_nulldist_cdf_callback calculates the CDF of null distribution of the random variable:
 * x=-log(1-z1/(z1+z2))/2,
 * where z1 ~ chi2(n1) and z2 ~ chi2(n2).
 */
//Parameter for pij_nulldist_cdf_callback
struct pij_nulldist_cdf_callback_param
{
	size_t	n1;
	size_t	n2;
};

double pij_nulldist_cdf_callback(double x,const void * param);
















#ifdef __cplusplus
}
#endif
#endif
