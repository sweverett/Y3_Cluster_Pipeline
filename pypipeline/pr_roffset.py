import numpy as np

def dumb_roffset(R, config=None):
    '''
    Simple R-offset distribution for a test run of the pipeline.
    '''

    p_R = np.exp(-R)
    return p_R
