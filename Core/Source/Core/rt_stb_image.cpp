// Disable strict warnings for this header from the Microsoft Visual C++ compiler.
#ifdef _MSC_VER
#pragma warning (push, 0)
#endif

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include "rt_stb_image.h"

namespace render_core {
    uchar rt_image::float_to_byte(float value)
    {
        if (value <= 0.0) { return 0; }
        if (1.0 <= value) { return 255; }
        return static_cast<uchar>(256.0 * value);
    }

    void rt_image::convert_to_bytes()
    {
        int total_bytes = image_width * image_height * bytes_per_pixel;
        bdata = new uchar[total_bytes];

        uchar* bptr = bdata;
        float* fptr = fdata;
        for (int i = 0; i < total_bytes; ++i, ++fptr, ++bptr) { *bptr = float_to_byte(*fptr); }
    }

    int rt_image::clamp(int x, int low, int high)
    {
        if (x < low) { return low; }
        if (x < high) { return x; }
        return high - 1;
    }

    rt_image::rt_image() {}
    rt_image::rt_image(const char* image_filename)
    {
        std::string filename = std::string(image_filename);
        char* imgdir = getenv("RTW_IMAGES");

        if (imgdir && load(std::string(imgdir) + "/" + image_filename)) { return; }
        if (load(filename)) { return; }
        if (load("images/" + filename)) { return; }
        if (load("../images/" + filename)) { return; }
        if (load("../../images/" + filename)) { return; }
        if (load("../../../images/" + filename)) { return; }
        if (load("../../../../images/" + filename)) { return; }
        if (load("../../../../../images/" + filename)) { return; }
        if (load("../../../../../../images/" + filename)) { return; }

        std::cerr << "ERROR: Could no load image file '" << image_filename << "'.\n";
    }
    rt_image::~rt_image() {
        delete[] bdata;
        stbi_image_free(fdata);
        fdata = nullptr;
    }

    bool rt_image::load(const std::string& filename)
    {
        int n = bytes_per_pixel;
        fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
        if (fdata == nullptr) { return false; }

        bytes_per_scanline = image_width * bytes_per_pixel;
        convert_to_bytes();
        return true;
    }

    int rt_image::width() const { return (fdata == nullptr) ? 0 : image_width; }
    int rt_image::height() const { return (fdata == nullptr) ? 0 : image_height; }

    const uchar* rt_image::pixel_data(int x, int y) const
    {
        static uchar magenta[] = { 255, 0, 255 };
        if (bdata == nullptr) { return magenta; }

        x = clamp(x, 0, image_width);
        y = clamp(y, 0, image_height);

        return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
    }
}