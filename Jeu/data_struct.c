#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include "data_struct.h"
#include "strmanip.h"

int nb_linesCSV(FILE *f)
{
    int i = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        i++;
    }
    rewind(f);
    return i;
}

int nb_columnsCSV(FILE *f)
{
    int i = 0;
    char *delim = ";";
    char buffer[1024];
    char *temp;
    temp = fgets(buffer, sizeof(buffer), f);
    while (strsep(&temp, delim)) {
        i++;
    }
    rewind(f);
    return i;
}

int max (int a, int b)
{
    return (a >= b) ? a : b;
}

int absolute_value(int a)
{
    return (a > -1) ? a : -a;
}

/*========== AVL Image ==========*/
//For debugging
void traversi(AVL_Image *avl) {
    if (avl) {
        printf("French = %s\nImage = %s\n", avl->french, avl->image);
        printf("LEFT : \n");
        traversi(avl->left);
        printf("RIGHT : \n");
        traversi(avl->right);
    }
}

AVL_Image *lri(AVL_Image *avl) 
{
    AVL_Image *right = avl;
    if (avl) {
        if (avl->right) {
            right = avl->right;
            if (avl->right->left) avl->right = right->left;
            else avl->right = NULL;
            right->left = avl;
        }
    }
    return right;
}

AVL_Image *rri(AVL_Image *avl)
{
    AVL_Image *left = avl;
    if (avl) {
        if (avl->left) {
            left = avl->left;
            if (avl->left->right) avl->left = left->right;
            else avl->left = NULL;
            left->right = avl;
        }
    }
    return left;
}

AVL_Image *lrri(AVL_Image *avl) 
{
    avl->left = lri(avl->left);
    return rri(avl);
}

AVL_Image *rlri(AVL_Image *avl)
{
    avl->right = rri(avl->right);
    return lri(avl);
}

int heighti(AVL_Image *avl) 
{
    if (avl) {
        return 1 + max(heighti(avl->left), heighti(avl->right));
    }
    else return -1;
}

int isAVLi(AVL_Image *avl)
{
    if (avl) {
        if (absolute_value(heighti(avl->left)-heighti(avl->right)) < 2) {
            return (isAVLi(avl->left) && isAVLi(avl->right));
        }
        else return 0;
    }
    return 1;
}

AVL_Image *rebalancei(AVL_Image *avl)
{
    int balance, balance2;
    if (!isAVLi(avl)) {
        balance = heighti(avl->left) - heighti(avl->right);

        switch (balance) {
            case 2:
                balance2 = heighti(avl->left->left)-heighti(avl->left->right);
                if (balance2 >= 0) avl = rri(avl);
                else avl = lrri(avl);
            break;
            case -2:
                balance2 = heighti(avl->right->left) - heighti(avl->right->right);
                if (balance2 > 0) avl = rlri(avl);
                else avl = lri(avl);
            break;
        }
    }
    return avl;
}

AVL_Image *add_AVLi(AVL_Image *avl, AVL_Image *elt)
{
    if (avl) {
        if (strcmp(elt->french, avl->french) < 0) {
            avl->left = rebalancei(add_AVLi(avl->left, elt));
            return rebalancei(avl);
        }
        else {
            avl->right = rebalancei(add_AVLi(avl->right, elt));
            return rebalancei(avl);
        }
    }
    return elt;
}

AVL_Image *find_AVLi(AVL_Image *avl, const char *french)
{
    if (avl) {
        if (strcmp(french, avl->french) < 0) {
            return find_AVLi(avl->left, french);
        }
        else if (strcmp(french, avl->french) > 0) {
            return find_AVLi(avl->right, french);
        }
    }    
    return avl;
}

void free_AVL(AVL_Image *avl) 
{
    if (avl) {
        free_AVL(avl->left);
        free_AVL(avl->right);
        free_AVLt(avl->translation);
        free(avl);
    }
}


/*========= AVL Translation ==========*/

void traverst(AVL_Trans *avl) {
    if (avl) {
        printf("Language = %s\nWord = %s\n", avl->language, avl->word);
        printf("LEFT : \n");
        traverst(avl->left);
        printf("RIGHT : \n");
        traverst(avl->right);
    }
}

AVL_Trans *lrt(AVL_Trans *avl) 
{
    AVL_Trans *right = avl;
    if (avl) {
        if (avl->right) {
            right = avl->right;
            if (avl->right->left) avl->right = right->left;
            else avl->right = NULL;
            right->left = avl;
        }
    }
    return right;
}

