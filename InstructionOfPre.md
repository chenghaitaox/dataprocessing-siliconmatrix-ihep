# 预处理程序操作说明 #
## 文件说明 ##
共包含17个文件：
  1. [Calibration.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/Calibration.cpp)操作刻度数据文件
  1. [CheckCRC.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/CheckCRC.cc)    CRC校验依赖文件
  1. [CleanData.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/CleanData.cpp)  操作清理数据
  1. [CommonData.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/CommonData.cpp) 操作正常模式数据
  1. [DecodeTime.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/DecodeTime.cc)  时间解码依赖文件
  1. [DrawPed.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/DrawPed.cpp)    操作绘制噪声及台阶谱
  1. [EnableCRC.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/EnableCRC.cc)   CRC校验开启依赖文件
  1. [FindPeak.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/FindPeak.cc)    寻峰依赖文件
  1. [FloatCheck.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/FloatCheck.cc)  未连接探测器通道筛选依赖文件
  1. [Identifier.h](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/Identifier.h)   模式依赖头文件
  1. [LinearCal.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/LinearCal.cc)   线性刻度依赖文件
  1. [LoopCRC.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/LoopCRC.cc)    CRC循环依赖文件
  1. [Pedestal.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/Pedestal.cpp)   操作台阶数据
  1. [QuickSort.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/QuickSort.cc)   重新排序依赖文件
  1. [RawData.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/RawData.cpp)    操作原始数据
  1. [ReadRaw.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/ReadRaw.cc)     读取原始数据依赖文件
  1. [WriteRMS.cc](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/WriteRMS.cc)    写入噪声值及阈值依赖文件
所有以 **cpp** 结尾的文件均为可执行文件（linux），其他文件均为依赖文件。
使用过程中，需要将所有文件拷贝至同一目录下。

---

## 台阶及噪声获取 ##
  * 运行(HowToRun) **[Pedestal.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/Pedestal.cpp)**
  * 按照提示，输入文件全名，包括后缀
  * 按照提示，输入是否需要CRC校验，输入 _y_ 或者 _n_
  * 等待数据分析结果
  * 数据分析完成，退出ROOT。
可以看到生成的数据包括：以输入文件名为名称的.root文件和RMSE.txt文件，及1-4.gate文件。

---

## 刻度数据判读 ##
  * 运行 **[Calibration.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/Calibration.cpp)**
  * 按照提示，输入文件全名，包括后缀
  * 按照提示，输入是否需要CRC校验，输入 _y_ 或者 _n_
  * 等待数据分析结果
  * 数据分析完成，会给出通道非线性直方图，退出ROOT。
得到包括：以输入文件名为名称的.root文件和results.pdf文件。

---

## 正常模式获取 ##
  * 运行 **[CommonData.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/CommonData.cpp)**
  * 按照提示，输入文件全名，包括后缀
  * 按照提示，输入是否需要CRC校验，输入 _y_ 或者 _n_
  * 等待数据分析结果
数据分析完成，退出ROOT。得到包括：以输入文件名为名称的.root文件。

---

## 原始数据模式获取 ##
  * 运行 **[RawData.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/RawData.cpp)**
  * 按照提示，输入文件全名，包括后缀
  * 按照提示，输入是否需要CRC校验，输入 _y_ 或者 _n_
  * 等待数据分析结果
  * 数据分析完成，退出ROOT。
得到包括：以输入文件名为名称的.root文件。

---

## 绘制噪声及台阶谱 ##
  * 运行 **[DrawPed.cpp](https://code.google.com/p/dataprocessing-siliconmatrix-ihep/source/browse/trunk/pre/DrawPed.cpp)**
  * 按照提示，输入文件全名，包括后缀
  * 等待数据分析结果
  * 数据分析完毕，给出画面为台阶ADC道数直方图，退出ROOT。
得到包括：results.pdf文件。

---

