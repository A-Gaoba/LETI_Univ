#include<BMP.h>

bool BMP::within_range(double x){
    return 0 <= x && x <= 1;
}

bool BMP::inside_triangle(int x, int y, Triangle tri){
    int x0 = tri.X1, y0 = tri.Y1, x1 = tri.X2, y1 = tri.Y2,
            x2 = tri.X3, y2 = tri.Y3;
    double det = (y1-y2)*(x0-x2) + (x2-x1)*(y0-y2);
    double factor_alpha = (y1-y2)*(x-x2)+(x2-x1)*(y-y2);
    double factor_beta = (y2-y0)*(x-x2)+(x0-x2)*(y-y2);
    double alpha = factor_alpha/det;
    double beta = factor_beta/det;
    double gamma = 1.0 - alpha - beta;

    return (x==x0 && y==y0) || (x==x1 && y==y1) || (x==x1 && y==y1) ||
            (within_range(alpha) && within_range(beta) && within_range(gamma));
}


BMP::BMP(const char *fname) {
    read(fname);
}

BMP::BMP(const char *fname, char* shapes_fname) {
    read(fname, shapes_fname);
}


BMP::BMP(int32_t width, int32_t height, bool has_alpha) {
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("The image width and height must be positive numbers.");
    }

    bmp_info_header.width = width;
    bmp_info_header.height = height;
    if (has_alpha) {
        bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);

        bmp_info_header.bit_count = 32;
        bmp_info_header.compression = 3;
        row_stride = width * 4;
        data.resize(row_stride * height);
        file_header.file_size = file_header.offset_data + data.size();
    }
    else {
        bmp_info_header.size = sizeof(BMPInfoHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

        bmp_info_header.bit_count = 24;
        bmp_info_header.compression = 0;
        row_stride = width * 3;
        data.resize(row_stride * height);

        uint32_t new_stride = make_stride_aligned(4);
        file_header.file_size = file_header.offset_data + static_cast<uint32_t>(data.size()) + bmp_info_header.height * (new_stride - row_stride);
    }
}


void BMP::read(const char *fname) {
    // inout file stream
    std::ifstream inp( fname, std::ifstream::binary ); //open binary file
    if (inp) {
        inp.read((char*)&file_header, sizeof(file_header)); // read the file and extracts sizeof(file_header) from file_header
        if(file_header.file_type != 0x4D42) { // it's bmp or not
            throw std::runtime_error("Error! Unrecognized file format.");
        }
        inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

        // The BMPColorHeader is used only for transparent images
        if(bmp_info_header.bit_count == 32) {
            // Check if the file has bit mask color information
            if(bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
                // Check if the pixel data is stored as BGRA and if the color space type is sRGB
                check_color_header(bmp_color_header);
            } else {
                // standard output stream for errors
                std::cerr << "Error! The file \"" << fname << "\" does not seem to contain bit mask information\n";
                throw std::runtime_error("Error! Unrecognized file format.");
            }
        }

        // Jump to the pixel data location
        inp.seekg(file_header.offset_data, inp.beg);// inp.beg beginning of stream

        // Adjust the header fields for output.
        // Some editors will put extra info in the image file, we only save the headers and the data.
        if(bmp_info_header.bit_count == 32) {
            bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        } else {
            bmp_info_header.size = sizeof(BMPInfoHeader);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
        }
       file_header.file_size = file_header.offset_data;

        bool scanning_bottom_up = true;
        // Pixles start from bottom left corner
        if (bmp_info_header.height < 0) {
            bmp_info_header.height = - bmp_info_header.height;
            scanning_bottom_up = false;
//            throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
        }

        data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

        // Here we check if we need to take into account row padding
        if (bmp_info_header.width % 4 == 0) {

            inp.read((char*)data.data(), data.size());
            if (!scanning_bottom_up){
                std::reverse(data.begin(), data.end());
                uint8_t R, G, B, A;
                for (int i=0; i<bmp_info_header.width; i++){
                    for (int j=0; j<bmp_info_header.height; j++){
                        get_pixel(i,j,&R,&G,&B,&A);
                        this->set_pixel(i, j, R, G, B, A);
                     }
                }
            }
            file_header.file_size += static_cast<uint32_t>(data.size());
        }
        else {
            row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
            uint32_t new_stride = make_stride_aligned(4);
            std::vector<uint8_t> padding_row(new_stride - row_stride);

            for (int y = 0; y < bmp_info_header.height; ++y) {
                inp.read((char*)(data.data() + row_stride * y), row_stride);
                inp.read((char*)padding_row.data(), padding_row.size());
            }
            if (!scanning_bottom_up){
                std::reverse(data.begin(), data.end());
                uint8_t R, G, B, A;
                for (int i=0; i<bmp_info_header.width; i++){
                    for (int j=0; j<bmp_info_header.height; j++){
                        get_pixel(i,j,&R,&G,&B,&A);
                        this->set_pixel(i, j, R, G, B, A);
                    }
                }
            }
            file_header.file_size += static_cast<uint32_t>(data.size()) + bmp_info_header.height * static_cast<uint32_t>(padding_row.size());
        }
    }
    else {
        throw std::runtime_error("Unable to open the input image file.");
    }
}

