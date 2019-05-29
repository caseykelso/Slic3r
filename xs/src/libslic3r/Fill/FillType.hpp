#pragma once

#include "Fill.hpp"

namespace Slic3r {

class FillType
{
    public:

        static Fill* new_from_type(const InfillPattern type);
        static Fill* new_from_type(const std::string &type);

};
} // namespace Slic3r
