# fbow_use

一个简单使用fbow的聚类demo,如何使用pytorch写的深度学习模型完成聚类 
```c
1.在python端完成描述子的添加，导入为.npy格式  
2.安装opencv,我这边用的是4.2.0，  
3.安装fbow   
```cpp
git clone https://github.com/rmsalinas/fbow.git  
mkdir build 
cd build
cmake ..
make -j
```
4.进入fbow_use,进行编译,这边cmakelist里面fbow换成自己的
```cpp
mkdir build 
cd build
cmake ..
make -j
```
5.测试demo，里面是数据路径改成自己的
```cpp
cd /build
./ceshi /home/lijiahao/vocabulary/demo2/output/endopoint.fbow
```
