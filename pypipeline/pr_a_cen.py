import numpy as np

def dumb_a_cen(A, lc, m, zo, config=None):
    '''
    Simple P(A|lambda_c, m, zo) for a test run of the pipeline.
    '''
    a = (lc / 50.0) * (np.log(m) / 15.0)
    b = 2.0 * zo
    p_A = np.random.beta(a,b)
    return p_A
