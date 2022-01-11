#include <bits/stdc++.h>
#include "huffman_tree.cpp"


int main(){
    // std::vector<std::tuple<int,int,char>> inorder={{4,4,'a'},{2,2,'a'},{5,5,'a'},{1,1,'a'},{6,6,'a'},{3,3,'a'}};
    // std::vector<std::tuple<int,int,char>> preorder={{1,1,'a'},{2,2,'a'},{4,4,'a'},{5,5,'a'},{3,3,'a'},{6,6,'a'}};
    
    std::vector<std::tuple<int,int,char>> preorder={{1,1,'a'},{2,2,'a'},{3,3,'a'},{4,4,'a'},{5,5,'a'},{6,6,'a'},{7,7,'a'}};
    std::vector<std::tuple<int,int,char>> inorder={{2,2,'a'},{4,4,'a'},{6,6,'a'},{5,5,'a'},{7,7,'a'},{3,3,'a'},{1,1,'a'}};

    huffman_tree* ht=make_tree_from_traversal(preorder,inorder);
}