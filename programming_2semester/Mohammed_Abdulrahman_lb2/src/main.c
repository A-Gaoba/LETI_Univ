#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct MusicalComposition{
    char name[80];
    int year;
    char author[80];
    struct MusicalComposition *next;
}MusicalComposition;


// ???????? ????????? MusicalComposition

MusicalComposition* createMusicalComposition(char* name, char* autor,int year){
    MusicalComposition *pu = malloc(sizeof(MusicalComposition));
    strcpy(pu ->name, name);
    strcpy(pu ->author, autor);
    pu ->year = year;
    pu ->next = NULL;
    return pu;
}

// ??????? ??? ?????? ?? ??????? MusicalComposition

MusicalComposition* createMusicalCompositionList(char** array_names, char** array_authors, int* array_years, int n){
    MusicalComposition *mus = malloc(sizeof(MusicalComposition));
    MusicalComposition *back = mus;
    strcpy(mus ->name, array_names[0]);
    strcpy(mus ->author, array_authors[0]);
    mus ->year = array_years[0];
    for(int i = 1; i < n; i++){
        mus ->next = malloc(sizeof(MusicalComposition));
        mus = mus ->next;
        strcpy(mus ->name, array_names[i]);
        strcpy(mus ->author, array_authors[i]);
        mus ->year = array_years[i];
    }
    mus ->next = NULL;
    return back;
}




void push(MusicalComposition* head, MusicalComposition* element){
    while(1){
        if(head ->next == NULL){
            head ->next = element;
            head ->next ->next = NULL;
            break;
        }
        head = head ->next;
    }
}

void removeEl(MusicalComposition* head, char* name_for_remove){
    while(head->next){
        if(!strcmp(head ->next ->name, name_for_remove)){
            MusicalComposition* x = head ->next ->next;
            free(head ->next);
            head ->next = x;
            break;
        }
        head = head ->next;
    }
}



int count(MusicalComposition* head){
    int count = 0;
    while(head){
        count++;
        head = head ->next;
    }
    return count;
}



void print_names(MusicalComposition* head){
    while(1){
        printf("%s\n", head ->name);
        head = head ->next;
        if(head ->next == NULL){
            printf("%s\n", head ->name);
            break;
        }
    }
}


int main(){
    int length;
    scanf("%d\n", &length);

    char** names = (char**)malloc(sizeof(char*)*length);
    char** authors = (char**)malloc(sizeof(char*)*length);
    int* years = (int*)malloc(sizeof(int)*length);

    for (int i=0;i<length;i++)
    {
        char name[80];
        char author[80];

        fgets(name, 80, stdin);
        fgets(author, 80, stdin);
        fscanf(stdin, "%d\n", &years[i]);

        (*strstr(name,"\n"))=0;
        (*strstr(author,"\n"))=0;

        names[i] = (char*)malloc(sizeof(char*) * (strlen(name)+1));
        authors[i] = (char*)malloc(sizeof(char*) * (strlen(author)+1));

        strcpy(names[i], name);
        strcpy(authors[i], author);

    }
    MusicalComposition* head = createMusicalCompositionList(names, authors, years, length);
    char name_for_push[80];
    char author_for_push[80];
    int year_for_push;

    char name_for_remove[80];

    fgets(name_for_push, 80, stdin);
    fgets(author_for_push, 80, stdin);
    fscanf(stdin, "%d\n", &year_for_push);
    (*strstr(name_for_push,"\n"))=0;
    (*strstr(author_for_push,"\n"))=0;

    MusicalComposition* element_for_push = createMusicalComposition(name_for_push, author_for_push, year_for_push);

    fgets(name_for_remove, 80, stdin);
    (*strstr(name_for_remove,"\n"))=0;

    printf("%s %s %d\n", head->name, head->author, head->year);
    int k = count(head);

    printf("%d\n", k);
    push(head, element_for_push);

    k = count(head);
    printf("%d\n", k);

    removeEl(head, name_for_remove);
    print_names(head);

    k = count(head);
    printf("%d\n", k);

    for (int i=0;i<length;i++){
        free(names[i]);
        free(authors[i]);
    }
    free(names);
    free(authors);
    free(years);

    return 0;

}