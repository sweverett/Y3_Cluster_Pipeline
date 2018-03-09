// model_lib.h

using namespace std

// Used to build MODEL_FUNCTIONS
const static map<string, function<double(double)> > mor_funs;
const static map<string, function<double(double)> > lo_lc_funs;
const static map<string, function<double(double)> > lc_lt_funs;
const static map<string, function<double(double)> > roffset_funs;
const static map<string, function<double(double)> > A_cen_funs;
const static map<string, function<double(double)> > A_mis_funs;
const static map<string, function<double(double)> > zo_zt_funs;
const static map<string, function<double(double)> > hmf_funs;
const static map<string, function<double(double)> > T_cen_funs;
const static map<string, function<double(double)> > T_mis_funs;
const static map<string, function<double(double)> > del_sig_cen_funs;
const static map<string, function<double(double)> > del_sig_mis_funs;

const static map< string, map<string, function<double(double)> > > MODEL_FUNCTIONS;

// P(MOR|...) functions (`pr_mor`)
double test_mor(double lt, double m, double zt, void * config);

// P(lo|lc) functions (`pr_lo_lc`)
double test_lo_lc(double lo, double lc, double R, void * config);

// P(lc|lt) functions (`pr_lc_lt`)
double test_lc_lt(double lc, double lt, void * config);

// P(R) functions (`pr_roffset`)
double test_roffset(double R, void * config);

// P(A_cen|...) functions (`pr_a_cen`)
double test_A_cen(double A, double lc, double m, double zt, void * config);

// P(A_mis|...) functions (`pr_a_mis`)
double test_A_mis(double A, double lc, double m, double zt, double R, void * config);

// P(zo|zt...) functions (`pr_zo_zt`)
double test_zo_zt(double zo, double zt, double lo, void * config);

// Halo-mass function (`hmf`)
double test_hmf(double m, double zt, void * config);

// Centered Triaxiality functions (`T_cen`)
double test_T_cen(double r, double m, void * config);

// Mis-centered Triaxiality functions (`T_mis`)
double test_T_mis(double r, double m, double R, void * config);

// Centered delta-sigma functions (`del_sig_cen`)
double test_del_sig_cen(double r, double m, void * config);

// Mis-centered delta-sigma functions (`del_sig_mis`)
double test_del_sig_cen(double r, double m, double R, void * config);

// etc ...
