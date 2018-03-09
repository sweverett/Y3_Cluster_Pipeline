#include <yaml-cpp/yaml.h>
#include <dlfcn.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

// PDF type definitions for all PDF pointers
// NOTE: This was working under the assumption of passing a config dict/map to the
// PDF's for extra model parameters as void ptrs. We can remove them and pick a better
// design choice.
// These PDF's are defined in Section 3 of the following document:
// https://www.overleaf.com/13697016cyvvqqfchfbg#/52989522/
typedef double (* pdf_mor)(double, double, double, void *);
typedef double (* pdf_lo_lc)(double, double, double, void *);
typedef double (* pdf_lc_lt)(double, double, void *);
typedef double (* pdf_zo_zt)(double, double, double, void *);
typedef double (* pdf_roffset)(double, void *);
typedef double (* pdf_A_cen)(double, double, double, double, void *);
typedef double (* pdf_A_mis)(double, double, double, double, double, void *);
typedef double (* pdf_hmf)(double, double, void *);
typedef double (* pdf_T_cen)(double, double, void *);
typedef double (* pdf_T_mis)(double, double, double, void *);
typedef double (* pdf_del_sig_cen)(double, double, void *);
typedef double (* pdf_del_sig_mis)(double, double, double, void *);

// Test example function from this module that could be called by integrand loader
extern "C" double test_func(double) {
    return 1;
}

// The gamma_t and N integrands are very similar, so here's a parent class
class Integrand {

protected:
  std::map<std::string, double(*)(double)> pdfs; // TODO: Make work for inhomogenous case!
  double fcen;
  pdf_mor mor;
  pdf_lo_lc lo_lc;
  pdf_lc_lt lc_lt;
  pdf_zo_zt zo_zt;
  pdf_roffset roffset;
  pdf_A_cen A_cen;
  pdf_A_mis A_mis;
  pdf_hmf hmf;
};

// This is still a scalar integrand; should vectorize in future
class Gamma_T_Integrand: public Integrand {
  // TODO: Constructor using load_integrand() below

  // Below works for homogeneous function arguments, but not for our case
  // std::vector<double (*)(double)> pdfs;

  void set_fcen(double fc) { fcen = fc; }

  Gamma_T_Integrand();

  // This eventually will be the integrand for gamma_t
  // NOTE: Again, we can replace config for a better solution
  double operator()(double lo, double lc, double lt, double zo, double zt,
                    double r, double R, double m, double A, void * config) {

    // Could get PDF outputs given MC sampling instead:
    // double mof = pdf_mof(lt, m, zt, config);

    // TODO: Make method for calculating msci (mean sig_crit inverse)

    double gamma_t_cen = msci
      * hmf(m, zt, config)
      * zo_zt(zo, zt, lo, config)
      * (
         fcen
         * exp(A * T_cen(r, m, config))
         * del_sig_cen(r, m, config)
         * A_cen(A, lc, m, zt, config)
         * lc_lt(lc, lt, config)
         * mor(lt, m, zt, config)
         );

    double gamma_t_mis = msci
      * hmf(m, zt, config)
      * zo_zt(zo, zt, lo, config)
      * (
         (1-fcen)
         * exp(A * T_mis(r, m, R, config))
         * del_sig_mis(r, m, R, config)
         * A_mis(A, lc, m, zt, R, config)
         * lo_lc(lo, lc, R, config)
         * lc_lt(lc, lt, config)
         * mor(lt, m, zt, config)
         * roffset(R, config)
         );

    // If PDFs are computed beforehand:
    // double gamma_t_cen = hmf * pr_zo_zt * msci * (
    //               fcen * (np.exp(A*T_cen) * delta_sigma_cen)
    //               * pr_a_cen * pr_lc_lt * pr_mor );

    // double gamma_t_mis = hmf * pr_zo_zt * msci * (
    //               (1-fcen) * (np.exp(A*T_mis) * delta_sigma_mis)
    //               * pr_a_mis * pr_lo_lc * pr_lc_lt * pr_roffset );

    // TODO: Actually calculate Nw. It is itself a multi-dimensional integral for each
    // sampling, so this will take some thought
    double Nw = 1.0;
    double gamma_t = (1.0 / Nw) * (gamma_t_cen + gamma_t_mis);

    return gamma_t

  }

private:
  pdf_T_cen T_cen;
  pdf_T_mis T_mis;
  pdf_del_sig_cen del_sig_cen;
  pdf_del_sig_mis del_sig_mis;
};

