#ifndef COMMANDS_H
#define COMMANDS_H


#ifndef UTILS_H
#include<Utils.h>
#endif
#if !defined(BMP_H)
#include<BMP.h>
#endif

Triangle draw_triangle_command(int argc, char *argv[]){//, struct BMP bmp

    printf("Drawing a Triangle\n");

    Triangle triangle;
    int opt= 0;

    static struct option long_options[] = {
        {"P0", required_argument, 0,  'A' }, // -A   --P0
        {"P1", required_argument,       0, 'B' },
        {"P2", required_argument, 0,  'C' },
        {"thick",    required_argument, 0,  't' },
        {"color",   required_argument, 0,  'c' }, // --color -c
        {"filled",   no_argument, 0,  'f' },
        {"fillColor",   required_argument, 0,  'l' },
        {"input",   required_argument, 0,  'i' },
        {0,           0,                 0,  0   }
    };

    int long_index =0;
    bool fill_colored=false;
    char* filename = NULL;
    unsigned int width=1, height=1;
    BMP bmp(width, height,false);
    try {
        if ((opt = getopt_long(argc, argv,"A:B:C:t:c:fl:i:",
                               long_options, &long_index ))==-1){throw exception();}
        if(argc < 18) throw exception();
        do{
            switch (opt) {
            case 'A' :
                    triangle.X1 = stoi(optarg);
                    triangle.Y1 = stoi(argv[optind]);
                    break;
            case 'B' :

                triangle.X2 = stoi(optarg);
                triangle.Y2 = stoi(argv[optind]);
                break;
            case 'C' :

                triangle.X3 = stoi(optarg);
                triangle.Y3 = stoi(argv[optind]);
                break;
            case 't' :
                    triangle.thickness = stoi(optarg);
                    break;
            case 'c' :
                    triangle.color_R = stoi(optarg);
                    triangle.color_G = stoi(argv[optind]);
                    triangle.color_B = stoi(argv[optind+1]);
                    break;
            case 'f':
                    triangle.filled = true;
                    break;
            case 'l':
                if (triangle.filled){
                   triangle.filled_color_R = stoi(argv[optind-1]);
                   triangle.filled_color_G = stoi(argv[optind]);
                   triangle.filled_color_B = stoi(argv[optind+1]);
               }
               fill_colored = true;
                break;
            case 'i':
                if (is_bmp_file(optarg)){
                    filename = optarg;
                }else{
                    printf("Error: Image file name is not recognized...\n");
                    throw exception();
                }
                //
                if (fexists(filename)){
                    bmp =  BMP(filename);
                    width = bmp.bmp_info_header.width;
                    height = bmp.bmp_info_header.height;
                }else{
                    try {
                        width = stoi(argv[optind]);
                        if (width<triangle.get_max_X()) throw exception();
                        height = stoi(argv[optind+1]);
                        if (height<triangle.get_max_Y()) throw exception();
                    } catch (exception const & e) {
                            width = triangle.get_max_X()*2, height = triangle.get_max_Y()*2;
                            printf("Warning: width and height should be unsigned integers.\nDefault values: Width = %d , Height = %d\n", width, height);
                    }
                }

                printf("Current Values: Width = %d, Height = %d\n", width, height);
                break;

            default: print_usage_tri(); exit(EXIT_FAILURE);
            }
        }while ((opt = getopt_long(argc, argv,"A:B:C:t:c:fl:i:",
                   long_options, &long_index )) != -1);
        if (filename==NULL || strcmp(filename,"")==0) throw exception();


        if(triangle.filled && !fill_colored){
           printf("Note: You should specify fill color for the triangle "
                  "but it will be assigned to border color\n");
           triangle.filled_color_R = triangle.color_R;
           triangle.filled_color_G = triangle.color_G;
           triangle.filled_color_B = triangle.color_B;
           // The user have to select filled
        }
    } catch (exception const & e) {
            printf("Command Options error \n");
            print_usage_tri();
            exit(EXIT_FAILURE);

    }

        char* shapes_filename = Shapes::get_shapes_filename(filename);
        bmp = BMP(width, height, false);
        if (fexists(string2array(filename)) && fexists(string2array(shapes_filename))){
            bmp = BMP(string2array(filename), string2array(shapes_filename));
        }
        bmp.draw_triangle(triangle);
        bmp.write(filename, shapes_filename);
    return triangle;
}

