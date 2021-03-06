//
//  VolumeDecomposer.cpp
//  5AxLer
//
//  Created by Alexandre Pauwels on 11/4/16.
//  Copyright © 2016 MAP MQP. All rights reserved.
//

#include "VolumeDecomposer.hpp"
#include "Slicer.hpp"

using namespace mapmqp;
using namespace std;

vector<shared_ptr<Mesh>> run(shared_ptr<Mesh> p_mesh, Plane orientation) {
	// The return vector
	vector<shared_ptr<Mesh>> decomposedVolumes;

	// Initializes the slicer for the decomposition
	Slicer slicer = Slicer(p_mesh);

	// The two slice planes that will be used, one on top of the other
	Plane firstPlane = orientation;

	// The comparison slice used to determine the buildable portions of the model
	Slicer::Slice comparisonSlice = slicer.slice(firstPlane);

	// The polygonal representations of both slices
	vector<Polygon> comparisonPoly = comparisonSlice.toPoly();

	// Retrieve the first slice
	Slicer::Slice currSlice = slicer.nextSlice();

	// TODO: Update the slicer to have hasNextSlice(), nextSlice(), and plane()
	// Go through all the slices
	while (currSlice.islands().size() > 0) {
		// Go through each face in the slice
		for (shared_ptr<const Mesh::Face> mf : currSlice.faces()) {
			// Check that all points of the face are inside the comparison poly
			bool allInsideComparison = true;
			bool allOutsideComparison = true;
			vector<Vector3D> flattenedPoints;

			for (int i = 0; i < 3 && allInsideComparison; ++i) {
				// TODO: write a dropVertexToPlane function that drops each vertex of a face
				// onto the plane along the normal of the plane
				Vector3D pt = dropVertexToPlane(mf->p_vertex(i)->vertex(), currSlice.plane());

				// Check if the point is in the polygon and update the flags
				// A point directly on the polygon boundary is considered outside
				bool inPoly = pointInPolygonArray(comparisonPoly, pt);
				allInsideComparison &= (inPoly == 1);
				allOutsideComparison &= (inPoly <= 0);

				// Add the point to our vector of flattened points
				flattenedPoints.push_back(pt);
			}

			// TODO: Figure out a better way to handle fatal errors
			// Error case where the vertices of the triangle are all both in and out of the comparison poly
			if (allInsideComparison && allOutsideComparison) {
				writeLog(ERROR, "all of the vertices of a triangle are both inside and outside the comparison polygon");
				exit(0);
			}
			// Second case where the triangle has some vertices inside and some vertices outside the polygon
			else if (!allInsideComparison && !allOutsideComparison) {
				// First, check to see if the comparison polygon and triangle are parallel to each other
				bool parallel = areParallel(comparisonPoly, mf);
				vector<std::shared_ptr<Mesh::Face>> newFaces;

				// If they are not parallel, we can find the intersection between comparison poly and face
				if (!parallel) {
					// TODO: Write function that takes the comparison poly and the mesh face
					// and returns the <x, y, z> intersecting point
					Vector3D intersection = findPolyFaceIntersection(mf, comparisonPoly);

					// TODO: Write a function that takes a point on the face and a split direction
					// and splits the face along that direction, creating three new faces
					// newFaces = splitFaceAlongDirection(mf, intersection, plane.normal());
				}
				// However if they are parallel, we must cut the triangle with the slice
				else {
					// TODO: Write a function that takes the face and the comparison polygon,
					// intersects them, 
					// newFaces = splitFaceAlongSlice(mf, comparisonPoly);
				}
			}
		}

		// Get the next slice
		currSlice = slicer.nextSlice();
	}

	return decomposedVolumes;
}

/**
 * findPolyFaceIntersection is fairly straightforward:
 * 1. Iterate through each line segment in the polygon
 * 2. Take the mesh face and the two points of the line and pass them through the following equations:
 *		d = (n_x * x_0) + (n_y * y_0) + (n_z * z_0)
 *		t = (d - x_0 - y_0 - z_0) / (deltaX + deltaY + deltaZ)
 *		
 *		d is part of the scalar equation of the plane
 *		<n_x, n_y, n_z> are the face's normal
 *		<x_0, y_0, z_0> in the top equation is the <x, y, z> of one of the face's vertices
 *		<x_0, y_0, z_0> in the bottom equation is the <x, y, z> of one of the endpoints of the line
 *		deltaX = x_1 - x_0, where x_1 is the other endpoint of the line's x-coordinate (etc for deltaY, deltaZ)
 * 3. Now we check t. If 0 <= t <= 1, then the line from the comparison poly intersects the face
 * 4. In that case, we plug t into the following equation to get the final intersection's <x, y, z>:
 *		r(t) = <x_0 + deltaX*t, y_0 + deltaY*t, z_0 + deltaZ*t>
 *		<x_0, y_0, z_0> is the first endpoint of the line
 *		deltaX etc are defined as above
 * 5. Otherwise, the line does not intersect and we move on to the next line
 * 6. If no intersections are found, an exception is thrown
 */
Vector3D findPolyFaceIntersection(shared_ptr<const Mesh::Face> face, Polygon intersectingPoly) {
	const Vector3D faceVert = face->p_vertex(0)->vertex();
	const Vector3D faceNorm = face->normal();
	const std::vector<Vector3D> polyPoints = intersectingPoly.points();

	double d = (faceNorm.x() * faceVert.x()) + (faceNorm.y() * faceVert.y()) + (faceNorm.z() * faceVert.z());

	for (int i = 0; i < 3; ++i) {
		Vector3D p1 = polyPoints[i];
		Vector3D p2 = (i == 2) ? polyPoints[0] : polyPoints[i + 1];

		double dX = p2.x() - p1.x();
		double dY = p2.y() - p1.y();
		double dZ = p2.z() - p1.z();

		double t = (d - p1.x() - p1.y() - p1.z()) / (dX + dY + dZ);

		if (t >= 0 && t <= 1) {
			double newX = p1.x() + dX * t;
			double newY = p1.y() + dY * t;
			double newZ = p1.z() + dZ * t;
			Vector3D intersectPoint = Vector3D(newX, newY, newZ);

			return intersectPoint;
		}
	}

	writeLog(ERROR, "when trying to find the intersection between a face and a polygon, no intersection was found");
	exit(0);
}