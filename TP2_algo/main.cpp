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

    // -------------------B&W Image Example------------------------
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
    decode_quadtree(size0, tree0, image_decoded0, true);
    putGreyImage(0, 0, image_decoded0, size0, size0);
    click();
    cout<<"Empirical compression rate: ";
    cout<<compression_rate(tree0, width0, height0)<<endl;
    closeWindow(window0);
    // Pointers suppression
    delete image0;
    delete image_decoded0;


    // -------------- Grey image example 1------------------------
    // Get image file
    const char *image_file = (argc > 1) ? argv[1] : srcPath("/lena.png");
    // Load image
    byte* image;
    int width, height;
    cout << "Loading image: " << image_file << endl;
    loadGreyImage(image_file, image, width, height);
    // Print statistics
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;
    //Display image
    Window window1 = openWindow(width, height);
    //putGreyImage(IntPoint2(0,0), image, width, height);
    int size = myPow(2, int(log2(max(width, height))) + 1);
    QuadTree<byte>* tree = create_quadtree(size, image, width, height, 20);
    byte *image_decoded = new byte[size*size];
    decode_quadtree(size, tree, image_decoded);
    putGreyImage(0, 0, image_decoded, size, size);

    //Pause
    click();
    cout<<"Empirical compression rate: ";
    cout<<compression_rate(tree, width, height)<<endl;
    closeWindow(window1);


    // -------------- Grey image example 2------------------------
    // Load image
    byte* image2;
    cout << "Loading image: " << image_file << endl;
    loadGreyImage(image_file, image2, width, height);
    // Print statistics
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;
    //Display image
    Window window2 = openWindow(width, height);
    //putGreyImage(IntPoint2(0,0), image, width, height);
    QuadTree<byte>* tree2 = create_quadtree(size, image2, width, height, 50);
    byte *image_decoded2 = new byte[size*size];
    decode_quadtree(size, tree2, image_decoded2, true);
    putGreyImage(0, 0, image_decoded2, size, size);
    click();
    cout<<"Empirical compression rate: ";
    cout<<compression_rate(tree, width, height)<<endl;
    closeWindow(window2);
    delete image_decoded2;
    delete image_decoded;
    delete image2;
    delete image;
    delete tree;
    delete tree2;

    // -------------- Color image example ------------------------
    // Get image file
    const char *image_file3 = (argc > 1) ? argv[1] : srcPath("/forest.png");
    // Load image
    int width3, height3;
    cout << "Loading image: " << image_file3 << endl;
    byte* red_img;
    byte* green_img;
    byte* blue_img;
    loadColorImage(image_file3, red_img, green_img, blue_img, width3, height3);
    // Print statistics
    cout << "Image size: " << width3 << "x" << height3 << endl;
    cout << "Number of pixels: " << width3*height3 << endl;
    //Display image
    Window window3 = openWindow(width3, height3);
    //putGreyImage(IntPoint2(0,0), image, width, height);
    int size3 = myPow(2, int(log2(max(width3, height3))) + 1);


    //Coding & decoding red
    QuadTree<byte> *red_tree, *green_tree, *blue_tree;
    create_quadtree_color(size3, red_img, green_img, blue_img,
                          red_tree, green_tree, blue_tree, width3, height3, 50);

    byte *red_imgd = new byte[size3*size3];
    decode_quadtree(size3, red_tree, red_imgd);
    //Coding & decoding green
    byte *green_imgd = new byte[size3*size3];
    decode_quadtree(size3, green_tree, green_imgd);
    //Coding & decoding blue
    byte *blue_imgd = new byte[size3*size3];
    decode_quadtree(size3, blue_tree, blue_imgd);

    putColorImage(0, 0, red_imgd, green_imgd, blue_imgd, size3, size3);

    //Pause
    click();

    closeWindow(window3);
    delete red_img, green_img, blue_img;
    delete red_tree, green_tree, blue_tree;
    delete red_imgd, green_imgd, blue_imgd;
}


