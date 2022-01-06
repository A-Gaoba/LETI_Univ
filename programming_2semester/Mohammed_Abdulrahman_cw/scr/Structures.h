#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

#define ALPHA 0 // this deactivates alpha channel of all processed images

using namespace std;


struct Rectangle{
public:
    int x_start = 0, y_start = 0, x_end = 0, y_end =0;
    int thickness = 1;
    unsigned int color_R = 255;
    uint8_t color_G = 255;
    uint8_t color_B = 255;
    bool filled = 0;
    uint8_t filled_color_R = 255;
    uint8_t filled_color_G = 255;
    uint8_t filled_color_B = 255;
    unsigned int width = 0, height = 0;

    double get_area(){
        return width * height;
    }


    unsigned int get_max_X(){
        x_end = x_start + width;
        return max(max(x_start, (int)width),x_end);
    }

    unsigned int get_max_Y(){
        y_end = y_start + height;
        return max(max(y_start, (int)height),y_end);
    }

    static bool serialize(Rectangle rectangle, FILE* outfile){
        if (outfile == NULL){
            fprintf(stderr,"Serialization Error\n");
            exit(1);
        }
        int fwrite_output = fwrite(&rectangle.x_start, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&rectangle.x_end, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&rectangle.y_start, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&rectangle.y_end, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&rectangle.width, sizeof (unsigned int), 1, outfile);
        fwrite_output *= fwrite(&rectangle.height, sizeof (unsigned int), 1, outfile);
        fwrite_output *= fwrite(&rectangle.filled, sizeof (bool), 1, outfile);
        fwrite_output *= fwrite(&rectangle.thickness, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&rectangle.color_B, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&rectangle.color_R, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&rectangle.color_G, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&rectangle.filled_color_B, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&rectangle.filled_color_G, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&rectangle.filled_color_R, sizeof (uint8_t), 1, outfile);

        if(fwrite_output==0)
            printf("Serialization Error\n");
        return fwrite_output;
    }

    static Rectangle deserialize(FILE* infile){
        if (infile == NULL){
            fprintf(stderr, "Deserialization Error\n");
            exit(1);
        }
        Rectangle rectangle;
        int fread_output = fread(&rectangle.x_start, sizeof (int), 1, infile);
        fread_output *= fread(&rectangle.x_end, sizeof (int), 1, infile);
        fread_output *= fread(&rectangle.y_start, sizeof (int), 1, infile);
        fread_output *= fread(&rectangle.y_end, sizeof (int), 1, infile);
        fread_output *= fread(&rectangle.width, sizeof (unsigned int), 1, infile);
        fread_output *= fread(&rectangle.height, sizeof (unsigned int), 1, infile);
        fread_output *= fread(&rectangle.filled, sizeof (bool), 1, infile);
        fread_output *= fread(&rectangle.thickness, sizeof (int), 1, infile);
        fread_output *= fread(&rectangle.color_B, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&rectangle.color_R, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&rectangle.color_G, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&rectangle.filled_color_B, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&rectangle.filled_color_G, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&rectangle.filled_color_R, sizeof (uint8_t), 1, infile);

        if(fread_output!=0){
            return rectangle;
        }else{
            printf("Deserialization Error\n");
            return Rectangle();
        }
    }

    // if (rect1==rect2)
    friend bool operator== (const Rectangle rect1, const Rectangle rect2){
        return (rect1.x_start==rect2.x_start &&
                rect1.x_end==rect2.x_end &&
                rect1.y_start==rect2.y_start &&
                rect1.y_end==rect2.y_end &&
                rect1.width==rect2.width &&
                rect1.height==rect2.height &&
                rect1.filled==rect2.filled &&
                rect1.color_B==rect2.color_B &&
                rect1.color_G==rect2.color_G &&
                rect1.color_R==rect2.color_R &&
                rect1.filled_color_B==rect2.filled_color_B &&
                rect1.filled_color_G==rect2.filled_color_G &&
                rect1.filled_color_R==rect2.filled_color_R &&
                rect1.thickness == rect2.thickness
                );
    }

    char* to_string(){
        char* arr = (char*)malloc(2000*sizeof (char));
        sprintf(arr, "=============================================\nRectangle Start (%d, %d), End (%d, %d), Width = %d, Height = %d\nBorder Color = {%d, %d, %d}, Fill Color = {%d, %d, %d}\nThickness = %d , Area = %f\n====================================================\n"
               ,this->x_start, this->y_start, this->x_end, this->y_end, width, height, color_R, color_G, color_B,
                filled_color_R, filled_color_G, filled_color_B, thickness, get_area());
        return arr;
    }
};

struct Triangle{
public:
    int X1 = 0, Y1 =0, X2 = 0, Y2 = 0,  X3 = 0, Y3 =0;
    int thickness = 1;
    uint8_t color_R = 0;
    uint8_t color_G = 0;
    uint8_t color_B = 0;
    bool filled = 0;
    uint8_t filled_color_R = 0;
    uint8_t filled_color_G = 0;
    uint8_t filled_color_B = 0;
    unsigned int image_width = 0, image_height = 0;