void BMP::read(const char *fname, char *shapes_fname) {
    // inout file stream
    std::ifstream inp{ fname, std::ios_base::binary }; //open binary file
    if (inp) {
        inp.read((char*)&file_header, sizeof(file_header)); // read the file and extracts sizeof(file_header) from file_header
        if(file_header.file_type != 0x4D42) { // it's bmp or not
            throw std::runtime_error("Error! Unrecognized file format.");
        }
        inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

        // The BMPColorHeader is used only for transparent images
        if(bmp_info_header.bit_count == 32) {
            // Check if the file has bit mask color information
            if(bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
                // Check if the pixel data is stored as BGRA and if the color space type is sRGB
                check_color_header(bmp_color_header);
            } else {
                // standard output stream for errors
                std::cerr << "Error! The file \"" << fname << "\" does not seem to contain bit mask information\n";
                throw std::runtime_error("Error! Unrecognized file format.");
            }
        }

        // Jump to the pixel data location
        inp.seekg(file_header.offset_data, inp.beg);// inp.beg beginning of stream

        // Adjust the header fields for output.
        // Some editors will put extra info in the image file, we only save the headers and the data.
        if(bmp_info_header.bit_count == 32) {
            bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        } else {
            bmp_info_header.size = sizeof(BMPInfoHeader);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
        }
        file_header.file_size = file_header.offset_data;

        bool scanning_bottom_up = true;
        // Pixles start from bottom left corner
        if (bmp_info_header.height < 0) {
            bmp_info_header.height = - bmp_info_header.height;
            scanning_bottom_up = false;

//            throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
        }

        data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

        // Here we check if we need to take into account row padding
        if (bmp_info_header.width % 4 == 0) {

            inp.read((char*)data.data(), data.size());
            if (!scanning_bottom_up){
                std::reverse(data.begin(), data.end());
                uint8_t R, G, B, A;
                for (int i=0; i<bmp_info_header.width; i++){
                    for (int j=0; j<bmp_info_header.height; j++){
                        get_pixel(i,j,&R,&G,&B,&A);
                        this->set_pixel(i, j, R, G, B, A);
                    }
                }
            }
            file_header.file_size += static_cast<uint32_t>(data.size());
        }
        else {
            row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
            uint32_t new_stride = make_stride_aligned(4);
            std::vector<uint8_t> padding_row(new_stride - row_stride);

            for (int y = 0; y < bmp_info_header.height; ++y) {
                inp.read((char*)(data.data() + row_stride * y), row_stride);
                inp.read((char*)padding_row.data(), padding_row.size());
            }
            if (!scanning_bottom_up){
                std::reverse(data.begin(), data.end());
                uint8_t R, G, B, A;
                for (int i=0; i<bmp_info_header.width; i++){
                    for (int j=0; j<bmp_info_header.height; j++){
                        get_pixel(i,j,&R,&G,&B,&A);
                        this->set_pixel(i, j, R, G, B, A);
                    }
                }
            }
            file_header.file_size += static_cast<uint32_t>(data.size()) + bmp_info_header.height * static_cast<uint32_t>(padding_row.size());
        }
        struct Shapes shapes = Shapes::read4file(shapes_fname);
        if (strcmp(shapes.filename, "")){
            printf("Error reading shapes file\n");
            exit(1);
        }else{
            this->shapes = shapes;
        }
    }
    else {
        throw std::runtime_error("Unable to open the input image file.");
    }
}



