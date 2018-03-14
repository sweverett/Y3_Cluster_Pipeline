import numpy as np

def dumb_lo_lc(lo, lc, R, config=None):
    '''
    Simple P(lambda_o|lambda_c, R) for a test run of the pipeline.
    '''
    # TODO: convert to a probablity!
    pr_lo = lc * np.exp(-R) # R in r_lambda
    return pr_lo
