#include <yaml-cpp/yaml.h>
#include <dlfcn.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

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

Integrator load_integrator(const std::string& config_file) {
    const auto config = YAML::LoadFile(config_file);
    const auto extra_files =
        config["extra-files"].as<std::vector<std::string>>();
    const auto pdf_names = config["pdfs"].as<std::vector<std::string>>();

    std::map<std::string, double(*)(double)> pdfs;
    for (auto& file: extra_files) {
        auto handle = dlopen(file.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!handle) {
            std::cerr << "ERROR: could not load file " << file << std::endl;
            std::cerr << dlerror() << std::endl;
            exit(EXIT_FAILURE);
        }
        for (auto& name: pdf_names) {
            if (!pdfs[name]) {
                pdfs[name] = (double (*)(double)) dlsym(handle, name.c_str());
            }
        }
    }
    {
        auto handle = dlopen(nullptr, RTLD_LAZY | RTLD_GLOBAL);
        for (auto& name: pdf_names) {
            if (!pdfs[name]) {
                pdfs[name] = (double (*)(double)) dlsym(handle, name.c_str());
            }
        }
    }
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

// Test example function from this module that could be called by integrand loader
extern "C" double test_func(double) {
  return 0.5;
}

int main(int argc, char **argv) {
    auto integrator = load_integrator("config.yaml");
    std::cout << "Result: " << integrator.integrate(0, 1, 0.001) << std::endl;
}
