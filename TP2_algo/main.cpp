#include <Imagine/Graphics.h>
#include <algorithm>
#include <iostream>
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



void create_quadtree(QuadTree<int>*& q, int i, int j, int depth, int pixel_depth, int size, const byte g){
    QuadTree<int>** sons = new QuadTree<int>*[nbQuadDir];
    for(int d=0; d < nbQuadDir; ++d){
        int i_new = 2*i + I[d];  // i "index" of the dth son of q
        int j_new = 2*j + J[d];  // j "index" of the dth son of q
        if(depth == pixel_depth){
            sons[d] = new QuadLeaf<int>(g[i_new + size*j_new]);
        }
        else{
            sons[d] = new QuadNode<int>();
            create_quadtree(sons[d], i_new, j_new, depth+1, pixel_depth, size, g);
        }

    }

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

    // Exit
    return 0;
}