Rectangle draw_rectangle_command(int argc, char *argv[]){

    printf("Drawing a Rectangle\n");

    Rectangle rect;
    int opt= 0;


    static struct option long_options[] = {
        {"start", required_argument, 0,  's' },
        {"width", required_argument, 0,  'w' },
        {"height", required_argument, 0,  'h' },
        {"thick",    required_argument, 0,  't' },
        {"color",   required_argument, 0,  'c' },
        {"filled",   no_argument, 0,  'f' },
        {"fillColor",   required_argument, 0,  'l' },
    {"input",   required_argument, 0,  'i' },
        {0,           0,                 0,  0   }
    };

    int long_index =0;
    bool fill_colored=false;
    char* filename = NULL;
    unsigned int width=1, height=1;
    BMP bmp(width, height,false);
    try {
        if((opt = getopt_long(argc, argv,"s:w:h:t:c:fl:i:",
                              long_options, &long_index ))==-1)throw exception();
        if (argc < 16) throw exception();
        do{
            switch (opt) {
                case 's' :
                        rect.x_start = stoi(optarg);
                        rect.y_start = stoi(argv[optind]);
                        break;
                case 'w' :

                    rect.width = stoi(optarg);
                    rect.x_end = rect.x_start+rect.width;
                    break;
                case 'h' :
                    rect.height = stoi(optarg);
                    rect.y_end = rect.y_start + rect.height;
                    break;
                case 't' :
                        rect.thickness = stoi(optarg);
                        break;
                case 'c' :
                        rect.color_R = stoi(optarg);
                        rect.color_G = stoi(argv[optind]);
                        rect.color_B = stoi(argv[optind+1]);
                        break;
                case 'f':
                        rect.filled = true;
                        break;
                case 'l':
                    if(rect.filled){
                       rect.filled_color_R = stoi(argv[optind-1]);
                       rect.filled_color_G = stoi(argv[optind]);
                       rect.filled_color_B = stoi(argv[optind+1]);
                   }
                   fill_colored = true;
                    break;
                case 'i':
                    if (is_bmp_file(optarg)){
                        filename = optarg;
                    }else{
                        printf("Error: Image file name is not recognized...\n");
                        throw exception();
                    }
                    if (fexists(filename)){
                        bmp =  BMP(filename);
                        width = bmp.bmp_info_header.width;
                        height = bmp.bmp_info_header.height;
                    }else{
                        try {
                            width = stoi(argv[optind]);
                            if (width<rect.get_max_X()) throw exception();
                            height = stoi(argv[optind+1]);
                            if (height<rect.get_max_Y()) throw exception();
                        } catch (exception const & e) {
                                width = rect.get_max_X()*2, height = rect.get_max_Y()*2;
                                printf("Warning: width and height should be unsigned integers.\nDefault values: Width = %d , Height = %d\n", width, height);
                        }
                    }

                    printf("Current Values: Width = %d, Height = %d\n", width, height);
                    break;

                default: print_usage_rect(); exit(EXIT_FAILURE);
            }
        } while ((opt = getopt_long(argc, argv,"s:w:h:t:c:fl:i:",
                   long_options, &long_index )) != -1);
        if (filename==NULL || strcmp(filename,"")==0) throw exception();


        if(rect.filled && !fill_colored){
           printf("Note: You should specify fill color for the triangle "
                  "but it will be assigned to border color\n");
           rect.filled_color_R = rect.color_R;
           rect.filled_color_G = rect.color_G;
           rect.filled_color_B = rect.color_B;
           // The user have to select filled
        }
    }catch (exception const & e) {
            printf("Command Options error \n");
            print_usage_rect();
            exit(EXIT_FAILURE);
    }

    char* shapes_filename = Shapes::get_shapes_filename(filename);
    bmp = BMP(width, height, false);
    if (fexists(string2array(filename)) && fexists(string2array(shapes_filename))){
        bmp = BMP(string2array(filename), string2array(shapes_filename));
    }
    bmp.draw_rectangle(rect);
    bmp.write(filename, shapes_filename);
    return rect;
}

