/* ========================================================================== */
/* FLYSPECK - CFSQP                                              */
/*                                                                            */
/* Nonlinear Inequalities, C++ Nonrigorous Numerical Optimization   */
/* Major GLPK Linear Programming Inequalities                               */
/* Author: Thomas C. Hales                                                    */
/* Date: 2009-06-17                                                           */
/* ========================================================================== */


// nonlinear inequalities for linear programming relaxation.
// basic functions to be studied: azim, rhazim, sol (3), tau_m (3).

#include <iomanip.h>
#include <iostream.h>
#include <math.h>
#include "Minimizer.h"
#include "numerical.h"


// lp2009.cc
// $ make lp2009.o
// $ ./lp2009.o

// constructor calls optimizer.  No need to place in main loop.
class trialdata {
public:
  trialdata(Minimizer M,char* s) {
    M.coutReport(s);
  };
};

int trialcount = 300;
double eps = 1.0e-6;
double s8 = sqrt(8.0);

void assert_(int j,char* u) {
  if (!j) { 
    cout << "!!FAILURE: " << u << "\n";
  }
}

int value(double t1,double x1) {
  return (abs(t1 - x1) < 1.0e-8);
}

double test_function() {
  assert_(value(0.0,1.0),"entering test");
  assert_(value(interp(2.0,3.0,3.0,4.0,1.0),2.0),"interp");
  assert_(value(c1(),0.1754796560918218),"c1");
  assert_(value(ly(3.0),-0.9230769230769229),"ly");
  assert_(value(rho(3.0),1.3374608770996574),"rho");
  assert_(value(rhazim(2.05,2.1,2.15,2.2,2.25,2.3),1.2238197064544751),"rhazim");
  assert_(value(tau_m(2.05,2.1,2.15,2.2,2.25,2.3),0.23816478183286893),"tau_m");
}

double tauq(double y1,double y2,double y3,double y4,double y5,double y6,double y7,double y8,double y9) {
  return tau_m(y1,y2,y3,y4,y5,y6)+tau_m(y7,y2,y3,y4,y8,y9);
}

double deltay(double y1,double y2,double y3,double y4,double y5,double y6) {
  return delta_x(y1*y1,y2*y2,y3*y3,y4*y4,y5*y5,y6*y6);
}

double deltaquad(int i,double y1,double y2,double y3,double y4,double y5,double y6,double y7,double y8,double y9) {
  return (i=0 ? deltay(y1,y2,y3,y4,y5,y6) : deltay(y7,y2,y3,y4,y8,y9));
}

//constraint: crossdiag > 2.52
void bigcross(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -crossdiag(y) +2.52;
};

//constraint: crossdiag > y[3]
void cross3(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -crossdiag(y) +y[3];
};


//constraint: y3+y4+y5 > 6.25
void bigtri(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -y[3]-y[4]-y[5] +6.25;
};


//constraint: y3+y4+y5 < 6.25
void smalltri(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = y[3]+y[4]+y[5] -6.25;
};


//constraint: y > 2.5
void testcon(int numargs,int whichFn,double* y, double* ret,void*) {
  switch(whichFn) {
  case 1: *ret = (-y[0] +2.5); break;
  default: *ret = -y[0] + 2.4; break;
  }
  
};

//constraint: crossdiag > 2.52, deltas positive.
void bigcrossdelta(int numargs,int whichFn,double* y, double* ret,void*) {
  switch(whichFn) {
  case 1: *ret = (-crossdiag(y) +2.52); break;
  case 2: *ret = -deltaquad(0,y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],y[8]); break;
  case 3: *ret = -deltaquad(1,y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],y[8]); break;
  }
};

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t44(int numargs,int whichFn,double* y, double* ret,void*) {
  double ee= 1.0e-8; // ee is not part of the inequality, for testing purposes only.
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5]) +ee ;
	}
Minimizer m44() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.25,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t44;
	return M;
}
trialdata d44(m44(),"ID[3336871894]:  tau_mar-triangle >=0");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t41(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5])  -0.2759;
	}
Minimizer m41() {
  double xmin[6]= {2,2,2,2,2.52,2.52};
  double xmax[6]= {2.52,2.52,2.52,2.52,s8,s8};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t41;
	return M;
}
trialdata d41(m41(),"ID[8880534953]:  APIECE-tau_mar");



////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t10(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m(y[0],y[1],y[2],y[3],y[4],y[5]) + 0.001
    - 0.18 * (y[0]+y[1]+y[2] - 6) - 0.125* (y[3]+y[4]+y[5]-6) ;
	}
Minimizer m10() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t10;
	//.
	return M;
}
trialdata d10(m10(),"ID[1395142356] tau_m:  tau_m-y-ineq");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t11(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = sol_y(y[0],y[1],y[2],y[3],y[4],y[5]) -
   0.55125
    - 0.196 *(y[3]+y[4]+y[5]-6) + 0.38*(y[0]+y[1]+y[2]-6);
	}
Minimizer m11() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t11;
	//.
	return M;
}
//compare J_544014470 from 1998
trialdata d11(m11(),"ID[7394240696]:  sol-ineq");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t13(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -sol_y(y[0],y[1],y[2],y[3],y[4],y[5]) + 0.5513
    + 0.3232 * (y[3]+y[4]+y[5]-6) 
    - 0.151*(y[0]+y[1]+y[2]-6);
	}
