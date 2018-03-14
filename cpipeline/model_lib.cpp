// model_lib.cpp

#include<iostream>
#include<map>
#include<string>
#include <functional>
// For the moment, we will register allowed function implementations here
// (Devon): if these are all global functions, we can use double(*)(double) instead
// (or whatever the function pointer syntax is)
// const static std::map<std::string, function<double(double)> or_trs = {
//   {"devon", or_tr_devon},
//   {"yuanyuan", or_tr_yuanuan},

//   {"tesla", or_tr_tesla}
// };

// NOTE: Likely will not be using this registry anymore...
// model_functions["mor"]["test_mor"] = "Value";

// Register any function implementation name in the correct std::map,
// along with an identifier (doesn't have to be same name)
// const static std::map<std::string, std::function<double(double)> > mor_funs = {
//     {"test_mor", test_mor}
// };
// const static std::map<std::string, std::function<double(double)> > lo_lc_funs = {
//     {"test_lo_lc", test_lo_lc}
// };
// const static std::map<std::string, std::function<double(double)> > lc_lt_funs = {
//     {"test_lc_lt", test_lc_lt}
// };
// const static std::map<std::string, std::function<double(double)> > roffset_funs = {
//     {"test_roffset", test_roffset}
// };
// const static std::map<std::string, std::function<double(double)> > A_cen_funs = {
//     {"test_A_cen", test_A_cen}
// };
// const static std::map<std::string, std::function<double(double)> > A_mis_funs = {
//     {"test_A_mis", test_A_mis}
// };
// const static std::map<std::string, std::function<double(double)> > zo_zt_funs = {
//     {"test_zo_zt", test_zo_zt}
// };
// const static std::map<std::string, std::function<double(double)> > hmf_funs = {
//     {"test_hmf", test_hmf}
// };
// const static std::map<std::string, std::function<double(double)> > T_cen_funs = {
//     {"test_T_cen", test_T_cen}
// };
// const static std::map<std::string, std::function<double(double)> > T_mis_funs = {
//     {"test_T_mis", test_T_mis}
// };
// const static std::map<std::string, std::function<double(double)> > del_sig_cen_funs = {
//     {"test_del_sig_cen", test_del_sig_cen}
// };


// // Global dict that contains all model definitions
// const static std::map< std::string, std::map<std::string, std::function<double(double)> > > MODEL_FUNCTIONS = {
//     {"mor", mor_funs},
//     {"lo_lc", lo_lc_funs},
//     {"lc_lt", lc_lt_funs},
//     {"roffset", roffset_funs},
//     {"A_cen", A_cen_funs},
//     {"A_mis", A_mis_funs},
//     {"hmf", hmf_funs},
//     {"T_cen", T_cen_funs},
//     {"T_mis", T_mis_funs},
//     {"del_sig_cen", del_sig_cen_funs},
//     {"del_sig_mis", del_sig_mis_funs}
// };

// P(MOR|...) std::functions (`pr_mor`)
double test_mor(double lt, double m, double zt, void * config) {
  return 1.0;
}

// P(lo|lc) functions (`pr_lo_lc`)
double test_lo_lc(double lo, double lc, double R, void * config) {
  return 1.0;
}

// P(lc|lt) functions (`pr_lc_lt`)
double test_lc_lt(double lc, double lt, void * config) {
  return 1.0;
}

// P(R) functions (`pr_roffset`)
double test_roffset(double R, void * config) {
  return 1.0;
}

// P(A_cen|...) functions (`pr_a_cen`)
double test_A_cen(double A, double lc, double m, double zt, void * config) {
  return 1.0;
}

// P(A_mis|...) functions (`pr_a_mis`)
double test_A_mis(double A, double lc, double m, double zt, double R, void * config) {
  return 1.0;
}

// P(zo|zt...) functions (`pr_zo_zt`)
double test_zo_zt(double zo, double zt, double lo, void * config) {
  return 1.0;
}

// Halo-mass function (`hmf`)
double test_hmf(double m, double zt, void * config) {
  return 1.0;
}

// Centered Triaxiality functions (`T_cen`)
double test_T_cen(double r, double m, void * config) {
  return 1.0;
}

// Mis-centered Triaxiality functions (`T_mis`)
double test_T_mis(double r, double m, double R, void * config) {
  return 1.0;
}

// Centered delta-sigma functions (`del_sig_cen`)
double test_del_sig_cen(double r, double m, void * config) {
  return 1.0;
}

// Mis-centered delta-sigma functions (`del_sig_mis`)
double test_del_sig_cen(double r, double m, double R, void * config) {
  return 1.0;
}


// etc ...
