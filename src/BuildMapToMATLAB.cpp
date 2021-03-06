//
//  BuildMapToMATLAB.cpp
//  5AxLer
//
//  Created by Ethan Coeytaux on 11/9/16.
//  Copyright © 2016 MAP MQP. All rights reserved.
//

#include "BuildMapToMATLAB.hpp"

#include <fstream>

#include "Utility.hpp"

using namespace mapmqp;
using namespace std;

bool BuildMapToMATLAB::parse(string filePath, const BuildMap & buildMap, OutputType type, int precision) {
    double oldPrecision = precision;
    precision = fmax(1, fmin(precision, fmin(A_AXIS_DISCRETE_POINTS, B_AXIS_DISCRETE_POINTS)));
    
    if (precision != oldPrecision) {
        writeLog(WARNING, "precision of BuildMap is out of range");
    }
    
    ofstream file;
    file.open(filePath, ios::out | ios::binary);
    
    if (file.is_open()) {
        ostringstream xStr, yStr, zStr;
        for (int y = 0; y <= A_AXIS_DISCRETE_POINTS; y += precision) {
            for (int x = 0; x <= B_AXIS_DISCRETE_POINTS; x += precision) {
                Vector3D v = BuildMap::mapToVector(x, y);
                bool valid = buildMap.checkVector(v);
                double weight = valid ? buildMap.averageCuspHeight(v) : 0;
                
                if (type == PLANE) {
                    xStr << x << " ";
                    yStr << y << " ";
                    zStr << (valid ? weight : -1) << " ";
                } else if (type == SPHERE) {
                    v.normalize(valid ? (weight + 1) : 0);
                    xStr << v.x() << " ";
                    yStr << v.y() << " ";
                    zStr << v.z() << " ";
                } else if (type == SPHERE_SMOOTH) {
                    weight = valid ? 1 : 0;
                    v.normalize(weight);
                    xStr << v.x() << " ";
                    yStr << v.y() << " ";
                    zStr << v.z() << " ";
                }
            }
            
            xStr << ";\n";
            yStr << ";\n";
            zStr << ";\n";
        }
        
        file << "X = [\n" << xStr.str() << "];\n";
        file << "Y = [\n" << yStr.str() << "];\n";
        file << "Z = [\n" << zStr.str() << "];\n";
        file << "figure\n";
        file << "surf(X, Y, Z)";
        file.close();
        
        return true;
    } else {
        writeLog(WARNING, "could not open MATLAB script file");
        return false;
    }
}