Minimizer m13() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t13;
	//.
	return M;
}
//compare J_38243071
trialdata d13(m13(),"ID[7726998381] tau_m:  sol-ineq");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t12(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y(y[0],y[1],y[2],y[3],y[4],y[5]) -
   1.2308
    +0.3639 *(y[1]+y[2]+y[4]+y[5]-8) - 0.235*(y[0]-2) - 0.685*(y[3]-2);
	}
Minimizer m12() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t12;
	//.
	return M;
}
//compare J_568731327 from 1998
trialdata d12(m12(),"model(azminA):ID[4047599236] tau_m:  dih-ineq");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t14(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -azim(y[0],y[1],y[2],y[3],y[4],y[5]) + 1.231
    - 0.152 * (y[1]+y[2]+y[4]+y[5]-8) 
    + 0.5*(y[0]-2)
    + 0.773*(y[3]-2);
	}
Minimizer m14() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t14;
	//.
	return M;
}
//compare J_507227930
trialdata d14(m14(),"ID[3526497018] tau_m:  dih-ineq");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t15(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = rhazim(y[0],y[1],y[2],y[3],y[4],y[5]) -
   1.2308
    +0.3639 *(y[1]+y[2]+y[4]+y[5]-8) - 0.60*(y[0]-2) - 0.685*(y[3]-2); 
	}
Minimizer m15() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t15;
	//.
	return M;
}
//compare J_507227930
trialdata d15(m15(),"ID[5957966880] tau_m:  rhazim-ineq");



////////// QUAD CASES:

// this is minimized.  failure reported if min is negative.
void t4a(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tauq(y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],y[8])  - 0.256;
	}
Minimizer m4a() {
  double xmin[9]= {2,2,2, s8,2,2,2,2,2};
  double xmax[9]= {2.52,2.52,2.52, 3,2.52,2.52, 2.52,2.52,2.52};
	Minimizer M(trialcount,9,1,xmin,xmax);
	M.func = t4a;
	M.cFunc = cross3;
	return M;
}
trialdata d4a(m4a(),"ID[4930647408] m4a: tauq: 0 tauq-quad-ineq > 0.256");


// this is minimized.  failure reported if min is negative.
void t4b(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = (y[1]+y[2]+y[4]+y[5]-8) - 2.75*(y[0]-s8);
	}
void c4b(int numargs,int whichFn,double* y, double* ret,void*) {
  switch(whichFn) {
  case 1: *ret = -y[3] + y[0]; break;
  case 2: *ret = -delta_y(y[0],y[1],y[2],y[3],y[4],y[5]);break;
  }
};
Minimizer m4b() {
  double xmin[6]= {s8,2,2, s8,2,2};
  double xmax[6]= {3,2.52,2.52,3,2.52,2.52};
	Minimizer M(trialcount,6,2,xmin,xmax);
	M.func = t4b;
	M.cFunc = c4b;
	return M;
}
trialdata d4b(m4b(),"ID[8673686234] m4b: super8 edge lengths");

// this is minimized.  failure reported if min is negative.
void t4c(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m(y[0],y[1],y[2],y[3],y[4],y[5]) - 0.128 -
    0.053*((y[4]+y[5]-4) - (2.75/2.0)*(y[3]-s8));
	}
Minimizer m4c() {
  double xmin[6]= {2,2,2, s8,2,2};
  double xmax[6]= {2.52,2.52,2.52,3,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t4c;
	return M;
}
trialdata d4c(m4c(),"ID[1642527039] m4c: superflat tau");

// this is minimized.  failure reported if min is negative.
void t4d(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5]) 
     -     0.053*((y[4]+y[5]-4) - (2.75/2.0)*(y[3]-s8))
     - 0.12
    -0.14132   *(y[0]+ y[1]/2.0 + y[2]/2.0  - 4)
    -0.328  *(y[4]+y[5] -4);
	}
