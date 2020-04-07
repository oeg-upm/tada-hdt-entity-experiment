# tada-hdt-entity-experiment
[![Build Status](https://semaphoreci.com/api/v1/ahmad88me/tada-hdt-entity-experiment/branches/master/badge.svg)](https://semaphoreci.com/ahmad88me/tada-hdt-entity-experiment)
[![codecov](https://codecov.io/gh/oeg-upm/tada-hdt-entity-experiment/branch/master/graph/badge.svg)](https://codecov.io/gh/oeg-upm/tada-hdt-entity-experiment)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3732641.svg)](https://doi.org/10.5281/zenodo.3732641)


Run experiments using tada-hdt-entity library

# Datasets
* T2Dv2

# T2Dv2
* `classes_with_col_GS.csv`: this file contains the file names, the column id of the subject columns, and their classes
This is update version from the `classes_GS.csv` provided by the T2Dv2. We changed the concept column with the subject column id and we fixed some mislabels.
* `classes_with_col_GS_2016_04`: a subset of `classes_with_col_GS.csv`, which contains the entity columns which exists in dbpedia's hdt of 04-2016.
* `properties_entity.csv`: contains the properties with entities as the objects
* `properties_2016_04`: a subset of `properties_entity.csv` which only contains the properties which exists in dbpedia's hdt of 04-2016.
