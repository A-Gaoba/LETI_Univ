#ifndef UTILS_H
#define UTILS_H

#ifndef STRUCTURES_H
    #include<Structures.h>
#endif

// checks if file exists or not
bool fexists(const char *filename){
    std::ifstream ifile(filename);
    return !(!ifile);
}

// converts the string or const array to dynamic array and initializes it with the content of input string
char* string2array(std::string str){
    char* arr = (char*) malloc(str.length()*sizeof (char));
    int i=0;
    for(; i<(int)str.length();i++){
        arr[i] = str[i];
    }
    arr[i]='\0';
    return arr;
}

bool is_bmp_file(char* filename){
    if (filename != NULL && strcmp(filename, "")!=0){
        std::string str = filename;
        if (str.size()>4){// "1.bmp"
            return strcmp(string2array(str.substr(str.size()-4)),".bmp") == 0;
        }
    }
    return false;
}

// print help of each command
void print_usage_tri() {
    printf("Drawing Triangle command usage:\ntri -A(--P0) x y -B(--P1) x y -C(--P2) x y "
           "-t(--thick) value -c(--color) R G B [-f(--filled) -l(--fillcolor) R G B] -i(--input) filename.bmp [width height]\n");
}


void print_usage_rect() {
    printf("Drawing Rectangle command usage:\nrect -s(--start) x y -w(--width) width -h(--height) height "
           "-t(--thick) value -c(--color) R G B [-f(--filled) -l(--fillcolor) R G B] -i(--input) filename.bmp [width height]\n");
}

void print_usage_repaint() {
    printf("Repainting shape command usage:\nrepaint -s(hape) [rect|tri] --(c)olor R G B -i(nput) filename.bmp\n");
}


void print_usage_collage() {
    printf("Creating a collage of images command usage:\ncollage -m(--M) number -n(--N) number -o(--output) output.bmp -i(--input) image1.bmp image2.bmp image3.bmp ........\n");
}

void print_usage_info(){
    printf("Getting info of image and shapes command usage:\ninfo image.bmp\n");
}


#endif


