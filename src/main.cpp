//
//  main.cpp
//  generator
//
//  Created by Ethan Coeytaux on 10/6/16.
//  Copyright © 2016 MAP MQP. All rights reserved.
//

#define RUN_TESTS

#ifdef RUN_TESTS
#define CATCH_CONFIG_MAIN
#include "../libs/Catch/catch.hpp"
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "Mesh.hpp"
#include "Utility.hpp"
#include "Clock.hpp"
#include "BuildMap.hpp"
#include "ProcessSTL.hpp"
#include <cmath>
#include <unordered_map>
#include <string>
//end debugging

using namespace mapmqp;

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
// #ifdef RUN_TESTS
//     //run tests
//     //mapmqp::writeLog(mapmqp::INFO, "running tests...");
//     int catch_argc = argc;
//     char ** catch_argvs;
//     char * catch_a = new char[argc];
//     catch_argvs = &catch_a;
//     catch_main(catch_argc, catch_argvs);
//     //return 0;
// #endif
    
//     mapmqp::writeLog(mapmqp::INFO, "starting 5AxLer at time %s", mapmqp::Clock::wallTimeString().c_str());
//     mapmqp::settingsDocument();
    
//     mapmqp::Mesh mesh;

    // Vector3D v1(Vector3D(0, 0, 0));
    // Vector3D v2(Vector3D(10, 10, 10));
    // Vector3D v3(Vector3D(-10, -10, -10));
    // Vector3D v4(Vector3D(2, 4, -10));
    // Vector3D v5(Vector3D(2, 4, -10));

    // writeLog(INFO, "v4 == v4 : %d", v4 == v5);

    /* testing ProcessSTL */
    mapmqp::ProcessSTL parser(argv[1]);
    std::shared_ptr<mapmqp::Mesh> mesh = parser.run();
    const std::vector<std::shared_ptr<mapmqp::MeshFace>> meshFaces = mesh->faces();
    for (uint32_t i = 0; i < meshFaces.size(); ++i) {
        const std::shared_ptr<mapmqp::MeshFace> currFace = meshFaces[i];

        printf("Face vertices are:\n");
        for (uint16_t j = 0; j < 3; ++j) {
            printf("\t[%f, %f, %f]", currFace->getVertex(j)->vertex().x(), currFace->getVertex(j)->vertex().y(), currFace->getVertex(j)->vertex().z());
        }
        printf("\n");
        printf("Connected faces are:\n");
        for (uint16_t j = 0; j < 3; ++j) {
            printf("%d.)", j);
            const std::shared_ptr<const mapmqp::MeshFace> connFace = currFace->getConnectedFace(j);
            for (uint16_t k = 0; k < 3; ++k) {
                printf("\t[%f, %f, %f]", connFace->getVertex(k)->vertex().x(), connFace->getVertex(k)->vertex().y(), connFace->getVertex(k)->vertex().z());
            }
            printf("\n");
        }
        printf("Vertex: %s, number of connected faces: %ld\n", currFace->getVertex(0)->vertex().toString().c_str(), currFace->getVertex(0)->p_faces().size());
        printf("\n");

    }


    /*
    //testing build map
    srand(16);
    mapmqp::Clock timer;
    
#define THETA_POLE M_PI
#define PHI_POLE (M_PI / 2)
#define VECTOR_DIVERGENCE M_PI_2
#define MAX_AREA 40.0
    
#define n 200000
    
    mapmqp::Vector3D faceNormals[n];
    double faceAreas[n];
    
    for (int i = 0; i < n; i++) {
        if (i % 1000 == 0) {
            printf("i: %d\n", i);
        }
        
        //randomly divert from pole with range of (-divergence, divergence) radians in either direction
        mapmqp::Angle theta(THETA_POLE + ((((rand() % 2) == 0) ? 1 : -1) * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/VECTOR_DIVERGENCE))));
        mapmqp::Angle phi(PHI_POLE + ((((rand() % 2) == 0) ? 1 : -1) * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/VECTOR_DIVERGENCE))));
        
        faceNormals[i] = mapmqp::Vector3D(theta, phi);
        
        faceAreas[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/MAX_AREA);
        
        //map.addConstraintVector(mapmqp::Vector3D(theta, phi));
    }
    printf("random surface data generated: %ldms\n", timer.delta());
    
    mapmqp::BuildMap map(faceNormals, faceAreas, n);
    map.solve();
    
    printf("build map constructed: %ldms\n", timer.delta());
    
    map.checkVector(mapmqp::Vector3D(mapmqp::Angle(0), mapmqp::Angle(0)));
    
    double originalArea = A_AXIS_RANGE * B_AXIS_RANGE;
    printf("original area: %f\n", originalArea);
    
    double area = map.area();
    printf("area: %f\n", area);
    printf("area difference: %f\n", area - originalArea);
    
    mapmqp::Vector3D validVector = map.findValidVector();
    printf("valid vector(theta:%f, phi:%f)\n", validVector.theta().val(), validVector.phi().val());
    
    //TODO check to make sure this value is correct
    mapmqp::Vector3D bestVector = map.findBestVector();
    printf("best vector(theta:%f, phi:%f)\n", bestVector.theta().val(), bestVector.phi().val());
    */
     
    return 0;
}
