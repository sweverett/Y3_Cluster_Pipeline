#include "cosmosis/datablock/datablock.hh"
#include "cosmosis/datablock/section_names.h"
#include "cubacpp/cubacpp.hh"
// TODO: Make a library file for each model function!
#include "model_lib.h"
#include "map.h"
// #include "my_calculation_code.h"

using namespace std

// For convenience (TODO: Can get from section_names)
string cosmo = "cosmological_parameters";

// Used for function map
// typedef void (*pfunc)(string);

// map<string, pfunc> funcMap; 
// funcMap["f1"] = f1; //and so forth

extern "C" {

  void * setup(DataBlock * options)
  {
    // Read options from the CosmoSIS configuration ini file,
    // passed via the "options" argument

    // We will save our model choices & mc options in maps
    // std::map<std::string> config
    map<string, pfunc> model_functions;
    map<string, double> int_params;

    vector<string> functions = {"mor", "lo_lc", "lc_lt", "roffset", "a_cen", "a_mis", "zo_zt", "T_cen",
                                "T_mis", "hmf", "del_sig_cen", "del_sig_mis"};

    for (string func : functions) {
      func_model = options->get_string("cluster_model_parameters", func);
      model_functions[func] = MODEL_FUNCTIONS[func][func_model];
    }

    // Set any global variables required
    double abs_err, rel_err;
    block->get_double("integral_parameters", "abs_err", 1.0e3-10, abs_err);
    block->get_double("integral_parameters", "rel_err", 1.03-3, rel_err);

    int_params["abs_err"] = abs_err;
    int_params["rel_err"] = rel_err;

    // Record any configuration information required

    // Pass back any object you like
    return config;
  }

  DATABLOCK_STATUS execute(DataBlock * block, void * config)
  {
    // Config is whatever you returned from setup above
    // Block is the collection of parameters and calculations for
    // this set of cosmological parameters
    DATABLOCK_STATUS status = 0;

    // TODO: Grab relevant info from config

    // Grab needed values from the block
    double omega_m, omega_b, h0, fcen
    block->get_val(cosmo, "omega_m", omega_m)
    Omega_b = block->get_val(cosmo, "omega_b", omega_b)
    block->get_double(cosmo, "h0", h0)
    block->get_double("cluster_model_parameters", "fcen", fcen)

    // We tell CUBA not to use mutiple processes. We intend our wrappers to be
    // used in MPI program, so parallelization through CUBA's internal forking
    // mechanism would cause conflicts with MPI.
    cubacores(0, 0);

    MyFunc ff{3.0};

    cout << scientific; // control output formatting of floating-point.

    // Our integration functions return a convergence status code, a value for the
    // integral, an uncertaintly, and a "chi-squared probability". We can unpack
    // them all on one line, using a "structured binding".
    auto[val, err, prob, neval, status] =
      cubacpp::VegasIntegrate(ff, int_params["rel_err"], int_params["abs_err"]);
    cout << "Vegas -> "
         << "Value: " << val << " +/- " << err << " prob: " << prob
         << " neval: " << neval << " status: " << status << std::endl;

    return status;
  }


  int cleanup(void * config)
  {
    // Config is whatever you returned from setup above
    // Free it 
  }

} // end of extern C
