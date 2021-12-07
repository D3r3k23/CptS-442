#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
using namespace std;

#include "geometry.h"
#include "wrap_cmath_inclusion.h"

#define CH_EOF (-1) // denotes end-of-file



Vector3 faceNormal(const Point3 &p0, const Point3 &p1, const Point3 &p2)
{
    //
    // Copy your previous (PA04) solution here.
    //
    return Vector3(); // permits template to compile cleanly
}


Matrix4 Matrix4::inverse(void) const
{
    const int n = 4;
    double delta = 1.0; // the determinant *if* the matrix is nonsingular

    double b[n], c[n];
    int z[n];
    Matrix4 inv = *this;

	for (int j = 0; j < n; j++)
		z[j] = j; // permutation vector

    // solve for inverse (with pivoting)
	for (int i = 0; i < n; i++) {
		int k = i;
		double y = inv.a[inv.ij(i,i)];
        // `k` is the pivot: the row between i+1 and n (inclusive)
        // with the largest diagonal element.
		for (int j = i + 1; j < n; j++) {
			double w = inv.a[ij(i,j)];
			if (fabs(w) > fabs(y)) {
				k = j;
				y = w;
			}
		}
		delta *= y;
        assert(fabs(delta) > EPSILON); // otherwise, assume matrix is singular
		y = 1.0 / y;
		for (int j = 0; j < n; j++) {
			c[j] = inv.a[ij(j,k)];
			inv.a[ij(j,k)] = inv.a[ij(j,i)];
			inv.a[ij(j,i)] = -c[j] * y;
			inv.a[ij(i,j)] = inv.a[ij(i,j)] * y;
			b[j] = inv.a[ij(i,j)];
		}
		inv.a[ij(i,i)] = y;
		int swap = z[i]; z[i] = z[k]; z[k] = swap; // swap z[i] and z[k]
		for (int k = 0; k < n; k++) {
			if (k != i) {
				for (int j = 0; j < n; j++) {
					if (j != i) {
						inv.a[ij(k,j)] = inv.a[ij(k,j)] - b[j] * c[k];
                    }
                }
            }
        }
	}

    // swap permuted rows back into their original location
	for (int i = 0; i < n; i++) {
		for (int k = z[i]; k != i; k = z[i]) {
			for (int j = 0; j < n; j++) {
				double w = inv.a[ij(i,j)];
				inv.a[ij(i,j)] = inv.a[ij(k,j)];
				inv.a[ij(k,j)] = w;
			}
			int swap = z[i]; z[i] = z[k]; z[k] = swap;
			delta = -delta;
		}
	}

	return inv;
}


const Matrix4 Matrix4::operator*(const Matrix4 &matrix) const
{
    Matrix4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.a[ij(i,j)] = 0.0;
            for (int k = 0; k < 4; k++)
                result.a[ij(i,j)] += a[ij(i,k)]*matrix.a[ij(k,j)];
        }
    }
    return result;
}

Matrix4 Matrix4::transpose(void) const
{
    Matrix4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.a[ij(i,j)] = a[ij(j,i)];
        }
    }
    return result;
}


vector<Point2> readPoint2s(string fname)
//
// Read a vector of Point2s from a (simplified) CSV file.
//
{
    ifstream inputStream(fname.c_str(), ios::in);
    vector<Point2> result;
    Point2 point2;
    char ch;

    if (inputStream.fail()) {
        cerr << "Unable to open \""
             << fname
             << "\" for reading -- exiting." << endl;
        exit(EXIT_FAILURE);
    }
    for (;;) {
        ch = inputStream.get();
        if (ch == '\n') {
            continue; // skip blank lines
        } else if (ch == CH_EOF) {
            break; // end of file reached
        } else if (ch == '#') { // start of comment line
            // skip the rest of the line
            while (ch != '\n' && ch != CH_EOF)
                ch = inputStream.get();
            assert(ch == '\n'); // comment lines must be '\n'-terminated
        } else {
            inputStream.unget(); // read `ch` again as part of a number

            inputStream >> point2.a[0]; // leading whitespace okay
            do {
                ch = inputStream.get();
            } while (ch != ',' && ch != CH_EOF && ch != '\n');
            assert(ch == ','); // in CSV file, ',' is expected

            inputStream >> point2.a[1]; // leading whitespace okay
            do {
                ch = inputStream.get();
            } while (ch != ',' && ch != CH_EOF && ch != '\n');
            assert(ch == '\n'); // in CSV file, '\n' is expected

            result.push_back(point2);
        }
    }
    return result;
}


vector<Point3> readPoint3s(string fname)
//
// Read a vector of Point3s from a (simplified) CSV file.
//
{
    ifstream inputStream(fname.c_str(), ios::in);
    vector<Point3> result;
    Point3 point3;
    char ch;

    if (inputStream.fail()) {
        cerr << "Unable to open \""
             << fname
             << "\" for reading -- exiting." << endl;
        exit(EXIT_FAILURE);
    }
    for (;;) {
        ch = inputStream.get();
        if (ch == '\n') {
            continue; // skip blank lines
        } else if (ch == CH_EOF) {
            break; // end of file reached
        } else if (ch == '#') { // start of comment line
            // skip the rest of the line
            while (ch != '\n' && ch != CH_EOF)
                ch = inputStream.get();
            assert(ch == '\n'); // comment lines must be '\n'-terminated
        } else {
            inputStream.unget(); // read `ch` again as part of a number

            inputStream >> point3.a[0]; // leading whitespace okay
            do {
                ch = inputStream.get();
            } while (ch != ',' && ch != CH_EOF && ch != '\n');
            assert(ch == ','); // in CSV file, ',' is expected

            inputStream >> point3.a[1]; // leading whitespace okay
            do {
                ch = inputStream.get();
            } while (ch != ',' && ch != CH_EOF && ch != '\n');
            assert(ch == ','); // in CSV file, ',' is expected

            inputStream >> point3.a[2]; // leading whitespace okay
            do {
                ch = inputStream.get();
                assert(ch != CH_EOF); // bad file format
            } while (ch != ',' && ch != CH_EOF && ch != '\n');
            assert(ch == '\n'); // in CSV file, '\n' is expected

            result.push_back(point3);
        }
    }
    return result;
}