void repaint_command(int argc, char *argv[]){
    printf("Repainting a shape!\n");
        uint8_t newR = 0; // 0-255
        uint8_t newG = 0;
        uint8_t newB = 0;
        bool is_rectangle = false;
        char* filename;
        int opt = 0;

        try{
            static struct option long_options[] = {
                {"shape", required_argument, 0,  's' },
                {"color", required_argument, 0,  'c' },
                {"input", required_argument, 0,  'i' },
                {0,           0,                 0,  0   }
            };

            int long_index = 0;
            if ((opt = getopt_long(argc, argv,"s:c:i:",
                                   long_options, &long_index ))==-1) throw exception();
            if (argc < 10) throw exception();
                do{
                    switch (opt) {
                        case 's' : // repaint -s circle
                            if (strcmp(optarg,"rect")==0) is_rectangle = true;
                            else if (strcmp(optarg,"tri")==0) is_rectangle = false;
                            else {printf("Only rect and tri is availibile\n");throw exception();}
                            break;
                        case 'c' :// repaint -s rect -c 150 40 0
                            newR = stoi(optarg);
                            newG = stoi(argv[optind]);
                            newB = stoi(argv[optind+1]);
                            break;
                    case 'i' :
                        if (is_bmp_file(optarg) && fexists(optarg)){
                            filename = optarg;
                        }else{
                            printf("Error: Image file name is not recognized or not found...\n");
                            throw exception();
                        }
                        break;
                    default: throw exception();
                    }
            }while ((opt = getopt_long(argc, argv,"s:c:i:",
                               long_options, &long_index )) != -1);
            if (!is_bmp_file(filename) || !fexists(filename)) {printf("Error: File not recognized\n");throw exception();}

            char* shapes_filename = Shapes::get_shapes_filename(filename);
            BMP bmp = BMP(string2array(filename), string2array(shapes_filename));

            int index;
            if (is_rectangle && bmp.shapes.rectangles.size()>0){
               printf("Get largest rectangle...\n");
               bmp.shapes.get_largest_rectangle(&index);

               printf("Found largest rectangle in index = %d \n", index);

               Rectangle rect = bmp.shapes.rectangles.at(index);
               rect.color_R = newR;
               rect.color_G = newG;
               rect.color_B = newB;
               bmp.shapes.rectangles.erase(bmp.shapes.rectangles.begin()+index);
               bmp.draw_rectangle(rect);

               printf("Redraw largest rectangle with color (rgb) = %d %d %d \n", rect.color_R,  rect.color_G, rect.color_B);

            }else if(bmp.shapes.triangles.size()>0){
                printf("Get largest triangle...\n");

               bmp.shapes.get_largest_triangle(&index);

               printf("Found largest triangle in index = %d \n", index);

               Triangle tri = bmp.shapes.triangles.at(index);
               tri.color_R = newR;
               tri.color_G = newG;
               tri.color_B = newB;
               bmp.shapes.triangles.erase(bmp.shapes.triangles.begin()+index);
               bmp.draw_triangle(tri);
               printf("Redraw largest triangle with color (rgb) = %d %d %d \n", tri.color_R,  tri.color_G, tri.color_B);
           }else{
                throw std::runtime_error("shape not found \n");
            }
            bmp.write(filename, shapes_filename);
        }catch (exception const & e) {
            printf("Command Options error\n");
            print_usage_repaint();
            exit(EXIT_FAILURE);
        }
}

unsigned int getLargestBMP(std::vector<BMP> images, int* max_width, int* max_height){
    unsigned int max = 0;
    unsigned int index = 0, max_index;
    *max_width = *max_height = 0;
    for(auto image:images){
        unsigned int value = image.bmp_info_header.width * image.bmp_info_header.height;
        if (value>max){
            max = value;
            max_index = index;
        }
        if (image.bmp_info_header.width>*max_width){
            *max_width = image.bmp_info_header.width;
        }
        if (image.bmp_info_header.height>*max_height){
            *max_height = image.bmp_info_header.height;
        }
        index++;
    }
    return index;
}

