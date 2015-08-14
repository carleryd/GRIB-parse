#!/bin/bash
./create_json_from_grib.bash t GRIB/NEW_GRIB /Users/christoferarleryd/Programming/GRIB-data/ &&
./create_json_from_grib.bash tccarmean GRIB/NEW_GRIB /Users/christoferarleryd/Programming/GRIB-data/ && 
./create_json_from_grib.bash parmean GRIB/NEW_GRIB /Users/christoferarleryd/Programming/GRIB-data/ && 
./create_json_from_grib.bash pcat GRIB/NEW_GRIB /Users/christoferarleryd/Programming/GRIB-data/ &&
./create_json_from_grib.bash u GRIB/NEW_GRIB /Users/christoferarleryd/Programming/GRIB-data/ && 
./create_json_from_grib.bash v GRIB/NEW_GRIB /Users/christoferarleryd/Programming/GRIB-data/
