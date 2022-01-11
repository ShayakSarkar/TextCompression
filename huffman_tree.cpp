#include <bits/stdc++.h>

class huffman_tree{
public:
    huffman_tree *left,*right;
    int id;
    int tot_freq;
    char char_val;
    huffman_tree(){
        id=0;
        left=right=nullptr;
    }
    huffman_tree(int f,char c){
        left=right=nullptr;
        id=-1;
        tot_freq=f;
        char_val=c;
    }
    huffman_tree(int id,int f,char c){
        left=right=nullptr;
        this->id=id;
        tot_freq=f;
        char_val=c;
    }
    ~huffman_tree(){
        if (left!=nullptr) delete left;
        if (right!=nullptr) delete right;
        left=right=nullptr;
    }
    void print(std::string indent){
        if(left) left->print(indent+"   ");
        std::cout<<indent<<tot_freq<<std::endl;
        if(right) right->print(indent+"   ");
    }
    void print(){
        print("");
    }
    void get_codes_helper(std::unordered_map<char,std::string>& codes,std::string code){
        if(!left && !right){
            codes[char_val]=code;
            return;
        }
        if(left) left->get_codes_helper(codes,code+"0");
        if(right) right->get_codes_helper(codes,code+"1");
    }
    std::unordered_map<char,std::string> get_codes(){
        std::unordered_map<char,std::string> codes;
        get_codes_helper(codes,"");
        return codes;
    }
};

class comp{
public:
    bool operator()(const huffman_tree* a,const huffman_tree* b) const {
        return a->tot_freq>=b->tot_freq;
    }
};

void add_ids(huffman_tree* ht,int& id){
    std::cout<<"node"<<std::endl;
    ht->id=id;
    id++;
    if(ht->left) add_ids(ht->left,id);
    if(ht->right) add_ids(ht->right,id);
}

huffman_tree* make_huffman_tree_helper(std::priority_queue<huffman_tree*,std::vector<huffman_tree*>,comp>& pq){
    while(pq.size()>1){
        huffman_tree* a=pq.top(); pq.pop();
        huffman_tree* b=pq.top(); pq.pop();
        huffman_tree* c=new huffman_tree(a->tot_freq+b->tot_freq,'$');
        c->left=a;
        c->right=b;
        pq.push(c);
    }
    int id=0;
    add_ids(pq.top(),id);
    return pq.top();
}

huffman_tree* make_huffman_tree(const std::unordered_map<char,int>& mapping){
    std::priority_queue<huffman_tree*,std::vector<huffman_tree*>,comp> pq;
    for(auto p : mapping){
        huffman_tree* ht=new huffman_tree(p.second,p.first);
        pq.push(ht);
    }
    return make_huffman_tree_helper(pq);
}

huffman_tree* make_tree_from_traversal_helper(std::vector<std::tuple<int,int,char>>& preorder,
                                              int pre_st,int pre_en,
                                              std::vector<std::tuple<int,int,char>>& inorder,
                                              int in_st,int in_en){
    if((pre_st>pre_en) || (in_st>in_en)){
        return nullptr;
    }
    huffman_tree* root=new huffman_tree(std::get<0>(preorder[pre_st]),std::get<1>(preorder[pre_st]),std::get<2>(preorder[pre_st]));
    int new_pre_st=pre_st+1;
    int i=in_st;
    for(;i<=in_en;i++){
        if(std::get<0>(inorder[i])==std::get<0>(preorder[pre_st])){
            break;
        }
    }
    // std::cout<<i<<std::endl;
    int new_in_en=i-1;
    int new_in_st=in_st;
    int new_pre_en;
    if(i-1<in_st) new_pre_en=pre_st;
    else{
        for(int j=pre_st;j<=pre_en;j++){
            if(std::get<0>(preorder[j])==std::get<0>(inorder[i-1])){
                new_pre_en=j;
                break;
            }
        }
    }
    root->left=make_tree_from_traversal_helper(preorder,new_pre_st,new_pre_en,inorder,new_in_st,new_in_en);

    new_pre_st=new_pre_en+1;
    new_pre_en=pre_en;
    new_in_st=i+1;
    new_in_en=in_en;

    root->right=make_tree_from_traversal_helper(preorder,new_pre_st,new_pre_en,inorder,new_in_st,new_in_en);

    return root;
}

huffman_tree* make_tree_from_traversal(std::vector<std::tuple<int,int,char>>& preorder,std::vector<std::tuple<int,int,char>>& inorder){
    huffman_tree* ht=make_tree_from_traversal_helper(preorder,0,preorder.size()-1,inorder,0,inorder.size()-1);
    ht->print();
    return ht;
}