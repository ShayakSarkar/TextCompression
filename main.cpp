#include "huffman_tree.cpp"
#include "file_manip.cpp"

void compress(std::string& ifilename,std::string& ofilename){
    std::unordered_map<char,int> freq=make_freq_table(ifilename);
    std::cout<<"freq table made"<<std::endl;
    huffman_tree* ht=make_huffman_tree(freq);
    std::cout<<"huffman tree made"<<std::endl;
    ht->print();
    std::unordered_map<char,std::string> codes=ht->get_codes();
    for(auto p : codes){
        std::cout<<p.first<<" -> "<<p.second<<std::endl;
    }
    write_to_file(ifilename,ofilename,ht,codes);
}

void decompress(std::string& ifilename,std::string& ofilename){
    read_from_file(ifilename,ofilename);
}

int main(int argc,char** argv){
    std::string operation=argv[1];
    if(operation=="--decompress"){
        std::string compressed_filename=argv[2];
        std::string decompressed_filename=argv[3];
        decompress(compressed_filename,decompressed_filename);
    }
    else if(operation=="--compress"){
        std::string ifilename=argv[2];
        std::string compressed_filename=argv[3];
        compress(ifilename,compressed_filename);
    }
    else{
        std::cout<<"invalid operation"<<std::endl;
    }
    // std::string ifilename=argv[1];
    // std::string compressed_filename=argv[2];
    // std::string decompressed_filename=argv[3];
    
    // std::cout<<"input file name: "<<ifilename<<std::endl;
    // std::cout<<"compressed file name: "<<compressed_filename<<std::endl;
    // std::cout<<"decompresssed file name: "<<decompressed_filename<<std::endl;


    // return 1;
    // std::unordered_map<char,int> freq=make_freq_table(ifilename);
    // std::cout<<"freq table made"<<std::endl;
    // huffman_tree* ht=make_huffman_tree(freq);
    // std::cout<<"huffman tree made"<<std::endl;
    // ht->print();
    // std::unordered_map<char,std::string> codes=ht->get_codes();
    // for(auto p : codes){
    //     std::cout<<p.first<<" -> "<<p.second<<std::endl;
    // }
    // write_to_file(ifilename,compressed_filename,ht,codes);
    // std::cout<<"written"<<std::endl;
    // std::string bin_contents=read_from_file(compressed_filename,decompressed_filename);
    // std::cout<<bin_contents<<std::endl;
    // decompress(bin_contents,"Decompressed.txt",ht);
    // read_from_file("Compressed2.bin");
}