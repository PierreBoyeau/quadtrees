/***************************************************************************
 * Example of use of quadtree.h
 *
 * Author: Renaud Marlet (c) 2011-2015
 ***************************************************************************/

#include "quadtree.h"

// Build a quadtree example and print it
int main()
{
    // Leaf and node creation
    QuadTree<int> *ql, *qn;
    ql = new QuadLeaf<int>(1);                          // ql = 1
    qn = new QuadNode<int>(0, 0, new QuadLeaf<int>(2), ql);// qn = [_,_,2,1]
    // Leaf value modifications
    ql->value() = 3;                                   // qn = [_,_,2,3]
    ql->value()++;                                     // qn = [_,_,2,4]
    // Node modifications
    qn->son(NW) = new QuadLeaf<int>(6);                // qn = [6,_,2,3]
    qn->son(NW) = new QuadNode<int>(0,0,0,qn->son(NW));// qn = [[_,_,_,6],_,2,3]
    qn->son(NE) = new QuadLeaf<int>(7);                // qn = [[_,_,_,6],7,2,4]
    qn->son(SE) = new QuadNode<int>(0,0,qn->son(SE),0);
                                               // qn = [[_,_,_,6],7,[_,_,2,_],4]
    // Modification with (sub)typing test: increment leaves at depth 1
    for (int d = 0; d < nbQuadDir; d++) {
	QuadTree<int> *q = qn->son(d);
	if (q && q->isLeaf()) q->value()++;    // qn = [[_,_,_,6],8,[_,_,2,_],5]
    }
    qn->son(NW) = new QuadNode<int>(0,0,qn->son(NW),0);
                                       // qn = [[_,_,_,[_,_,6,_]],8,[_,_,2,_],5]
    /*
       This corresponds to the following quadtree:

         +---------------+---------------+
         |       |       |               |
         |       |       |               |
         |       |       |               |
         +-------+-------+       8       |
         |       |   |   |               |
         |       +---+---+               |
         |       | 6 |   |               |
         +-------+-------+-------+-------+
         |               |       |       |
         |               |       |       |
         |               |       |       |
         |       5       +-------+-------+
         |               |       |       |
         |               |       |   2   |
         |               |       |       |
         +---------------+-------+-------+
    */
    // Display quadtree, e.g. for debugging
    display(qn);
    // Don't forget to delete it when not needed anymore
    delete qn;
}
