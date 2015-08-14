#!/bin/bash
count=1
for file in $2/PMP_P_*; do
    source=$file
    destination=$3time_$count/$1

    if [ ! -d "$3time_$count" ]; then
        # Control will enter here if $DIRECTORY doesn't exist.
        mkdir "$3time_$count"
    fi

    printf "Writing data for parameter $1 from file $source to file $destination . . . "
    ./grib_get_data -w shortName=$1 $source > $destination
    echo "done."

    ((count++))
done
