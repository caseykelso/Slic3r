#include <cassert>
#include <math.h>
#include <stdio.h>

#include "../ClipperUtils.hpp"
#include "../Geometry.hpp"
#include "../Surface.hpp"
#include "../PrintConfig.hpp"

#include "Fill.hpp"

namespace Slic3r {

Polylines
Fill::fill_surface(const Surface &surface)
{
    if (this->density == 0) return Polylines();
    
    // Perform offset.
    ExPolygons expp = offset_ex(surface.expolygon, -scale_(this->min_spacing)/2);
    
    // Implementations can change this if they adjust the flow.
    this->_spacing = this->min_spacing;
    
    // Create the infills for each of the regions.
    Polylines polylines_out;
    for (size_t i = 0; i < expp.size(); ++i)
        this->_fill_surface_single(
            surface.thickness_layers,
            this->_infill_direction(surface),
            expp[i],
            &polylines_out
        );
    return polylines_out;
}

// Returns orientation of the infill and the reference point of the infill pattern.
// For a normal print, the reference point is the center of a bounding box of the STL.
Fill::direction_t
Fill::_infill_direction(const Surface &surface) const
{
    // set infill angle
    float out_angle = this->angle;

    // Bounding box is the bounding box of a Slic3r::PrintObject
    // The bounding box is only undefined in unit tests.
    Point out_shift = this->bounding_box.defined
        ? this->bounding_box.center()
    	: surface.expolygon.contour.bounding_box().center();

    #if 0
        if (!this->bounding_box.defined) {
            printf("Fill::_infill_direction: empty bounding box!");
        } else {
            printf("Fill::_infill_direction: reference point %d, %d\n", out_shift.x, out_shift.y);
        }
    #endif

    if (surface.bridge_angle >= 0) {
	    // use bridge angle
        #ifdef SLIC3R_DEBUG
        printf("Filling bridge with angle %f\n", Slic3r::Geometry::rad2deg(surface.bridge_angle));
        #endif
        out_angle = surface.bridge_angle;
    } else if (this->layer_id != size_t(-1)) {
        // alternate fill direction
        out_angle += this->_layer_angle(this->layer_id / surface.thickness_layers);
    }

    out_angle += float(M_PI/2.);
    return direction_t(out_angle, out_shift);
}

} // namespace Slic3r
