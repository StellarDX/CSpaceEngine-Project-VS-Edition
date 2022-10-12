[English](README.md)
<div align = "center">

# CSpaceEngine · 冥古显生

开源的SpaceEngine静态库与工具箱，兼容SE0.990。(也是造星之法)

协议：[GPLv2 License](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

</div>

## 功能
 - 来自GLSL的数据类型
 - 读写SpeceEngine的sc天体数据文件
 - 时间计算
 - 星等/光度计算
 - 类似于Python的随机数生成器
 - 天体生成
 - ...更多的还在开发中

### 来自GLSL的数据类型
 - 2，3，4维向量模板
 - 矩阵，理论上不限大小，支持线性代数计算，可以当容器使用

### 读写SpeceEngine的sc天体数据文件
 - 代码示例：
 - * 读取SC文件
```读取SC文件
ISCStream is = ParseFile("Catalog.sc");
Object obj = GetSEObject(is, "StarName");
```
 - * 写入SC文件，支持自定义编码
```写入SC文件
ofstream fout("out.sc");
OSCStream os(fout);
os << obj1 << obj2 << ...;
// os.encod(CP_UTF8);
os.Write();
```

### 时间计算 | 星等/光度计算
 - CSE提供了一些计算工具：
 - * [时间](https://github.com/StellarDX/CSpaceEngine-Project/blob/main/CSE_Core/datetime/JulianCalculator.cpp)
 - * [星等/光度](https://github.com/StellarDX/CSpaceEngine-Project/blob/main/CSE_Core/lumine/StarMagLumCalculator.cpp)

### 天体生成
 - 基于真实数据的天体生成器
 - 支持的天体列表：
 - * 恒星

| 模型名 | 类型 | 精度 | 基准值 |
|:------|:-----|:----|:-----|
| MainSequenceStarModel | 主序星 | 中高 | 光谱型 |
| HPMainSequenceStarModel | 主序星 | 高 | 光谱型 |
| WolfRayetStarModel | 沃尔夫-拉叶星 | 中低 | 光谱型 |
| HPWolfRayetStarModel | 沃尔夫-拉叶星 | 高 | 光谱型 |
| BrownDwarfModel | 褐矮星 | 中 | 光谱型 |
| HPBrownDwarfModel | 褐矮星 | 中高 | 光谱型 |
| LowMassSubgiantModel | 小质量亚巨星(0.4-0.9太阳质量) | 低 | 质量 |
| MidSizedSubgiantModel | 亚巨星(0.9-8太阳质量) | 低 | 质量 |
| MassiveSubgiantModel | 大质量亚巨星(8-12太阳质量) | 低 | 质量 |
| RedGiantBrunch | 红巨星分支 | 低 | 质量 |
| HorizontalBrunch(未完成) | 水平分支 | 低 | 可变 |
| RedClumpGiantModel(未完成) | 红团簇巨星 | 低 | 无 |

 - 代码示例：
```generating
_CSE_Random_Engine<mt19937_64> Engine;
MainSequenceStarModel StarModel("G2V");
object_generator<_CSE_Random_Engine<mt19937_64>, MainSequenceStarModel> gen(Engine, StarModel);
Object Obj = gen();
```

_天动万象，山海化形。荒地生星，璨若烈阳。_

## 遇到问题或想要新功能？
 - 如果发现无法解决的问题或者bug，在issues告诉我。