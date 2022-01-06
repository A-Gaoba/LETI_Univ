
#include<Commands.h>




int main(int argc, char *argv[]) {

    if (argv[1]==NULL) {error_command();return 0;}
    if (strcmp("tri", argv[1])==0){ // the 1st command is tri
        draw_triangle_command(argc, argv);
    }else if(strcmp("rect", argv[1])==0){ // the 2nd command is rect
        draw_rectangle_command(argc, argv);
    }else if(strcmp("repaint", argv[1])==0){ // the 3rd command is repaint
        repaint_command(argc, argv);
    }else if(strcmp("collage", argv[1])==0){ // the 4th command is collage
        collage_command(argc, argv);
    }else if (strcmp("info", argv[1])==0){
        info_command(argc,argv);
    }else{
        error_command();
    }
    return 0;
}

