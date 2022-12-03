# CSpaceEngine 恒星

这一部分存放与恒星有关的一些实现

## 恒星模型

支持的天体列表：

| 模型名 | 生成范围 | 精度 | 基准参数 |
|:------|:-----|:----|:-----|
| MainSequenceStarModel | 主序星 | 中高 | 光谱型 |
| HPMainSequenceStarModel | 主序星 | 高 | 光谱型 |
| RedGiantBranch | 红巨星分支 | 低 | 质量 |
| HorizontalBranch | 水平分支 | 低 | 可变 |
| RedClumpGiantModel | 红团簇巨星 | 中 | 无 |
| AsymptoticGiantBranch | 渐近巨星分支 | 中低 | 质量 |
| SuperAsymptoticGiantBranch | 渐近巨星分支(大质量恒星) | 低 | 质量 |
| CoolSubdwarfModel | 次矮星 | 中 | 光谱型 |
| SubdwarfOStarModel | O型次矮星 | 中 | 无 |
| SubdwarfBStarModel | B型次矮星 | 中高 | 无 |
| WolfRayetStarModel | 沃尔夫-拉叶星 | 中低 | 光谱型 |
| HPWolfRayetStarModel | 沃尔夫-拉叶星 | 高 | 光谱型 |
| LowMassSubgiantModel | 小质量亚巨星(0.4-0.9太阳质量) | 低 | 质量 |
| MidSizedSubgiantModel | 亚巨星(0.9-8太阳质量) | 低 | 质量 |
| MassiveSubgiantModel | 大质量亚巨星(8-12太阳质量) | 低 | 质量 |
| BlueGiantModel | 蓝巨星 | 中 | 光谱型 |
| SuperGiantModel | 超巨星 | 中 | 光谱型 |
| HyperGiantModel | 特超巨星 | 中 | 光谱型 |
| CarbonStarModel | 碳星，S型恒星 | 中 | 光谱型 |
| WhiteDwarfModel | 白矮星 | 中高 | 光谱型 |
| NeutronStarModel | 中子星 | 高 | 无 |
| BrownDwarfModel | 褐矮星 | 中 | 光谱型 |
| HPBrownDwarfModel | 褐矮星 | 中高 | 光谱型 |

## 内置函数：

```
Object RandomMainSequenceStar();
```
随机生成一颗主序星，概率如下：

| 光谱型 | 基础概率的负对数[-log(P)] |
|:-------|:--------|
| O | 6.4771212547 |
| B | 2.9030899870 |
| A | 2.2041199826 |
| F | 1.5185139399 |
| G | 1.1139433523 |
| K | 0.9208187540 |
| M | 0.1191864077 |

```
Object RandomOBStar();
```
由于随机主序星生成OB型恒星的概率极低，所以添加这个函数，必出OB恒星

```
Object RandomBlueSupergiant();
Object RandomRedSupergiant();
Object RandomYellowSupergiant();
```
随机生成对应分类的超巨星

## 关于黑洞

注：至于为何没有黑洞，是因为黑洞无毛理论认为，黑洞的性质只取决于三个物理量：质量(Mass)，自转(KerrSpin)和电荷(KerrCharge)，所以这边打算把黑洞单独实现，不归类于恒星模型。