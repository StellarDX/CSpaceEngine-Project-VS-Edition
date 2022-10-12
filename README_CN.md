[English](README.md)
<div align = "center">
# CSpaceEngine �� ڤ������

��Դ��SpaceEngine��̬���빤���䣬����SE0.990��(Ҳ������֮��)

Э�飺[GPLv2 License](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

</div>
## ����
 - ����GLSL����������
 - ��дSpeceEngine��sc���������ļ�
 - ʱ�����
 - �ǵ�/��ȼ���
 - ������Python�������������
 - ��������
 - ...����Ļ��ڿ�����

### ����GLSL����������
 - 2��3��4ά����ģ��
 - ���������ϲ��޴�С��֧�����Դ������㣬���Ե�����ʹ��

### ��дSpeceEngine��sc���������ļ�
 - ����ʾ����
 - * ��ȡSC�ļ�
```��ȡSC�ļ�
ISCStream is = ParseFile("Catalog.sc");
Object obj = GetSEObject(is, "StarName");
```
 - * д��SC�ļ���֧���Զ������
```д��SC�ļ�
ofstream fout("out.sc");
OSCStream os(fout);
os << obj1 << obj2 << ...;
// os.encod(CP_UTF8);
os.Write();
```

### ʱ����� | �ǵ�/��ȼ���
 - CSE�ṩ��һЩ���㹤�ߣ�
 - * [ʱ��](https://github.com/StellarDX/CSpaceEngine-Project/blob/main/CSE_Core/datetime/JulianCalculator.cpp)
 - * [�ǵ�/���](https://github.com/StellarDX/CSpaceEngine-Project/blob/main/CSE_Core/lumine/StarMagLumCalculator.cpp)

### ��������
 - ������ʵ���ݵ�����������
 - ֧�ֵ������б�
 - * ����
| ģ���� | ���� | ���� | ��׼ֵ |
|:------|:-----|:----|:-----|
| MainSequenceStarModel | ������ | �и� | ������ |
| HPMainSequenceStarModel | ������ | �� | ������ |
| WolfRayetStarModel | �ֶ���-��Ҷ�� | �е� | ������ |
| HPWolfRayetStarModel | �ֶ���-��Ҷ�� | �� | ������ |
| BrownDwarfModel | �ְ��� | �� | ������ |
| HPBrownDwarfModel | �ְ��� | �и� | ������ |
| LowMassSubgiantModel | С�����Ǿ���(0.4-0.9̫������) | �� | ���� |
| MidSizedSubgiantModel | �Ǿ���(0.9-8̫������) | �� | ���� |
| MassiveSubgiantModel | �������Ǿ���(8-12̫������) | �� | ���� |
| RedGiantBrunch | ����Ƿ�֧ | �� | ���� |
| HorizontalBrunch(δ���) | ˮƽ��֧ | �� | �ɱ� |
| RedClumpGiantModel(δ���) | ���Ŵؾ��� | �� | �� |

 - ����ʾ����
```generating
_CSE_Random_Engine<mt19937_64> Engine;
MainSequenceStarModel StarModel("G2V");
object_generator<_CSE_Random_Engine<mt19937_64>, MainSequenceStarModel> gen(Engine, StarModel);
Object Obj = gen();
```

_�춯����ɽ�����Ρ��ĵ����ǣ����������_

## �����������Ҫ�¹��ܣ�
 - ��������޷�������������bug����issues�����ҡ