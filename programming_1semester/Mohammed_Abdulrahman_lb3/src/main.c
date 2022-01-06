#include <stdio.h>

#include <string.h>

#include <stdlib.h>

int main()

{

    int Sum_of_sentence_after = 0, Sum_of_sentence_befor = 0;

    char **Text = (char **)calloc(Sum_of_sentence_after, sizeof(char*)), Symbol = 0;

    for (;;)

    {

        char *Sentence = (char *)malloc(10000*sizeof(char));

        scanf("%[^.;?!]%c", Sentence, &Symbol);

        strncat(Sentence, &Symbol, 1);

        if(!(strstr(Sentence, "555 ") || strstr(Sentence, " 555 ") || strstr(Sentence, " 555.") || strstr(Sentence, " 555?") || strstr(Sentence, " 555;")))

        {

            Text[Sum_of_sentence_after] = (char*)malloc((strlen(Sentence)+1)*sizeof(char));

            strcpy(Text[Sum_of_sentence_after], Sentence);

            Sum_of_sentence_after++;

            if (strstr(Sentence, "Dragon flew away!")) break;

            Text = realloc(Text, (Sum_of_sentence_after+1)*sizeof(char*));

        }

        free(Sentence);

        Sum_of_sentence_befor++;

    }

    for(int i = 0; i<Sum_of_sentence_after; i++)

        if((Text[i][0]=='\t')||(Text[i][0]==' '))

            for(int j = 0; j < strlen(Text[i]); j++) Text[i][j]=Text[i][j+1];

            

    for(int i = 0; i<Sum_of_sentence_after; i++) puts(Text[i]);

    printf("Количество предложений до %d и количество предложений после %d\n", Sum_of_sentence_befor, Sum_of_sentence_after-1);

    for(int i = 0; i<Sum_of_sentence_after; i++) free(Text[i]);

return 0;

}
