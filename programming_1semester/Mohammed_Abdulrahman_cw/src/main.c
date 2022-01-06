#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include<math.h>

#define SEARCH_WORD "garbage"
#define TEXT_EXAMPLE "Before the early 1960s,grabage computers  were mainly garbage garbagegarbage garbage garbage used grabage for number-crunching rather than for text, and memory was extremely expensive. Computers often allocated only 6 bits for each character, permitting only 64 characters—assigning codes for A-Z, a-z, and 0-9 would leave only 2 codes: nowhere near enough. Most computers opted not to support lower-case letters. Thus, early text projects such as Roberto Busa's Index Thomisticus, the Brown Corpus, and others had to resort to conventions such as keying an asterisk preceding letters actually intended to be  garbage garbage upper-case."

char** readText(int *sentence_counter){
    char **text = (char**)calloc(1, sizeof(char*));
    if (text == NULL){return NULL;}
    int i=0; 
    int sentence_max=0; 
    char c = getchar(); 
    do{ 
        i=0;
        if((text=(char**)realloc(text, ((*sentence_counter)+1)*sizeof(char*)))==NULL)  
            return NULL;
        do{ 
            if ((text[*sentence_counter]=(char*)realloc(text[*sentence_counter], sizeof(char)*(i+3)))==NULL) 
                return NULL;
            text[*sentence_counter][i]=c;
            i++;
        }while((c=getchar())!='.');
        text[*sentence_counter][i]='.';
        text[*sentence_counter][i+1]='\0';
        (*sentence_counter)++;
        if (i+2>sentence_max)
            sentence_max=i+2;
    }while((c = getchar())!='\n');

    
    char checker[*sentence_counter][sentence_max];
    int j, k;
    
    for(i=0; i<*sentence_counter; i++){
        strcpy(checker[i], text[i]);
        for(j=0; j<strlen(checker[i]); j++){
            checker[i][j]=tolower(checker[i][j]);
        }
    }

    for(i=0; i<*sentence_counter; i++)
        for(j=i+1; j<*sentence_counter; j++){
            
            int l=0;
            int found = 0; 
            if (strlen(checker[i]) == strlen(checker[j]))
              while (l < strlen(checker[i])){
                if (checker[i][l] == checker[j][l]){
                  found = 1;
                  l++;
                }else{
                  found = 0;
                  break;
                }
              }
              if (found == 1){
                (*sentence_counter)--;
                for(k=j; k<*sentence_counter; k++){
                    text[k]=text[k+1];
                    strcpy(checker[k], checker[k+1]);
                    }
                    j--;
                }
            }
    return text;
    }


void printText(char **text, int sentence_counter){
    for(int i=0; i<sentence_counter; i++){
        printf("========================================\n");
        printf("%d|   '%s'\n", i, text[i]);
        }
  }


char** convert_to_lowercase(char **text, int sentence_counter){
  char** text2 = (char**)malloc((sentence_counter)*sizeof(char*));
  int j = 0;
  if (text2 == NULL) return NULL;
    for(int i=0; i<sentence_counter; i++){
      text2[i] = (char*)realloc(text2[i], sizeof(char)*(strlen(text[i])+1));
      if (text2[i]==NULL) return NULL;
      j = 0;
      while(text[i][j] != '\0'){
        text2[i][j] = tolower(text[i][j]);
        j++;
      }
      text2[i][strlen(text2[i])] = '\0';
    }
    return text2;
  }



int count_garbage(char **text, int sentence_counter){

    char** text2 = convert_to_lowercase(text,sentence_counter);
    int count, letter = 0, total_count = 0;
    int search_word_len = strlen(SEARCH_WORD);
    char *word = (char*)calloc( 1,search_word_len * sizeof(char));
    if (word==NULL) return total_count;
    for(int i=0; i<sentence_counter; i++){
        count = 0;
        for (int j=0; j< strlen(text2[i]); j++){
          int k=0;
          for (k=0; k<search_word_len; k++){
            word[k] = text2[i][k+j];
          }
          word[strlen(word)] = '\0';
          if (strcmp(word, SEARCH_WORD) == 0){
            count++;
          }
        }
        printf("\n==========================\n");
        printf("For sentence: '%s'\n=============\n",text2[i]);
        if(count == 0 )
          printf("Clean\n");
        else if(count > 5) 
          printf("It is a dump\n");
        else
          printf("Must be washed\n");
        printf("=============\n");
        total_count += count;
        }
        return total_count;
}

int replace_all_digits(char **text, int *sentence_counter, char* input_string){
  int len = strlen(input_string);
  int count = 0;
  for(int i=0; i<*sentence_counter; i++){
    for (int j=0; j<strlen(text[i]); j++){
      if (isdigit(text[i][j])){
        count++;
        
        text[i] = (char*)realloc(text[i], (strlen(text[i])+len+1) * sizeof(char));
        if (text[i]==NULL) return count;
        char* temp = (char*)malloc(strlen(text[i])+sizeof(char));
        if(temp==NULL) return count;
        
        for (int k=j; k<strlen(text[i]); k++){
          temp[k-j] = text[i][k+1];
        }
        temp[strlen(temp)] = '\0';
         
        for (int k=0; k<len; k++){
          text[i][k+j] = input_string[k];
        }
        
        
        for(int k=0; k<strlen(temp); k++){
          text[i][j+k+len] = temp[k];
        }
        text[i][strlen(text[i])] = '\0';
        j--;
      }
    }
  }
  return count;
}


int three_consecutive_letters_in_upper_case(char *sentence){

    for(int i=0; i < strlen(sentence)-2; i++){
        if(isupper(sentence[i]) && isupper(sentence[i+1]) && isupper(sentence[i+2]))
          return 1;
    }
    return 0;
}

