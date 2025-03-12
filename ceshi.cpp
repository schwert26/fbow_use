#include "DBoW3.h"//词袋支持头文件
#include <opencv2/core/core.hpp>//opencv核心模块
#include <opencv2/highgui/highgui.hpp>//gui模块
#include <opencv2/features2d/features2d.hpp>//特征点头文件
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h> 
#include <pybind11/numpy.h>
#include <boost/filesystem.hpp>
#include <sstream>
#include <dirent.h>

namespace py = pybind11;
namespace fs = boost::filesystem;
using namespace cv;
using namespace std;

bool cmp(string file1,string file2)
{
    file1.erase(file1.end()-4,file1.end());
    file2.erase(file2.end()-4,file2.end());
    //file1.erase(file1.begin(),file1.begin()+6);
    //file2.erase(file2.begin(),file2.begin()+6);
    return atoi(file1.c_str())<atoi(file2.c_str());
}
vector<string> getFiles(string dirc){
    vector<string> files;
    struct dirent *ptr;
    char base[1000];
    DIR *dir;
    dir = opendir(dirc.c_str());
    /*
    if(dir == NULL){
        perror("open dir error ...");
        exit(1);
    }*/
    while((ptr = readdir(dir)) != NULL){
        if(ptr->d_type == 8)//it;s file
        {
            files.push_back(ptr->d_name);
        }

        else if(ptr->d_type == 10)//link file
            continue;
        else if(ptr->d_type == 4){
            continue;
        }
    }
    closedir(dir);
    sort(files.begin(),files.end(),cmp);
    return files;
}

int main( int argc, char** argv )
{
    // read the image
    cout<<"reading images... "<<endl;//输出reading images...
    vector<Mat> images; //图像
    vector<Mat> descriptors;//描述子
    vector<string> result1 = getFiles("/home/lijiahao/vocabulary/tupian");
    for(size_t i = 0; i < result1.size();++i)
    {
        ostringstream stringStream1;
        stringStream1 << result1[i];
        cv::Mat image = cv::imread("/home/lijiahao/桌面/jiahao/test2/"+stringStream1.str(),cv::IMREAD_COLOR);
        cout<<i<<endl;//输出done
        
    }

    //cout<<"creating vocabulary ... "<<endl;//输出(creating vocabulary ...)创建字典
    //DBoW3::Vocabulary vocab;//默认构造函数 k=10,d=5
    //vocab.create( descriptors );
    //cout<<"vocabulary info: "<<vocab<<endl;//字典信息
    //vocab.save( "vocabulary.yml.gz" );//保存字典压缩包
    //cout<<"done"<<endl;//输出done

    return 0;
}