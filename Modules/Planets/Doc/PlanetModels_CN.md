# CSpaceEngine · 行星

_天动万象，山海化形。荒地生星，璨如烈阳。 ——若陀_

## 行星模型

原本想着做物理模型的，结果后来发现难度实在太大，短期怕是搞不出来了，所以搞了些近似模型先用着。

| 模型名 | 生成范围 | 基准参数 |
|:------|:-----|:----|
| TerrestrialPlanetBase | 岩质行星 | 半径(单位：地球半径) |
| GaseousPlanetBase | 气态行星 | 质量(木星质量) |
| UBox_PlanetBase_Composition(未完成) | 所有行星(物理模型) | 成分(包含铁，硅酸盐，水和氢气) |

## 轨道

一些轨道相关的实现：
```
float64 RocheLimit(const Object* Primary, const Object* Companion, int Mode); // 返回两个天体的洛希极限
float64 HillSphere(const Object* Primary, const Object* Companion); // 返回第二个天体在第一个天体影响下的希尔球
int MakeOrbit(Object* Primary, Object* Companion, Object* ThirdGravSourse, OrbitParam Args); // 创建轨道
shared_ptr<Object> MakeBinary(Object* Primary, Object* Companion, Object* ThirdGravSourse, OrbitParam Args); // 创建双星系统
```