BMP create_collage(int M, int N, std::vector<BMP> bmps){
    int max_width, max_height;
    getLargestBMP(bmps, &max_width, &max_height);
    BMP collage = BMP(max_width*M, max_height*N, false);
    int next = 0;
    int x, y;
    for (int i=0; i < M; i++){
        for (int j=0; j < N; j++){
           x = i*max_width; // x = 0 , y = 0 ,
           y = j*max_height;
           // center the image
            x += (max_width-bmps.at(next).bmp_info_header.width)/2;
            y += (max_height-bmps.at(next).bmp_info_header.height)/2;

            collage.add_image(bmps[next++], x, y, max_width, max_height);
            next = next % bmps.size(); // next 0 , 1 , 2 , 3 , 0 , 1 , 2 , 3
        }
    }
    return collage;
}

void collage_command(int argc, char *argv[]){
    printf("Creating a collage of images!\n");
        std::vector<std::string> images;
        unsigned int M, N;
        int opt = 0;
        char* output_filename;

        try{
            static struct option long_options[] = {
                {"M", required_argument, 0,  'm' },
                {"N", required_argument, 0,  'n' },
                {"output", required_argument, 0,  'o' },
                {"images", required_argument, 0,  'i' },
                {0,           0,                 0,  0   }
            };

            int long_index = 0;
            if ((opt = getopt_long(argc, argv,"m:n:o:i:",
                                   long_options, &long_index )) == -1) throw exception();
            if (argc<9) throw exception();
            do{
                switch (opt) {
                case 'm' : M = stoi(optarg);
                    break;
                case 'n' : N = stoi(optarg);
                    break;
                case 'o':
                    output_filename = optarg;
                    break;
                case 'i' ://collage -m 4 -n 6 -i image1.bmp image2.bmp image3.bmp image4.bmp -o output.bmp
                    for (int i=optind-1; i<argc; i++){
                        if(argv[i][0]=='-') break; // end of arguments
                        if (is_bmp_file(argv[i]) && fexists(argv[i])){
                            images.push_back(argv[i]);
                        }
                    }
                break;
                default:
                    throw exception();
                }
            }while ((opt = getopt_long(argc, argv,"m:n:o:i:",
                           long_options, &long_index )) != -1);

            std::vector<BMP> bmps;
            if (images.size()<=0) {printf("No valid images found! :(\n");throw exception();}

            for (std::string str : images){
                char* filename = string2array(str);// char* str // std::string str2 ="sdfsdfs";

                try {
                    bmps.push_back(BMP(filename));
                } catch (exception const & e) {
                    printf("Unrecognized %s image file", filename);
                    throw e;
                }
            }
            BMP collage = create_collage(M, N, bmps);
            printf("Saving collage in file %s ...\n", output_filename);
            collage.write(output_filename);
            printf("Saved successfully!\n");

        }catch (exception const & e) {
            printf("Command Options error\n");
            print_usage_collage();
            exit(EXIT_FAILURE);
        }
}

void error_command(){
    printf("available commands are:\n1- Draw Triangle\n");
    print_usage_tri();
    printf("2- Draw Rectangle\n");
    print_usage_rect();
    printf("3- Repaint Rectangle or Triangle\n");
    print_usage_repaint();
    printf("4- Create collage of images\n");
    print_usage_collage();
    printf("5- Get info of image and shapes\n");
    print_usage_info();
    printf("Re-run the program with available commands :)\n");
}


void info_command(int argc, char *argv[]){
    char* filename = argv[argc-1];
    if (!is_bmp_file(filename)){
        printf("Error: You should specify an image file....\n");
        exit(EXIT_FAILURE);
    }
    Shapes shapes = Shapes::read4file(Shapes::get_shapes_filename(filename));
    if (shapes.triangles.size()>0 || shapes.rectangles.size()>0){
        shapes.to_string();
    }else{
        printf("The image doesnt contain any recognizable shapes.....\n");
    }
}

#endif 
