from cosmosis.datablock import names, option_section
import numpy as np
import sys
sys.path.append('../likelihood/y3_cluster')
import twopoint


"""
Using the data_prep binning scheme:
    let "model_n" be the cluster abundace modeling code
    and "model_t" be gamma_t modeling code.
Then
    binning = block["model_bins","binning"] 
    n_models = binning["n_cluster_models"]
    n_source_bins = binning_scheme["n_source_bins"] 
    n_radial_bins =  binning_scheme["n_radial_bins"] 
    count = np.zeros(n_models)
    for i in n_models:
        zmin,zmax = binning[i,"z_min z_max"] 
        lmin,lmax = binning[i,"lambda_min lambda_max"] 
        count[i] = model_n(zmin,zmax,lmin,lmax)
    gammat = np.zeros(n_models*n_source_bins*n_radial_bins)
    for i in n_models:
       zmin,zmax = binning[i,"z_min z_max"] 
       lmin,lmax = binning[i,"lambda_min lambda_max"] 
       for j in n_source_bins:
           gammat = np.concat(gammat, model_t(zmin,zmax,lmin,lmax))
           # assuming model_t returns a vector of length n_radial_bins

bin1, bin2 index into angular_bins
bin1 is the cluster bins, bin2 is the source bins

As of the lighthouse/Feb 2018, the defining use case of the
data vector is in Niall's test_cluster.py
 at: https://github.com/joezuntz/2point/blob/lighthouse/test/test_cluster.py
"""
def setup(options):
    # data vector
    data_vector_name = options[option_section, "data_vector_name"]
    data_vector = twopoint.TwoPointFile.from_fits(data_vector_name, covmat_name=None)

    #e.g. counts, richness and redshift lims
    count_data = data_vector.get_measurement('cluster_counts')
    cluster_z_bin_edges = count_data.z_lims
    cluster_lambda_bin_edges = count_data.lambda_lims
    cluster_z_bins = count_data.z_bins
    cluster_lambda_bins = count_data.lambda_bins
    n_models = num_bin1

    bins_z_list = []
    bins_l_list = []
    for i in range(n_models) :
        bins_z_list.append([cluster_z_bin_edges[i][0],cluster_z_bin_edges[i][1]])
        bins_l_list.append([cluster_lambda_bin_edges[i][0],cluster_lambda_bin_edges[i][1]])
    
    #Get the profile for cluster 
    cgt = data_vector.get_measurement('cluster_gamma_t')
    n_source_bins = cgt.num_bin2
    n_radial_bins = cgt.bin1.size/n_models/n_source_bins

    angle_unit = cgt.angle_unit
    angular_bins=cgt.angle
    bin1 = cgt.bin1 # number of cluster bins
    bin2 = cgt.bin2 # number of source  bins
        
    return n_models, n_radial_bins, n_source_bins, bins_z_list, bins_l_list, \
            angle_unit, angular_bins, bin1, bin2

def execute(block, config):
    (n_models, n_radial_bins, n_source_bins, bins_z_list, bins_l_list,
            angle_unit, angular_bins, bin1, bin2) = config
    block["model_bins","n_models"] = n_models
    block["model_bins","n_source_bins"] = n_source_bins
    block["model_bins","n_radial_bins"] = n_radial_bins
    for i in range(n_models) :
        block["model_bins","{}_z_min_z_max".format(i)] = bins_z_list[i]
        block["model_bins","{}_lambda_min_lambda_max".format(i)] = bins_l_list[i]
    block["model_bins","angle_unit"] = angle_unit
    block["model_bins","angular_bins"] = angular_bins
    block["model_bins","bin1"] = bin1
    block["model_bins","bin2"] = bin2


    return 0
def cleanup(config):
    pass


# after model is computed we must evaluate at the observed angular bins
# a) we think Tom has code for this
# b) think about benefits of doing this as a cosmosis module
def convert_model_radial_bins_to_observered_angle_bins (model, mpc) :
    angle = mpc
    return angle
