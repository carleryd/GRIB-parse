#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <math.h>
using namespace std;

// Works exactly like parseParameter.cc except it does a simple
// conversion from Kelvin to Celsius.
int main(int argc, char **argv)
{
    if(argc < 4 || argc > 5) {
        cout << "INCORRECT ARGUMENTS TO RUN." << endl;
        return 0;
    }
    string in_grib = argv[1];
    string out_json = argv[2];
    vector<double> temperatures;
    int point_interval = (argc == 5) ? atoi(argv[4]) : 14;
    int num_of_files = atoi(argv[3]);
    string description;
    ofstream temperature_file_out(out_json.c_str());
    double temperature;

    // Used to flush temperatures in between intervals
    string trash;

    // This is the size of the grid found in the GRIB-files.
    int latAmount = 753;
    int lonAmount = 613;
    int latCount = 0;
    int lonCount = 0;
    int count = 0;

    // All values belong to array "values" in the JSON file.
    temperature_file_out << "{\"values\":[";

    // Read in temperature files for all time steps(varies around 70? we currently only have 16 sample files)
    for(int i = 0; i < num_of_files; ++i) {
        string temperature_path = in_grib + "time_" + to_string(i+1) + "/t";
        ifstream temperature_file_in(temperature_path.c_str());
        cout << "Reading file at path: " << temperature_path << endl;
        cout << "i: " << i << " num_of_files-1: " << num_of_files-1 << endl;
        // Flush first line
        getline(temperature_file_in, description);

        while(!temperature_file_in.eof()) {
            temperature_file_in >> trash >> trash >> temperature;
            temperature -= 273.15;
            temperature = floor(temperature * 10.0) / 10.0;
            temperatures.push_back(temperature);
        }

        // This takes the first temperature at the south-west corner and then skips over temperatures defined by point_interval.
        // If point_interval is set to 1, every temperature is written to JSON. If for example point_interval is set to 14
        // 0:0 is written, then 14:0, 28:0 ... 742:602, a total of 2376 temperature written to JSON file.
        for(int latCount = 0; latCount < latAmount; ++latCount) {
            for(int lonCount = 0; lonCount < lonAmount; ++lonCount) {
                if(latCount % point_interval == 0 && lonCount % point_interval == 0) {
                    int pos = latCount * lonAmount + lonCount;
                    //cout << "in if: " << pos*2 << " " << latCount << " " << lonCount << endl;
                    //if(latCount == (latAmount - (latAmount % point_interval)))
                        //cout << "### latCount ==" << endl;
                    //if(lonCount == (lonAmount - (lonAmount % point_interval)))
                        //cout << "$$$ lonCount==" << endl;
                          

                    temperature_file_out << temperatures[pos];
                    if(!(i == num_of_files-1 
                                && latCount == (latAmount - (latAmount % point_interval))
                                && lonCount == (lonAmount - (lonAmount % point_interval)))) {
                        temperature_file_out << ',';
                    }
                    else
                        cout << "no , at: " << latCount << " " << lonCount << endl;
                    count++;
                }
            }
        }
        /*if((latCount % point_interval == 0) && (lonCount % point_interval == 0)) {
          temperature -= 273.15;
          temperature_file_out << temperature << ',';
          count++;
          }
          else {
          temperature_file_in >> trash >> trash >> trash;
          }
          latCount++;

        // Reached end of longitude line, start again from start at next longitude
        if(latCount % latAmount == 0) {
        lonCount++;
        latCount = 0;
        }
        }
        latCount = lonCount = 0;
        */
        // Useful to check so that it adds up to expectations
        cout << count << " amount of temperatures stored from file time_" << i+1 << "/temperature" << endl;
        cout << "temperatures vector size: " << temperatures.size() << endl;
        temperatures.clear();
        count = 0;
}
temperature_file_out << "]}";

return 0;
}