Minimizer m4d() {
  double xmin[6]= {2,2,2, s8,2,2};
  double xmax[6]= {2.52,2.52,2.52,3,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t4d;
	return M;
}
trialdata d4d(m4d(),"ID[7863247282] m4d: superflat tau");


// this is minimized.  failure reported if min is negative.
void t4e(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y (y[0],y[1],y[2],y[3],y[4],y[5]) -1.903
    -0.4*(y[0]-2)
    +0.49688*(y[1]+y[2]+y[4]+y[5]-8)
    - 1.0*(y[3]-s8);
	}
Minimizer m4e() {
  double xmin[6]= {2,2,2, s8,2,2};
  double xmax[6]= {2.52,2.52,2.52,3,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t4e;
	return M;
}
trialdata d4e(m4e(),"ID[1085358243] m4e: superflat azim dart");

// this is minimized.  failure reported if min is negative.
void t4f(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -dih_y (y[0],y[1],y[2],y[3],y[4],y[5]) + 1.911
    +1.01*(y[0]-2)
    -0.284*(y[1]+y[2]+y[4]+y[5]-8)
    + 1.07*(y[3]-s8);
	}
Minimizer m4f() {
  double xmin[6]= {2,2,2, s8,2,2};
  double xmax[6]= {2.52,2.52,2.52,3,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t4f;
	return M;
}
trialdata d4f(m4f(),"ID[3566713650] m4f: superflat azim dart");

// this is minimized.  failure reported if min is negative.
void t4g(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y (y[0],y[1],y[2],y[3],y[4],y[5]) - 0.955
    - 0.2356*(y[0]-2)
    +0.32*(y[1]-2)
    +0.792*(y[2]-2)
    -0.707*(y[3]-2)
    +0.0844*(y[4]-2)
    +0.821*(y[5]-s8);
	}
Minimizer m4g() {
  double xmin[6]= {2,2,2, 2,2,s8};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,3};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t4g;
	return M;
}
trialdata d4g(m4g(),"ID[7718591733] m4g: superflat azim dart");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
/* moved to tame_hypermap.cc 
void t4(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tauq(y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],y[8]) +4.72*azim(y[0],y[1],y[2],y[3],y[4],y[5]) - 6.248;
	}
Minimizer m4() {
  double xmin[9]= {2,2,2, 2.52,2,2, 2,2,2};
  double xmax[9]= {2.52,2.52,2.52, 4.37,2.52,2.52, 2.52,2.52,2.52};
	Minimizer M(trialcount,9,1,xmin,xmax);
	M.func = t4;
	M.cFunc = bigcross;
	return M;
}
trialdata d4(m4(),"ID[7043724150] tauq: 0 tauq-quad-ineq");

*/

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
/* moved to tame_hypermap.cc 
void t3(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = azim(y[0],y[1],y[2],y[3],y[4],y[5]) - 0.852;
	}
Minimizer m3() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t3;
	//.
	return M;
}
trialdata d3(m3(),"ID[5735387903] dihmin");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t3a(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -azim(y[0],y[1],y[2],y[3],y[4],y[5]) + 1.893;
	}
Minimizer m3a() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t3a;
	//.
	return M;
}
trialdata d3a(m3a(),"ID[5490182221] dihmax");



////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t0(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m(y[0],y[1],y[2],y[3],y[4],y[5]) + 0.626*azim(y[0],y[1],y[2],y[3],y[4],y[5]) -0.77;
	}
Minimizer m0() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t0;
	//.
	return M;
}
trialdata d0(m0(),"ID[3296257235] tau_m0: 0th tau_m-tri-ineq");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t1(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m(y[0],y[1],y[2],y[3],y[4],y[5]) -0.259*azim(y[0],y[1],y[2],y[3],y[4],y[5]) +0.32;
	}
Minimizer m1() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t1;
	//.
	return M;
}
trialdata d1(m1(),"ID[8519146937] tau_m1: 1st tau_m-tri-ineq");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t2(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m(y[0],y[1],y[2],y[3],y[4],y[5]) -0.507*azim(y[0],y[1],y[2],y[3],y[4],y[5]) +0.724;
	}
Minimizer m2() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t2;
	//.
	return M;
}
trialdata d2(m2(),"ID[4667071578] tau_m2: 2nd tau_m-tri-ineq");
*/


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
/* moved to tame_hypermap.cc 
void t5(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tauq(y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],y[8]) +0.972*azim(y[0],y[1],y[2],y[3],y[4],y[5]) - 1.707;
	}
Minimizer m5() {
  double xmin[9]= {2,2,2, 2.52,2,2, 2,2,2};
  double xmax[9]= {2.52,2.52,2.52, 4.37,2.52,2.52, 2.52,2.52,2.52};
	Minimizer M(trialcount,9,1,xmin,xmax);
	M.func = t5;
	M.cFunc = bigcross;
	return M;
}
trialdata d5(m5(),"ID[6944699408] tauq: 1 tauq-quad-ineq");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t6(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tauq(y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],y[8]) 
    +0.7573*azim(y[0],y[1],y[2],y[3],y[4],y[5])
   - 1.433;
	}
Minimizer m6() {
  double maxv = 3.5; // should really go to 2*2.52
  double xmin[9]= {2,2,2, 2.52,2,2, 2,2,2};
  double xmax[9]= {2.52,2.52,2.52, maxv,2.52,2.52, 2.52,2.52,2.52};
	Minimizer M(trialcount,9,3,xmin,xmax);
	M.func = t6;
	M.cFunc = bigcrossdelta;
	return M;
}
trialdata d6(m6(),"ID[4240815464] tauq: 2 tauq-quad-ineq");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t9(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tauq(y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],y[8]) 
    -0.453*azim(y[0],y[1],y[2],y[3],y[4],y[5])
   + 0.777;
	}
Minimizer m9() {
  double maxv = 3.3; // should really go to 2*2.52
  double xmin[9]= {2,2,2, 2.52,2,2, 2,2,2};
  double xmax[9]= {2.52,2.52,2.52, maxv,2.52,2.52, 2.52,2.52,2.52};
	Minimizer M(trialcount,9,3,xmin,xmax);
	M.func = t9;
	M.cFunc = bigcrossdelta;
	return M;
}
trialdata d9(m9(),"ID[3862621143] tauq: 5 tauq-quad-ineq");




////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t8(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tauq(y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],y[8]) 
    //0*azim(y[0],y[1],y[2],y[3],y[4],y[5])
    //(azim(y[2],y[0],y[1],y[5],y[3],y[4])+azim(y[2],y[6],y[1],y[8],y[3],y[7])) 
   - 0.206;
	}
Minimizer m8() {
  double maxv = 3.3; // should really go to 2*2.52
  double xmin[9]= {2,2,2, 2.52,2,2, 2,2,2};
  double xmax[9]= {2.52,2.52,2.52, maxv ,2.52,2.52, 2.52,2.52,2.52};
	Minimizer M(trialcount,9,3,xmin,xmax);
	M.func = t8;
	M.cFunc = bigcrossdelta;
	return M;
}
trialdata d8(m8(),"ID[5464178191] tauq: 4 tauq-quad-ineq");
*/

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t7(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = y[0];
	}
Minimizer m7() {
  double xmin[1]= {2};
  double xmax[1]= {3};
	Minimizer M(trialcount,1,1,xmin,xmax);
	M.func = t7;
	M.cFunc = testcon;
	return M;
}
//trialdata d7(m7(),"test of constraints");








////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t16(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y(y[0],y[1],y[2],y[3],y[4],y[5]) -
   1.629
    +0.402 *(y[1]+y[2]+y[4]+y[5]-8) - 0.315*(y[0]-2) ;
	}
Minimizer m16() {
  double xmin[6]= {2,2,2,2.52,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t16;
	//.
	return M;
}
//compare J_568731327 from 1998
trialdata d16(m16(),"ID[3020140039]:  dih-quad-min-ineq");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t17(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y(y[0],y[1],y[2],y[3],y[4],y[5]) -
   1.91
    +0.458 *(y[1]+y[2]+y[4]+y[5]-8) - 0.342*(y[0]-2) ; 
	}
Minimizer m17() {
  double s8 = sqrt(8.0);
  double xmin[6]= {2,2,2,s8,2,2};
  double xmax[6]= {2.52,2.52,2.52,s8,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t17;
	//.
	return M;
}
//compare J_568731327 from 1998
trialdata d17(m17(),"ID[9414951439]:  dih-super8-min-ineq");



////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t30(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m(y[0],y[1],y[2],y[3],y[4],y[5])  
    -0.1  - 0.265*(y[4]+y[5]-4) -0.06 *(y[3]-2.52) -0.16*(y[0]-2) - 0.115*(y[1]+y[2]-4);
    	}
Minimizer m30() {
  double xmin[6]= {2,2,2,2.52,2,2};
  double xmax[6]= {2.52,2.52,2.52,s8,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t30;
	return M;
}
trialdata d30(m30(),"ID[8248508703] tau_m: flat quarter");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t33(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y(y[0],y[1],y[2],y[3],y[4],y[5]) -
   1.629
    +0.414 *(y[1]+y[2]+y[4]+y[5]-8) - 0.763*(y[3]-2.52) - 0.315*(y[0]-2) ;
	}
Minimizer m33() {
  double xmin[6]= {2,2,2,2.52,2,2};
  double xmax[6]= {2.52,2.52,2.52,s8,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t33;
	//.
	return M;
}
trialdata d33(m33(),"ID[3318775219]:  dih-flat-min-ineq");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t34(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -dih_y(y[0],y[1],y[2],y[3],y[4],y[5]) +
   1.6294
    -0.2213 *(y[1]+y[2]+y[4]+y[5]-8) +0.913*(y[3]-2.52) + 0.728*(y[0]-2) ;
	}
Minimizer m34() {
  double xmin[6]= {2,2,2,2.52,2,2};
  double xmax[6]= {2.52,2.52,2.52,s8,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t34;
	//.
	return M;
}
trialdata d34(m34(),"ID[9922699028]:  dih-flat-max-ineq");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t35(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y(y[1],y[2],y[0],y[4],y[5],y[3]) 
   -1.083
    +0.6365*(y[0]-2) - 0.198*(y[1]-2) + 0.352*(y[2]-2) + 0.416*(y[3]-2.52)
    -0.66*(y[4]-2) + 0.071*(y[5]-2);
	}
Minimizer m35() {
  double xmin[6]= {2,2,2,2.52,2,2};
  double xmax[6]= {2.52,2.52,2.52,s8,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t35;
	//.
	return M;
}
trialdata d35(m35(),"ID[5000076558]:  dih2-flat-min-ineq");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t36(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = rhazim(y[0],y[1],y[2],y[3],y[4],y[5]) 
   -1.629
    -0.866*(y[0]-2) + 0.3805*(y[1]-2) + 0.3805*(y[2]-2) - 0.841*(y[3]-2.52)
    +0.501*(y[4]-2) + 0.501*(y[5]-2);
	}
Minimizer m36() {
  double xmin[6]= {2,2,2,2.52,2,2};
  double xmax[6]= {2.52,2.52,2.52,s8,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t36;
	//.
	return M;
}
trialdata d36(m36(),"ID[9251360200]:  flat-rhazim-min-ineq");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t37(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = rhazim(y[1],y[2],y[0],y[4],y[5],y[3]) 
   -1.08
    +0.6362*(y[0]-2) - 0.565*(y[1]-2) + 0.359*(y[2]-2) +0.416*(y[3]-2.52)
    -0.666*(y[4]-2) + 0.061*(y[5]-2);
	}
Minimizer m37() {
  double xmin[6]= {2,2,2,2.52,2,2};
  double xmax[6]= {2.52,2.52,2.52,s8,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t37;
	//.
	return M;
}
trialdata d37(m37(),"ID[9756015945]:  flat-rhazim2-min-ineq");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t38(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y(y[0],y[1],y[2],y[3],y[4],y[5]) 
   -1.448
    -0.266*(y[0]-2) + 0.295*(y[1]-2) + 0.57*(y[2]-2) -0.745*(y[3]-2.52)
    +0.268*(y[4]-2) + 0.385*(y[5]-2.52);
	}
Minimizer m38() {
  double xmin[6]= {2,2,2,2.52,2,2.52};
  double xmax[6]= {2.52,2.52,2.52,s8,2.52,s8};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t38;
	//.
	return M;
}
trialdata d38(m38(),"ID[1812128999]:  APIECE-dih-non-apex");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t39(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y(y[0],y[1],y[2],y[3],y[4],y[5]) 
   -1.0705
    -0.1*(y[0]-2) + 0.424*(y[1]-2) + 0.424*(y[2]-2) -0.594*(y[3]-2)
    +0.124*(y[4]-2.52) + 0.124*(y[5]-2.52);
	}
Minimizer m39() {
  double xmin[6]= {2,2,2,2,2.52,2.52};
  double xmax[6]= {2.52,2.52,2.52,2.52,s8,s8};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t39;
	//.
	return M;
}
trialdata d39(m39(),"ID[5760733457]:  APIECE-dih-apex");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t40(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = rhazim(y[0],y[1],y[2],y[3],y[4],y[5]) 
   -1.0685
    -0.4635*(y[0]-2) + 0.424*(y[1]-2) + 0.424*(y[2]-2) -0.594*(y[3]-2)
    +0.124*(y[4]-2.52) + 0.124*(y[5]-2.52);
	}
Minimizer m40() {
  double xmin[6]= {2,2,2,2,2.52,2.52};
  double xmax[6]= {2.52,2.52,2.52,2.52,s8,s8};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t40;
	//.
	return M;
}
trialdata d40(m40(),"ID[2563100177]:  APIECE-rhazim-apex");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t42(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5])  
  -0.27
    +0.0295*(y[0]-2) -0.0778*(y[1]-2) -0.0778*(y[2]-2) -0.37*(y[3]-2)
    -0.27*(y[4]-2.52) -0.27*(y[5]-2.52);
	}
Minimizer m42() {
  double xmin[6]= {2,2,2,2,2.52,2.52};
  double xmax[6]= {2.52,2.52,2.52,2.52,s8,s8};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t42;
	return M;
}
trialdata d42(m42(),"ID[7931207804]:  APIECE-tau_mar-y");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t43(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5])  
  +0.0034
    -0.166*(y[0]+y[1]+y[2]-6)
    -0.22*(y[3]+y[4]+y[5]-6);
	}
Minimizer m43() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t43;
	return M;
}
trialdata d43(m43(),"ID[9225295803]:  tau_mar-smalltri");



////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t45(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y (y[0],y[1],y[2],y[3],y[4],y[5])  
    -1.23 - 0.235*(y[0]-2)   +0.362*(y[1]+y[2]-4)
    -0.694*(y[3]-2)    +0.26*(y[4]+y[5]-4);
	}
Minimizer m45() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t45;
	return M;
}
trialdata d45(m45(),"ID[9291937879]:  dihmin-smalltri");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t46(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5])  
        -0.05 - 0.137*(y[0]+y[1]+y[2]-6)
   -0.17*(y[3]+y[4]+y[5]-6.25);
	}
