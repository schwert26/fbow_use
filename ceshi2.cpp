//Second step,creates the vocabulary from the set of features. It can be slow
#include <iostream>
#include <fstream>
#include <vector>
#include "vocabulary_creator.h"
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include "vocabulary_creator.h"
#include "fbow_exports.h"
#include "cnpy.h"
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using namespace cv;
using namespace std;

class CmdLineParser{int argc; char **argv; public: CmdLineParser(int _argc,char **_argv):argc(_argc),argv(_argv){}  bool operator[] ( string param ) {int idx=-1;  for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i;    return ( idx!=-1 ) ;    } string operator()(string param,string defvalue="-1"){int idx=-1;    for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i; if ( idx==-1 ) return defvalue;   else  return ( argv[  idx+1] ); }};

void load_descriptors_from_npy(const std::string& npy_file, std::vector<cv::Mat>& descriptors) {
    try {
        // 加载 .npy 文件
        cnpy::NpyArray arr = cnpy::npy_load(npy_file);
        // 获取数据指针（假设存储的是 float）
        float* data = arr.data<float>();
        // 获取形状 (N, D)
        size_t N = arr.shape[0];  // 行数
        size_t D = arr.shape[1];  // 列数 (假设为 64)

        // 检查列数是否为 64
        if (D != 64) {
            std::cerr << "错误：文件 " << npy_file << " 的列数应为 64，但实际为 " << D << std::endl;
            return;
        }

        // 将数据转换为 cv::Mat 并追加到 descriptors
        for (size_t i = 0; i < N; i++) {
            cv::Mat descriptor(1, D, CV_32F);
            for (size_t j = 0; j < D; j++) {
                descriptor.at<float>(0, j) = data[i * D + j];
            }
            descriptors.push_back(descriptor);
        }
        std::cout << "从 " << npy_file << " 加载了 " << N << " 个描述子" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "加载 " << npy_file << " 时出错: " << e.what() << std::endl;
    }
}
int main(int argc,char **argv)
{

    try{
        CmdLineParser cml(argc,argv);
        if (cml["-h"] || argc<2){
            cerr<<"Usage:  features output.fbow [-k k] [-l L] [-t nthreads] [-maxIters <int>:0 default] [-v verbose on]. "<<endl;
            cerr<<"Creates the vocabylary of k^L"<<endl;
            cerr<<"By default, we employ a random selection center without runnning a single iteration of the k means.\n"
                  "As indicated by the authors of the flann library in their paper, the result is not very different from using k-means, but speed is much better\n";
            return -1;
        }
        std::vector<cv::Mat> descriptors;
        // 加载 endopoint.npy
        load_descriptors_from_npy("/media/lijiahao/T9/数据集/endopoint.npy", descriptors);

        // 加载 endopointothers.npy
        load_descriptors_from_npy("/media/lijiahao/T9/数据集/endopointothers.npy", descriptors);
        

        // 输出第一行的描述子
        std::cout << "总共加载了 " << descriptors.size() << " 个描述子" << std::endl;
        std::cout << "First descriptor: " << descriptors[0] << std::endl;
        string desc_name="endopoint";

        cout<<"DescName="<<desc_name<<endl;
        fbow::VocabularyCreator::Params params;
        params.k = stoi(cml("-k","10"));
        params.L = stoi(cml("-l","6"));
        params.nthreads=stoi(cml("-t","1"));
        params.maxIters=std::stoi (cml("-maxIters","0"));
        params.verbose=cml["-v"];
        srand(0);
        fbow::VocabularyCreator voc_creator;
        fbow::Vocabulary voc;
        cout << "Creating a " << params.k << "^" << params.L << " vocabulary..." << endl;
        auto t_start=std::chrono::high_resolution_clock::now();
        voc_creator.create(voc,descriptors,desc_name, params);
        auto t_end=std::chrono::high_resolution_clock::now();
        cout<<"time="<<double(std::chrono::duration_cast<std::chrono::milliseconds>(t_end-t_start).count())<<" msecs"<<endl;
        cout<<"nblocks="<<voc.size()<<endl;
        cerr<<"Saving "<<argv[1]<<endl;
        voc.saveToFile(argv[1]);


    }catch(std::exception &ex){
        cerr<<ex.what()<<endl;
    }

    return 0;
}
