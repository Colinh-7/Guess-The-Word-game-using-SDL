#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_struct.h"
#include "strmanip.h"


char *remove_slashN(char *str) 
{
    char *temp = str;
    while (*temp) {
        if ((*temp == 13) || *temp == '\n') *temp = '\0'; 
        temp++;
    }
    return str;
}

char *lower(char *str)
{
    char *p = str;
    while (*p) {
        if ((*p >= 'A') && (*p <= 'Z')) {
            *p = *p-'A'+'a';
        }
        ++p;
    }
    return str;
}

char *capitalize(char *str)
{
    if (str) {
        str[0] = *str - 97 + 65;
    }
    return str;
}

int is_valid(const char *str, int mode)
{
    int valid = 1;
    if (!str) return 1;
    if (mode) {
        while (*str && valid) {
            if (!(((*str >= 'a') && (*str <= 'z')) || ((*str >= 'A') && (*str <= 'Z')) || (*str == '.') || (*str == '/'))) // check if letter is between A and Z or a and z OR = / or = .
                valid = 0;
            ++str;
        }
    }
    else {
        while (*str && valid) {
            if (!(((*str >= 'a') && (*str <= 'z')) || ((*str >= 'A') && (*str <= 'Z')))) // check if letter is between A and Z or a and z
                valid = 0;
            ++str;
        }
    }
    return valid;
}

int is_number(char *str)
{
    char *temp = str;
    int isnumber = (*str) ? 1 : 0;
    while (*temp && isnumber) {
        if (!((*temp >= '0') && (*temp <= '9'))) {
            isnumber = 0;
        }
        temp++;
    }

    return isnumber;
}

int is_in_array(char **array, int array_size, char *word) 
{
    int find = 0, start, end, mid, result;
    start = 0;
    end = array_size - 1;

    while (!find && (start <= end)) {
        mid = (start+end) / 2;
        result = strcmp(word, array[mid]);
        if (!result) find = 1;
        else if (result < 0) end = mid-1;
        else start = mid + 1;
    }
    return find;
}

char *find_in_array(char **array, int array_size, char *word)
{
    int find, start, end, mid, result;
    char *str = NULL;

    find = 0;
    start = 0;
    end = array_size - 1;
    //Dichotomic search
    while (!find && (start <= end)) {
        mid = (start+end)/2;
        result = strncmp(word, array[mid], strlen(word));
        if (!result) {
            find = 1;
            str = array[mid];
        }
        else if (result < 0) end = mid-1;
        else start = mid+1;
    }
    return str;
}

int is_PNG_or_JPG(char *image)
{
    int find = 0, size = strlen(image), i = 0, nameOK = 0, extensionOK = 0;
    char imagecpy[size], *extension = NULL;

    //Copy image in image copy to avoid image modification
    strcpy(imagecpy, image);

    //Find the "." and remplace it by '\0'
    while (!find && i<size) {
        if (imagecpy[i] == '.') {
            find = 1;
            imagecpy[i] = '\0';
            extension = imagecpy+(i+1);
        }
        i++;
    }
    //If there is a dot
    if (find) {
        //Check if the image has a name
        if (strlen(imagecpy) > 0) nameOK = 1;
        //Check if extension is correct
        if (!strcmp(extension, "jpg") || !strcmp(extension, "png")) extensionOK = 1;
        if (nameOK && extensionOK) return 1;
    }
    return 0;
}

int compare(const void *a, const void *b)
{
    //a and b are type **char
    return strcmp(*((char**)a), *((char**)b));
}

//WIP -> Python doesn't work on school's pc
//French plural -> OK
void plural(AVL_Image *image, AVL_Trans *translation, char *array)
{
    FILE *file = NULL;
    int len;
    char *ou[] = {"bijou", "caillou", "chou", "genou", "hibou", "joujou", "pou"};
    char *ail[] = {"bail", "corail", "email", "gemmail", "soupirail", "travail", "vantail", "vitrail"};
    char *au[] = {"bleu", "emeu", "landau", "pneu", "sarrau"};

    file = fopen("temp.txt", "w");
    strcpy(array, image->french);
    len = strlen(array);

    if (array[len-1] == 's' || array[len-1] == 'z' || array[len-1] == 'x')  {
        // No plural
    }
    else if ((array[len-2] == 'a' && array[len-1] == 'u') || (array[len-2] == 'e' && array[len-1] == 'u')) {
        if (is_in_array(au, 5, array)) {
            array[len] = 's';
            array[len+1] = '\0';
        }
        else {
            array[len] = 'x';
            array[len+1] = '\0';
        }
    }
    else if (array[len-2] == 'a' && array[len-1] == 'l') {
        array[len-1] = 'u';
        array[len] = 'x';
        array[len+1] = '\0';
    }
    else if (is_in_array(ail, 8, array)){
        array[len-2] = 'u';
        array[len-1] = 'x';
    }
    else if (is_in_array(ou, 7, array)) {
        array[len] = 'x';
        array[len+1] = '\0';
    }
    else {
        array[len] = 's';
        array[len+1] = '\0';
    }

    if (file && (strcmp(translation->language, "francais") != 0)) {
        fprintf(file, "%s %s", array, translation->language);
        fclose(file);
        system("python3 plural.py");
        file = fopen("temp.txt", "r");
        fscanf(file, "%s", array);
    }
    system("rm temp.txt");
    remove_slashN(lower(array));
    fclose(file);
}

/*========= Password ==========*/

char *hash_password(char *password) {
    int lg = strlen(password);
    
    for (int i=0; i<lg; ++i) {
        password[i] = (password[i]/2 +32)*0.55+27;
    }
    return password;
    //LKIM:9::
}
