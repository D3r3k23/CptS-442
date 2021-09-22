#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
using namespace std;

#include "geometry.h"
#include "wrap_cmath_inclusion.h"

#define CH_EOF (-1) // denotes end-of-file



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

