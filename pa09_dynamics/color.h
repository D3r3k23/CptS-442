#ifndef INCLUDED_COLOR

//
// The "color" module provides two color-related classes, Color and
// Rgb, defined here, both derived from Vec3.
//
// The main difference between them is that Color components are
// presumed to have dimensions and units and Rgb has the same
// attributes as Color, but its components are dimensionless.
//
// A light source, for example, gives off a spectral energy
// distribution so would have a Color, but a filter only modifies any
// Color passing through it, so it would have an Rgb.
//
// For this reason, we permit "Rgb * Rgb", but not "Color * Color":
// "Rgb * Rgb -> Rgb" is dimensionless, but the dimensions of "Color
// * Color" make no sense. (We can always add the operator if we come
// up with a good reason to do so.)
//
// Rgbs are designed to be vector "scale factors" that multiply
// colors ("Rgb * Color -> Color" and "Color * Rgb -> Color". Again,
// if we can come up with a good reason to add Rgbs (as we did for
// Point3's -- see "geometry.h"), we can always add it.
//
// Making this distinction allows us to claim that *coaster* lighting
// is physically correct. If irradiances are actual irradiances (in
// watts per square meter), then the reflected radiances are in those
// same units.
//

#include "vec.h"

class Color : public Vec3
//
// see comments above
//
{

public:

Color()
    : Vec3()
    { };

    Color(double r, double g, double b)
    : Vec3(r, g, b)
    { };

    Color(const double a[3])
    : Vec3(a)
    { };

    // Color + Color -> Color
    const Color operator+(const Color color) const
    {
        return Color(a[0] + color.a[0],
                     a[1] + color.a[1],
                     a[2] + color.a[2]);
    }

    // Color * scalar -> Color
    const Color operator*(const double scale) const
    {
        return Color(a[0] * scale,
                     a[1] * scale,
                     a[2] * scale);
    }
};

// scalar * Color -> Color
inline Color operator*(const double scale, const Color color)
{
    return color * scale; // commutative
}


class Rgb : public Vec3
//
// see comments above
//
{

public:

    Rgb()
    : Vec3()
    { };

    Rgb(double r, double g, double b)
    : Vec3(r, g, b)
    { };

    Rgb(double a[3])
    : Vec3(a)
    { };

    // Rgb * Color -> Color
    const Color operator*(const Color color) const
    {
        return Color(a[0] * color.a[0],
                     a[1] * color.a[1],
                     a[2] * color.a[2]);
    }

    // Rgb * Rgb -> Rgb
    const Rgb operator*(const Rgb rgb) const
    {
        return Rgb(a[0] * rgb.a[0],
                   a[1] * rgb.a[1],
                   a[2] * rgb.a[2]);
    }

    // Rgb * scalar -> Rgb
    const Rgb operator*(const double scale) const
    {
        return Rgb(a[0] * scale,
                   a[1] * scale,
                   a[2] * scale);
    }
};

// Color * Rgb -> Color
inline Color operator*(const Color color, const Rgb rgb)
{
    return rgb * color; // commutative
}

// scalar * Rgb -> Rgb
inline Rgb operator*(const double scale, const Rgb rgb)
{
    return rgb * scale; // commutative
}

//
// Useful shorthand. Remember that these are *Colors*, not Rgbs.
//
extern const Color blackColor;
extern const Color blueColor;
extern const Color cyanColor;
extern const Color greenColor;
extern const Color magentaColor;
extern const Color redColor;
extern const Color whiteColor;
extern const Color yellowColor;

extern const Rgb blackRgb;

#define INCLUDED_COLOR
#endif // INCLUDED_COLOR