void BMP::write(const char *fname) {
    std::ofstream of{ fname, std::ofstream::binary };
    if (of) {
        if (bmp_info_header.bit_count == 32) {
            write_headers_and_data(of);
        }
        else if (bmp_info_header.bit_count == 24) {
            if (bmp_info_header.width % 4 == 0) {
                write_headers_and_data(of);
            }
            else {
                uint32_t new_stride = make_stride_aligned(4);
                std::vector<uint8_t> padding_row(new_stride - row_stride);

                write_headers(of);

                for (int y = 0; y < bmp_info_header.height; ++y) {
                    of.write((const char*)(data.data() + row_stride * y), row_stride);
                    of.write((const char*)padding_row.data(), padding_row.size());
                }
            }
        }
        else {
            throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
        }
    }
    else {
        throw std::runtime_error("Unable to open the output image file.");
    }
}


void BMP::write(const char *fname, char *shapes_fname) {
    std::ofstream of{ fname, std::ofstream::binary };
    if (of) {
        if (bmp_info_header.bit_count == 32) {
            write_headers_and_data(of);
        }
        else if (bmp_info_header.bit_count == 24) {
            if (bmp_info_header.width % 4 == 0) {
                write_headers_and_data(of);
            }
            else {
                uint32_t new_stride = make_stride_aligned(4);
                std::vector<uint8_t> padding_row(new_stride - row_stride);

                write_headers(of);

                for (int y = 0; y < bmp_info_header.height; ++y) {
                    of.write((const char*)(data.data() + row_stride * y), row_stride);
                    of.write((const char*)padding_row.data(), padding_row.size());
                }
            }
        }
        else {
            throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
        }
    }
    else {
        throw std::runtime_error("Unable to open the output image file.");
    }
    if (shapes.write2file(shapes_fname)){
        printf("Shapes are successfully stored!\n");
    }else{
        printf("error in storing shapes in file!\n");
    }
}


void BMP::fill_region(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
    if (x0 + w > (uint32_t)bmp_info_header.width || y0 + h > (uint32_t)bmp_info_header.height) {
        throw std::runtime_error("The region does not fit in the image!");
    }

    uint32_t channels = bmp_info_header.bit_count / 8;
    for (uint32_t y = y0; y < y0 + h; ++y) {
        for (uint32_t x = x0; x < x0 + w; ++x) {
            data[channels * (y * bmp_info_header.width + x) + 0] = B;
            data[channels * (y * bmp_info_header.width + x) + 1] = G;
            data[channels * (y * bmp_info_header.width + x) + 2] = R;
            if (channels == 4) {
                data[channels * (y * bmp_info_header.width + x) + 3] = A;
            }
        }
    }
}


void BMP::set_pixel(uint32_t x0, uint32_t y0, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
    if (x0 > (uint32_t)bmp_info_header.width || y0 > (uint32_t)bmp_info_header.height) {
        throw std::runtime_error("The point is outside the image boundaries!");
    }

    uint32_t channels = bmp_info_header.bit_count / 8;
    data[channels * (y0 * bmp_info_header.width + x0) + 0] = B;
    data[channels * (y0 * bmp_info_header.width + x0) + 1] = G;
    data[channels * (y0 * bmp_info_header.width + x0) + 2] = R;
    if (channels == 4) {
        data[channels * (y0 * bmp_info_header.width + x0) + 3] = A;
    }
}


void BMP::get_pixel(uint32_t x0, uint32_t y0, uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A) {
    if (x0 > (uint32_t)bmp_info_header.width || y0 > (uint32_t)bmp_info_header.height) {
        throw std::runtime_error("The point is outside the image boundaries!");
    }

    uint32_t channels = bmp_info_header.bit_count / 8;
    *B = data[channels * (y0 * bmp_info_header.width + x0) + 0];
    *G = data[channels * (y0 * bmp_info_header.width + x0) + 1];
    *R = data[channels * (y0 * bmp_info_header.width + x0) + 2];
    if (channels == 4) {
        *A = data[channels * (y0 * bmp_info_header.width + x0) + 3];
    }
}

