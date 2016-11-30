#include <Imagine/Graphics.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include "quadtree.h"

using namespace Imagine;
using namespace std;

#ifndef srcPath
#define srcPath(s) ("." s)
#endif

int myPow(int x, int p) {
  if (p == 0) return 1;
  if (p == 1) return x;
  return x * myPow(x, p-1);
}

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


// The path and name of the town file
string default_image_file = srcPath("/running-horse-square.png");


bool are_four_equal_leaves(QuadTree<byte>* son0, QuadTree<byte>* son1,
                           QuadTree<byte>* son2, QuadTree<byte>* son3){
    bool result = son0->isLeaf() && son1->isLeaf()
            && son2->isLeaf() && son3->isLeaf();
    if(result){
        result = (son0->value() == son1->value()) && (son1->value() == son2->value())
                && (son2->value() == son3->value());
    }
    return(result);
}


QuadTree<byte>* create_quadtree_BW(int i, int j, int size, byte*& g,
                                   Whiteleaf *whiteleaf_ptr, Blackleaf *blackleaf_ptr, int local_size=1){
    if(local_size == size){
        if (g[i+size*j] == 0)
            return(whiteleaf_ptr);
        else if(g[i+size*j] == 255)
            return(blackleaf_ptr);
        else
            throw invalid_argument("This image isn't B&W");

    }
    else{
        QuadTree<byte>* son0 = create_quadtree_BW(2*i, 2*j, size, g,
                                                  whiteleaf_ptr, blackleaf_ptr, 2*local_size);
        QuadTree<byte>* son1 = create_quadtree_BW(2*i+1, 2*j, size, g,
                                                  whiteleaf_ptr, blackleaf_ptr, 2*local_size);
        QuadTree<byte>* son2 = create_quadtree_BW(2*i+1, 2*j+1, size, g,
                                                  whiteleaf_ptr, blackleaf_ptr, 2*local_size);
        QuadTree<byte>* son3 = create_quadtree_BW(2*i, 2*j+1, size, g,
                                                  whiteleaf_ptr, blackleaf_ptr, 2*local_size);

        if(are_four_equal_leaves(son0, son1, son2, son3)){
            byte color = son0->value();
            return(new QuadLeaf<byte>(color));
        }

        else{
            QuadNode<byte>* new_node = new QuadNode<byte>(son0, son1, son2, son3);
            return(new_node);
        }
    }
}

QuadTree<byte>* create_quadtree(int i, int j, int size, byte*& g, int local_size=1){
    if(local_size == size)
        return(new QuadLeaf<byte>(g[i+size*j]));
    else{
        QuadTree<byte>* son0 = create_quadtree(2*i, 2*j, size, g, 2*local_size);
        QuadTree<byte>* son1 = create_quadtree(2*i+1, 2*j, size, g, 2*local_size);
        QuadTree<byte>* son2 = create_quadtree(2*i+1, 2*j+1, size, g, 2*local_size);
        QuadTree<byte>* son3 = create_quadtree(2*i, 2*j+1, size, g, 2*local_size);

        if(are_four_equal_leaves(son0, son1, son2, son3)){
            byte color = son0->value();
            return(new QuadLeaf<byte>(color));
        }

        else{
            QuadNode<byte>* new_node = new QuadNode<byte>(son0, son1, son2, son3);
            return(new_node);
        }
    }
}


void decode_quadtree(int i, int j, int size, QuadTree<byte>* q, byte* image, int local_size=1){
    if(q->isLeaf()){
        int nb_cells = int(size/local_size);
        //Coloring the pixels corresponding to the leaf
        for(int i_pix=nb_cells*i; i_pix<nb_cells*(i+1);++i_pix){
            for(int j_pix=nb_cells*j; j_pix<nb_cells*(j+1);++j_pix){
                if(i_pix==nb_cells*i || i_pix==nb_cells*(i+1)-1
                        || j_pix==nb_cells*j || j_pix==nb_cells*(j+1)-1)
                    image[i_pix + size*j_pix]= 128;
                else
                    image[i_pix + size*j_pix] = q->value();
            }
        }
    }
    else{
        decode_quadtree(2*i, 2*j, size, q->son(0), image, 2*local_size);
        decode_quadtree(2*i+1, 2*j, size, q->son(1), image, 2*local_size);
        decode_quadtree(2*i+1, 2*j+1, size, q->son(2), image, 2*local_size);
        decode_quadtree(2*i, 2*j+1, size, q->son(3), image, 2*local_size);
    }
}

int number_elements(QuadTree<byte>* q){
    if(q->isLeaf())
        return(1);
    else{
        return(number_elements(q->son(0)) + number_elements(q->son(1))
               + number_elements(q->son(2)) + number_elements(q->son(3)));
    }
}
float compression_rate(QuadTree<byte>* q, int width, int height){
    return( 1.f - (float)number_elements(q)/(float)(width * height) );
}



int main(){
    // Get image file
    const char *image_file = (argc > 1) ? argv[1] : srcPath("/running-horse-square.png");
    // Load image
    byte* image;
    int width, height;
    cout << "Loading image: " << image_file << endl;
    loadGreyImage(image_file, image, width, height);
    // Print statistics
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;
    //Display image
    Window window = openWindow(width, height);
    //putGreyImage(IntPoint2(0,0), image, width, height);

    QuadTree<byte>* tree = create_quadtree(0, 0, height, image);
    byte *image_decoded = new byte[height*height];
    decode_quadtree(0, 0, height, tree, image_decoded);
    putGreyImage(0, 0, image_decoded, width, height);

    //Pause
    click();
    cout<<"Empirical compression rate: ";
    cout<<compression_rate(tree, width, height)<<endl;
    // Exit

    return 0;
}




//    int width = 4;
//    byte* image = new byte[16];
//    image[0]= 255;
//    image[1]= 255;
//    image[2]= 255;
//    image[3]= 255;
//    image[4]= 255;
//    image[5]= 255;
//    image[6]= 255;
//    image[7] = 255;
//    image[8] = 255;
//    image[9] = 255;
//    image[10] = 255;
//    image[11] = 255;
//    image[12] = 255;
//    image[13] = 255;
//    image[14] = 255;
//    image[15] = 255;
//    QuadTree<byte>* q = create_quadtree(0, 0, 4, image);
//    display(q);

//    byte *image_decoded = new byte[width*width];
//    decode_quadtree(0, 0, 4, q, image_decoded);
//    for(int i = 0; i<16; ++i){
//        cout<<i;
//        cout<<": ";
//        cout<<int(image_decoded[i])<<endl;
//    }


//void create_quadtree(QuadTree<byte>* q, int i, int j, int depth, int pixel_depth, int size, byte*& g){
//    QuadTree<byte> **sons = new QuadTree<byte>*[nbQuadDir];
//    cout<<depth<<endl;
//    for(int d=0; d < nbQuadDir; ++d){
//        int i_new = 2*i + I[d];  // i "index" of the dth son of q
//        int j_new = 2*j + J[d];  // j "index" of the dth son of q
//        if(depth == pixel_depth-1){
//            sons[d] = new QuadLeaf<byte>(g[i_new + size*j_new]);
//        }
//        else{
//            sons[d] = new QuadNode<byte>();
//            create_quadtree(sons[d], i_new, j_new, depth+1, pixel_depth, size, g);
//        }
//    }
//    if(are_four_equal_leaves(sons)){
//        *q = QuadLeaf<byte>(sons[0]->value());
//    }
//    else{
//        *q = QuadNode<byte>(sons[0], sons[1], sons[2], sons[3]);
//    }
//    delete[] sons;
//}

