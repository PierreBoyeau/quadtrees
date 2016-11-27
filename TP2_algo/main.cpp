#include <Imagine/Graphics.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include "quadtree.h"

using namespace Imagine;
using namespace std;

#ifndef srcPath
#define srcPath(s) ("." s)
#endif

static int I[4] = {0, 0, 1, 1};
static int J[4] = {0, 1, 1, 0};


// The path and name of the town file
string default_image_file = srcPath("/running-horse-square.png");


bool are_four_equal_leaves(QuadTree<byte>** sons){
    bool result = sons[0]->isLeaf();
    int d = 1;
    while(result && d < nbQuadDir){
        if(sons[d]->isLeaf())
            result = (sons[d]->value() == sons[0]->value());
        else
            result = false;
        ++d;
    }
    return(result);
}

void create_quadtree(QuadTree<byte>*& q, int i, int j, int depth, int pixel_depth, int size, byte*& g){
    QuadTree<byte>** sons = new QuadTree<byte>*[nbQuadDir];
    cout<<depth<<endl;
    for(int d=0; d < nbQuadDir; ++d){
        int i_new = 2*i + I[d];  // i "index" of the dth son of q
        int j_new = 2*j + J[d];  // j "index" of the dth son of q
        if(depth == pixel_depth-1){
            sons[d] = new QuadLeaf<byte>(g[i_new + size*j_new]);
        }
        else{
            sons[d] = new QuadNode<byte>();
            create_quadtree(sons[d], i_new, j_new, depth+1, pixel_depth, size, g);
        }
    }
    if(are_four_equal_leaves(sons)){
        *q = QuadLeaf<byte>(sons[0]->value());
    }
    else{
        *q = QuadNode<byte>(sons[0], sons[1], sons[2], sons[3]);
    }
    delete[] sons;

}


void recursive_from_leaf(int i, int j, int depth, int pixel_depth, int size, byte value, byte*& g){
    for(int d=0; d < nbQuadDir; ++d){
        int i_new = 2*i + I[d];  // i "index" of the dth son of q
        int j_new = 2*j + J[d];  // j "index" of the dth son of q
        if(depth == pixel_depth){
            g[i_new + size*j_new] = value;
        }
        else{
            recursive_from_leaf(i_new, j_new, depth+1, pixel_depth, size, value, g);
        }
    }
}


void decode_quadtree(QuadTree<byte>*& q, int i, int j, int depth, int pixel_depth, int size, byte*& g){
    for(int d=0; d < nbQuadDir; ++d){
        int i_new = 2*i + I[d];  // i "index" of the dth son of q
        int j_new = 2*j + J[d];  // j "index" of the dth son of q
        QuadTree<byte>* q_son = q->son(d);
        if(q_son->isLeaf()){
            byte value = q->value();
        }
        else{
            decode_quadtree(q_son, i_new, j_new, depth+1, pixel_depth, size, g);
        }
    }

}


int main(){
//    // Get image file
//    const char *image_file =
//    (argc > 1) ? argv[1] : srcPath("/running-horse-square.png");
//    // Load image
//    byte* image;
//    int width, height;
//    cout << "Loading image: " << image_file << endl;
//    loadGreyImage(image_file, image, width, height);
//    // Print statistics
//    cout << "Image size: " << width << "x" << height << endl;
//    cout << "Number of pixels: " << width*height << endl;
//    //Display image
//    Window window = openWindow(width, height);
//    putGreyImage(IntPoint2(0,0), image, width, height);
//    //Pause
//    click();

    int width = 4;
    byte* image = new byte[8];
    image[0]= 255;
    image[1]= 0;
    image[2]= 0;
    image[3]= 0;
    image[4]= 255;
    image[5]= 255;
    image[6]= 0;
    image[7] = 255;
    Window window = openWindow(width, width);
    putGreyImage(IntPoint2(0,0), image, width, width);
    //Pause
    click();

    // Question 3
    QuadTree<byte>* q = new QuadNode<byte>();
    int pixel_depth = int(log2(width));
    create_quadtree(q, 0, 0, 0, pixel_depth, width, image);

    display(q);
//    byte* decoded_image = new byte[width*width];
//    decode_quadtree(q, 0, 0, 0, pixel_depth, width, decoded_image);
//    Window window = openWindow(width, height);
//    putGreyImage(IntPoint2(0,0), decoded_image, width, height);
//    click();
    // Exit
    return 0;
}
