### 1. Download the opencv 3.4.0 source code from [opencv](https://opencv.org/)
```shell
wget https://github.com/opencv/opencv/archive/3.4.0.zip
```
### 2. Download application srouce code from github
```shell
git clone git@github.com:ljy980330/NanoPi_Face_sys.git
```
### 3. Compile opencv source code
1. install the cmake first
```shell
apt-get install cmake
```
2. unzip package
```shell
unzip opencv-3.4.0.zip
cd opencv-3.4.0
mkdir build && cd build
```
3. install some depend packages
```shell
// must have
apt-get install build-essential
apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
//nice to have
apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
```
4. compile
```shell
cmake ..
make -j4
make install
sh -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
ldconfig
```
5. install folder description
```shell
include dir : /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
lib dir : /usr/local/lib/
```
### Download & install Qt on Ubuntu
1. [wiki](
https://wiki.qt.io/Install_Qt_5_on_Ubuntu)
2. [install application](
http://download.qt.io/official_releases/qt/5.6/5.6.3/qt-opensource-linux-x64-5.6.3.run)
3. install commond
```shell
chmod +x qt-opensource-linux-x64-5.6.3.run
./ qt-opensource-linux-x64-5.6.3.run
```
4. install OpenGL depend packages
```shell
apt-get install mesa-common-dev
apt-get install libglu1-mesa-dev -y
```
