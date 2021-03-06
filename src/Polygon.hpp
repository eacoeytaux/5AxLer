//
//  Polygon.hpp
//  5AxLer
//
//  Created by Ethan Coeytaux on 11/1/16.
//  Copyright © 2016 MAP MQP. All rights reserved.
//

#ifndef Polygon_hpp
#define Polygon_hpp

#include <string>
#include <vector>

#include "../libs/clipper/clipper.hpp"

#include "Vector3D.hpp"
#include "Plane.hpp"

namespace mapmqp {
    class Polygon {
    public:
        Polygon(const std::vector<Vector3D> & points); //assumes all points form one complete polygon on singular plane
        
        //getters
        const std::vector<Vector3D> & points() const;
        const Plane & plane() const;
        
        Polygon mapToXYPlane() const;
        Vector3D mapPointToXYPlane(const Vector3D & point) const;
        Vector3D mapPointToPolygonPlane(const Vector3D & point) const; //only looks at x and y values of point
        
        double area() const;
        
        bool pointInPolygon(const Vector3D & point) const;

        // Flips the polygon to be a hole if it's not, and vice-versa
        void togglePolygonType();
        
        //static member's getters/setters
        static uint64_t mappedPointPrecision();
        static void mappedPointPrecision(int mappedPointPrecision);
        
        std::string toString() const;
        
    private:
        std::vector<Vector3D> m_points; //stored in counter-clockwise form
        Plane m_plane;
        Vector3D m_planeAxisX, m_planeAxisY; //create x and y axes relative to polygon plane
        
        //TODO should these be #defines instead of variables?
        static uint64_t s_mappedPointPrecision; //since clipper only uses integers as coordinates, coordinates are multiplied by mappedPointPrecision_ when stored in polygonXYPlane_ and divided back by mappedPointPrecision_ when returned (clipper integers range from +/- 4.6e18 ~= 2^62)
        
        ClipperLib::Path m_polygonXYPlane; //clipper representation of polygon
        bool m_solved; //whether or not polygon_ has been updated to represent polygon
    };
}

#endif /* Polygon_hpp */
