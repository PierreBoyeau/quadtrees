#include <math.h>
#include <stdexcept>
#include <iostream>
#include <Imagine/Graphics.h>
#include "quadtree.h"

using namespace Imagine;
using namespace std;

// --- Classes
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


// Recursive function
// Create Quadtreee function


QuadTree<byte>* create_quadtree_BW(int size,int width, int height, byte*& g,
                                   Whiteleaf *whiteleaf_ptr, Blackleaf *blackleaf_ptr, int local_size=1, int i=0, int j=0); // Recursive function


QuadTree<byte>* create_quadtree(int i, int j, int size, byte*& g, int width, int height, int epsilon, int local_size=1); // Recursive function


void decode_quadtree(int i, int j, int size, QuadTree<byte>* q, byte* image, bool drawRectangles=false, int local_size=1); // Recursive function

// --- Utilities and secondary Quadtree function
bool are_four_equal_leaves(QuadTree<byte>* son0, QuadTree<byte>* son1,
                           QuadTree<byte>* son2, QuadTree<byte>* son3);
bool are_four_similar_leaves(QuadTree<byte>* son0, QuadTree<byte>* son1,
                           QuadTree<byte>* son2, QuadTree<byte>* son3, int epsilon);
int myPow(int x, int p);
int number_elements(QuadTree<byte>* q);
float compression_rate(QuadTree<byte>* q, int width, int height);

