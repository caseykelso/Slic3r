#include "FillConcentric.hpp"
#include "FillHoneycomb.hpp"
#include "Fill3DHoneycomb.hpp"
#include "FillGyroid.hpp"
#include "FillPlanePath.hpp"
#include "FillRectilinear.hpp"
#include "FillType.hpp"

Fill* FillType::new_from_type(const InfillPattern type)
{
    switch (type) {
        case ipConcentric:          return new FillConcentric();
        case ipHoneycomb:           return new FillHoneycomb();
        case ip3DHoneycomb:         return new Fill3DHoneycomb();
        case ipGyroid:              return new FillGyroid();
        
        case ipRectilinear:         return new FillRectilinear();
        case ipAlignedRectilinear:  return new FillAlignedRectilinear();
        case ipGrid:                return new FillGrid();
        
        case ipTriangles:           return new FillTriangles();
        case ipStars:               return new FillStars();
        case ipCubic:               return new FillCubic();
        
        case ipArchimedeanChords:   return new FillArchimedeanChords();
        case ipHilbertCurve:        return new FillHilbertCurve();
        case ipOctagramSpiral:      return new FillOctagramSpiral();
        
        default: CONFESS("unknown type"); return NULL;
    }
}

Fill* FillType::new_from_type(const std::string &type)
{
    static t_config_enum_values enum_keys_map = ConfigOptionEnum<InfillPattern>::get_enum_values();
    t_config_enum_values::const_iterator it = enum_keys_map.find(type);
    return (it == enum_keys_map.end()) ? NULL : new_from_type(InfillPattern(it->second));
}
