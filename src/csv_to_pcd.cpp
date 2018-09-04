#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include "../include/pc_type.hpp"

using namespace std;


int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cout << "Usage format: ./csv_to_pcd <file_path/input_file.csv> <file_path/output_file.pcd>" << endl;
        return (-1);
    }

    ifstream readFile(argv[1]);
    if(!readFile.is_open())
    {
        cout << "Error opening file!" << endl;
        return (-1);
    }
    
    int cntln;
    string line;
    vector<string> fields;
    Point pt;
    vector<Point> pointVec;

    while(getline(readFile, line))
    {
        istringstream sin(line);
        vector<string> fields;
        string field;
        while (getline(sin, field, ','))
        {
            fields.push_back(field);
        }

        /* csv file fields: "t, intensity, id, x, y, z, azimuth, range, pid" */
        pt.x = stof(fields[3]);
        pt.y = stof(fields[4]);
        pt.z = stof(fields[5]);
        pointVec.push_back(pt);
    }
    cntln = pointVec.size();

    if(cntln < 1)
    {
        cout << "Error streaming data!" << endl;
        return (-1);
    }
    else
    {
        pcl::PointCloud<pcl::PointXYZ> cloud;
        cloud.width = cntln;
        cloud.height = 1;
        cloud.is_dense = false;
        cloud.points.resize(cloud.width * cloud.height);
        for(size_t i = 0; i < cloud.points.size (); ++i)
        {
            cloud.points[i].x = pointVec[i].x;
            cloud.points[i].y = pointVec[i].y;
            cloud.points[i].z = pointVec[i].z;
        }
        pcl::io::savePCDFileBinaryCompressed((string)(argv[argc - 1]), cloud);
    }


    return 0;
}
