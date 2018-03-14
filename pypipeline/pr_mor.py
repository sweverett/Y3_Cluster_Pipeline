import numpy as np

def dumb_mor(lt, m, zt, config=None):
    '''
    Simple MOF for a test run of the pipeline.
    '''
    lambda_t = 10.0 * np.log(m) - 50.0

    return lambda_t

def better_mor():
    pass

def matteo_mor(ltr, m, z, config=None):
    import scipy.special as spc
    from scipy.interpolate import InterpolatedUnivariateSpline as ius
    def gauss(x,mu,sig):
        return np.exp(-0.5*(x-mu)**2./sig**2.)/np.sqrt(2.*np.pi*sig**2.)
    def l_sat(M,Mmin,Mpivot,a):
        return ((M-Mmin)/Mpivot)**a

    test_sigintr=np.loadtxt('./modules/PlobMz/data/skew_norm_talbes/sig_intr_grid.dat') # values of sigma intrisic
    test_lsat=np.loadtxt('./modules/PlobMz/data/skew_norm_talbes/l_sat_grid.dat') # values of lambda_sat
    skewl_M=np.loadtxt('./modules/PlobMz/data/skew_norm_talbes/skew_table.dat') # table for skewness values
    sig_skewnorml=np.loadtxt('./modules/PlobMz/data/skew_norm_talbes/sig_skew_table.dat') # table for sigma of skew-normal distribution
    # ETRAPOLATION GRID
    test_lsat2=np.append(test_lsat,np.arange(254,400,10))
    # print test_lsat2[:-6]
    def interp_at_x(xbins,param,xout):
        dummy=ius(xbins,param,k=1)
        return dummy(xout)

    skew_interp=np.zeros((len(test_sigintr),len(test_lsat2)))
    for nz in range(len(test_sigintr)):
        skew_interp[nz,:]=interp_at_x(test_lsat[:],skewl_M[nz,:],test_lsat2)
    skew=interpolate.interp2d(test_lsat2[:-6],test_sigintr,skew_interp[:,:-6], kind='linear')

    sigsk_interp=np.zeros((len(test_sigintr),len(test_lsat2)))
    for nz in range(len(test_sigintr)):
        sigsk_interp[nz,:]=interp_at_x(test_lsat[:],sig_skewnorml[nz,:],test_lsat2)
    sig_skew=interpolate.interp2d(test_lsat2,test_sigintr,sigsk_interp, kind='linear')

    ''' ## need to read this from config
    M_min=config['cluster_parameters']['M_min'] 
    M_pivot=config['cluster_parameters']['M_pivot'] 
    alpha=config['cluster_parameters']['alpha'] 
    sigm_intr=config['cluster_parameters']['sigma_intr'] 
    '''
    ### The following is just place holder
    #M_min=
    #M_pivot=
    #alpha=
    #sigm_intr=
    ####
    lsat=l_sat(m,M_min,M_pivot,alpha)
    skw=skew(lsat,sigm_intr)
    sig=sig_skew(lsat,sigm_intr)

    return gauss(ltr,lsat,sig)*spc.erfc(-skw*(ltr-lsat)/np.sqrt(2)/sig)/normalization 

