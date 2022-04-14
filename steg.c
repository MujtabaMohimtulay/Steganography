#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel
{
    int red;
    int green;
    int blue;
};
struct com
{
    char * commemnt;
    struct com *next;
};

struct com *head = NULL;
struct com *current = NULL;

void insertComment(char * comments) {
   
   struct com *new = (struct com*) malloc(sizeof(struct com));
   new->commemnt = (char *)malloc(70*sizeof(char));	
   strcpy(new->commemnt, comments);
	
   
   new->next = head;
	
   
   head = new;
}
// refernce for function reverse - https://www.geeksforgeeks.org/reverse-a-linked-list/#:~:text=1%29%20Divide%20the%20list%20in%20two%20parts%20-,the%20stack%20until%20all%20the%20values%20are%20entered.
void reverse(struct com** h) {
   struct com* prev   = NULL;
   struct com* curr = *h;
   struct com* next;
	
   while (curr != NULL) {
      next  = curr->next;
      curr->next = prev;   
      prev = curr;
      curr = next;
   }
	
   *h = prev;
}

int commentNum() {
   int count = 0;
   struct com *current;
   current = head;
   while (current !=NULL)
   {
       count++;
       current = current->next;
   }
	
   return count;
}

/* An image loaded from a PPM file. */
struct PPM
{
    /* TODO: Question 1 */
    char type[2]; //array to hold type value of PPM image
    int width; // holds width value of PPM image
    int height; //holds height value of PPM image
    int max; //holds max value of PPM image
    struct Pixel *p; // pointer to dynamically created array to hold pixel values
    struct com *cp;//pointer to linked list of comments
    int commentCount;//holds nuber of comments
};

/* Reads an image from an open PPM file.
 * Returns a new struct PPM, or NULL if the image cannot be read. */
struct PPM *getPPM(FILE *f)
{
    /*new struct PPM image is assigned an array to the size of struct PPM
    we scan the file and assign values to the struct PPM according to the PPM specification
    struct Pixel pointer is assigned to dynamically created array to the size of the image(no of pixels) using malloc
    for loop scans file and assigns RGB values to the struct Pixel
    returns PPM image if not null
    */
    /* TODO: Question 2a */
    struct PPM *image = malloc(sizeof(struct PPM));
    fscanf(f,"%[^\n]",image->type);
    if(image->type[0]!='P' || image->type[1]!='3'){
        //checks if file is PPM
        fprintf(stderr,"file is not Simple PPM!\n");
        exit(0);
    }
    char x = getc(f);
    char y = getc(f);
    char z;
    if(y=='#'){
        ungetc(y,f);
        while( getc(f) =='#'){
            
            ungetc(y,f);
            char *commentLine =(char *) malloc(70*sizeof(char));
            fscanf(f,"%[^\n]",commentLine);
            insertComment(commentLine);
            
            getc(f);
            z=getc(f);
            if(z!='#'){
                ungetc(z,f);
                break;
            }
            ungetc(z,f);
        }
        
    }
    else{
        ungetc(y,f);
    }
    ungetc(x,f);
    
    image->commentCount=0;
    image->commentCount=commentNum();
    
    fscanf(f, "%d %d %d ",&image->width, &image->height, &image->max);
    image->p = malloc((image->width) * (image->height) * (sizeof(struct Pixel)));
    for (int i = 0; i < (image->height) * (image->width); i++)
    {
        fscanf(f, "%d %d %d", &(image->p[i].red), &(image->p[i].green), &(image->p[i].blue));
    }

    if (image != NULL)
    {
        return image;
    }

    return NULL;
}

/* Write img to stdout in PPM format. */
void showPPM(const struct PPM *img)
{
    /* TODO: Question 2b */
    struct Pixel p;
    /*prints values from the PPM struct according to PPM specification
    for loop is used to print RGB values of each pixel in the array of pixels
     */
    printf("%c%c\n",(char)img->type[0],(char)img->type[1]);
    if(img->commentCount!=0){
        reverse(&head);
        struct com *ptr = head;
        
        while (ptr !=NULL)
        {
            printf("%s\n",ptr->commemnt);
            ptr=ptr->next;
        }
        
    }
    
    printf("%d %d\n", img->width, img->height);
    printf("%d\n", img->max);
    for (int i = 0; i < (img->height) * (img->width); i++)
    {
        printf("%d %d %d\n", (img->p[i].red), (img->p[i].green), (img->p[i].blue));
    }
}

/* Opens and reads a PPM file, returning a pointer to a new struct PPM.
 * On error, prints an error message and returns NULL. */
struct PPM *readPPM(const char *filename)
{
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    /* Load the image using getPPM */
    struct PPM *img = getPPM(f);

    /* Close the file */
    fclose(f);

    if (img == NULL)
    {
        fprintf(stderr, "File %s could not be read.\n", filename);
        return NULL;
    }

    return img;
}

/* Encode the string text into the red channel of image img.
 * Returns a new struct PPM, or NULL on error. */