Minimizer m46() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,1,xmin,xmax);
	M.func = t46;
	M.cFunc = bigtri;
	return M;
}
trialdata d46(m46(),"ID[7761782916]:  tau_m-bigtri");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t47(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5])  - sol_y(y[0],y[1],y[2],y[3],y[4],y[5])
   +0.27564
    +2.43334*(pi()/2.0 - dih_y(y[0],y[1],y[2],y[3],y[4],y[5]));
	}
Minimizer m47() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t47;
	return M;
}
//trialdata d47(m47(),"ID[]:  tau_m-sol-tri");



////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t48(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5])  -0.1054
    -0.14132   *(y[0] + y[1]/2.0 + y[2]/2.0 - 4)
    -0.36499   *(y[4]+y[5]-4);
	}
Minimizer m48() {
  double xmin[6]= {2,2,2,s8,2,2};
  double xmax[6]= {2.52,2.52,2.52,3,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t48;
	return M;
}
trialdata d48(m48(),"ID[4840774900]:  tau_m-s8-quad, one diag <= 3");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t48a(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5]) 
    +tau_m(y[6],y[1],y[2],y[3],y[7],y[8])  - 0.24
    -0.14132   *(y[0] + y[1] + y[2] + y[6] - 8)
    -0.38  *(y[4]+y[5] + y[7]+y[8] -8);
	}
