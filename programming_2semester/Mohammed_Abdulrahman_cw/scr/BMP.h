#ifndef BMP_H
#define BMP_H
#pragma once

#ifndef STRUCTURES_H
#include <Structures.h>
#endif

using namespace std;

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 }; 
    uint32_t file_size{ 0 };      
    uint16_t reserved1{ 0 };     
    uint16_t reserved2{ 0 };     
    uint32_t offset_data{ 0 };  
}; // 14 bytes

struct BMPInfoHeader {
    uint32_t size{ 0 }; 
    int32_t width{ 0 }; 
    int32_t height{ 0 };      
                                            
                                             
    uint16_t planes{ 1 };  
    uint16_t bit_count{ 0 }; 
    uint32_t compression{ 0 };
    uint32_t size_image{ 0 }; 
    int32_t x_pixels_per_meter{ 0 }; 
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };    
    uint32_t colors_important{ 0 }; 
}; // 40 bytes

struct BMPColorHeader {
    uint32_t red_mask{ 0x00ff0000 }; 
    uint32_t green_mask{ 0x0000ff00 }; 
    uint32_t blue_mask{ 0x000000ff }; 
    uint32_t alpha_mask{ 0xff000000 };
    uint32_t color_space_type{ 0x73524742 }; 
    uint32_t unused[16]{ 0 }; 
}; // 24 bytes
#pragma pack(pop)

struct BMP {
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    BMPColorHeader bmp_color_header;
    std::vector<uint8_t> data;
    struct Shapes shapes;

    BMP(const char *fname, char* shapes_fname);

    BMP(const char *fname);

    BMP(int32_t width, int32_t height, bool has_alpha = true);

    void read(const char *fname, char *shapes_fname);

    void read(const char *fname);

    void write(const char *fname);

    void write(const char *fname, char *shapes_fname);

    void fill_region(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t B, uint8_t G, uint8_t R, uint8_t A) ;

    void set_pixel(uint32_t x0, uint32_t y0, uint8_t B, uint8_t G, uint8_t R, uint8_t A);

    void get_pixel(uint32_t x0, uint32_t y0, uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A);

    void draw_rectangle(Rectangle rect);

    void draw_triangle(Triangle  triangle);

    std::vector<uint32_t> drawColoredLine(int x0, int y0, int x1, int y1, float wd, uint8_t B, uint8_t G, uint8_t R, uint8_t A);

    void add_image(BMP current, int x, int y, int image_panel_width, int image_panel_height);


private:
    uint32_t row_stride{ 0 };

    void write_headers(std::ofstream &of);

    void write_headers_and_data(std::ofstream &of);


    // Add 1 to the row_stride until it is divisible with align_stride
    uint32_t make_stride_aligned(uint32_t align_stride);

    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
    void check_color_header(BMPColorHeader &bmp_color_header);

    static bool inside_triangle(int x, int y, Triangle tri);

    static bool within_range(double x);

};


#endif

