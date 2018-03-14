
import numpy as np

def dumb_lc_lt(lc, lt, config=None):
    '''
    Simple P(lambda_c|lambda_t) for a test run of the pipeline.
    '''
    sig = 5.0
    p_lc = np.random.normal(loc=lt, scale=sig)
    return p_lc