    double get_area(){
        double A = sqrt((double)(X2-X1)*(X2-X1)+(Y2-Y1)*(Y2-Y1));
        double B = sqrt((double)(X2-X3)*(X2-X3)+(Y2-Y3)*(Y2-Y3));
        double C = sqrt((double)(X1-X3)*(X1-X3)+(Y1-Y3)*(Y1-Y3));
        double s = (A+B+C) / 2;
        return sqrt(s*(s-A)*(s-B)*(s-C));
    }


    unsigned int get_max_X(){
        return max(max(X1,X2),X3);
    }

    unsigned int get_max_Y(){
        return max(max(Y1, Y2),Y3);
    }

    static bool serialize(Triangle triangle, FILE* outfile){
        if (outfile == NULL){
            fprintf(stderr,"Serialization Error\n");
            exit(1);
        }
        int fwrite_output = fwrite(&triangle.X1, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&triangle.X2, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&triangle.X3, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&triangle.Y1, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&triangle.Y2, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&triangle.Y3, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&triangle.filled, sizeof (bool), 1, outfile);
        fwrite_output *= fwrite(&triangle.thickness, sizeof (int), 1, outfile);
        fwrite_output *= fwrite(&triangle.color_B, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&triangle.color_R, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&triangle.color_G, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&triangle.filled_color_B, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&triangle.filled_color_G, sizeof (uint8_t), 1, outfile);
        fwrite_output *= fwrite(&triangle.filled_color_R, sizeof (uint8_t), 1, outfile);

        if(fwrite_output==0)
            printf("Serialization Error\n");

        return fwrite_output;
    }

    static Triangle deserialize(FILE* infile){
        if (infile == NULL){
            fprintf(stderr, "Deserialization Error\n");
            exit(1);
        }
        Triangle triangle;
        int fread_output = fread(&triangle.X1, sizeof (int), 1, infile);
        fread_output *= fread(&triangle.X2, sizeof (int), 1, infile);
        fread_output *= fread(&triangle.X3, sizeof (int), 1, infile);
        fread_output *= fread(&triangle.Y1, sizeof (int), 1, infile);
        fread_output *= fread(&triangle.Y2, sizeof (int), 1, infile);
        fread_output *= fread(&triangle.Y3, sizeof (int), 1, infile);
        fread_output *= fread(&triangle.filled, sizeof (bool), 1, infile);
        fread_output *= fread(&triangle.thickness, sizeof (int), 1, infile);
        fread_output *= fread(&triangle.color_B, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&triangle.color_R, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&triangle.color_G, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&triangle.filled_color_B, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&triangle.filled_color_G, sizeof (uint8_t), 1, infile);
        fread_output *= fread(&triangle.filled_color_R, sizeof (uint8_t), 1, infile);

        if(fread_output!=0){
            return triangle;
        }else{
            printf("Deserialization Error\n");
            return Triangle();
        }
    }

    friend bool operator== (const Triangle tri1, const Triangle tri2){
        if (    tri1.X1==tri2.X1 &&
                tri1.X2==tri2.X2 &&
                tri1.X3==tri2.X3 &&
                tri1.Y1==tri2.Y1 &&
                tri1.Y2==tri2.Y2 &&
                tri1.Y3==tri2.Y3 &&
                tri1.filled==tri2.filled &&
                tri1.color_B==tri2.color_B &&
                tri1.color_G==tri2.color_G &&
                tri1.color_R==tri2.color_R &&
                tri1.filled_color_B==tri2.filled_color_B &&
                tri1.filled_color_G==tri2.filled_color_G &&
                tri1.filled_color_R==tri2.filled_color_R &&
                tri1.thickness == tri2.thickness
                ){
            return true;
        }
        return false;
    }


    char* to_string(){
        std::string str1;
        char* arr = (char*)malloc(2000*sizeof(char));
        sprintf(arr, "=============================================\nTriangle A (%d, %d), B (%d, %d), C = (%d, %d)\nBorder Color = {%d, %d, %d}, Fill Color = {%d, %d, %d}\nThickness = %d , Area = %f\n====================================================\n"
               ,this->X1, this->Y1, this->X2, this->Y2,this->X3, this->Y3, color_R, color_G, color_B,
                filled_color_R, filled_color_G, filled_color_B, thickness, get_area());
        return arr;
    }

};

struct Shapes{
    vector<Rectangle> rectangles;
    vector<Triangle> triangles;
    char* filename;// ""
    Shapes(char* filename){
        this->filename = filename;
    }

    Shapes(){
        char* nothing = (char*)malloc(sizeof (char));
        strcat(nothing, "");
        this->filename = nothing;
    }

