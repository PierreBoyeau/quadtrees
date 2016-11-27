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
    }
    return(result);
}

void create_quadtree(QuadTree<byte>*& q, int i, int j, int depth, int pixel_depth, int size, const byte g){
    QuadTree<byte>** sons = new QuadTree<byte>*[nbQuadDir];
    for(int d=0; d < nbQuadDir; ++d){
        int i_new = 2*i + I[d];  // i "index" of the dth son of q
        int j_new = 2*j + J[d];  // j "index" of the dth son of q
        if(depth == pixel_depth){
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


int main(int argc, char **argv)
{
    // Get image file
    const char *image_file =
    (argc > 1) ? argv[1] : srcPath("/running-horse-square.png");
    // Load image
    byte* image;
    int width, height;
    cout << "Loading image: " << image_file << endl;
    loadGreyImage(image_file, image, width, height);
    // Print statistics
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;
    // Display image
    Window window = openWindow(width, height);
    putGreyImage(IntPoint2(0,0), image, width, height);
    // Pause
    click();

    // Question 3
    QuadTree<byte>* q = new QuadNode<byte>();
    int pixel_depth = int(log2(width));
    create_quadtree(q, 0, 0, 0, pixel_depth, width, image);

    // Exit
    return 0;
}
