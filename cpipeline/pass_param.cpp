
#include <cmath>
#include <iostream>
#include <map>
#include <string>

struct Gaussian2D_pdf_config {
    double x_width;
    double x_mean;
    double y_width;
    double y_mean;
};

void* parse_config(void *config) {
    const auto conf =
        *reinterpret_cast<const std::map<std::string, std::string> *>(config);
    return new Gaussian2D_pdf_config {
        std::stod(conf.at("x_width")),
        std::stod(conf.at("x_mean")),
        std::stod(conf.at("y_width")),
        std::stod(conf.at("y_mean"))
    };
}

void delete_config(void *config) {
    delete reinterpret_cast<const Gaussian2D_pdf_config *>(config);
}

double Gaussian2D_pdf(double x, double y, void *config) {
    const auto conf = *reinterpret_cast<const Gaussian2D_pdf_config *>(config);
    return 1.0 / (2.0 * M_PI * conf.x_width * conf.y_width)
        * std::exp(-std::pow((x - conf.x_mean) / conf.x_width, 2.0) / 2.0)
        * std::exp(-std::pow((y - conf.y_mean) / conf.y_width, 2.0) / 2.0);
}

// -------------
// --- y3ccp ---
// -------------

typedef std::map<std::string, std::string> config_params;
typedef double (* some_fun_t)(double, double, void *);

// function which loads config from some file (eventually 'ini' sub-block)
config_params load_config() {
    return {
        {"x_width", "1"},
        {"x_mean", "0"},
        {"y_width", "1"},
        {"y_mean", "0"}
    };
}

double do_integral(some_fun_t f, void *config) {
    double sum = 0;
    double dx = 0.01;
    double dy = 0.01;
    for (double x = -20; x < 20; x += dx) {
        for (double y = -20; y < 20; y += dy) {
            sum += f(x, y, config) * dx * dy;
        }
    }
    return sum;
}

int main() {
    auto file_config = load_config();
    void *config = parse_config(&file_config);
    auto result = do_integral(Gaussian2D_pdf, config);
    std::cout << result << std::endl;
    delete_config(config);
}
