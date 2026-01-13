#pragma once

#include <memory>

#include "hittable_list.h"

namespace render_core {

    class obj_loader
    {
    private:
        std::shared_ptr<hittable_list> sides;
    public:
        obj_loader();

        void load_models(const char* filename, const char* mtlsdir);

        std::shared_ptr<hittable_list> get_sides();
    };
}
