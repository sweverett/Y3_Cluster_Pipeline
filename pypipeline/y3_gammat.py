from cosmosis.datablock import names, option_section
import numpy as np
import pudb

# TODO: Replace w/ general version!
from pr_mor import *
from pr_lo_lc import *
from pr_lc_lt import *
from pr_roffset import *
from pr_a_cen import *
from pr_a_mis import *
from pr_zo_zt import *
from hmf import *
from delta_sigma import *
from triaxiality import *

# For convenience
cosmo = names.cosmological_parameters

def setup(options):

    # We will save our model choices & mc options in dictionaries
    config = {}
    model_functions = {}
    bins = {}
    mc_options = {}

    # Load the model function names from the `.ini` file, and grab the
    # function objects from the local namespace (this allows for
    # any model defined in any local file to be used!)
    # TODO: Should make this adaptive!
    functions = ['mor', 'lo_lc', 'lc_lt', 'roffset', 'a_cen', 'a_mis', 'zo_zt', 'T_cen', 'T_mis',
                'hmf', 'del_sig_cen', 'del_sig_mis']
    for fun in functions:
	model_functions[fun] = globals()[options.get_string(option_section, fun)]
    #model_functions['mor'] = globals()[options.get_string(option_section, 'mor')]
    #model_functions['lo_lc'] = globals()[options.get_string(option_section, 'lo_lc')]
    #model_functions['lc_lt'] = globals()[options.get_string(option_section, 'lc_lt')]
    #model_functions['roffset'] = globals()[options.get_string(option_section, 'roffset')]
    #model_functions['a_cen'] = globals()[options.get_string(option_section, 'a_cen')]
    #model_functions['a_mis'] = globals()[options.get_string(option_section, 'a_mis')]
    #model_functions['zo_zt'] = globals()[options.get_string(option_section, 'zo_zt')]
    #model_functions['hmf'] = globals()[options.get_string(option_section, 'hmf')] # halo-mass function



    config['model_functions'] = model_functions

    # Integration bounds
    # TODO: Implement eventually!
    mc = None
    # mc['m_i'], mc['m_f'] = block[names.mc_parameters, 'm_i'], block[names.mc_parameters, 'm_i']
    # mc['zt_i'], mc['zt_f'] = block[names.mc_parameters, 'zt_i'], block[names.mc_parameters, 'zt_f']
    # mc['zo_i'], mc['zo_f'] = block[names.mc_parameters, 'zo_i'], block[names.mc_parameters, 'zo_f']
    # mc['lc'], mc['mf'] = block[names.mc_parameters, 'lc_i'], block[names.mc_parameters, 'lc_f']
    # mc['lo'], mc['mf'] = block[names.mc_parameters, 'lo_i'], block[names.mc_parameters, 'lo_f']
    # mc['lt'], mc['mf'] = block[names.mc_parameters, 'lt_i'], block[names.mc_parameters, 'lt_']
    # mc['Ri'], mc['Rf'] = block[names.mc_parameters, 'R_i'], block[names.mc_parameters, 'R_f']

    config['mc_options'] = mc

    # Read in binning scheme from the file
    config['bins'] = {}
    # TODO: Actually do it!
    z_edges = [0.2, 0.35, 0.5, 0.65]
    l_edges = [5.0, 10.0, 14, 20.0, 30.0, 45.0, 60.0, 999.0]

    n_theta = 15
    theta_min = 2.5
    theta_max = 250.0
    n_theta, log_theta_min, log_theta_max = n_theta, np.log(theta_min), np.log(theta_max)
    log_theta_edges = np.linspace(log_theta_min, log_theta_max, n_theta+1)

    # TODO: Write single dict for config output
    config['bins']['z_edges'] = z_edges
    config['bins']['l_edges'] = l_edges
    config['bins']['log_theta_edges'] = log_theta_edges

    # return model_functions, mc
    return config