AVL_Trans *rrt(AVL_Trans *avl)
{
    AVL_Trans *left = avl;
    if (avl) {
        if (avl->left) {
            left = avl->left;
            if (avl->left->right) avl->left = left->right;
            else avl->left = NULL;
            left->right = avl;
        }
    }
    return left;
}

AVL_Trans *lrrt(AVL_Trans *avl) 
{
    avl->left = lrt(avl->left);
    return rrt(avl);
}

AVL_Trans *rlrt(AVL_Trans *avl)
{
    avl->right = rrt(avl->right);
    return lrt(avl);
}

int heightt(AVL_Trans *avl) 
{
    if (avl) {
        return 1 + max(heightt(avl->left), heightt(avl->right));
    }
    else return -1;
}

int isAVLt(AVL_Trans *avl)
{
    if (avl) {
        if (absolute_value(heightt(avl->left)-heightt(avl->right)) < 2) {
            return (isAVLt(avl->left) && isAVLt(avl->right));
        }
        else return 0;
    }
    return 1;
}

AVL_Trans *rebalancet(AVL_Trans *avl)
{
    int balance, balance2;
    if (!isAVLt(avl)) {
        balance = heightt(avl->left) - heightt(avl->right);

        switch (balance) {
            case 2:
                balance2 = heightt(avl->left->left)-heightt(avl->left->right);
                if (balance2 >= 0) avl = rrt(avl);
                else avl = lrrt(avl);
            break;
            case -2:
                balance2 = heightt(avl->right->left) - heightt(avl->right->right);
                if (balance2 > 0) avl = rlrt(avl);
                else avl = lrt(avl);
            break;
        }
    }
    return avl;
}

AVL_Trans *add_AVLt(AVL_Trans *avl, AVL_Trans *elt)
{
    if (avl) {
        if (strcmp(elt->language, avl->language) < 0) {
            avl->left = rebalancet(add_AVLt(avl->left, elt));
            return rebalancet(avl);
        }
        else {
            avl->right = rebalancet(add_AVLt(avl->right, elt));
            return rebalancet(avl);
        }
    }
    return elt;
}

AVL_Trans *find_AVLt(AVL_Trans *avl, const char *language, char **const struct2, int struct2_size)
{
    srand(time(NULL));
    int random;

    if (struct2) {
        random = rand() % struct2_size;
        language = struct2[random];
    }
    if (avl) {
        if (strcmp(language, avl->language) < 0) {
            return find_AVLt(avl->left, language, NULL, 0);
        }
        else if (strcmp(language, avl->language) > 0) {
            return find_AVLt(avl->right, language, NULL, 0);
        }
    }
    return avl;
}

void free_AVLt(AVL_Trans *avl) 
{
    if (avl) {
        free_AVLt(avl->left);
        free_AVLt(avl->right);
        free(avl);
    }
}

/*========== CSV ==========*/

void load_from_CSV(char ***struct1, char ***struct2, AVL_Image **image)
{
    int nblines, nbcolumns, i=0, j=0, isOK = 1;     
    char buffer[10000], *buffptr, *word, *delim = ";";
    FILE *f = NULL;
    AVL_Trans *temp = NULL;
    f = fopen("data/loading-files/data.csv", "r");

    if (f) {
        nblines = nb_linesCSV(f) - 1;       //Nb lines-1, first line in the CSV is not an image
        nbcolumns = nb_columnsCSV(f) - 1;   //Nb columns-1, first column is not translation

        //Struct allocation
        AVL_Image *tabi[nblines];
        AVL_Trans *tabt[nblines][nbcolumns];

        *struct1 = (char**)calloc(nblines, sizeof(char*));
        *struct2 = (char**)calloc(nbcolumns, sizeof(char*));
        
        for (i; i<nblines;++i) {
            for(j=0; j<nbcolumns; ++j) {
                tabt[i][j] = (AVL_Trans*)malloc(sizeof(AVL_Trans));
                tabt[i][j]->left = NULL;
                tabt[i][j]->right = NULL;
                if (i==0) (*struct2)[j] = calloc(28, sizeof(char));
            }
            (*struct1)[i] = calloc(28, sizeof(char));
            tabi[i] = (AVL_Image*)malloc(sizeof(AVL_Image));
            tabi[i]->left = NULL;
            tabi[i]->right = NULL;
        }

        i=0;
        j=0;

        //Struct initialization
        while (isOK && ((buffptr = fgets(buffer, sizeof(buffer), f)) != NULL)) {
            buffptr = remove_slashN(buffptr);
         
            while (isOK && ((word = strsep(&buffptr, delim)) != NULL)) {
                if (!is_valid(word, 1)) {
                    printf("%s is not a valid word. (line %d)\n", word, i+1);
                    isOK = 0;
                }

                word = lower(word);
                if (i == 0) {
                    if (j > 0) {
                        strcpy((*struct2)[j-1], word);

                        for(int k=0; k<nblines; ++k) {
                            strcpy(tabt[k][j-1]->language, word);
                        }
                    }
                }
               else {
                    if (j == 0) strcpy(tabi[i-1]->image, word);
                    else if (j == 1) {
                        strcpy((*struct1)[i-1], word);
                        strcpy(tabi[i-1]->french, word);
                        strcpy(tabt[i-1][j-1]->word, word);
                    }
                    else strcpy(tabt[i-1][j-1]->word, word);
                }
                j++;
            }
            //If a word is missing
            if (isOK && j<nbcolumns+1) {
                printf("Error: a word is missing at lines %d in \"data.csv\".\n", i+1);
                isOK = 0;
            }
            i++;
            j=0;
        }
        if (isOK) {
            //AVLs Setup
            for(i=0; i<nblines; ++i) {
                for(j=0; j<nbcolumns; ++j) {
                    temp = add_AVLt(temp, tabt[i][j]);
                }
                tabi[i]->translation = temp;
                *image = add_AVLi(*image, tabi[i]);
                temp = NULL;

                //Sort arrays
                qsort(*struct1, nblines, sizeof(char*), compare);   
            }
        }
        fclose(f);
    }
}

