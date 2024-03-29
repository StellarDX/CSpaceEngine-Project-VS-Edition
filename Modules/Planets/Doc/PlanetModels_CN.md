﻿# CSpaceEngine · 行星

_天动万象，山海化形。荒地生星，璨如烈阳。 ——若陀_

## 行星模型

原本想着做物理模型的，结果后来发现难度实在太大，短期怕是搞不出来了，所以搞了些近似模型先用着。

| 模型名 | 生成范围 | 基准参数 |
|:------|:-----|:----|
| TerrestrialPlanetBase | 岩质行星 | 半径(单位：地球半径) |
| GaseousPlanetBase | 气态行星 | 质量(木星质量) |
| UBox_PlanetBase_Composition(未完成) | 所有行星(物理模型) | 成分(包含铁，硅酸盐，水和氢气) |

## 轨道

 * 一些轨道相关的实现：
```
float64 RocheLimit(const Object* Primary, const Object* Companion, int Mode); // 返回两个天体的洛希极限
float64 HillSphere(const Object* Primary, const Object* Companion); // 返回第二个天体在第一个天体影响下的希尔球
int MakeOrbit(Object* Primary, Object* Companion, Object* ThirdGravSourse, OrbitParam Args); // 创建轨道
shared_ptr<Object> MakeBinary(Object* Primary, Object* Companion, Object* ThirdGravSourse, OrbitParam Args); // 创建双星系统
```

 * TLE数据分析

CSE实现了一个叫TLE的类，用于分析TLE数据并转化为轨道。

示例：
```
TLE TLEData = TLE::ParseString(TLEString); // 从string导入TLE数据

Object Spacecraft =
{
    .Type = "Spacecraft",
    .Name = {"Test Spacecraft"},
    .ParentBody = "Earth",

    .Orbit = TLEData.Orbit() // 将轨道数据赋值给Object
};
```

## 小行星带生成器C++版

基于JackDole的Python版本转写，引入一些“真实”的数学模型。

 * Python版本链接：[SE Forum](https://forum.spaceengine.org/viewtopic.php?f=3&t=114#p1126)
 * 源码：[Here](https://github.com/StellarDX/CSpaceEngine-Project/blob/main/Applications/AsterBeltCreator/AsterBeltCreator.cpp)