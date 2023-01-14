#ifndef STRMANIP_H
#define STRMANIP_H

//Remove the '\\n' at the end of str
char *remove_slashN(char *str);
//Set all letters of str to lowercase
char *lower(char *str);
//Capitalize the first letter of str
char *capitalize(char *str);
/**
 * \param mode Set to 1 for CSV file, 0 for user's input
 * \return 1 if str doesn't contain letters with accent
*/
int is_valid(const char *str, int mode);
//Check if the string is a number.
int is_number(char *str);
//Check if 'word' is in 'array'. The array need to be sorted
int is_in_array(char **array, int array_size, char *word);
//Return the word in the array
char *find_in_array(char **array, int array_size, char *word);
//Return if an image is a PNG or JPG
int is_PNG_or_JPG(char *image);
//For qsort function
int compare(const void *str1, const void *str2);
/**
 * Set "image->french" in the plural then translate it in "tranlsation->language".
 * \param array Will contain the plural.
*/
void plural(AVL_Image *image, AVL_Trans *translation, char *array);

/*==== Password ====*/
//Crypt de password
char *hash_password(char *password);

#endif