void writerow_CSV(char *str, int end_bool)
{
    FILE *f = NULL;
    f = fopen("data/loading-files/data.csv", "a");

    if (f) {
        if (end_bool) fprintf(f, "%s\n", str);
        else fprintf(f, "%s;", str);
    }

    fclose(f);
}

void writeFrenchArray_CSV(AVL_Image *image_avl, char **french_array, int french_array_size, char **language_array, int lang_array_size)
{
    FILE *f = NULL;
    AVL_Image *tempi = NULL;
    AVL_Trans *tempt = NULL;
    int i, j;

    f = fopen("data/loading-files/data.csv", "w");

    if (f) {
        for (i=0; i<=french_array_size; ++i) {
            if (i==0) {
                for (j=0; j<lang_array_size; ++j) {
                    if (j==0) {
                        fprintf(f, "image;%s;", language_array[j]);
                    }
                    else {
                        //End row
                        if (j == lang_array_size-1) fprintf(f, "%s\n", language_array[j]);
                        else fprintf(f, "%s;", language_array[j]);
                    }
                }
            }
            else {
                tempi = find_AVLi(image_avl, french_array[i-1]);
                if (tempi) {
                    fprintf(f, "%s;", tempi->image);
                    for (j=0; j<lang_array_size; ++j) {
                        tempt = find_AVLt(tempi->translation, language_array[j], NULL, 0);
                        if (tempt) {
                            //End row
                            if (j == lang_array_size-1) fprintf(f, "%s\n", tempt->word);
                            else fprintf(f, "%s;", tempt->word);
                        }
                        tempt = NULL;
                    }
                    tempi = NULL;
                }
            }
        }
    }

    fclose(f);
}

/*========== Array ==========*/

void rand_image_array(char **struct1, int struct1_size, char **array, int array_size)
{
    int random, j, find;
    srand(time(NULL));

    j=0;
    find=0;

    for(int i=0; i<array_size; ++i){
        random = rand() % struct1_size;
        array[i]= struct1[random];
        while (!find && j<i) {
            if (!strcmp(array[j], array[i])) {
                find = 1;
                i--;
            }
            j++;
        }
        find=0;
        j=0;
    }
}

void free_array(char **tab, int tab_size)
{
    for (int i=0; i<tab_size; ++i) {
        free(tab[i]);
    }
    free(tab);
}

/*========== Files ==========*/

int is_in_dir(char *image, char* folder)
{
    DIR *images_dir = NULL;
    struct dirent *readimage = NULL;
    int find = 0;
    char path[40];

    //Set path
    sprintf(path, "data/%s", folder);
    
    //Open directory
    images_dir = opendir(path);

    if (images_dir) {
        while ((readimage = readdir(images_dir)) && !find) {
            if (!strcmp(readimage->d_name, image)) {
                find = 1;
            }
        } 
    }

    closedir(images_dir);
    return find;
}