Minimizer m48a() {
  double xmin[9]= {2,2,2,s8,2,2,2,2,2};
  double xmax[9]= {2.52,2.52,2.52,3,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,9,1,xmin,xmax);
	M.func = t48a;
	M.cFunc = cross3;
	return M;
}
trialdata d48a(m48a(),"ID[5451229371]: m48a: tau_m-s8-quad, one diag <= 3");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t49(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y (y[0],y[1],y[2],y[3],y[4],y[5])  -2.09
    -0.54   *(y[0] -2)
    +0.578  *(y[1]+y[2]+y[4]+y[5]-8);
	}
Minimizer m49() {
  double xmin[6]= {2,2,2,3,2,2};
  double xmax[6]= {2.52,2.52,2.52,3,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t49;
	return M;
}
trialdata d49(m49(),"ID[9995621667]:  azim-sd8-quad, two diag >= 3");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t49a(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m(y[0],y[1],y[2],y[3],y[4],y[5]) +
    tau_m(y[6],y[1],y[2],y[3],y[7],y[8]) - 0.496;
	}
Minimizer m49a() {
  double fake = 3.8;  // (should really be 2*2.52)
  double xmin[9]= {2,2,2,3,2,2,2,2,2};
  double xmax[9]= {2.52,2.52,2.52,fake,2.52,2.52,2.52,2.52,2.52};
  Minimizer M(trialcount,9,1,xmin,xmax);
	M.func = t49a;
	M.cFunc = cross3;
	return M;
}
trialdata d49a(m49a(),"ID[9563139965]:  tau_mQ-sd8-quad, two diag >= 3");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t49c(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m(y[0],y[1],y[2],y[3],y[4],y[5]) +
    tau_m(y[6],y[1],y[2],y[3],y[7],y[8]) - 0.46 - (y[4]+y[5]+y[7]+y[8]-8.472)*0.45;
  // holds with -0.46 -> -0.49, but the weaker ineq is sufficient.
	}
Minimizer m49c() {
  double fake = 3.8;  // (should really be 2*2.52)
  double xmin[9]= {2,2,2,3,2,2,2,2,2};
  double xmax[9]= {2.52,2.52,2.52,fake,2.52,2.52,2.52,2.52,2.52};
  Minimizer M(trialcount,9,1,xmin,xmax);
	M.func = t49c;
	M.cFunc = cross3;
	return M;
}
trialdata d49c(m49c(),"49c: ID[7676202716]:  tau_mQ-sd8-quad, two diag >= 3");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t49b(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = y[4]+y[5]+y[7]+y[8]-8.0;
	}
Minimizer m49b() {
  double fake = 3.8;  // (should really be 2*2.52)
  double xmin[9]= {2,2,2,3,2,2,2,2,2};
  double xmax[9]= {2.52,2.52,2.52,fake,2.52,2.52,2.52,2.52,2.52};
  Minimizer M(trialcount,9,1,xmin,xmax);
	M.func = t49b;
	M.cFunc = cross3;
	return M;
}
trialdata d49b(m49b(),"ID[]:  m49b: perimeter-sd8-quad, two diag >= 3");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
/* sum over two adjacent elongated flats and combine with
    ID[9995621667] to get tau_mQ = tau_m1 + tau_m2 >= 2*0.231.
    on a quad with both diags >= 3.
   (Need to make a deformation argument to reduce to the case of one diag at most 3.3) 
 */
void t50(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5])  -0.231
    - 0.622*(dih_y(y[0],y[1],y[2],y[3],y[4],y[5])  -2.09/2
	     -0.54/2 *(y[0]-2)
	     +0.578*(y[1]+y[5]-4));
	}
