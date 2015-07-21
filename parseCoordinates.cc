#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdlib.h>

using namespace std;

// Converts from Regular to Rotated coordinates if option == 1 and the opposite if option == 2
void rotated_grid_transform(vector<double> grid_in, vector<double>& grid_out, int option, double sp_coord_lat, double sp_coord_lon) {
    double lat, lon;
    double x, y, z;
    double theta, phi;
    double x_new, y_new, z_new;
    double lat_new, lon_new;

    for(int i = 0; i < grid_in.size(); i += 2) {
        lat = grid_in[i];
        lon = grid_in[i+1];

        lat = (lat * M_PI) / 180; // Convert degrees to radians
        lon = (lon * M_PI) / 180;

        theta = 90 + sp_coord_lat; // Rotation around y-axis
        phi = sp_coord_lon; // Rotation around z-axis

        phi = (phi * M_PI) / 180; // Convert degrees to radians
        theta = (theta * M_PI) / 180;

        x = cos(lon) * cos(lat); // Convert from spherical to cartesian coordinates
        y = sin(lon) * cos(lat);
        z = sin(lat);

        if(option == 1) { // Regular -> Rotated
            x_new = cos(theta) * cos(phi) * x + cos(theta) * sin(phi) * y + sin(theta) * z;
            y_new = -sin(phi) * x + cos(phi) * y;
            z_new = -sin(theta) * cos(phi) * x - sin(theta) * sin(phi) * y + cos(theta) * z;
        }
        else if(option == 2) { // Rotated -> Regular
            phi = -phi;
            theta = -theta;

            x_new = cos(theta) * cos(phi) * x + sin(phi) * y + sin(theta) * cos(phi) * z;
            y_new = -cos(theta) * sin(phi) * x + cos(phi) * y - sin(theta) * sin(phi) * z;
            z_new = -sin(theta) * x + cos(theta) * z;
        }

        lon_new = atan2(y_new, x_new); // Convert cartesian back to spherical coordinates 
        lat_new = asin(z_new);

        lon_new = (lon_new * 180) / M_PI; // Convert radians back to degrees
        lat_new = (lat_new * 180) / M_PI;

        grid_out.push_back(lat_new);
        grid_out.push_back(lon_new);
    }
}