struct PPM *encode(const char *text, const struct PPM *img)
{
    /* TODO: Question 2c */
    struct PPM *newImage; //define new struct pointer 
    newImage = malloc(sizeof(struct PPM)); // malloc pointer to the size of struct PPM
    newImage->p = malloc((img->width) * (img->height) * (sizeof(struct Pixel))); //malloc pointer in new struct PPM to the size of PPM image
    //copies values from previous PPM struct to newly defined PPM struct
    newImage->type[0] = img->type[0];
    newImage->type[1] = img->type[1];
    newImage->cp=img->cp;
    newImage->commentCount=img->commentCount;
    newImage->width = img->width;
    newImage->height = img->height;
    newImage->max = img->max;
    for (int i = 0; i < img->height * img->width; i++)
    {
        newImage->p[i].red = img->p[i].red;
        newImage->p[i].green = img->p[i].green;
        newImage->p[i].blue = img->p[i].blue;
    }
    int i = 0, j = 0;
    srand(0);//to generate random pixels
    //created while loop to run the size of text to encode
    while (j < strlen(text))
    {
        i = i + rand() % (img->width); // to get random position
        if (newImage->p[i].red != text[j]) // checks if existing value of R value of selected pixel matches ascii value of character to encode
        {
            newImage->p[i].red = text[j];//reassigns red value in pixel with ascii value of character
            j++;
            if (i >= (img->height * img->width) - 1)
            {
                //end loop if end of image is reached without complete encoding
                fprintf(stderr, "due to less space in image your message was encoded upto %d characters\n", j);
                break;
            }
        }
    }
    //return new PPM image-newImage if not null
    if (newImage != NULL)
    {
        return newImage;
    }
    return NULL;
}

/* Extract the string encoded in the red channel of newimg, by comparing it
 * with oldimg. The two images must have the same size.
 * Returns a new C string, or NULL on error. */
char *decode(const struct PPM *oldimg, const struct PPM *newimg)
{
    /* TODO: Question 2d */
    //checks if both PPM images are of same type
    if (oldimg->height != newimg->height || oldimg->width != newimg->width || oldimg->max != newimg->max)
    {
        fprintf(stderr, "incorrect PPM files being comapred!\n");
        exit(0);
    }
    //create char array to hold decoded message
    char *msg = malloc(100 * sizeof(char));
    int j = 0;
    //runs for loop for size of image while comaparing red values of each pixel of both PPM images
    for (int i = 0; i < oldimg->height * oldimg->width; i++)
    {
        if ((newimg->p[i].red) != (oldimg->p[i].red))
        {
            //if red values are differnet add character to char array
            msg[j] = (newimg->p[i].red);
            j++;
        }
    }
    int k = 0;
    //loops throught char array to find new line character
    //replaces new line with '\0' to indicate end of string
    while (msg[k] != '\n')
    {
        k++;
    }
    msg[k] = '\0';

    // if char array is not empty return string(char array)
    if (msg != NULL)
    {
        return msg;
    }
    return NULL;
}

/* TODO: Question 3 */
int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    srand(time(NULL));

    /* Parse command-line arguments */
    if (argc == 3 && strcmp(argv[1], "t") == 0)
    {
        /* Mode "t" - test PPM reading and writing */

        struct PPM *img = readPPM(argv[2]);
        showPPM(img);
    }
    else if (argc == 3 && strcmp(argv[1], "e") == 0)
    {
        /* Mode "e" - encode PPM */
        // reads first PPM image and assigns to oldimg
        struct PPM *oldimg = readPPM(argv[2]);

        /* TODO: prompt for a message from the user, and read it into a string */
        //prompt to enter message to encode
        fprintf(stderr, "Please enter text to encode(max 100 characters)\n");
        //created char array to hold text entered
        char string[100];
        //gets string enterd by user upto 100 characters
        fgets(string, 100, stdin);
        //replaces last element in array with new line character
        string[99] = '\n';
        //calls encode function by passing string and oldimg
        //output  of encode is assigned to newimg
        struct PPM *newimg = encode(string, oldimg);
        /* TODO: encode the text into the image with encode, and assign to newimg */
        /* TODO: write the image to stdout with showPPM */
        //prints out newimg (PPM image with encoded matter)
        showPPM(newimg);
    }
    else if (argc == 4 && strcmp(argv[1], "d") == 0)
    {
        /* Mode "d" - decode PPM */

        struct PPM *oldimg = readPPM(argv[2]);
        // reads original ppm image using readPPPM and assigns to oldimg
        /* TODO: get original file filename from argv, load it with
           readPPM, then assign to oldimg */

        struct PPM *newimg = readPPM(argv[3]);
        // reads encoded ppm image using readPPPM and assigns to newimg
        /* TODO: get encoded file filename from argv, load it with
           readPPM, then assign to newimg */

        char *message = decode(oldimg, newimg);
        //calls decode function by passing oldimg and new image and the output is assigned to char  message
        ;
        /* TODO: decode the encodedPPM with the comparisonPPM and assign to message */

        /* TODO: print the decoded message to stdout */
        //prints decoded message into terminal
        printf("your message was: \n%s \n", message);
    }
    else
    {
        fprintf(stderr, "Unrecognised or incomplete command line.\n");
        return 1;
    }

    return 0;
}