Minimizer m50() {
  double xmin[6]= {2,2,2,2,3,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,3.3,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t50;
	return M;
}
//{deprecated} trialdata d50(m50(),"ID[5769230427]:  tau_m-sd8-quad, two diag >= 3");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t51(int numargs,int whichFn,double* y, double* ret,void*) {
  double t = 4.52 - y[4];
  *ret = dih_y (y[0],y[1],y[2],y[3],t,y[5])  -1.083
    -0.2   *(y[0] -2)
    +0.3778*(y[1]-2) + 0.151*(y[2]-2)
    -0.658*(y[3]-2)
        -0.053  *(y[4]-2)  
    + 0.1408*(y[5]-2);
	}
Minimizer m51() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.18,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t51;
	return M;
}
trialdata  d51(m51(),"ID[]:  azim-low");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t52(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = dih_y (y[0],y[1],y[2],y[3],y[4],y[5])  -1.230
    -0.2357   *(y[0] -2)
    +0.2493*(y[1]+y[2]-4)
    -0.682*(y[3]-2)
    +0.3035  *(y[4]+y[5]-4)  ;
	}
Minimizer m52() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.18,2.18,2.18,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t52;
	return M;
}
trialdata  d52(m52(),"ID[9229542852]:  azim-low-small");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t53(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = -dih_y (y[0],y[1],y[2],y[3],y[4],y[5])  + 1.232
    +0.261   *(y[0] -2)
    -0.203*(y[1]+y[2]-4)
    +0.772*(y[3]-2)
    -0.191  *(y[4]+y[5]-4)  ;
	}
