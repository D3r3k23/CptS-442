#ifndef INCLUDED_OBJ_IO

#include <vector>

#include "geometry.h" // for Point*, Vector3, and Face classes

using namespace std;

bool readObj(const string fname,
            vector<Point3>& vertexPositions, vector<Vector3>& vertexNormals,
            vector<Point2>& textureCoordinates,
            vector<Face>& faces);

#define INCLUDED_OBJ_IO
#endif // INCLUDED_OBJ_IO
