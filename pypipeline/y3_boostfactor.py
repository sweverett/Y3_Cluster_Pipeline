# CosmoSIS module for the Y3 clusters cosmology boost factor
from cosmosis.datablock import option_section, names
import numpy as np


def setup(options):
    section = option_section
    new = 1
    return  new


def execute(block, config):
    new = config

    block["counts"] = new
    return 0


def cleanup(config):
        pass
