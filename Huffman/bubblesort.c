#include "bubblesort.h"

void swap(NodeStr** Node1, NodeStr** Node2)
{
    NodeStr* temp = *Node1;
    *Node1 = *Node2;
    *Node2 = temp;
}

void BubbleSort(NodeStr* Nodes[256], int SymbAmount)
{
    for(int i = SymbAmount - 1 ; i > 1; --i)
    {
        for(int j = SymbAmount - 1 ; j > SymbAmount - 1 - i ; --j)
        {
            if(Nodes[j-1]->Freq < Nodes[j]->Freq)
            {
                swap(&Nodes[j-1], &Nodes[j]);
            }
        }
    }
}
