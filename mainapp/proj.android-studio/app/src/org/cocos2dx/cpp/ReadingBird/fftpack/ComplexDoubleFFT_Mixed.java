package org.cocos2dx.cpp.ReadingBird.fftpack;
/**
  * @author Baoshe Zhang
  * @author Astronomical Instrument Group of University of Lethbridge.
*/
class ComplexDoubleFFT_Mixed
{

/*----------------------------------------------------------------------
   passf2: Complex FFT's forward/backward processing of factor 2;
   isign is +1 for backward and -1 for forward transforms
  ----------------------------------------------------------------------*/

     void passf2(int ido, int l1, final double cc[], double ch[], final double wtable[], int offset, int isign) 
                  /*isign==+1 for backward transform*/
     {
          int     i, k, ah, ac;
          double  ti2, tr2;
          int iw1;

          iw1 = offset;
          if(ido<=2)
          {
	       for(k=0; k<l1; k++)
	       {
	           ah=k*ido;
	           ac=2*k*ido;
	           ch[ah]=cc[ac]+cc[ac+ido];
	           ch[ah+ido*l1]=cc[ac]-cc[ac+ido];
	           ch[ah+1]=cc[ac+1]+cc[ac+ido+1];
	           ch[ah+ido*l1+1]=cc[ac+1]-cc[ac+ido+1];
	       }
          }
          else
          {
	       for(k=0; k<l1; k++)
	       {
	           for(i=0; i<ido-1; i+=2)
	           {
		        ah=i+k*ido;
		        ac=i+2*k*ido;
		        ch[ah]=cc[ac]+cc[ac+ido];
		        tr2=cc[ac]-cc[ac+ido];
		        ch[ah+1]=cc[ac+1]+cc[ac+1+ido];
		        ti2=cc[ac+1]-cc[ac+1+ido];
		        ch[ah+l1*ido+1]=wtable[i+iw1]*ti2+isign*wtable[i+1+iw1]*tr2;
		        ch[ah+l1*ido]=wtable[i+iw1]*tr2-isign*wtable[i+1+iw1]*ti2;
	           }
	       }
         }
     }

/*----------------------------------------------------------------------
   passf3: Complex FFT's forward/backward processing of factor 3;
   isign is +1 for backward and -1 for forward transforms
  ----------------------------------------------------------------------*/
     void passf3(int ido, int l1, final double cc[], double ch[], final double wtable[], int offset, int isign)
     {
          final double taur=-0.5;
          final double taui=0.866025403784439;
          int     i, k, ac, ah;
          double  ci2, ci3, di2, di3, cr2, cr3, dr2, dr3, ti2, tr2;
          int iw1, iw2;
 
          iw1 = offset;
          iw2 = iw1 + ido;

          if(ido==2)
          {
	       for(k=1; k<=l1; k++)
	       {
	           ac=(3*k-2)*ido;
	           tr2=cc[ac]+cc[ac+ido];
	           cr2=cc[ac-ido]+taur*tr2;
	           ah=(k-1)*ido;
	           ch[ah]=cc[ac-ido]+tr2;

	           ti2=cc[ac+1]+cc[ac+ido+1];
	           ci2=cc[ac-ido+1]+taur*ti2;
	           ch[ah+1]=cc[ac-ido+1]+ti2;

	           cr3=isign*taui*(cc[ac]-cc[ac+ido]);
	           ci3=isign*taui*(cc[ac+1]-cc[ac+ido+1]);
	           ch[ah+l1*ido]=cr2-ci3;
	           ch[ah+2*l1*ido]=cr2+ci3;
	           ch[ah+l1*ido+1]=ci2+cr3;
	           ch[ah+2*l1*ido+1]=ci2-cr3;
	       }
          }
          else
          {
	       for(k=1; k<=l1; k++)
	       {
	            for(i=0; i<ido-1; i+=2)
	            {
		         ac=i+(3*k-2)*ido;
		         tr2=cc[ac]+cc[ac+ido];
		         cr2=cc[ac-ido]+taur*tr2;
		         ah=i+(k-1)*ido;
		         ch[ah]=cc[ac-ido]+tr2;
		         ti2=cc[ac+1]+cc[ac+ido+1];
		         ci2=cc[ac-ido+1]+taur*ti2;
		         ch[ah+1]=cc[ac-ido+1]+ti2;
		         cr3=isign*taui*(cc[ac]-cc[ac+ido]);
		         ci3=isign*taui*(cc[ac+1]-cc[ac+ido+1]);
		         dr2=cr2-ci3;
		         dr3=cr2+ci3;
		         di2=ci2+cr3;
		         di3=ci2-cr3;
		         ch[ah+l1*ido+1]=wtable[i+iw1]*di2+isign*wtable[i+1+iw1]*dr2;
		         ch[ah+l1*ido]=wtable[i+iw1]*dr2-isign*wtable[i+1+iw1]*di2;
		         ch[ah+2*l1*ido+1]=wtable[i+iw2]*di3+isign*wtable[i+1+iw2]*dr3;
		         ch[ah+2*l1*ido]=wtable[i+iw2]*dr3-isign*wtable[i+1+iw2]*di3;
	            }
	       }
          }
     }

/*----------------------------------------------------------------------
   passf4: Complex FFT's forward/backward processing of factor 4;
   isign is +1 for backward and -1 for forward transforms
  ----------------------------------------------------------------------*/
     void passf4(int ido, int l1, final double cc[], double ch[], final double wtable[], int offset, int isign)
     {
           int i, k, ac, ah;
           double  ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, tr2, tr3, tr4;
           int iw1, iw2, iw3;
           iw1 = offset;
           iw2 = iw1 + ido;
           iw3 = iw2 + ido;

           if(ido==2)
           {
	        for(k=0; k<l1; k++)
	        {
	             ac=4*k*ido+1;
	             ti1=cc[ac]-cc[ac+2*ido];
	             ti2=cc[ac]+cc[ac+2*ido];
	    	     tr4=cc[ac+3*ido]-cc[ac+ido];
	    	     ti3=cc[ac+ido]+cc[ac+3*ido];
	    	     tr1=cc[ac-1]-cc[ac+2*ido-1];
	             tr2=cc[ac-1]+cc[ac+2*ido-1];
	    	     ti4=cc[ac+ido-1]-cc[ac+3*ido-1];
	             tr3=cc[ac+ido-1]+cc[ac+3*ido-1];
	    	     ah=k*ido;
	    	     ch[ah]=tr2+tr3;
	     	     ch[ah+2*l1*ido]=tr2-tr3;
	    	     ch[ah+1]=ti2+ti3;
	    	     ch[ah+2*l1*ido+1]=ti2-ti3;
	    	     ch[ah+l1*ido]=tr1+isign*tr4;
	    	     ch[ah+3*l1*ido]=tr1-isign*tr4;
	    	     ch[ah+l1*ido+1]=ti1+isign*ti4;
	    	     ch[ah+3*l1*ido+1]=ti1-isign*ti4;
	        }
           }
           else
           {
	        for(k=0; k<l1; k++)
		{
	    	     for(i=0; i<ido-1; i+=2)
	    	     {
			ac=i+1+4*k*ido;
			ti1=cc[ac]-cc[ac+2*ido];
			ti2=cc[ac]+cc[ac+2*ido];
			ti3=cc[ac+ido]+cc[ac+3*ido];
			tr4=cc[ac+3*ido]-cc[ac+ido];
			tr1=cc[ac-1]-cc[ac+2*ido-1];
			tr2=cc[ac-1]+cc[ac+2*ido-1];
			ti4=cc[ac+ido-1]-cc[ac+3*ido-1];
			tr3=cc[ac+ido-1]+cc[ac+3*ido-1];
			ah=i+k*ido;
			ch[ah]=tr2+tr3;
			cr3=tr2-tr3;
			ch[ah+1]=ti2+ti3;
			ci3=ti2-ti3;
			cr2=tr1+isign*tr4;
			cr4=tr1-isign*tr4;
			ci2=ti1+isign*ti4;
			ci4=ti1-isign*ti4;
			ch[ah+l1*ido]=wtable[i+iw1]*cr2-isign*wtable[i+1+iw1]*ci2;
			ch[ah+l1*ido+1]=wtable[i+iw1]*ci2+isign*wtable[i+1+iw1]*cr2;
			ch[ah+2*l1*ido]=wtable[i+iw2]*cr3-isign*wtable[i+1+iw2]*ci3;
			ch[ah+2*l1*ido+1]=wtable[i+iw2]*ci3+isign*wtable[i+1+iw2]*cr3;
			ch[ah+3*l1*ido]=wtable[i+iw3]*cr4-isign*wtable[i+1+iw3]*ci4;
			ch[ah+3*l1*ido+1]=wtable[i+iw3]*ci4+isign*wtable[i+1+iw3]*cr4;
	    	     }
		}
    	   }
     } 

/*----------------------------------------------------------------------
   passf5: Complex FFT's forward/backward processing of factor 5;
   isign is +1 for backward and -1 for forward transforms
  ----------------------------------------------------------------------*/
     void passf5(int ido, int l1, final double cc[], double ch[], final double wtable[], int offset, int isign)
               /*isign==-1 for forward transform and+1 for backward transform*/
     {
    	final double tr11=0.309016994374947;
    	final double ti11=0.951056516295154;
    	final double tr12=-0.809016994374947;
    	final double ti12=0.587785252292473;
    	int     i, k, ac, ah;
    	double  ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, cr4,
        	    ti2, ti3, ti4, ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5;
        int iw1, iw2, iw3, iw4;

        iw1 = offset;
        iw2 = iw1 + ido;
        iw3 = iw2 + ido;
        iw4 = iw3 + ido;

	if(ido==2)
    	{
	    for(k=1; k<=l1;++k)
	    {
	    	ac=(5*k-4)*ido+1;
	    	ti5=cc[ac]-cc[ac+3*ido];
	    	ti2=cc[ac]+cc[ac+3*ido];
	    	ti4=cc[ac+ido]-cc[ac+2*ido];
	    	ti3=cc[ac+ido]+cc[ac+2*ido];
	    	tr5=cc[ac-1]-cc[ac+3*ido-1];
	    	tr2=cc[ac-1]+cc[ac+3*ido-1];
	    	tr4=cc[ac+ido-1]-cc[ac+2*ido-1];
	    	tr3=cc[ac+ido-1]+cc[ac+2*ido-1];
	    	ah=(k-1)*ido;
	    	ch[ah]=cc[ac-ido-1]+tr2+tr3;
	    	ch[ah+1]=cc[ac-ido]+ti2+ti3;
	    	cr2=cc[ac-ido-1]+tr11*tr2+tr12*tr3;
	    	ci2=cc[ac-ido]+tr11*ti2+tr12*ti3;
	    	cr3=cc[ac-ido-1]+tr12*tr2+tr11*tr3;
	    	ci3=cc[ac-ido]+tr12*ti2+tr11*ti3;
	    	cr5=isign*(ti11*tr5+ti12*tr4);
	    	ci5=isign*(ti11*ti5+ti12*ti4);
	    	cr4=isign*(ti12*tr5-ti11*tr4);
	    	ci4=isign*(ti12*ti5-ti11*ti4);
	    	ch[ah+l1*ido]=cr2-ci5;
	    	ch[ah+4*l1*ido]=cr2+ci5;
	    	ch[ah+l1*ido+1]=ci2+cr5;
	    	ch[ah+2*l1*ido+1]=ci3+cr4;
	    	ch[ah+2*l1*ido]=cr3-ci4;
	    	ch[ah+3*l1*ido]=cr3+ci4;
	    	ch[ah+3*l1*ido+1]=ci3-cr4;
	    	ch[ah+4*l1*ido+1]=ci2-cr5;
	    }
    	}
    	else
    	{
	    for(k=1; k<=l1; k++)
	    {
	    	for(i=0; i<ido-1; i+=2)
	    	{
			ac=i+1+(k*5-4)*ido;
			ti5=cc[ac]-cc[ac+3*ido];
			ti2=cc[ac]+cc[ac+3*ido];
			ti4=cc[ac+ido]-cc[ac+2*ido];
			ti3=cc[ac+ido]+cc[ac+2*ido];
			tr5=cc[ac-1]-cc[ac+3*ido-1];
			tr2=cc[ac-1]+cc[ac+3*ido-1];
			tr4=cc[ac+ido-1]-cc[ac+2*ido-1];
			tr3=cc[ac+ido-1]+cc[ac+2*ido-1];
			ah=i+(k-1)*ido;
			ch[ah]=cc[ac-ido-1]+tr2+tr3;
			ch[ah+1]=cc[ac-ido]+ti2+ti3;
			cr2=cc[ac-ido-1]+tr11*tr2+tr12*tr3;

			ci2=cc[ac-ido]+tr11*ti2+tr12*ti3;
			cr3=cc[ac-ido-1]+tr12*tr2+tr11*tr3;

			ci3=cc[ac-ido]+tr12*ti2+tr11*ti3;
			cr5=isign*(ti11*tr5+ti12*tr4);
			ci5=isign*(ti11*ti5+ti12*ti4);
			cr4=isign*(ti12*tr5-ti11*tr4);
			ci4=isign*(ti12*ti5-ti11*ti4);
			dr3=cr3-ci4;
			dr4=cr3+ci4;
			di3=ci3+cr4;
			di4=ci3-cr4;
			dr5=cr2+ci5;
			dr2=cr2-ci5;
			di5=ci2-cr5;
			di2=ci2+cr5;
			ch[ah+l1*ido]=wtable[i+iw1]*dr2-isign*wtable[i+1+iw1]*di2;
			ch[ah+l1*ido+1]=wtable[i+iw1]*di2+isign*wtable[i+1+iw1]*dr2;
			ch[ah+2*l1*ido]=wtable[i+iw2]*dr3-isign*wtable[i+1+iw2]*di3;
			ch[ah+2*l1*ido+1]=wtable[i+iw2]*di3+isign*wtable[i+1+iw2]*dr3;
			ch[ah+3*l1*ido]=wtable[i+iw3]*dr4-isign*wtable[i+1+iw3]*di4;
			ch[ah+3*l1*ido+1]=wtable[i+iw3]*di4+isign*wtable[i+1+iw3]*dr4;
			ch[ah+4*l1*ido]=wtable[i+iw4]*dr5-isign*wtable[i+1+iw4]*di5;
			ch[ah+4*l1*ido+1]=wtable[i+iw4]*di5+isign*wtable[i+1+iw4]*dr5;
	    	}
	    }
         }
     }

/*----------------------------------------------------------------------
   passfg: Complex FFT's forward/backward processing of general factor;
   isign is +1 for backward and -1 for forward transforms
  ----------------------------------------------------------------------*/
     void passfg(int nac[], int ido, int ip, int l1, int idl1,
                       final double cc[], double c1[], double c2[], double ch[], double ch2[],
                       final double wtable[], int offset, int isign)
     {
          int idij, idlj, idot, ipph, i, j, k, l, jc, lc, ik, idj, idl, inc, idp;
          double  wai, war;
          int iw1;

          iw1 = offset;
    	  idot=ido / 2;
          ipph=(ip+1)/ 2;
          idp=ip*ido;
          if(ido>=l1)
          {
	      for(j=1; j<ipph; j++)
	      {
	           jc=ip-j;
	           for(k=0; k<l1; k++)
	           {
		        for(i=0; i<ido; i++)
		        {
		            ch[i+(k+j*l1)*ido]=cc[i+(j+k*ip)*ido]+cc[i+(jc+k*ip)*ido];
		            ch[i+(k+jc*l1)*ido]=cc[i+(j+k*ip)*ido]-cc[i+(jc+k*ip)*ido];
		        }
	           }
	      }
	      for(k=0; k<l1; k++)
	         for(i=0; i<ido; i++)
		     ch[i+k*ido]=cc[i+k*ip*ido];
          }
          else
          {
	      for(j=1; j<ipph; j++)
	      {
	          jc=ip-j;
	          for(i=0; i<ido; i++)
	          {
		      for(k=0; k<l1; k++)
		      {
		          ch[i+(k+j*l1)*ido]=cc[i+(j+k*ip)*ido]+cc[i+(jc+k*ip)*ido];
		          ch[i+(k+jc*l1)*ido]=cc[i+(j+k*ip)*ido]-cc[i+(jc+k*ip)*ido];
		      }
	          }
	      }
	      for(i=0; i<ido; i++)
	         for(k=0; k<l1; k++)
		     ch[i+k*ido]=cc[i+k*ip*ido];
          }

          idl=2-ido;
          inc=0;
          for(l=1; l<ipph; l++)
          {
	      lc=ip-l;
	      idl+=ido;
	      for(ik=0; ik<idl1; ik++)
	      {
	          c2[ik+l*idl1]=ch2[ik]+wtable[idl-2+iw1]*ch2[ik+idl1];
	          c2[ik+lc*idl1]=isign*wtable[idl-1+iw1]*ch2[ik+(ip-1)*idl1];
	      }
	      idlj=idl;
	      inc+=ido;
	      for(j=2; j<ipph; j++)
	      {
	          jc=ip-j;
	          idlj+=inc;
	          if(idlj>idp) idlj-=idp;
	          war=wtable[idlj-2+iw1];
	          wai=wtable[idlj-1+iw1];
	          for(ik=0; ik<idl1; ik++)
	          {
		      c2[ik+l*idl1]+=war*ch2[ik+j*idl1];
		      c2[ik+lc*idl1]+=isign*wai*ch2[ik+jc*idl1];
	          }
	      }
          }
          for(j=1; j<ipph; j++)
	     for(ik=0; ik<idl1; ik++)
	         ch2[ik]+=ch2[ik+j*idl1];
          for(j=1; j<ipph; j++)
          {
	      jc=ip-j;
	      for(ik=1; ik<idl1; ik+=2)
	      {
	          ch2[ik-1+j*idl1]=c2[ik-1+j*idl1]-c2[ik+jc*idl1];
	          ch2[ik-1+jc*idl1]=c2[ik-1+j*idl1]+c2[ik+jc*idl1];
	          ch2[ik+j*idl1]=c2[ik+j*idl1]+c2[ik-1+jc*idl1];
	          ch2[ik+jc*idl1]=c2[ik+j*idl1]-c2[ik-1+jc*idl1];
	      }
          }
          nac[0]=1;
          if(ido==2) return;
          nac[0]=0;
          for(ik=0; ik<idl1; ik++) c2[ik]=ch2[ik];
          for(j=1; j<ip; j++)
          {
	      for(k=0; k<l1; k++)
	      {
	          c1[(k+j*l1)*ido+0]=ch[(k+j*l1)*ido+0];
	          c1[(k+j*l1)*ido+1]=ch[(k+j*l1)*ido+1];
	      }
          }
          if(idot<=l1)
          {
	      idij=0;
	      for(j=1; j<ip; j++)
	      {
	          idij+=2;
	          for(i=3; i<ido; i+=2)
	          {
		      idij+=2;
		      for(k=0; k<l1; k++)
		      {
		          c1[i-1+(k+j*l1)*ido]=
			       wtable[idij-2+iw1]*ch[i-1+(k+j*l1)*ido]-
			       isign*wtable[idij-1+iw1]*ch[i+(k+j*l1)*ido];
		          c1[i+(k+j*l1)*ido]=
			       wtable[idij-2+iw1]*ch[i+(k+j*l1)*ido]+
			       isign*wtable[idij-1+iw1]*ch[i-1+(k+j*l1)*ido];
		      }
	          }
	      }
          }
          else
          {
	      idj=2-ido;
	      for(j=1; j<ip; j++)
	      {
	          idj+=ido;
	          for(k=0; k<l1; k++)
	          {
		      idij=idj;
		      for(i=3; i<ido; i+=2)
		      {
		          idij+=2;
		          c1[i-1+(k+j*l1)*ido]=
			       wtable[idij-2+iw1]*ch[i-1+(k+j*l1)*ido]-
			       isign*wtable[idij-1+iw1]*ch[i+(k+j*l1)*ido];
		          c1[i+(k+j*l1)*ido]=
			       wtable[idij-2+iw1]*ch[i+(k+j*l1)*ido]+
			       isign*wtable[idij-1+iw1]*ch[i-1+(k+j*l1)*ido];
		      }
	          }
	      }
          }
      }

/*---------------------------------------------------------
   cfftf1: further processing of Complex forward FFT
  --------------------------------------------------------*/
     void cfftf1(int n, double c[], final double wtable[], int isign)
     {
          int     idot, i;
          int     k1, l1, l2;
          int     na, nf, ip, iw, ido, idl1;
          int[]  nac = new int[1];

          int     iw1, iw2;
          double[] ch = new double[2*n];

          iw1=2*n;
          iw2=4*n;
          System.arraycopy(wtable, 0, ch, 0, 2*n);

          nac[0] = 0;

          nf=(int)wtable[1+iw2];
          na=0;
          l1=1;
          iw=iw1;
          for(k1=2; k1<=nf+1; k1++)
          {
	      ip=(int)wtable[k1+iw2];
	      l2=ip*l1;
	      ido=n / l2;
	      idot=ido+ido;
	      idl1=idot*l1;
	      if(ip==4)
	      {
	          if(na==0)
                  {
                      passf4(idot, l1, c, ch, wtable, iw, isign);
                  }
	          else
                  {
                      passf4(idot, l1, ch, c, wtable, iw, isign);
                  }
	          na=1-na;
	      }
	      else if(ip==2)
	      {
	          if(na==0)
                  {
                          passf2(idot, l1, c, ch, wtable, iw, isign);
                  }
	          else
                  {
                          passf2(idot, l1, ch, c, wtable, iw, isign);
                  }
	          na=1-na;
	      }
	      else if(ip==3)
	      {
	          if(na==0)
                  {
                        passf3(idot, l1, c, ch, wtable, iw, isign);
                  }
	          else
                  {
                        passf3(idot, l1, ch, c, wtable, iw, isign);
                  }
	          na=1-na;
	      }
	      else if(ip==5)
	      {
	          if(na==0)
                  {
                         passf5(idot, l1, c, ch, wtable, iw, isign);
                  }
	          else
                  {
                         passf5(idot, l1, ch, c, wtable, iw, isign);     
                  }
	          na=1-na;
	      }
	      else
	      {
	          if(na==0)
                  {
                        passfg(nac, idot, ip, l1, idl1, c, c, c, ch, ch, wtable, iw, isign);
                  }
	          else
                  {
                        passfg(nac, idot, ip, l1, idl1, ch, ch, ch, c, c, wtable, iw, isign);
                  }
	          if(nac[0] !=0) na=1-na;
	      }
	      l1=l2;
	      iw+=(ip-1)*idot;
          }
          if(na==0) return;
          for(i=0; i<2*n; i++) c[i]=ch[i];
     } 

/*---------------------------------------------------------
   cfftf: Complex forward FFT
  --------------------------------------------------------*/
     void cfftf(int n, double c[], double wtable[])
     {
          cfftf1(n, c, wtable, -1);
     } 

/*---------------------------------------------------------
   cfftb: Complex borward FFT
  --------------------------------------------------------*/
     void cfftb(int n, double c[], double wtable[])
     {
          cfftf1(n, c, wtable, +1);
     } 

/*---------------------------------------------------------
   cffti1: further initialization of Complex FFT
  --------------------------------------------------------*/
     void cffti1(int n, double wtable[])
     {

          final int[] ntryh = {3, 4, 2, 5};
          final double twopi=2.0D*Math.PI;
          double  argh;
          int     idot, ntry=0, i, j;
          double  argld;
          int     i1, k1, l1, l2, ib;
          double  fi;
          int     ld, ii, nf, ip, nl, nq, nr;
          double  arg;
          int     ido, ipm;

           nl=n;
           nf=0;
           j=0;

      factorize_loop:
           while(true)
           {
               j++;
               if(j<=4)
	           ntry=ntryh[j-1];
               else
	           ntry+=2;
               do
               {
	            nq=nl / ntry;
	            nr=nl-ntry*nq;
	            if(nr !=0) continue factorize_loop;
	            nf++;
	            wtable[nf+1+4*n]=ntry;
	            nl=nq;
	            if(ntry==2 && nf !=1)
	            {
	                 for(i=2; i<=nf; i++)
	                 {
		             ib=nf-i+2;
		             wtable[ib+1+4*n]=wtable[ib+4*n];
	                 }
	                 wtable[2+4*n]=2;
	            }
               } while(nl !=1);
               break factorize_loop;
           }
           wtable[0+4*n]=n;
           wtable[1+4*n]=nf;
           argh=twopi /(double)n;
           i=1;
           l1=1;
           for(k1=1; k1<=nf; k1++)
           {
	       ip=(int)wtable[k1+1+4*n];
	       ld=0;
	       l2=l1*ip;
	       ido=n / l2;
	       idot=ido+ido+2;
	       ipm=ip-1;
	       for(j=1; j<=ipm; j++)
	       {
	           i1=i;
	           wtable[i-1+2*n]=1;
	           wtable[i+2*n]=0;
	           ld+=l1;
	           fi=0;
	           argld=ld*argh;
	           for(ii=4; ii<=idot; ii+=2)
	           {
		       i+=2;
		       fi+=1;
		       arg=fi*argld;
		       wtable[i-1+2*n]=Math.cos(arg);
		       wtable[i+2*n]=Math.sin(arg);
	           }
	           if(ip>5)
	           {
		       wtable[i1-1+2*n]=wtable[i-1+2*n];
		       wtable[i1+2*n]=wtable[i+2*n];
	           }
	       }
	       l1=l2;
           }

     } 

/*---------------------------------------------------------
   cffti:  Initialization of Real forward FFT
  --------------------------------------------------------*/
     void cffti(int n, double wtable[])
     {
         if(n==1) return;
         cffti1(n, wtable);
     }	/*cffti*/

}
