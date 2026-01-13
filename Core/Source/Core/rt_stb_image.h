#pragma once

#include <string>

typedef unsigned char uchar;

namespace render_core {

    class rt_image
    {
    private:
        const int   bytes_per_pixel = 3;
        float*      fdata = nullptr;
        uchar*      bdata = nullptr;
        int         image_width = 0;
        int         image_height = 0;
        int         bytes_per_scanline = 0;

        static uchar float_to_byte(float value);

        void convert_to_bytes();

        static int clamp(int x, int low, int high);
    public:
        rt_image();
        rt_image(const char* image_filename);

        ~rt_image();

        bool load(const std::string& filename);

        int width() const;
        int height() const;

        const uchar* pixel_data(int x, int y) const;
    };
}