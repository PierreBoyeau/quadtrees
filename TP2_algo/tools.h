/***************************************************************************
 * Basic datatypes to construct a quadtree
 *
 * Author: Pierre Boyeau
 ***************************************************************************/



#include <math.h>
#include <stdexcept>
#include <iostream>
#include <Imagine/Graphics.h>
#include "quadtree.h"

using namespace Imagine;
using namespace std;

// --- Classes
/*
 * Blackleaf and Whiteleaf inherit from QuadLeaf (see quadtree.h)
 * They are used to decript B&W pictures in order to reduce the space complexity
 */

class Blackleaf:public QuadLeaf<byte>{
public:
   Blackleaf()
       :QuadLeaf<byte>(255){}
   ~Blackleaf(){}
};


class Whiteleaf:public QuadLeaf<byte>{
public:
    Whiteleaf()
        :QuadLeaf<byte>(0){
    }
    ~Whiteleaf(){}
};


// --- Primary Quadtree functions

// Returns a Quadtree pointer that stores the image g data. REQUIRES a bivalue B&W pictures (i.e. g values must be equal
// to 0 OR to 255)
/*
 * size : size of the SQUARE image associated with the picture
 * width : width of the original image one wishes to compress
 * height : height of the original image one wishes to compress
 * g : image data
 * whiteleaf_ptr : Whiteleaf pointer
 * blackleaf_ptr : Blackleaf pointer
 * local_size : level of recursion (should not be used if not understood)
 * i, j  : helps determine pixel(s) associated with node (should not be used if not understood)
 *
 */
QuadTree<byte>* create_quadtree_BW(int size,int width, int height, byte*& g,
                                   Whiteleaf *whiteleaf_ptr, Blackleaf *blackleaf_ptr, int local_size=1, int i=0, int j=0); // Recursive function

// Returns a QuadTree pointer that stores the image g data. Works with grey images
/*
 * epsilon : max difference of value between bytes to be stored in an unique leaf
 */
QuadTree<byte>* create_quadtree(int size, byte*& g, int width, int height, int epsilon, int local_size=1, int i=0, int j=0); // Recursive function


void create_quadtree_color(int size, byte*& red_img, byte*& green_img, byte*& blue_img,
                           QuadTree<byte>*& red_tree, QuadTree<byte>*& green_tree, QuadTree<byte>*& blue_tree,
                           int width, int height, int epsilon);


// Decodes the QuadTree q in image.
/*
 * drawRectangles : precise if we want to construct rectangles to "see" leaves
 */
void decode_quadtree(int size, QuadTree<byte>* q, byte* image, bool drawRectangles=false, int local_size=1, int i=0, int j=0); // Recursive function


void decode_quadtree_color(int size, QuadTree<byte> *red_tree, QuadTree<byte> *green_tree, QuadTree<byte> *blue_tree,
                           byte* red_imagedecoded, byte* green_imagedecoded, byte* blue_imagedecoded);

// --- Utilities and secondary Quadtree function
bool are_four_equal_leaves(QuadTree<byte>* son0, QuadTree<byte>* son1,
                           QuadTree<byte>* son2, QuadTree<byte>* son3);
bool are_four_similar_leaves(QuadTree<byte>* son0, QuadTree<byte>* son1,
                           QuadTree<byte>* son2, QuadTree<byte>* son3, int epsilon);
int myPow(int x, int p);
int number_elements(QuadTree<byte>* q);
float compression_rate(QuadTree<byte>* q, int width, int height);

