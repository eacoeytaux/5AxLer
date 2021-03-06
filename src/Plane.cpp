//
//  Plane.cpp
//  5AxLer
//
//  Created by Ethan Coeytaux on 11/5/16.
//  Copyright © 2016 MAP MQP. All rights reserved.
//

#include "Plane.hpp"

#include <cmath>

#include "Utility.hpp"

using namespace mapmqp;
using namespace std;

double Plane::s_faultTolerance = 0.0;

Plane::Plane(const Vector3D & normal, double scalar) :
m_normal(normal), m_scalar(scalar) {
    m_origin = normal;
    m_origin.normalize(scalar);
}

const Vector3D & Plane::normal() const {
    return m_normal;
}

const Vector3D & Plane::origin() const {
    return m_origin;
}

const double Plane::scalar() const {
    return m_scalar;
}

void Plane::normal(Vector3D normal) {
    m_normal = normal;
}

void Plane::origin(Vector3D origin) {
    m_origin = origin;
}

Plane::PLANE_POSITION Plane::pointOnPlane(const Vector3D & point, double faultTolerance) const {
    if (point == m_origin) {
        return ON;
    }
    
    double dotVal = Vector3D::dotProduct(point - m_origin, m_normal);
    
    if (doubleEquals(dotVal, 0.0, faultTolerance)) {
        return ON;
    } else if (dotVal > 0) {
        return ABOVE;
    } else {
        return BELOW;
    }
}

double Plane::faultTolerance() {
    return Plane::s_faultTolerance;
}

void Plane::faultTolerance(double faultTolerance) {
    Plane::s_faultTolerance = faultTolerance;
}

string Plane::toString() const {
    ostringstream stream;
    stream << "[plane:" << m_normal.toString() << ", origin:" << m_origin.toString() << "]";
    return stream.str();
}
