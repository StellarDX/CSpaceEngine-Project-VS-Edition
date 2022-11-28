#include <iostream>
#include <fstream>
#include "CSE/Core.h"

using namespace std;
using namespace cse;

CSELog CoreLog;

int main()
{
    // Vector basic output:
    CoreLog.Out("Test-Core", "INFO", "Testing: Vector basic output...");
    vec3 alf = vec3(1, 2, 3);
    cout << alf << '\n';

    // Vector Operations:
    CoreLog.Out("Test-Core", "INFO", "Testing: Vector Operations...");
    vec3 alf1(1, 2, 3);
    vec3 alf2(2, 3, 4);
    vec3 alf3(3, 4, 5);

    vec3 alf0 = (3. / 4.) * alf1 - (7. / 4.) * alf2 + alf3;
    cout << alf0 << '\n';

    // Matrix basic output
    CoreLog.Out("Test-Core", "INFO", "Testing: Matrix basic output...");
    mat4x3 A({ 2,1,0,4,0,1,6,4,2,3,5,3 });
    cout << A << '\n';

    // Matrix Operations
    CoreLog.Out("Test-Core", "INFO", "Testing: Matrix Operations...");
    mat3x2 Ax0({ {1,0},{2,1},{3,-2} });
    mat3x2 Bx0({ {3,5},{4,-2},{2,-0} });
    auto Cx0 = 2. * Ax0 - Bx0;
    auto Zx0 = (1. / 2.) * (Bx0 - Ax0);
    cout << Cx0 << '\n';
    cout << Zx0 << '\n';

    mat2x3 Ax1({ 1,-2,3,-3,2,1 });
    mat2 Bx1({ 4,-6,5,7 });
    auto Cx1 = Ax1 * Bx1;
    cout << Cx1 << '\n';

    mat2 Ax2({ {-2,1},{4,-2} });
    mat2 Bx2({ {2,-3},{4,-6} });
    auto Cx2 = Ax2 * Bx2;
    auto Dx2 = Bx2 * Ax2;
    cout << Cx2 << '\n' << Dx2 << '\n';

    CoreLog.Out("Test-Core", "INFO", "Testing: Matrix Transpose...");
    mat2 Ax3({ {1,3},{2,4} });
    auto Cx3 = Ax3.Transpose();
    cout << Cx3 << '\n';

    CoreLog.Out("Test-Core", "INFO", "Testing: Matrix Inverse...");
    mat3 Ax4({ {1,3,5},{2,4,-4},{-1,-2,1} });
    auto Bx4 = Ax4.Inverse();
    cout << Bx4 << '\n';

    CoreLog.Out("Test-Core", "INFO", "Testing: Matrix Submatrices...");
    matrix<float64, 6, 6> Ax5
    ({
        {1,4,7,0,0,0},
        {2,5,8,0,0,0},
        {3,6,9,0,0,0},
        {0,0,0,10,0,0},
        {0,0,0,0,11,13},
        {0,0,0,0,12,14}
    });
    auto A11 = Ax5.submat<3, 3>(1, 1);
    auto A33 = Ax5.submat<2, 2>(5, 5);
    cout << A11 << '\n' << A33 << '\n';

    CoreLog.Out("Test-Core", "INFO", "Testing: Matrix Ranks...");
    mat3x4 Ax6({ {3,1,4},{2,2,4},{1,-3,-2},{1,2,3} });
    auto RA = Ax6.Rank();
    cout << RA << '\n';

    CoreLog.Out("Test-Core", "INFO", "Testing: Matrix Determinants...");
    mat4 Dx7({ {3,2,0,0},{0,0,1,2},{1,0,4,3},{0,5,1,1} });
    auto DR = Dx7.Determinant();
    cout << DR << '\n';

    matrix<float64, 18, 18> ExMat18
    ({
	    {3974,6313, 377, 635,2788,1786,8396,3680,5642,2496,3145,5107,3385,7295,9901,3911,4744,8357},
	    {1302,2156,1279,2039,4489,3183,3737,4565, 993, 288,6037,2504,7612,4475,6145, 999,1314,7880},
	    {8965,1632,1522,6170,2653,7499,7842,9970,7228,5645, 612,3662,4050,3917,3151,9135,9248,4897},
	    {1258, 376,1387,8177,8982,4917,5484,3851, 294,1492,3314,4058, 470,5849,7445,1566,6565,8256},
	    {6261,6140,4283,8328,2422,6718,6863, 129,2448,3932,8728,7827, 842,5958,3248,4376,5414,8573},
	    {4706,  18,9548,4067,1474,2528,5516,4384,6740,1178,3813,1578,2274,8309,1366, 691, 400,7390},
	    {3952,6913,9632,3374,5863,8283,8817,8469,8294,2842,2232,8617,2657,9608,8367,9310,1819, 474},
	    {5953,4323,7168,4602,3003,1467,3429, 678,7800,1158,7767,4444,7108,1324,2838,9376,6058,7536},
	    {4710,   4,9760,6271,9447, 889,5756,6014,5613,1691,9037,7924,7726,2620,9635,1148,6566,7208},
	    {5306,1440,9163,2783,8130,9994,4723,2433,8619,3865,6828,1486,8123,1707,3240,2663,6195,5029},
	    { 430,4855,2057,8172,9346,6198,7928,1848,2120,5218,2926,3792,2062,3101,7441,8319,4964,6956},
	    {3989,9633,4139,7925,3879,2659,3234,3888,4946,3617,3721,1282,3232,5230,5682,1103,4315,6332},
	    {4111,9639,9524,3093,7300,1435,7551,1933,7163,4610,8040,6807, 811,6519,8591,3913,4703,7059},
	    {6667,6599,6688,6215,7755,9182, 804,5407,8229,2197,4385,7901,7810,6257,3547,5019,8131,6362},
	    {1295,5408,5159,3404,3690,9311,6750,7477,4707,8615,7816,4022,9838,3100,2671,5660,1755, 794},
	    {  68,4351,1621, 559,3929,7877,7609,3443,3646,4854,3597, 337,5345, 660,6533,8093,7558,2377},
	    {9929,9933,9962,2891,8507,3313,4977,4396,6919,5450,6846,9218,6773,3767,9839,2434,4160,6009},
	    {9165,6150,3291,8125,7423,  40,2333,6938,1682,2416,6683,1529,7374,5405,9514,3801,7765,9121}
    });
    float64 Det = ExMat18.Determinant();
    cout << Det << '\n';

    // Generate test report
    ofstream Report("TestReport_GLTypes.txt");
    Report << "Vector basic output: " << alf << '\n';
    Report << "Vector Operations: " << alf << '\n';
    Report << "Matrix basic output: \n" << A << '\n';
    Report << "Matrix Operations Result 1: \n" << Cx0 << '\n';
    Report << "Matrix Operations Result 2: \n" << Zx0 << '\n';
    Report << "Matrix Operations Result 3: \n" << Cx2 << '\n' << Dx2 << '\n';
    Report << "Matrix Transpose: \n" << Cx3 << '\n';
    Report << "Matrix Inverse: \n" << Bx4 << '\n';
    Report << "Matrix Submatrices: \n" << A11 << '\n' << A33 << '\n';
    Report << "Matrix Rank: \n" << RA << '\n';
    Report << "Matrix Determinants Normal: \n" << DR << '\n';
    Report << "Matrix Determinants Extreme: \n" << Det << '\n';
}
