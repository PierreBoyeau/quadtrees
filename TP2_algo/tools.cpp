#include "tools.h"



int myPow(int x, int p) {
  if (p == 0) return 1;
  if (p == 1) return x;
  return x * myPow(x, p-1);
}

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

bool are_four_similar_leaves(QuadTree<byte>* son0, QuadTree<byte>* son1,
                           QuadTree<byte>* son2, QuadTree<byte>* son3, int epsilon){
    bool result = son0->isLeaf() && son1->isLeaf()
            && son2->isLeaf() && son3->isLeaf();
    if(result){
        result = (abs(son1->value() - son0->value()) < epsilon);
        result = result && (abs(son2->value() - son0->value()) < epsilon);
        result = result && (abs(son3->value() - son0->value()) < epsilon);
    }
    return(result);
}



QuadTree<byte>* create_quadtree_BW(int size,int width, int height, byte*& g,
                                   Whiteleaf *whiteleaf_ptr, Blackleaf *blackleaf_ptr, int local_size, int i, int j){
    if(local_size == size){
        if(i<width && j<height){
            if (g[i+width*j] == 0)
                return(whiteleaf_ptr);
            else if(g[i+width*j] == 255)
                return(blackleaf_ptr);
            else
                throw invalid_argument("This image isn't B&W");
        }
        else{
            return(whiteleaf_ptr);
        }
    }

    else{
        QuadTree<byte>* son0 = create_quadtree_BW(size, width, height, g,
                                                  whiteleaf_ptr, blackleaf_ptr, 2*local_size, 2*i, 2*j);
        QuadTree<byte>* son1 = create_quadtree_BW(size, width, height, g,
                                                  whiteleaf_ptr, blackleaf_ptr, 2*local_size, 2*i+1, 2*j);
        QuadTree<byte>* son2 = create_quadtree_BW(size, width, height, g,
                                                  whiteleaf_ptr, blackleaf_ptr, 2*local_size, 2*i+1, 2*j+1);
        QuadTree<byte>* son3 = create_quadtree_BW(size, width, height, g,
                                                  whiteleaf_ptr, blackleaf_ptr, 2*local_size, 2*i, 2*j+1);

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


QuadTree<byte>* create_quadtree(int i, int j, int size, byte*& g, int width, int height, int epsilon, int local_size){
    if(local_size == size){
        if(i<width && j<height){
            //In this case the pixel is a image-pixel
            return(new QuadLeaf<byte>(g[i+width*j]));
        }
        else{
            return(new QuadLeaf<byte>(255));
        }
    }
    else{
        QuadTree<byte>* son0 = create_quadtree(2*i, 2*j, size, g, width, height, epsilon, 2*local_size);
        QuadTree<byte>* son1 = create_quadtree(2*i+1, 2*j, size, g, width, height, epsilon, 2*local_size);
        QuadTree<byte>* son2 = create_quadtree(2*i+1, 2*j+1, size, g, width, height, epsilon, 2*local_size);
        QuadTree<byte>* son3 = create_quadtree(2*i, 2*j+1, size, g, width, height, epsilon, 2*local_size);

        if(are_four_similar_leaves(son0, son1, son2, son3, epsilon)){
            byte color = son0->value();
            return(new QuadLeaf<byte>(color));
        }

        else{
            QuadNode<byte>* new_node = new QuadNode<byte>(son0, son1, son2, son3);
            return(new_node);
        }
    }
}



void decode_quadtree(int i, int j, int size, QuadTree<byte>* q, byte* image, bool drawRectangles, int local_size){
    if(q->isLeaf()){
        int nb_cells = int(size/local_size);
        //Coloring the pixels corresponding to the leaf
        bool isPartofRectangle;
        for(int i_pix=nb_cells*i; i_pix<nb_cells*(i+1);++i_pix){
            for(int j_pix=nb_cells*j; j_pix<nb_cells*(j+1);++j_pix){
                isPartofRectangle = (i_pix==nb_cells*i || i_pix==nb_cells*(i+1)-1
                                     || j_pix==nb_cells*j || j_pix==nb_cells*(j+1)-1);
                if(drawRectangles && isPartofRectangle)
                    image[i_pix + size*j_pix]= 128;
                else
                    image[i_pix + size*j_pix] = q->value();
            }
        }
    }
    else{
        decode_quadtree(2*i, 2*j, size, q->son(0), image, drawRectangles, 2*local_size);
        decode_quadtree(2*i+1, 2*j, size, q->son(1), image, drawRectangles, 2*local_size);
        decode_quadtree(2*i+1, 2*j+1, size, q->son(2), image, drawRectangles, 2*local_size);
        decode_quadtree(2*i, 2*j+1, size, q->son(3), image, drawRectangles, 2*local_size);
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