void BMP::add_image(BMP current, int x, int y, int image_panel_width, int image_panel_height){
    if (current.bmp_info_header.width <= this->bmp_info_header.width
        && current.bmp_info_header.height <= this->bmp_info_header.height){
        uint8_t R, G, B, A;

        for (int i=0; i<current.bmp_info_header.width; i++){
            for (int j=0; j<current.bmp_info_header.height; j++){
                current.get_pixel(i,j,&R,&G,&B,&A);
                this->set_pixel(i+x, j+y, B, G, R, A);
            }
        }
    }else{
        printf("Error: The image doesn't fit...");
        exit(EXIT_FAILURE);
    }
}

void BMP::draw_rectangle(Rectangle rect){
    uint32_t x0 = rect.x_start, y0 = rect.y_start, w = rect.width, h = rect.height;
    uint8_t B = rect.color_B, G = rect.color_G, R = rect.color_R, A = ALPHA, line_w = rect.thickness;
    bool filled = rect.filled;
    uint8_t BF = rect.filled_color_B, GF = rect.filled_color_G, RF = rect.filled_color_R;
    if (x0 + w > (uint32_t)bmp_info_header.width || y0 + h > (uint32_t)bmp_info_header.height) {
        throw std::runtime_error("The rectangle does not fit in the image!");
    }

    fill_region(x0, y0, w, line_w, B, G, R, A);                                             // top line
    fill_region(x0, (y0 + h - line_w), w, line_w, B, G, R, A);                              // bottom line
    fill_region((x0 + w - line_w), (y0 + line_w), line_w, (h - (2 * line_w)), B, G, R, A);  // right line
    fill_region(x0, (y0 + line_w), line_w, (h - (2 * line_w)), B, G, R, A);                 // left line
    if (filled==1)
        fill_region(x0+line_w, y0+line_w, w-2*line_w, h-2*line_w, BF, GF, RF, A);

    // Adding the triangle to shapes of bmp
    (shapes).add_rectangle(rect);
}


void BMP::draw_triangle(Triangle triangle){
    uint32_t x0 = triangle.X1, y0 = triangle.Y1, x1 = triangle.X2, y1 = triangle.Y2, x2 = triangle.X3, y2 = triangle.Y3;
    uint8_t B = triangle.color_B, G = triangle.color_G, R = triangle.color_R, A = ALPHA, line_w = triangle.thickness;
    bool filled = triangle.filled;
    uint8_t BF = triangle.filled_color_B, GF = triangle.filled_color_G, RF = triangle.filled_color_R;
//    triangle.filename

    uint32_t xmax_image = bmp_info_header.width;
    uint32_t ymax_image = bmp_info_header.height;
    uint32_t xmax, ymax, xmin, ymin;
    if ((x0==x1 &&y0==y1) || (x1==x2 && y1==y2) || (x0==x2 && y0==y2)){
        throw std::runtime_error("The coordinates do not form a triangle!");
    }
    if (x0<xmax_image && x1<xmax_image && x2<xmax_image &&
            y0<ymax_image && y1<ymax_image && y2<ymax_image){


        std::vector<uint32_t> line_1_pixels =
                drawColoredLine(x0,y0,x1,y1,line_w, B,G,R,A);
        std::vector<uint32_t> line_2_pixels =
                drawColoredLine(x1,y1,x2,y2,line_w, B,G,R,A);
        std::vector<uint32_t> line_3_pixels =
                drawColoredLine(x2,y2,x0,y0,line_w, B,G,R,A);
        if (filled){
            // x0 x1 x2
            // y0 y1 y2

            xmax = max(max(x0,x1),x2);
            ymax = max(max(y0,y1),y2);
            xmin = min(min(x0,x1),x2);
            ymin = min(min(y0,y1),y2);

            for (uint32_t xi = xmin; xi<xmax; xi++){
                for (uint32_t yi = ymin; yi<ymax; yi++){
                    if (inside_triangle(xi, yi, triangle)){
                        vector<uint32_t>::iterator
                                itx1= std::find(line_1_pixels.begin(), line_1_pixels.end(),xi),
                                itx2= std::find(line_2_pixels.begin(), line_2_pixels.end(),xi),
                                itx3= std::find(line_3_pixels.begin(), line_3_pixels.end(),xi);
                        vector<uint32_t>::iterator
                                ity1= std::find(line_1_pixels.begin(), line_1_pixels.end(),yi),
                                ity2= std::find(line_2_pixels.begin(), line_2_pixels.end(),yi),
                                ity3= std::find(line_3_pixels.begin(), line_3_pixels.end(),yi);
                        int index1 = std::distance(line_1_pixels.begin(), itx1);
                        int index2 = std::distance(line_2_pixels.begin(), itx2);
                        int index3 = std::distance(line_3_pixels.begin(), itx3);

                        int index1y = std::distance(line_1_pixels.begin(), ity1);
                        int index2y = std::distance(line_2_pixels.begin(), ity2);
                        int index3y = std::distance(line_3_pixels.begin(), ity3);


                        if (itx1 != line_1_pixels.end() && index1%2==0
                                && ity1 != line_1_pixels.end() && index1y==index1+1
                                && itx2 != line_2_pixels.end() && index2%2==0
                                && ity2 != line_2_pixels.end() && index2y==index2+1
                                && itx3 != line_3_pixels.end() && index3%2==0
                                && ity3 != line_3_pixels.end() && index3y==index3+1){
                            ;//do nothing
                        }else{
                            set_pixel(xi,yi,BF,GF,RF,A); // inside triangle and not borders
                        }
                    }
                }
            }
        }
            // Adding the triangle to shapes of bmp
            shapes.add_triangle(triangle);

        }else{
            throw std::runtime_error("The triangle does not fit in the image!");
        }
}


