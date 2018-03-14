from cosmosis.datablock import names, option_section
import numpy as np

def setup(options):
    return 0

def execute(block, config):
    model_gamma_t = block['model_values', 'gamma_t']
    data_gamma_t = 1.0
    m = model_gamma_t - data_gamma_t
    likelihood = np.random.normal(loc=m)
    block['likelihood', 'likelihood'] = likelihood

    return 0
