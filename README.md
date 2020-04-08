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
* `properties_2016_04.csv`: a subset of `properties_entity.csv` which only contains the properties which exists in dbpedia's hdt of 04-2016.
* `tranformer.py`: a python script to generate `properties_entity.csv` from the annotation files of T2Dv2.

# To generate properties files
1. To generate `properties_entity.csv`.
`python tranformer.py "T2Dv2_dir/property" classes_with_col_GS_2016_04.csv properties_entity.csv`
You can replace `T2Dv2_dir` with the directory containing the T2Dv2 folder. You can download it from [here](http://webdatacommons.org/webtables/goldstandardV2.html).
2. To generate `properties_2016_04.csv`, you need to change the `main.cpp` and include the following source code:
```c++
#include<iostream>
#include<string>
#include<stdio.h>
#include "t2dv2.h"
#include <tabular_parser/parser.h>
using namespace std;

int main(int argc, char* argv[]) {
    T2Dv2* t2d = new T2Dv2(hdt_file, log_file, classes_file, files_dir);
    t2d->set_lang_tag("@en");
    t2d->set_title_case(false);
    t2d->generate_properties_file("datasets/t2dv2/properties_entity.csv", "datasets/t2dv2/properties_2016_04.csv");
    return 0;
}
```
and then compile and run `make;bin/tadaeexperiment`
