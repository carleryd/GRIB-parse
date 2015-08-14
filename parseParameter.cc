#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <math.h>
using namespace std;

// This program reads GRIB-data stored in format
// Latitude, Longitude, Value
// and writes it into format
// {values":[value1,value2,value3,...]}
// USAGE: ./parseParameter ../GRIB-data/ /Users/christoferarleryd/Development/Visiarc_tools/Solklart/solklart/Other/SMHI-data/pcat.json pcat 16
int main(int argc, char **argv)
{
    if(argc < 5 || argc > 6) {
        cout << "INCORRECT ARGUMENTS TO RUN." << endl;
        return 0;
    }
    string in_grib = argv[1];
    string out_json = argv[2];
    vector<double> values;
    int point_interval = (argc == 6) ? atoi(argv[5]) : 14;
    int num_of_files = atoi(argv[4]);
    string description;
    ofstream parameter_file_out(out_json.c_str());
    double value;

    // Used to flush values in between intervals
    string trash;

    // This is the size of the grid found in the GRIB-files.
    int latAmount = 753;
    int lonAmount = 613;
    int latCount = 0;
    int lonCount = 0;
    int count = 0;

    // All values belong to array "values" in the JSON file.
    parameter_file_out << "{\"values\":[";

    // Read in parameter files for all time steps(varies around 70? we currently only have 16 sample files)
    for(int i = 0; i < num_of_files; ++i) {
        string parameter_path = in_grib + "time_" + to_string(i+1) + "/" + argv[3];
        ifstream parameter_file_in(parameter_path.c_str());
        cout << "Reading file at path: " << parameter_path << endl;
        cout << "i: " << i << " num_of_files-1: " << num_of_files-1 << endl;
        // Flush first line
        getline(parameter_file_in, description);

        while(!parameter_file_in.eof()) {
            parameter_file_in >> trash >> trash >> value;
            // Round cloud parameter to 2 decimals instead of 7.
            //value = (int)(value * 100);
            //value = value / 100;
            value = floor(value * 10000000.0) / 10000000.0;
            //value = floor(value * 100.0) / 100.0;
            values.push_back(value);
        }

        // This takes the first value at the south-west corner and then skips over values defined by point_interval.
        // If point_interval is set to 1, every value is written to JSON. If for example point_interval is set to 14
        // 0:0 is written, then 14:0, 28:0 ... 742:602, a total of 2376 value written to JSON file.
        for(int latCount = 0; latCount < latAmount; ++latCount) {
            for(int lonCount = 0; lonCount < lonAmount; ++lonCount) {
                if(latCount % point_interval == 0 && lonCount % point_interval == 0) {
                    int pos = latCount * lonAmount + lonCount;

                    parameter_file_out << values[pos];
                    if(!(i == num_of_files-1 
                                && latCount == (latAmount - (latAmount % point_interval))
                                && lonCount == (lonAmount - (lonAmount % point_interval)))) {
                        parameter_file_out << ',';
                    }
                    else
                        cout << "no , at: " << latCount << " " << lonCount << endl;
                    count++;
                }
            }
        }
        // Useful to check so that it adds up to expectations
        cout << count << " amount of values stored from file time_" << i+1 << "/" << argv[3] << endl;
        cout << "values vector size: " << values.size() << endl;

        // Clear vector and use it to store values for next file.
        values.clear();
        count = 0;
    }
    parameter_file_out << "]}";

    return 0;
}
