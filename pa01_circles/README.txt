pa01_circles: Making OpenGL Draw Circles
========================================

OpenGL does not draw curved lines.  Even when drawing Bezier curves or
NURBS surfaces (we'll discuss these later in the course), ultimately
OpenGL is drawing straight lines (including triangle edges).  It is
possible, however, to approximate a circle by drawing a regular
polygon with a large enough value of *n*, the number of sides.

A traditional rule of thumb in graphics when doing this is to let *n*
be a fixed value (typically 50), but we're going to do better than
that by drawing a polygon that (a) is indistinguishable from a circle
on the output hardware and (b) is drawn with the minimum number of
lines needed to satsify (a) according to the circle's radius.

We can think of a circle as a regular polygon with an infinite number
of sides, but we are too impatient to actually let *n* be infinite!
Instead, we'll realize that the resolution of our display is not
infinitesimal -- in the end, we're setting pixels on and off -- and
find the value of *n* that lets us draw a regular polygon that comes
within half a pixel of the true circle, and then just draw a regular
polygon with that many sides with that value.

This figure shows the approximation problem for *n* = 6:

.. image:: drawcircle.pdf
   :align: center

*D* is the maximum departure of the polygon from circularity.

Perform these steps:

#. Derive a trigonometric formula relating *D* to *r* and *n*.  (Note
   the dependence on *r*: Approximating bigger circles requires more
   sides than little ones.)

#. Let *P* be the resolution of the screen in NDC coordinates per
   pixel and solve *D = P/2* for *n*. Using this value of *n* (or
   greater) will guarantee that your circle is within a half-pixel of
   exactness.

#. Incorporate your newly-discovered formula into
   ``Circle::tesselate()``, making it tesselate the circle into a
   regular polygon as a (closed) PolyLine with *n* sides. (Use
   ``ceil(3)`` to make sure *n* is an integer.)

#. Make other changes as instructed in the ``ASSIGNMENT`` comments.

Assume that *P* is 0.00119.  This is the ratio of the width of the
window in NDC coordinates (2.0) to a typical window width in pixels
(1680).
