# EasyPR-DLL-CSharp

### 待完善
### Need to be complete!
* 下载OpenCV3.1.0，解压
* 新建系统变量OPENCV为：D:\OpenCV310\opencv\build
* 添加环境变量： %OPENCV%\x64\vc12\bin
* 配置“CreateDLL”项目属性：

项目 | 内容
---------------------|--------------------------------------------------------------
C/C++-附加包含目录    | [opencv的include目录]; \CreateDLL\main_file; \CoreCode\include
链接器-附加库目录     | [opencv的lib目录]
链接器-输入-附加依赖项 | opencv_world310d.lib;%(AdditionalDependencies)

* 配置“LibLPS”项目属性：

项目 | 内容
---------------------|--------------------------------------------------------------
C/C++-附加包含目录    | [opencv的include目录];$(SolutionDir)include; \CoreCode\include

* 重新生成“CreateDLL”项目