    static char* get_shapes_filename(char* filename){
        char* str = (char*)malloc(sizeof (char));
        return strcat(strcpy(str,filename), ".data");
        // filename = "file.bmp";
        // strcat(filename,".data"); // file.bmp.data
    }

    void add_rectangle(Rectangle rect){
        if (rectangles.size()==0){
            rectangles.push_back(rect);
        }else{
            for (Rectangle rex :rectangles)
                if(rex==rect)
                    return;
            rectangles.push_back(rect);
        }
    }

    void add_triangle(Triangle tri){
        if (triangles.size()==0){
            triangles.push_back(tri);
        }else{
            for (Triangle trx : triangles)
                if(trx==tri)
                    return;
            triangles.push_back(tri);
        }
    }

    Rectangle get_largest_rectangle(int* index) {
        Rectangle largest_rect;
        int counter=0;
        for (Rectangle rex :rectangles) {
            if(counter == 0|| rex.get_area() > largest_rect.get_area()){
                largest_rect = rex;
                (*index) = counter;
            }

            (counter) ++;
        }
        return largest_rect;
    }

    Triangle get_largest_triangle(int* index){
        Triangle largest_triangle;
        int counter=0;

        for (Triangle t :triangles) {
            if(counter == 0|| t.get_area() > largest_triangle.get_area()){
                largest_triangle = t;
                (*index) = counter;
            }

            (counter) ++;
        }
        
        return largest_triangle;
        
    }


    static bool serialize(Shapes shapes, FILE* outfile){
        if (outfile == NULL){
            fprintf(stderr,"Serialization Error\n");
            exit(1);
        }
        int tris_size = shapes.triangles.size();
        int rect_size = shapes.rectangles.size();

        int fwrite_output = fwrite(&tris_size, sizeof (int), 1, outfile);
        for (Triangle tri : shapes.triangles){
            fwrite_output *= Triangle::serialize(tri, outfile);
        }
        fwrite_output *= fwrite(&rect_size, sizeof (int), 1, outfile);
        for (Rectangle rect : shapes.rectangles){
            fwrite_output *= Rectangle::serialize(rect, outfile);
        }

        if(fwrite_output==0)
            printf("Serialization Error\n");

        return fwrite_output;
    }

    static Shapes deserialize(FILE* infile){
        if (infile == NULL){
            fprintf(stderr, "Deserialization Error\n");
            exit(1);
        }
        Shapes shapes;
        int tris_size;

        int fread_output = fread(&tris_size, sizeof (int), 1, infile);
        for (int i = 0; i< tris_size; i++){
            Triangle t = Triangle::deserialize(infile);
            if (t.get_area() !=0){
                shapes.add_triangle(t);
                fread_output*=1;
            }else{
                fread_output*=0;
            }
        }

        int rect_size;
        fread_output *= fread(&rect_size, sizeof (int), 1, infile);
        for (int i = 0; i< rect_size; i++){
            Rectangle r = Rectangle::deserialize(infile);
            if (r.get_area()!=0){
                shapes.add_rectangle(r);
                fread_output*=1;
            }else{
                fread_output*=0;
            }
        }

        if(fread_output!=0){
            return shapes;
        }else{
            printf("Deserialization Error\n");
            return Shapes();
        }
    }

    int write2file(char* filename){
        FILE* outfile = fopen(filename, "w");
        if (outfile == NULL){
            fprintf(stderr, "\nError open file\n");
            exit(1);
        }
        struct Shapes shapes = *this;
        printf("Storing shapes to data file....\n");
        int fwrite_output = Shapes::serialize(shapes, outfile);
        if(fwrite_output!=0){
            printf("Data to file %s successfully stored!\n", filename);
        }else{
            printf("error writing to file !\n");
        }

        fclose(outfile);
        return fwrite_output;
    }

    static Shapes read4file(char* filename){
        FILE* infile = fopen(filename, "r");
        if (infile == NULL){
            fprintf(stderr, "\nError: can't open file %s\n",filename);
            exit(1);
        }
        struct Shapes shapes(filename);
        shapes = Shapes::deserialize(infile);
        int fread_output = 0;
        if (strcmp(shapes.filename,"")==0){
            fread_output = 1;
        }else{
            printf("Deserialization Error\n");
        }

        if(fread_output!=0){
            printf("Data from file %s successfully read!\n", filename);
            fclose(infile);
            return shapes;
        }else{
            printf("error reading file !\n");
            // assign "" to filename of shapes
            return Shapes();
        }
    }

    void to_string(){
        printf("We have %d shapes: %d Rectangles and %d Triangles\n", int(this->triangles.size()+this->rectangles.size()),
               int(rectangles.size()), int(triangles.size()));
        for(auto tri: triangles){
            printf("%s",tri.to_string());
        }

        for(auto rect: rectangles){
            printf("%s",rect.to_string());
        }
    }

};





#endif