Minimizer m53() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.18,2.18,2.18,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t53;
	return M;
}
trialdata  d53(m53(),"ID[1550635295]:  azimmax-low-small");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t54(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5])  + 0.0008
    -0.1631*(y[0]+y[1]+y[2]-6)
    -0.2127  *(y[3]+y[4]+y[5]-6)  ;
	}
Minimizer m54() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.18,2.18,2.18,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t54;
	return M;
}
trialdata  d54(m54(),"ID[4491491732]:  tau-low-small");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t77(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 1.2777;  
double c[6] = 
  {0.281, -0.278364, -0.278364, 0.7117, -0.34336, -0.34336};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = dih_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m77() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t77;
	return M;
}
trialdata  d77(m77(),"ID[2151506422]:  d77, dih >= high-low-low");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t78(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = -1.27799;  
double c[6] = 
  {-0.356217, 0.229466, 0.229466, -0.949067, 0.172726, 0.172726};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = - dih_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m78() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t78;
	return M;
}
trialdata  d78(m78(),"ID[6836427086]:  d78, dih <= high-low-low");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t79(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 0.0345;
double c[6] = 
  {0.185545, 0.193139, 0.193139, 0.170148, 0.13195, 0.13195};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = tau_m(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m79() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t79;
	return M;
}
trialdata  d79(m79(),"ID[3636849632]:  d79, tau >= .. high-low-low");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t80(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 1.277;
double c[6] = 
  {0.273298, -0.273853, -0.273853, 0.708818, -0.313988, -0.313988};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = dih_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m80() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t80;
	return M;
}
trialdata  d80(m80(),"ID[8657368829]:  d80, dih >= .. high-low-low-small");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t81(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = -1.27799;
double c[6] = 
  {-0.439002, 0.229466, 0.229466, -0.771733, 0.208429, 0.208429};
  //  {0.273298, -0.273853, -0.273853, 0.708818, -0.313988, -0.313988};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = -dih_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m81() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t81;
	return M;
}
trialdata  d81(m81(),"ID[6619134733]:  d81, dih <= .. high-low-low-small");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t82(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 1.185;
double c[6] = 
  {-0.372262, 0.214849, -0.163775, -0.293508, 0.656172, -0.267157};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = dih2_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m82() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t82;
	return M;
}
trialdata  d82(m82(),"ID[1284543870]:  d82, dih2 => .. high-low-low-small");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t83(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = -1.1864;
double c[6] = 
  {0.20758, -0.236153, 0.14172, 0.263109, -0.737003, 0.12047};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = -dih2_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m83() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.25,2.25,2.25};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t83;
	return M;
}
trialdata  d83(m83(),"ID[4041673283]:  d83, dih2 <= .. high-low-low-small");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 1.185;
double c[6] = 
  {-0.302913, 0.214849, -0.163775, -0.443449, 0.67364, -0.314532};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = dih2_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84;
	return M;
}
trialdata  d84(m84(),"ID[5298513205]:  d84, dih2 => .. high-low-low");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84a(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = -1.542;
double c[6] = 
  {-0.362519, 0.298691, 0.287065, -0.920785, 0.190917, 0.219132};
 double ymin[6]= {2.36, 2, 2, 2.25, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = -dih_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84a() {
  double xmin[6]= {2.18,2,2,2.25,2,2};
  double xmax[6]= {2.36,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84a;
	return M;
}
trialdata  d84a(m84a(),"ID[3872614111]:  1825a d84a,  dih<= .. high-low-low, 2.25 -+cutoffs");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84b(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = -1.542;
double c[6] = 
  {-0.346773, 0.300751, 0.300751, -0.702567, 0.172726, 0.172727};
 double ymin[6]= {2.36, 2, 2, 2.25, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = -dih_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84b() {
  double xmin[6]= {2.18,2,2,2.0,2,2};
  double xmax[6]= {2.36,2.18,2.18,2.25,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84b;
	return M;
}
trialdata  d84b(m84b(),"ID[3139693500]:  1800b d84b,  dih<= .. high-low-low, 2.25 --cutoffs");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84c(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = -1.542;
double c[6] = 
  {-0.490439, 0.318125, 0.32468, -0.740079, 0.178868, 0.205819};
  double ymin[6]= {2.36, 2, 2, 2.25, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = -dih_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84c() {
  double xmin[6]= {2.36,2,2,2.0,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.25,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84c;
	return M;
}
trialdata  d84c(m84c(),"ID[4841020453]:  3600c d84c,  dih<= .. high-low-low, 2.25 +-cutoffs");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84d(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = -1.542;
double c[6] = 
  {-0.490439, 0.321849, 0.320956, -1.00902, 0.240709, 0.218081};
 double ymin[6]= {2.36, 2, 2, 2.25, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = -dih_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84d() {
  double xmin[6]= {2.36,2,2,2.25,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84d;
	return M;
}
trialdata  d84d(m84d(),"ID[9925287433]:  3625d d84d,  dih<= .. high-low-low, 2.25 ++cutoffs");



////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84e(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 1.0494;
double c[6] = 
  {-0.297823, 0.215328, -0.0792439, -0.422674, 0.647416, -0.207561};
//  {-0.310445, 0.214944, -0.0914828, -0.422674, 0.619404, -0.179549};
 double ymin[6]= {2.36, 2, 2, 2.25, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = dih2_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84e() {
  double xmin[6]= {2.18,2,2,2.25,2,2};
  double xmax[6]= {2.36,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84e;
	return M;
}
trialdata  d84e(m84e(),"ID[7409690040]:  1825e d84e,  dih2>= .. high-low-low, 2.25 -+cutoffs");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84f(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 1.0494;
double c[6] = 
  {-0.29013, 0.215328, -0.0715511, -0.267157, 0.650269, -0.295198};
//  {-0.293139, 0.214623, -0.0738556, -0.267157, 0.650269, -0.291848};
 double ymin[6]= {2.36, 2, 2, 2.25, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = dih2_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84f() {
  double xmin[6]= {2.18,2,2,2.0,2,2};
  double xmax[6]= {2.36,2.18,2.18,2.25,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84f;
	return M;
}
trialdata  d84f(m84f(),"ID[4002562507]:  1800f d84f,  dih2>= .. high-low-low, 2.25 --cutoffs");


////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84g(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 1.0494;
double c[6] = 
  {-0.404131, 0.212119, -0.0402827, -0.299046, 0.643273, -0.266118};
  double ymin[6]= {2.36, 2, 2, 2.25, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = dih2_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84g() {
  double xmin[6]= {2.36,2,2,2.0,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.25,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84g;
	return M;
}
trialdata  d84g(m84g(),"ID[5835568093]:  3600g d84g,  dih2>= .. high-low-low, 2.25 +-cutoffs");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t84h(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = 1.0494;
double c[6] = 
  {-0.401543, 0.207551, -0.0294227, -0.494954, 0.605453, -0.156385};
 double ymin[6]= {2.36, 2, 2, 2.25, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = dih2_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m84h() {
  double xmin[6]= {2.36,2,2,2.25,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t84h;
	return M;
}
trialdata  d84h(m84h(),"ID[1894886027]:  3625h d84h,  dih2>= .. high-low-low, 2.25 ++cutoffs");




////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t85(int numargs,int whichFn,double* y, double* ret,void*) {
  double f0 = -1.1865;
double c[6] = 
  {0.20758, -0.236153, 0.14172, 0.263834, -0.771203, 0.0457292};
 double ymin[6]= {2.18, 2, 2, 2, 2, 2};
 double r = f0;
 for (int i=0;i<6;i++) r += c[i]*(y[i]-ymin[i]);
  *ret = -dih2_y(y[0], y[1],y[2],y[3],y[4],y[5]) -    r;
	}
Minimizer m85() {
  double xmin[6]= {2.18,2,2,2,2,2};
  double xmax[6]= {2.52,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t85;
	return M;
}
trialdata  d85(m85(),"ID[7743522046]:  d85, dih2 <= .. high-low-low");






////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t55(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = tau_m (y[0],y[1],y[2],y[3],y[4],y[5]) -0.1413
    -0.214*(y[0]-2.18)
    -0.1259*(y[1]+y[2]-4)
    -0.067*(y[3]-2.52)
    -0.241*(y[4]+y[5]-4)  ;
	}
Minimizer m55() {
  double xmin[6]= {2.18,2,2,2.52,2,2};
  double xmax[6]= {2.52,2.18,2.18,s8,2.52,2.52};
	Minimizer M(trialcount,6,0,xmin,xmax);
	M.func = t55;
	return M;
}
trialdata  d55(m55(),"ID[8282573160]:  tau-flat-high-low");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t56(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = sol_y (y[0],y[1],y[2],y[3],y[4],y[5]) -0.589   
    +0.24*(y[0]+y[1]+y[2]-6)
    -0.16*(y[3]+y[4]+y[5]-6.25);
	}
Minimizer m56() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.18,2.18,2.18,2.52,2.52,2.52};
	Minimizer M(trialcount,6,1,xmin,xmax);
	M.func = t56;
	M.cFunc = bigtri;
	return M;
}
trialdata  d56(m56(),"ID[8611785756]:  big-sol-low");

////////// NEW INEQ
// this is minimized.  failure reported if min is negative.
void t57(int numargs,int whichFn,double* y, double* ret,void*) {
  *ret = sol_y (y[0],y[1],y[2],y[3],y[4],y[5]) -0.589   
    +0.39*(y[0]+y[1]+y[2]-6)
    -0.235*(y[3]+y[4]+y[5]-6.25);
	}
Minimizer m57() {
  double xmin[6]= {2,2,2,2,2,2};
  double xmax[6]= {2.52,2.52,2.52,2.52,2.52,2.52};
	Minimizer M(trialcount,6,1,xmin,xmax);
	M.func = t57;
	M.cFunc = bigtri;
	return M;
}
trialdata  d57(m57(),"ID[6224332984]:  big-sol");



int main() {
  test_function();
}
