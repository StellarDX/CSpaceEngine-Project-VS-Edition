[English](README.md)
<div align = "center">

# CSpaceEngine · 见证伟业

开源的SpaceEngine静态库与工具箱，兼容SE0.990。(也是造星之法)

协议：[GPLv2 License](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

</div>

## 置顶通知：
本工程已重构，[点此](https://github.com/StellarDX/CSpaceEngine-Project)进入新仓库。

## 功能
 - 来自GLSL的数据类型
 - 读写SpeceEngine的sc天体数据文件
 - 时间计算
 - 星等/光度计算
 - 类似于Python的随机数生成器
 - 天体生成
 - 轨道计算
 - ...更多的还在开发中

### 来自GLSL的数据类型
 - 2，3，4维向量模板
 - 矩阵，理论上不限大小，支持线性代数计算，可以当容器使用

### 读写SpeceEngine的sc天体数据文件
 - 代码示例：

读取SC文件
```读取SC文件
ISCStream is = ParseFile("Catalog.sc");
Object obj = GetSEObject(is, "StarName");
```

写入SC文件，支持自定义编码
```写入SC文件
ofstream fout("out.sc");
OSCStream os(fout);
os << obj1 << obj2 << ...;
// os.encod(CP_UTF8);
os.Write();
```

### 时间计算 | 星等/光度计算
 - CSE提供了一些计算工具：
 - * [时间](Applications/JulianConvert/JulianCalculator.cpp)
 - * [星等/光度](Applications/StarMagLumCalculator/StarMagLumCalculator.cpp)

### 天体生成
 - 基于真实数据的天体生成器，详见：
 - * 恒星库：[CSE_Stars](Modules/Stars/Doc/StarModels_CN.md)
 - * 行星库：[CSE_Planets](Modules/Planets/Doc/PlanetModels_CN.md)

 - 代码示例：
```generating
_CSE_Random_Engine<mt19937_64> Engine;
MainSequenceStarModel StarModel("G2V");
object_generator<_CSE_Random_Engine<mt19937_64>, MainSequenceStarModel> gen(Engine, StarModel);
Object Obj = gen();
```

## 编译和安装
打开工程以后在Target分类下找到ALL_BUILD项目，右键生成以编译。编译以后可以在Libraries目录下找到编译好的库。如果要把所有的库合并起来，找到INSTALL项目，生成即可。

## 遇到问题或想要新功能？
 - 如果发现无法解决的问题或者bug，在issues告诉我。