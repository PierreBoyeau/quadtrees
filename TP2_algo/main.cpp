#include <Imagine/Graphics.h>
#include <algorithm>
#include <iostream>
#include "quadtree.h"
#include "tools.h"

using namespace Imagine;
using namespace std;

#ifndef srcPath
#define srcPath(s) ("." s)
#endif



int main(){

    // B&W Image Example
    // Get image file
    const char *image_file0 = (argc > 1) ? argv[1] : srcPath("/running-horse-rect.png");
    // Load image
    byte* image0;
    int width0, height0;
    cout << "Loading image: " << image_file0 << endl;
    loadGreyImage(image_file0, image0, width0, height0);
    // Print statistics
    cout << "Image size: " << width0 << "x" << height0 << endl;
    //Display image
    Window window0 = openWindow(width0, height0);
    //putGreyImage(IntPoint2(0,0), image, width, height);

    int size0 = myPow(2, int(log2(max(width0, height0))) + 1); // Size of the square QuadTree
    Whiteleaf* myWhiteleaf;
    myWhiteleaf = new Whiteleaf();
    Blackleaf* myBlackleaf;
    myBlackleaf = new Blackleaf();
    QuadTree<byte>* tree0 = create_quadtree_BW(size0, width0, height0, image0, myWhiteleaf, myBlackleaf);
    byte *image_decoded0 = new byte[size0*size0];
    decode_quadtree(0, 0, size0, tree0, image_decoded0, true);
    putGreyImage(0, 0, image_decoded0, size0, size0);

    //Pause
    click();
    cout<<"Empirical compression rate: ";
    cout<<compression_rate(tree0, width0, height0)<<endl;

//    // Grey image example
//    // Get image file
//    const char *image_file = (argc > 1) ? argv[1] : srcPath("/lena.png");
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
//    //putGreyImage(IntPoint2(0,0), image, width, height);


//    int size = myPow(2, int(log2(max(width, height))) + 1);
//    QuadTree<byte>* tree = create_quadtree(0, 0, size, image, width, height, 30);
//    byte *image_decoded = new byte[size*size];
//    decode_quadtree(0, 0, size, tree, image_decoded);
//    putGreyImage(0, 0, image_decoded, size, size);

//    //Pause
//    click();
//    cout<<"Empirical compression rate: ";
//    cout<<compression_rate(tree, width, height)<<endl;
//    // Exit



    return 0;
}