def execute(block, config):

    # we'll name the model functions as just `model`
    model, mc, bins = config['model_functions'], config['mc_options'], config['bins']

    # External quantities that will come from CosmoSIS
    Omega_m = block.get_double(cosmo, 'omega_m')
    Omega_b = block.get_double(cosmo, 'omega_b')
    h0      = block.get_double(cosmo, 'h0')
    # TODO: make this adaptive for different parameter inputs defined by user!
    fcen    = block.get_double('cluster_model_parameters', 'fcen')

    # TODO: Config dictionary that will be passed to
    # all model functions
    config = None

    # Load lens z and mass from counts
    # z,M = get_zM() # someone write this
    # NOTE: For now, we just hardcode a 'sampling' of these values for a MC realization
    zt = 0.25 #Mean redshift of the lenses. This might end up being integrated over.
    zo = 0.245
    m = 1e14 #Msun/h; mass of the bin
    lc = 47.0
    lt = 50.0
    lo = 42.0
    R = 0.2 # MPc
    r = 0.15
    A = 0.1

    # Draw probability values given sampled inputs
    pr_mor = model['mor'](lt, m, zt, config)
    pr_lo_lc = model['lo_lc'](lo, lc, R, config)
    pr_lc_lt = model['lc_lt'](lc, lt, config)
    pr_roffset = model['roffset'](R, config)
    pr_a_cen = model['a_cen'](A, lc, m, zt, config)
    pr_a_mis = model['a_mis'](A, lc, m, zt, R, config)
    pr_zo_zt = model['zo_zt'](zo, zt, lo, config)

    hmf = model['hmf'](m, zt, config)
    T_cen = model['T_cen'](r, m, config)
    T_mis= model['T_mis'](r, m, R, config)
    delta_sigma_cen = model['del_sig_cen'](r, m, config)
    delta_sigma_mis = model['del_sig_mis'](r, m, R, config)
    msci = mean_sigma_crit_inv(zt)

    # TODO: Add in selection function!
    # TODO: only integrands at the moment!
    # pudb.set_trace()
    gamma_t_cen = hmf * pr_zo_zt * msci * (
        fcen * (np.exp(A*T_cen) * delta_sigma_cen) * pr_a_cen * pr_lc_lt * pr_mor
    )

    gamma_t_mis = hmf * pr_zo_zt * msci * (
        (1-fcen) * (np.exp(A*T_mis) * delta_sigma_mis) * pr_a_mis * pr_lo_lc * pr_lc_lt * pr_roffset
    )

    # TODO: Calculate Nw!
    Nw = 1.0
    gamma_t = (1.0 / Nw) * (gamma_t_cen + gamma_t_mis)

    print "Writing to the block"
    block['model_values', 'gamma_t'] = gamma_t

    # TODO: Figure out when to multiply by multiplicative shear bias (1+m_shear)

    #Load the matter power spectra
    # z_lin,k_lin,p_lin = block.get_grid(names.matter_power_lin,'z','k_h','p_k')
    # z_nl,k_nl,p_nl = block.get_grid(names.matter_power_nl,'z','k_h','p_k')


    #Radial bin edges of the bins. These will either be passed in from
    #CosmoSIS as thetas and then converted here based on the cosmology,
    #or computed elsewhere and passed in here as true distances.
    #These bin edges were used in Y1
    # R_min = 0.0323 #Mpc physical
    # R_max = 30.    #Mpc physical
    # Nbins = 15
    # R_edges = np.logspace(np.log10(R_min), np.log10(R_max), Nbins+1) #Mpc physical
    # R_edges *= h0*(1+z) #Convert to Mpc/h comoving until we write to the block
    
    #Start by calculating the concentration
    # concentration = ct.concentration.concentration_at_M(M, k_lin, p_lin, ns, Omega_b, Omega_m, h, Mass_type="mean", delta=200)

    #Calculate the halo-matter correlation function
    # r = np.logspace(-2, 3, num=1000) #Mpc/h comoving; 3d radii use for integration
    # xi_1halo = ct.xi.xi_nfw_at_R(r, M, concentration, Omega_m) 
    # xi_mm = ct.xi.xi_mm_at_R(r, k_lin, p_nl)
    # bias = ct.bias.bias_at_M(M, k_nl, p_lin, Omega_m)
    # xi_2halo = bias*xi_mm
    # xi_hm = ct.xi.xi_hm(xi_1halo, xi_2halo) #does max(1h, 2h)

    # #Calculate Sigma(R)
    # R_perp = np.logspace(-2, 2.4, 1000) #Mpc/h; can't go as high as the xi_hm 3d distances
    # Sigma = ct.deltasigma.Sigma_at_R(R_perp, r, xi_hm, M, concentration, Omega_m)

    # #Calculate DeltaSigma(R)
    # DeltaSigma = ct.deltasigma.DeltaSigma_at_R(R_perp, R_perp, Sigma, M, concentration, Omega_m)

    # #As per Eduardo's equations, modify by baryons and projection+triaxiality
    # Baryons = 1
    # AT = 0 #in the log
    # DeltaSigma = Baryons*DeltaSigma * np.exp(AT)

    # #Average in the radial bins
    # ave_DeltaSigma = ct.averaging.average_profile_in_bins(R_edges, R_perp, DeltaSigma)

    # #Convert to Msun/pc^2 physical distances
    # ave_DeltaSigma *= h0*(1+z)**2

    # #Convert to gamma(theta)
    # print "Converting to gamma(theta)"

    return 0

def cleanup(config):
        pass

def mean_sigma_crit_inv(z):
    '''
    Returns the mean sigma crit inverse of a redshift
    averaged over the source redshift distribution
    '''
    # TODO: Implement!
    return 1.0