int main(int argc, char **argv)
{
    vector<double> grid_in;
    vector<double> grid_out;
    vector<double> grid_out_check;
    // Latitude and longitude for target South Pole
    double sp_coord_lat = -30;
    double sp_coord_lon = 15;

    // Only uses the parameter file to get coordinates in the same order as the parameter files.
    // This way they can be used together when finding, for example, temperature for a certain coordinate.
    string in_grib = argv[1];
    string out_json = argv[2];
    int point_interval = (argc == 4) ? atoi(argv[3]) : 14;
    ifstream parameter_file_in(in_grib.c_str());
    ofstream coordinates_file_out(out_json.c_str());
    // Flush first line
    string description;
    getline(parameter_file_in, description);
    // Used to flush values.
    string trash;

    double longitude, latitude;
    double lon_min, lon_max, lat_min, lat_max;
    lon_min = lat_min = 9999;
    lon_max = lat_max = -9999;

    int file_in_count = 0;
    while(!parameter_file_in.eof()) {
        parameter_file_in >> latitude >> longitude >> trash;
        grid_in.push_back(latitude);
        grid_in.push_back(longitude);
        file_in_count++;
    }
    //cout << "Amount of coordinates found in parameter_file_in: " << file_in_count << endl;
    //cout << "Last pair in grid_in: " << grid_in[grid_in.size()-2] << " " << grid_in[grid_in.size()-1] << endl;
    //cout << "Size of grid_in: " << grid_in.size() << endl;

    rotated_grid_transform(grid_in, grid_out, 2, sp_coord_lat, sp_coord_lon);

    /*cout << "Start of first latitude row: " << grid_out[0] << " " << grid_out[1] << endl;
    cout << "End of first latitude row: " << grid_out[613*2-2] << " " << grid_out[613*2-1] << endl;
    cout << "Start of second latitude row: " << grid_out[613*2] << " " << grid_out[613*2+1] << endl;
    cout << "End of second latitude row: " << grid_out[613*4-2] << " " << grid_out[613*4-1] << endl;
    cout << "Start of third latitude row: " << grid_out[613*4] << " " << grid_out[613*4+1] << endl;
    
    cout << endl;
    cout << "North-west:    " << grid_out[2*(613*753-613-1)] << ", " << grid_out[2*(613*753-613-1)+1] << endl;
    cout << "North-west:    " << grid_out[2*(613*753-613)] << ", " << grid_out[2*(613*753-613)+1] << endl;
    cout << "North-west:    " << grid_out[2*(613*753-613+1)] << ", " << grid_out[2*(613*753-613+1)+1] << endl;

    cout << "North-east:    " << grid_out[2*(613-11)*(753-11)] << ", " << grid_out[2*(613-11)*(753-11)+1] << endl;
    cout << "North-east:    " << grid_out[2*(613)*(753)] << ", " << grid_out[2*(613)*(753)+1] << endl;
    cout << endl;

    cout << "South-east:    " << grid_out[2*(613-1)] << ", " << grid_out[2*(613-1)+1] << endl;
    cout << "South-east:    " << grid_out[2*(613-11)] << ", " << grid_out[2*(613-11)+1] << endl;
    cout << "South-east:    " << grid_out[2*(613+1)] << ", " << grid_out[2*(613+1)+1] << endl;
    cout << endl;

    cout << "South-west:    " << grid_out[0] << ", " << grid_out[1] << endl;*/

    cout << endl << endl;

    // This is the size of the grid found in the GRIB-files.
    int latAmount = 753;
    int lonAmount = 613;
    int latCount = 0;
    int lonCount = 0;
    int count = 0;
    // This takes the first coordinate at the south-west corner and then skips over coordinates defined by point_interval.
    // If point_interval is set to 1, every coordinate is written to JSON. If for example point_interval is set to 14
    // 0:0 is written, then 14:0, 28:0 ... 742:602, a total of 2376 coordinates(twice as many values) written to JSON file.
    cout << "Last pair in grid_out: " << grid_out[grid_out.size()-2] << " " << grid_out[grid_out.size()-1] << endl;
    cout << "Size of grid_out: " << grid_out.size() << endl;

    for(int latCount = 0; latCount < latAmount; ++latCount) {
        for(int lonCount = 0; lonCount < lonAmount; ++lonCount) {
            if(latCount % point_interval == 0 && lonCount % point_interval == 0) {
                int pos = latCount * lonAmount + lonCount;
                //cout << "in if: " << pos*2 << " " << latCount << " " << lonCount << endl;
                coordinates_file_out << grid_out[2*pos] << ',';
                coordinates_file_out << grid_out[2*pos+1] << ',';
                grid_out_check.push_back(grid_out[2*pos]);
                grid_out_check.push_back(grid_out[2*pos+1]);
                count++;
            }
        }
    }

    /*cout << endl;
    cout << "North-west:    " << grid_out_check[2*(44*54-43-2)] << ", " << grid_out_check[2*(44*54-43-2)+1] << endl;
    cout << "North-west:    " << grid_out_check[2*(44*54-43-1)] << ", " << grid_out_check[2*(44*54-43-1)+1] << endl; // rätt
    cout << "North-west:    " << grid_out_check[2*(44*54-43)] << ", " << grid_out_check[2*(44*54-43)+1] << endl;

    cout << grid_out_check.size() << endl;
    cout << "North-east:    " << grid_out_check[2*44*54-2] << ", " << grid_out_check[2*44*54-1] << endl; // rätt
    cout << endl;

    cout << "South-east:    " << grid_out_check[2*(43-1)] << ", " << grid_out_check[2*(43-1)+1] << endl;
    cout << "South-east:    " << grid_out_check[2*(43)] << ", " << grid_out_check[2*(43)+1] << endl; // rätt
    cout << "South-east:    " << grid_out_check[2*(43+1)] << ", " << grid_out_check[2*(43+1)+1] << endl;
    cout << endl;

    cout << "South-west:    " << grid_out_check[0] << ", " << grid_out_check[1] << endl; // rätt

    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cout << "North-west:    " << grid_out_check[2*(44*54-43-2)] << ", " << grid_out_check[2*(44*54-43-2)+1] << endl;
    cout << "North-west:    " << grid_out_check[2*(44*54-43-1)] << ", " << grid_out_check[2*(44*54-43-1)+1] << endl;
    cout << "North-west:    " << grid_out_check[2*(44*54-43)] << ", " << grid_out_check[2*(44*54-43)+1] << endl;

    cout << endl;
    cout << "North-west adj:" << grid_out_check[2*(44*50-43-1)] << ", " << grid_out_check[2*(44*50-43-1)+1] << endl; // 1 5
    cout << "North-west adj:" << grid_out_check[2*(44*49-43-1)] << ", " << grid_out_check[2*(44*49-43-1)+1] << endl; // 1 6
    
    cout << "WNorth-west adj:" << grid_out_check[2*(44*50-43-2)] << ", " << grid_out_check[2*(44*50-43-2)+1] << endl; // 0 5 ska vara 0, är fel
    cout << "North-west adj:" << grid_out_check[2*(44*50-43-1)] << ", " << grid_out_check[2*(44*50-43-1)+1] << endl; // 1 5
    cout << "North-west adj:" << grid_out_check[2*(44*50-42-1)] << ", " << grid_out_check[2*(44*50-42-1)+1] << endl; // 2 5
    cout << "North-west adj:" << grid_out_check[2*(44*50-41-1)] << ", " << grid_out_check[2*(44*50-41-1)+1] << endl; // 3 5
    cout << "North-west adj:" << grid_out_check[2*(44*50-40-1)] << ", " << grid_out_check[2*(44*50-40-1)+1] << endl; // 4 5
    cout << "North-west adj:" << grid_out_check[2*(44*50-39-1)] << ", " << grid_out_check[2*(44*50-39-1)+1] << endl; // 5 5

    cout << grid_out_check.size() << endl;
    cout << "North-east1:    " << grid_out_check[2*44*54-2] << ", " << grid_out_check[2*44*54-1] << endl; // 1
    cout << "North-east2:    " << grid_out_check[2*44*53-2] << ", " << grid_out_check[2*44*53-1] << endl; // 2
    cout << "North-east3:    " << grid_out_check[2*44*52-2] << ", " << grid_out_check[2*44*52-1] << endl; // 3
    cout << "North-east4:    " << grid_out_check[2*44*51-2] << ", " << grid_out_check[2*44*51-1] << endl; // 4
    cout << "North-east5:    " << grid_out_check[2*44*50-2] << ", " << grid_out_check[2*44*50-1] << endl; // 5
    cout << "North-east6:    " << grid_out_check[2*44*49-2] << ", " << grid_out_check[2*44*49-1] << endl; // 6
    cout << endl;

    cout << "South-east:    " << grid_out_check[2*(43-1)] << ", " << grid_out_check[2*(43-1)+1] << endl;
    cout << "South-east:    " << grid_out_check[2*(43)] << ", " << grid_out_check[2*(43)+1] << endl;
    cout << "South-east:    " << grid_out_check[2*(43+1)] << ", " << grid_out_check[2*(43+1)+1] << endl;
    cout << endl;

    cout << "South-west:    " << grid_out_check[0] << ", " << grid_out_check[2*0+1] << endl;    // 1
    cout << "South-west:    " << grid_out_check[0+2*44*1] << ", " << grid_out_check[2*0+2*44*1+1] << endl;    // 1
    cout << "South-west:    " << grid_out_check[0+2*44*2] << ", " << grid_out_check[2*0+2*44*2+1] << endl;    // 1
    cout << "South-west:    " << grid_out_check[0+2*44*2-2] << ", " << grid_out_check[2*0+2*44*2-1] << endl;    // 1
    // Find out 40x50
    cout << "South-west:    " << grid_out_check[2*0] << ", " << grid_out_check[2*0+1] << endl;  // 1
    cout << "South-west:    " << grid_out_check[2*1] << ", " << grid_out_check[2*1+1] << endl;  // 2
    cout << "South-west:    " << grid_out_check[2*2] << ", " << grid_out_check[2*2+1] << endl;  // 3
    cout << "South-west:    " << grid_out_check[2*3] << ", " << grid_out_check[2*3+1] << endl;  // 4
    cout << "South-west:    " << grid_out_check[2*4] << ", " << grid_out_check[2*4+1] << endl;  // 5
    cout << "South-west:    " << grid_out_check[2*5] << ", " << grid_out_check[2*5+1] << endl;  // 6
    cout << "South-west:    " << grid_out_check[2*6] << ", " << grid_out_check[2*6+1] << endl;  // 7
*/

    cout << endl;
    cout << "North-west:    " << grid_out_check[2*(44*54-43+3)] << ", " << grid_out_check[2*(44*54-43+3)+1] << endl;    // 5

    /*for(int i = 0; i < grid_out.size(); i+=2) {
        // Save latitude and longitude for every point_interval:th coordinate
        if(latCount % point_interval == 0 && lonCount % point_interval == 0) {
            cout << "in if: " << i << " " << latCount << " " << lonCount << endl;
            coordinates_file_out << grid_out[i] << ',';
            coordinates_file_out << grid_out[i+1] << ',';
            count++;
        }
        lonCount++;

        // Reached end of longitude line, start again from start at next longitude
        if(lonCount % lonAmount == 0) {
            latCount++;
            lonCount = 0;
        }
    }*/
    // Useful to check so that it adds up to expectations
    cout << count << " amount of coordinates stored." << endl;

    return 0;
}