// Unfinished first pass at a constructor
Gamma_T_Integrand::Gamma_T_Integrand(const std::string& config_file) {
  // load options from the .ini file
  // TODO: convert below from yaml to ini!
  const auto config = YAML::LoadFile(config_file);
  const auto extra_files =
    config["extra-files"].as<std::vector<std::string>>();
  const auto pdf_names = config["pdfs"].as<std::vector<std::string>>();

  // load PDFs from our extra files
  for (auto& file: extra_files) {
      auto handle = dlopen(file.c_str(), RTLD_LAZY | RTLD_GLOBAL);
      if (!handle) {
          std::cerr << "ERROR: could not load file " << file << std::endl;
          std::cerr << dlerror() << std::endl;
          exit(EXIT_FAILURE);
      }
      for (auto& name: pdf_names) {
          // if pdfs aren't found, they will be null
          if (!pdfs[name]) {
              pdfs[name] = (double (*)(double)) dlsym(handle, name.c_str());
          }
      }
  }
  // load anything remaining from local namespace (e.g. our test func)
  {
      auto handle = dlopen(nullptr, RTLD_LAZY | RTLD_GLOBAL);
      for (auto& name: pdf_names) {
          // if pdfs aren't found, they will be null
          if (!pdfs[name]) {
              pdfs[name] = (double (*)(double)) dlsym(handle, name.c_str());
          }
      }
  }
  // check success
  // Integrator integrator;
  for (auto& name: pdf_names) {
      auto pdf = pdfs.at(name);
      if (!pdf) {
          std::cerr << "ERROR: could not load function " << name << std::endl;
          exit(EXIT_FAILURE);
      }
      pdfs.push_back(pdf);
  }
  return;
}

//***********************************************************************************
// This is some prototype code (w/ Devon's help) that tested setting PDF function
// implementations dynamically without needing to recompile the main pipeline code.
// It's similar to what I want to accomplish, but is too simple as it assumes all
// PDFs have the same argument signature.
//
// PDFs are set in 'config.yaml', and the PDF implementations are in lib1.cpp and
// lib2.cpp (as well as one local one).

// Dummy Integrator class
// Actually 'integrates' w/o Cuba to check if the dynamic loading works
struct Integrator {
  std::vector<double (*)(double)> pdfs;
  double integrate(double low, double high, double dx) {
    double sum = 0;
    for (auto x = low; x < high; x += dx) {
      double product = 1;
      for (auto& pdf : pdfs) {
        product *= pdf(x);
      }
      sum += product * dx;
    }
    return sum;
  }
};

// Set PDF implementations to those chosen in the config
Integrator load_integrator(const std::string& config_file) {
    // load options from "config.yaml"
    const auto config = YAML::LoadFile(config_file);
    const auto extra_files =
        config["extra-files"].as<std::vector<std::string>>();
    const auto pdf_names = config["pdfs"].as<std::vector<std::string>>();

    // load pdf1, pdf2 from our extra files
    // NOTE: I'm trying to find a non-hacky way to use a heterogeneous function
    // ptr container to do this for our case. If not, we can set each pdf by hand
    std::map<std::string, double(*)(double)> pdfs;
    for (auto& file: extra_files) {
        auto handle = dlopen(file.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!handle) {
            std::cerr << "ERROR: could not load file " << file << std::endl;
            std::cerr << dlerror() << std::endl;
            exit(EXIT_FAILURE);
        }
        for (auto& name: pdf_names) {
            // if pdfs aren't found, they will be null
            if (!pdfs[name]) {
                pdfs[name] = (double (*)(double)) dlsym(handle, name.c_str());
            }
        }
    }
    // load anything remaining from local namespace (e.g. our test func)
    {
        auto handle = dlopen(nullptr, RTLD_LAZY | RTLD_GLOBAL);
        for (auto& name: pdf_names) {
            // if pdfs aren't found, they will be null
            if (!pdfs[name]) {
                pdfs[name] = (double (*)(double)) dlsym(handle, name.c_str());
            }
        }
    }
    // check success and load into integrator
    Integrator integrator;
    for (auto& name: pdf_names) {
        auto pdf = pdfs.at(name);
        if (!pdf) {
            std::cerr << "ERROR: could not load function " << name << std::endl;
            exit(EXIT_FAILURE);
        }
        integrator.pdfs.push_back(pdf);
    }
    return integrator;
}

int main(int argc, char **argv) {
    auto integrator = load_integrator("config.yaml");
    std::cout << "Result: " << integrator.integrate(0, 1, 0.001) << std::endl;
}
