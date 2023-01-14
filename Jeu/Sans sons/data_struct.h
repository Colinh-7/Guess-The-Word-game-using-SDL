#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

typedef struct node1 {
    char language[30];
    char word[46];      //Longest word 45 letters
    struct node1 *left, *right;
} AVL_Trans;

typedef struct node2 {
    char french[28];     //french word
    char image[50];      //image_path
    AVL_Trans *translation;
    struct node2 *left, *right;
} AVL_Image;

/*==== CSV ====*/
int nb_linesCSV(FILE *f);
int nb_columnsCSV(FILE *f);
//Load everything from "data/loading-files/data.csv" into an AVL Image
void load_from_CSV(char ***struct1, char ***struct2, AVL_Image **image);
//Write a row at the end of data.csv
void writerow_CSV(char *str, int end_bool);
void writeFrenchArray_CSV(AVL_Image *image_avl, char **french_array, int french_array_size, char **language_array, int lang_array_size);
/*==== Utils ====*/
//Max between a and b. (return a if a = b)
int max (int a, int b);
//Absolute value of a
int absolute_value(int a);

/*==== AVL Image ====*/
//Binary tree traversal
void traversi(AVL_Image *avl);
//left rotation
AVL_Image *lri(AVL_Image *avl);
//right rotation
AVL_Image *rri(AVL_Image *avl);
//left right rotation
AVL_Image *lrri(AVL_Image *avl);
//right left rotation
AVL_Image *rlri(AVL_Image *avl);
//height of AVL
int heighti(AVL_Image *avl);
//Return 1 if avl is an AVL_Image, else 0
int isAVLi(AVL_Image *avl);
//Rebalance avl
AVL_Image *rebalancei(AVL_Image *avl);
//Add leaf to an AVL by his name
AVL_Image *add_AVLi(AVL_Image *avl, AVL_Image *elt);
/**
 * Find element in an AVL by its name
 * \param french A pointer to the french word of the image you want.
*/
AVL_Image *find_AVLi(AVL_Image *avl, const char *french);
//Free all AVLs
void free_AVL(AVL_Image *avl);

/*==== AVL Translation ====*/
//Binary tree traversal
void traverst(AVL_Trans *avl);
//left rotation
AVL_Trans *lrt(AVL_Trans *avl);
//right rotation
AVL_Trans *rrt(AVL_Trans *avl);
//left right rotation
AVL_Trans *lrrt(AVL_Trans *avl);
//right left rotation
AVL_Trans *rlrt(AVL_Trans *avl);
//height of AVL
int heightt(AVL_Trans *avl);
//Return 1 if avl is an AVL_Image, else 0
int isAVLt(AVL_Trans *avl);
//Rebalance avl
AVL_Trans *rebalancet(AVL_Trans *avl);
//Add leaf to an AVL by his name
AVL_Trans *add_AVLt(AVL_Trans *avl, AVL_Trans *elt);
/**
 * Find a language in a AVL_Trans
 * \param language The language you are looking for, set it to NULL for random language.
 * \param struct2 The language array, if it is not NULL, language will be randomized.
*/
AVL_Trans *find_AVLt(AVL_Trans *avl, const char *language, char **const struct2, int struct2_size);
//Free AVL_Image
void free_AVLt(AVL_Trans *avl);

/*==== Array ====*/
//Return a array with random french word (from structure 1)
void rand_image_array(char **struct1, int struct1_size, char **array, int array_size);
//Free the array
void free_array(char **tab, int tab_size);

/*==== Files ====*/
int is_in_dir(char *image, char* folder);

#endif