std::vector<uint32_t> BMP::drawColoredLine(int x0, int y0, int x1, int y1, float wd, uint8_t B, uint8_t G, uint8_t R, uint8_t A)
{                                    /* plot an anti-aliased line of width wd */
   int dx = abs(int(x1-x0)), sx = x0 < x1 ? 1 : -1;
   int dy = abs(int(y1-y0)), sy = y0 < y1 ? 1 : -1;
   int err = dx-dy, e2, x2, y2;                           /* error value e_xy */
   float ed = dx+dy == 0 ? 1 : std::sqrt((float)dx*dx+(float)dy*dy);
   uint32_t x,y;
   std::vector<uint32_t> pixels;


   for (wd = (wd+1)/2; ; ) {                              /* pixel loop */
      x= x0; y=y0;
      set_pixel(x, y, B, G, R, A);
      pixels.push_back(x);
      pixels.push_back(y);
      e2 = err; x2 = x0;
      if (2*e2 >= -dx) {   /* x step */
         for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx)
          {
            x = x0;
            y = y2;

            set_pixel(x, y, B, G, R, A);
            pixels.push_back(x);
            pixels.push_back(y);
            y2 += sy;
         }
         if (x0 == x1) break;
         e2 = err; err -= dy; x0 += sx;
      }
      if (2*e2 <= dy) {    /* y step */
         for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy)
         {
            x = x2;
            y = y0;
            set_pixel(x, y, B, G, R, A);
            pixels.push_back(x);
            pixels.push_back(y);
            x2 += sx;
         }
         if (y0 == y1) break;
         err += dx; y0 += sy;
      }
   }
   return pixels;
}

void BMP::write_headers(std::ofstream &of) {
    of.write((const char*)&file_header, sizeof(file_header));
    of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
    if(bmp_info_header.bit_count == 32) {
        of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
    }
}

void BMP::write_headers_and_data(std::ofstream &of) {
    write_headers(of);
    of.write((const char*)data.data(), data.size());
}

// Add 1 to the row_stride until it is divisible with align_stride
uint32_t BMP::make_stride_aligned(uint32_t align_stride) {
    uint32_t new_stride = row_stride;
    while (new_stride % align_stride != 0) {
        new_stride++;
    }
    return new_stride;
}

// Check if the pixel data is stored as BGRA and if the color space type is sRGB
void BMP::check_color_header(BMPColorHeader &bmp_color_header) {
    BMPColorHeader expected_color_header;
    if(expected_color_header.red_mask != bmp_color_header.red_mask ||
        expected_color_header.blue_mask != bmp_color_header.blue_mask ||
        expected_color_header.green_mask != bmp_color_header.green_mask ||
        expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
        throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
    }
    if(expected_color_header.color_space_type != bmp_color_header.color_space_type) {
        throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
    }
}






