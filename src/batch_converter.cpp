#include <iostream>
#include <fstream>
#include <sstream>
// #include <string>
#include <cstring>
#include <vector>

#include <dirent.h>
#include <boost/algorithm/string.hpp>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#include "../include/pc_type.hpp"

using namespace std;

vector<string> selected_files;
vector<string> csv_filename;

void ReadFile(const std::string filePath)
{
    DIR* dir = opendir(filePath.c_str());
    struct dirent* ptr;

    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name,".") != 0 && strcmp(ptr->d_name,"..") != 0)
        {
            string subDirect = filePath;
            // d_type == 8: file
            // d_type == 4: directory
            if (ptr->d_type == 4) 
            {
                subDirect += "/";
                subDirect += ptr->d_name;
                ReadFile(subDirect);
            }
            else if(ptr->d_type == 8)
            {
                vector<string> fields;
                boost::split(fields, ptr->d_name, boost::is_any_of("."));
                string suffix = *(fields.end()-1);
                if(suffix == "csv")
                {
                    csv_filename.push_back(ptr->d_name);
                    string absolutePath = subDirect + "/";
                    absolutePath += ptr->d_name;
                    selected_files.push_back(absolutePath);
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cout << "Usage format: ./batch_converter <input_directory> <output_directory>" << endl;
        return (-1);
    }

    /************ read file *****************/
    string file_dir = argv[1];
    ReadFile(file_dir);
    
    if(selected_files.size() != csv_filename.size())
        cout << "Error loading csv files!" << endl;

    
    /************ convert file **************/
    for(unsigned int iter=0; iter<selected_files.size(); ++iter)
    {
        string file_path = selected_files[iter];
        ifstream readFile(file_path.c_str());
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
            
            // save pcd files into the target directory
            string target_path = argv[argc - 1];
            string last = target_path.substr(target_path.length()-1);
            if(last != "/")
                target_path += "/";
            string filename = csv_filename[iter];
            string target_file = target_path + filename.substr(0,filename.length()-4) + ".pcd";

            pcl::io::savePCDFileBinaryCompressed(target_file, cloud);
        }

    }

    return 0;
}
