#!/bin/bash
./parseTemperature ../GRIB-data/ /Users/christoferarleryd/Development/Visiarc_tools/Solklart/solklart/Other/SMHI-data/temperature.json $1
./parseParameter ../GRIB-data/ /Users/christoferarleryd/Development/Visiarc_tools/Solklart/solklart/Other/SMHI-data/parmean.json parmean $1
./parseParameter ../GRIB-data/ /Users/christoferarleryd/Development/Visiarc_tools/Solklart/solklart/Other/SMHI-data/tccarmean.json tccarmean $1
./parseParameter ../GRIB-data/ /Users/christoferarleryd/Development/Visiarc_tools/Solklart/solklart/Other/SMHI-data/pcat.json pcat $1
./parseParameter ../GRIB-data/ /Users/christoferarleryd/Development/Visiarc_tools/Solklart/solklart/Other/SMHI-data/wind_u.json u $1
./parseParameter ../GRIB-data/ /Users/christoferarleryd/Development/Visiarc_tools/Solklart/solklart/Other/SMHI-data/wind_v.json v $1
