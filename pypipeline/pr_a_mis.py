import numpy as np

def dumb_a_mis(A, lc, m, zo, R, config=None):
    '''
    Simple P(A|lambda_c, m, zo, R) for a test run of the pipeline.
    '''
    a = (lc / 50.0) * (np.log(m) / 15.0)
    b = 2.0 * zo * (R / 0.5) # in r_lambda
    p_A = np.random.beta(a,b)

    return p_A
