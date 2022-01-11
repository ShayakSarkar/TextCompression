#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
// #include "huffman_tree.cpp"
/*
OP file format i.e *.bin is 
----------------------------+
int padding                 |
preorder tot_freq           |
preorder char_val           |
postorder tot_freq          |
postorder char_val          |
                            |
-----COMPRESSED CONTENTS----|
-----COMPRESSED CONTENTS----|
-----COMPRESSED CONTENTS----|
-----COMPRESSED CONTENTS----|
*/

std::unordered_map<char,int> make_freq_table(std::string filename){   
    std::ifstream ifile(filename);
    std::string data;
    std::unordered_map<char,int> mapping;
    char buf[100000];
    while(!ifile.eof()){
        memset(buf,0,100000);
        ifile.getline(buf,100000,'\n');
        for(int i=0;buf[i]!='\0';i++){
            if(mapping.find(buf[i])==mapping.end()){
                mapping[buf[i]]=0;
            }
            mapping[buf[i]]++;
        }
        if(mapping.find('\n')==mapping.end()) mapping['\n']=0;
        mapping['\n']++;
    }
    ifile.close();
    return mapping;
}

int get_padding(std::string& op){
    return 8-op.size()%8;
}

std::string pad(std::string& byte){
    if(byte.size()==8) return byte;
    int zeros=8-byte.size();
    for(int i=0;i<zeros;i++) byte+="0";
    return byte;
}

char convert_to_dec(std::string byte){
    char dec=0;
    int pow=0;
    for(int i=byte.size()-1;i>=0;i--){
        if(byte[i]=='1'){
            dec+=(char)(std::pow(2,pow));
        }
        pow++;
    }
    return dec;
}

int get_node_count(huffman_tree* ht){
    if(!ht) return 0;
    return get_node_count(ht->left)+get_node_count(ht->right)+1;
}

void preorder(huffman_tree* ht,std::vector<std::tuple<int,int,char>>& tr){
    if(!ht) return;
    tr.push_back({ht->id,ht->tot_freq,ht->char_val});
    preorder(ht->left,tr);
    preorder(ht->right,tr);
}

void inorder(huffman_tree* ht,std::vector<std::tuple<int,int,char>>& tr){
    if(!ht) return;
    inorder(ht->left,tr);
    tr.push_back({ht->id,ht->tot_freq,ht->char_val});
    inorder(ht->right,tr);
}

void write_traversal(std::ofstream& ofile,std::vector<std::tuple<int,int,char>>& traversal){
    for(auto& elm : traversal){
        int first=std::get<0>(elm);
        int second=std::get<1>(elm);
        char third=std::get<2>(elm);
        ofile.write((char*)&first,sizeof(int));
        ofile.write((char*)&second,sizeof(int));
        ofile.write((char*)&third,sizeof(char));
    }
}

void write_binary(std::string& op,huffman_tree* ht,std::ofstream& ofile){
    int padding=get_padding(op);
    std::cout<<"padding is: "<<padding<<std::endl;
    ofile.write((char*)&padding,sizeof(padding));
    int tot_nodes=get_node_count(ht);
    ofile.write((char*)&tot_nodes,sizeof(int));
    std::vector<std::tuple<int,int,char>> traversal;
    preorder(ht,traversal);
    write_traversal(ofile,traversal);
    traversal={};
    inorder(ht,traversal);
    write_traversal(ofile,traversal);

    for(int i=0;i<op.size();i+=8){
        std::string byte=op.substr(i,std::min(8,(int)op.size()-i));
        byte=pad(byte);
        char c=convert_to_dec(byte);
        ofile.write(&c,sizeof(char));
    }
}

void write_to_file(std::string ifilename,std::string ofilename,huffman_tree* ht,std::unordered_map<char,std::string>& codes){
    std::string op="";
    std::ifstream ifile(ifilename);
    std::string data;
    char buf[100000];
    bool first=true;
    while(!ifile.eof()){
        if(!first) op+=codes['\n'];
        else first=false;
        memset(buf,0,100000);
        ifile.getline(buf,100000,'\n');
        std::cout<<buf<<std::endl;
        // int t;
        // std::cin>>t;
        for(int i=0;buf[i]!='\0';i++){
            op+=codes[buf[i]];
        }
    }
    std::cout<<op<<std::endl;
    std::ofstream ofile(ofilename,std::ios::out | std::ios::binary);
    write_binary(op,ht,ofile);
    ofile.close();
}

void print(std::unordered_map<char,int>& mapping){
    for(auto& p : mapping){
        if(p.first==' ') std::cout<<"sp";
        else if(p.first=='\n') std::cout<<"nl";
        else std::cout<<p.first;
        std::cout<<" -> "<<p.second<<std::endl;
    }
}

std::string get_string_bin(unsigned char buf){
    std::string string_bin="";
    // std::cout<<"buf: "<<(unsigned int)buf<<std::endl;
    int ctr=0;
    while(ctr<8){
        if(buf%2==0){
            string_bin="0"+string_bin;
        }
        else{
            string_bin="1"+string_bin;
        }
        buf/=2;
        ctr++;
    }
    return string_bin;
}

char get_next_char(std::string& bin_string,int& ptr,huffman_tree* ht){
    if(!ht->left && !ht->right){
        return ht->char_val;
    }
    if(bin_string[ptr]=='1'){
        ptr++;
        return get_next_char(bin_string,ptr,ht->right);
    }
    ptr++;
    return get_next_char(bin_string,ptr,ht->left);
    
}

void decompress(std::string& bin_string,std::string ofilename,huffman_tree* ht){
    std::ofstream ofile(ofilename,std::ios::out);
    int ptr=0;
    while(ptr<bin_string.size()){
        char c=get_next_char(bin_string,ptr,ht);
        ofile.write(&c,1);
        std::cout<<c;
    }
    ofile.close();
}

std::string read_from_file(std::string ifilename,std::string ofilename){
    std::ifstream ifile(ifilename,std::ios::in | std::ios::binary);
    int padding,tot_nodes;
    std::cout<<"eof: "<<ifile.eof()<<std::endl;
    ifile.read((char*)&padding,sizeof(int));
    ifile.read((char*)&tot_nodes,sizeof(int));
    std::vector<std::tuple<int,int,char>> preorder;
    std::vector<std::tuple<int,int,char>> inorder;
    for(int i=0;i<tot_nodes;i++){
        int id,f;
        char c;
        ifile.read((char*)&id,sizeof(int));
        ifile.read((char*)&f,sizeof(int));
        ifile.read((char*)&c,sizeof(char));
        preorder.push_back({id,f,c});
    }
    for(int i=0;i<tot_nodes;i++){
        int id,f;
        char c;
        ifile.read((char*)&id,sizeof(int));
        ifile.read((char*)&f,sizeof(int));
        ifile.read((char*)&c,sizeof(char));
        inorder.push_back({id,f,c});
    }
    huffman_tree* ht=make_tree_from_traversal(preorder,inorder);
    char buf;
    std::string op="";
    
    // ifile.read(&buf,sizeof(char));

    // std::cout<<"eof: "<<ifile.eof()<<std::endl;

    while(true){
        ifile.read((char*)&buf,sizeof(char));
        if(ifile.eof()) break;
        std::string string_bin=get_string_bin((unsigned char)buf);
        op+=string_bin;
    }
    op=op.substr(0,op.size()-padding);
    decompress(op,ofilename,ht);
    return op;
}
