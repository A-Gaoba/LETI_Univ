#include <stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#define MAX_LENGTH 2000

int is_add(char* folder){
    return (strcmp(folder, "add")==0)?1:0;
}

void error(){
    perror("error\n");
    exit(EXIT_FAILURE);
}

char* line;
char* copy_line;
int access_file(char* path, int is_add_operation){
    int acc = 1;
    if (is_add_operation) acc=0;
    FILE *in_file;
    line[0]='\0';

    if ((in_file=fopen(path, "r")) == NULL) error();
    fgets(line, MAX_LENGTH, in_file);

    strcpy(copy_line, line);
    line[0]='\0';
    char* word = strtok(copy_line, " \n");
    while (word!=NULL) {
        if (is_add_operation) acc+=atoi(word);
        else acc*=atoi(word);
        word = strtok(NULL, " \n");
    }
    fclose(in_file);
    return acc;
}

int access_dir(char* root, int is_add_operation){

  int acc = 1;
  if (is_add_operation) acc=0;
  DIR* dir = opendir(root);
  if (dir == NULL){
    error();
    return acc;
  }
  int is_add_operation_temp = 1;
  struct dirent *de;
  char* path = (char*)malloc(MAX_LENGTH* sizeof (char));
  while ((de = readdir(dir))!=NULL){
      path[0] = '\0';
      strcpy(path, root);
      strcat(path, "/");
      strcat(path, de->d_name);
      path[strlen(path)] = '\0';
    if (de->d_type==DT_DIR && strcmp(de->d_name,".")!=0
     && strcmp(de->d_name,"..")!=0
    && (strcmp(de->d_name, "add")==0 || strcmp(de->d_name, "mul")==0)){
        if (is_add(de->d_name)) is_add_operation_temp = 1;
        else is_add_operation_temp = 0;
        if (is_add_operation)
            acc += access_dir(path, is_add_operation_temp);
        else
            acc *= access_dir(path, is_add_operation_temp);
    }else if (de->d_type==DT_REG){
      if (is_add_operation)
          acc += access_file(path, is_add_operation);
      else
          acc *= access_file(path, is_add_operation);
    }
 }
 free(path);
 free(de);
 closedir(dir);
 return acc;
}


int main(void) {
    char l[] ="tmp";
    line = (char*)malloc(MAX_LENGTH*sizeof (char));
    copy_line = strdup(line);
    int value = access_dir(l, 1);
    FILE* file;
    file = fopen("result.txt", "w");
    if (file==NULL) error();
    char* str = (char*)malloc(18*sizeof (char));
    snprintf(str, 16, "%d", value);
    fputs(str, file);
    fclose(file);
    return 0;
}

