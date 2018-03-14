import numpy as np

def dumb_zo_zt(zo, zt, lo, config=None):
    '''
    Simple P(z_o|z_t,lambda_o) for a test run of the pipeline.
    '''
    # TODO: convert to probability!
    pr_zo = zt * (lo / 50.0) + 0.02

    return pr_zo