int delete_if_three_consecutive_letters_in_upper_case(char **text, int *sentence_counter){
  int num = 0;
    for(int i=0; i<*sentence_counter; i++){
        if(three_consecutive_letters_in_upper_case(text[i])){
            free(text[i]);
            (*sentence_counter)--;
            for (int j = i; j < *sentence_counter; j++)
              text[j] = text[j + 1];
          i--;
          num++;
        }
    }
    return num;
  }

int is_vowel(char ch){
    
  if(ch=='a' || ch=='e' || ch=='i' || ch=='o' || ch=='u' || 
      ch=='A' || ch=='E' || ch=='I' || ch=='O' || ch=='U')
      return 1;
  else
      return 0;
}

int num_words_beginning_with_vowels(char* sentence){
  int start=0;
  int num_words=0;
  if (is_vowel(sentence[start])){
      num_words++;
    }
  int i=0;
  while(sentence[i] != '.'){
    if (sentence[i]==' ' || sentence[i]==',') {
        start = i+1;
        if (is_vowel(sentence[start])){
          num_words++;
          }
      }
      i++;
  }
  return num_words;
}

int cmp_ptr(const void *a, const void *b)
{
    const int **left  = (const int **)a;
    const int **right = (const int **)b;

    return (**left < **right) - (**right < **left);
}

int * order_indices(const int *a, int n)
{
    const int **pointers = malloc(n * sizeof(const int *));
    if (pointers==NULL) return NULL;
    for (int i = 0; i < n; i++) pointers[i] = a + i;

    qsort(pointers, n, sizeof(const int *), cmp_ptr);

    int *indices = malloc(n * sizeof(int));
    if (indices==NULL) return NULL;
    for (int i = 0; i < n; i++) indices[i] = pointers[i] - a;

    free(pointers);

    return indices;
}


void sort_by_descending_number_of_words_beginning_with_vowels(char** text, int sentence_counter){
  int* indices = (int*)malloc(sentence_counter*sizeof(int));
  if (indices == NULL) return;

  for(int i=0; i<sentence_counter; i++){
    int num = num_words_beginning_with_vowels(text[i]);
    indices[i] = num;
  }
  indices = order_indices(indices, sentence_counter);

  char** text_copy = (char**)malloc(sentence_counter*sizeof(char*));
  if (text_copy == NULL) return;
  for(int i=0; i<sentence_counter; i++){
    text_copy[i] = (char*)malloc((strlen(text[i])+1)*sizeof(char));
    if (text_copy[i]==NULL) return;
    strcpy(text_copy[i], text[i]);
  }
  for(int i=0; i<sentence_counter; i++){
    text[i] = (char*)realloc(text[i],sizeof(char)*(strlen(text_copy[indices[i]])+1));
    if (text[i]==NULL) return;
    text[i] = text_copy[indices[i]];
  }
}




int main(){
    int sentence_counter=0;
    int command;
    int num;
    // char input_string[50];
    char* input_string = (char*) malloc(sizeof(char)*1);
    printf("Здравствуйте! Введите Ваш текст.\nПодсказка: текст состоит из предложений, которые состоят из набора слов, разделённых запятой ИЛИ пробелом.\nСлова состоят из цифр и латинских букв.\n\n");
    char** text = readText(&sentence_counter);
    if(text==NULL){
        printf("Ошибка выделения памяти.\n");
        return 1;}

    printText(text, sentence_counter);
    
    printf("--------------------------------------\n");
    printf("Введите одно из чисел ниже, чтобы выбрать действие:\n1) Для каждого предложения посчитать количество слов “garbage” в нем (без учета регистра). \n2) Заменить все цифры в предложении на введенную строку.\n3) Удалить все предложения в которых есть три подряд идущие буквы в верхнем регистре.\n4) Отсортировать по уменьшению количества слов начинающихся с гласной буквы.\n0) Выйти из программы.\n");
    printf("--------------------------------------\n");
    printf("------Type command № >> ");
    scanf("%d", &command);
    printf("\n");
    int sentences_count = 0;
    while(command!=0){
      switch(command){
          case 1:
              printf("Number of '%s' occuring in text is %d\n", SEARCH_WORD, count_garbage(text, sentence_counter));
              break;

          case 2:
              printf("Please input the string(50 chars) to replace digits with: ");
              scanf("%s", input_string);
              num = replace_all_digits(text, &sentence_counter,input_string);
              printText(text, sentence_counter);
              printf("--------------------------------------\n");
              printf("Number of Replaces is %d\n", num);
              break;

          case 3:
              sentences_count = sentence_counter;
              num = delete_if_three_consecutive_letters_in_upper_case(text, &sentence_counter);
              printText(text, sentence_counter);
              printf("--------------------------------------\n");
              printf("Number of Deletions is %d\n", num);
              printf("Remaining sentences is %d\n", sentences_count - num);
              break;
          case 4:
            sort_by_descending_number_of_words_beginning_with_vowels(text, sentence_counter);
            printText(text, sentence_counter);
            break;

          case 0:
            break;

          default:
            printf("Неправильный ввод!\n");
            break;
        }
        printf("--------------------------------------\n");
        printf("Введите одно из чисел ниже, чтобы выбрать действие:\n1) Для каждого предложения посчитать количество слов “garbage” в нем (без учета регистра). \n2) Заменить все цифры в предложении на введенную строку.\n3) Удалить все предложения в которых есть три подряд идущие буквы в верхнем регистре.\n4) Отсортировать по уменьшению количества слов начинающихся с гласной буквы.\n0) Выйти из программы.\n");
        printf("--------------------------------------\n");
        printf("------Type command № >> ");
        scanf("%d", &command);
    }
    printf("Program is Exited Successfully");
    return 0;

}