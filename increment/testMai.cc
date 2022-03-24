#include <iostream>
#include <vector>
#include <cmath>
#include "mai.h"

using namespace std;

int main() {
  //g4m::mai maiV3({.8, 0., 0., 0., .75, 0.004609729, -32.90496, 0.6081886, 1.719932, 1.323597, 0.003, 50., 0.1291932, 0.17115},std::valarray<double>(10., 24),std::valarray<double>(70., 24),std::valarray<double>(180., 24),100,0,0.038,0,50,30,0,7,.8,0,std::valarray<double>({0.35}),std::valarray<double>({0.}),std::valarray<double>({std::numeric_limits<double>::infinity()}),std::valarray<double>({0.}),std::valarray<double>({std::numeric_limits<double>::infinity()}),std::valarray<double>({0.}),std::valarray<double>({std::numeric_limits<double>::infinity()}),std::valarray<double>({0.}),std::valarray<double>({std::numeric_limits<double>::infinity()}),std::valarray<double>({0.}),false);
  g4m::mai maiV3;
  maiV3.setSwr(0.8);
  maiV3.setcNpp2mai({1./3.});

  double BITT = 99.; //Boundary Increase Tropical Temperature

  //lc1er1 Nadel-Evergreen-Tropical
  maiV3.setCoef(0, {0.8, 0., 0., 0., 0.75, 0.996, -20., 0.648, 0.00544, 5., 0.0178, 2.57, 0.003, 50., 0.1, 0.25, -0.1, 0.702, -0.1, 0.705, 0.03, 2., -3.71, -1.74});
  maiV3.setBoundaries(0,4.3 + 13.8,29.1 + 2.2 + BITT,200. - 0,6900. + 4000.,-3.3 + 14.6,33.5 + 2.3,0,1300 + 2000,0.64);
  //lc1er2 Nadel-Evergreen-Subtropical
  maiV3.setCoef(1, {0.8, 0., 0., 0., 0.75, 1.63, -51.1, 0.614, 0.00543, 1.9, 0.0129, 1.83, 0.003, 50., 0.1, 0.25, -0.1, 0.514, -0.4, 10., 0.02, 4., -3.22, -0.923});
  maiV3.setBoundaries(1,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);
//lc1er3 Nadel-Evergreen-Temperate
  maiV3.setCoef(2, {0.8, 0., 0., 0., 0.75, 0.734, -26., 0.743, 0.00363, 0.335, 0.022, 3.86, 0.003, 50., 0.1, 0.25, -0.179, 0.504, -0.1, 0.5, 0.02, 2.59, -3.36, -0.883});
  maiV3.setBoundaries(2,-4.9 + 5,14.6 - 2. + 3,220. - 0,6900. + 4000.,-27.2 + 0.,25.1 + 4.5,0,1300 + 2000,1.18);
//lc1er4 Nadel-Evergreen-Boreal
  maiV3.setCoef(3, {0.8, 0., 0., 0., 0.75, 1.29, -21.1, 0.588, 0.00308, -0.839, 0.0264, 5.91, 0.003, 50., 0.1, 0.25, -0.4, 0.584, -0.1, 0.5, 0.02, 2., -2.01, -0.518});
  maiV3.setBoundaries(3,-10.3 - 8,6.6 - 6.,175. - 0,6900. + 4000.,-39.9 - 8.,19.0 + 2.,2-2,1300 + 2000,0.53);
//lc2er1 Laub-Evergreen-Tropical
  maiV3.setCoef(4, {0.8, 0., 0., 0., 0.75, 0.881, -20., 0.615, 0.00476, 2.99, 0.0194, 3.67, 0.003, 50., 0.1, 0.25, -0.244, 0.5, -0.297, 1.24, 0.0243, 2.01, -2.08, -0.692});
  maiV3.setBoundaries(4,10.1 + 8.,28.3 + 3. + BITT,600. - 0,6900. + 4000.,7.2 + 4.1,33.6 + 2.2,0,1300 + 2000,4.6);
//lc2er2 Laub-Evergreen-Subtropical
  maiV3.setCoef(5, {0.8, 0., 0., 0., 0.75, .9 * 1.09, -42.8, 0.67, 0.00517, 2., 0.0179, 2.9, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.4, 4.65, 0.03, 2., -3.95, -1.39});
  maiV3.setBoundaries(5,3.4 + 9,23.7 - 5. + 2.5,290. - 0,6900. + 4000.,-5.8 + 7.,29.9 + 2.,0,1300 + 2000,0.85);
//lc2er3 Laub-Evergreen-Temperate
  maiV3.setCoef(6, {0.8, 0., 0., 0., 0.75, .3 * 0.975, -37.8, 0.724, 0.00439, -0.0408, 0.0207, 3.55, 0.003, 50., 0.1, 0.25, -0.4, 0.5, -0.327, 0.5, 0.02, 2., -3.33, -1.12});
  maiV3.setBoundaries(6,-5.8 + 5.9,15.9 - 3.3 + 2.,330. - 0,6900. + 4000.,-25.7 + 0.,25.1 - 0.,1,1300 + 2000,3.18);
//lc2er4 Laub-Evergreen-Boreal
  maiV3.setCoef(7, {0.8, 0., 0., 0., 0.75, .3 * 1.72, -41.5, 0.482, 0.00607, -1.47, 0.0322, 6.74, 0.003, 50., 0.1, 0.402, -0.4, 0.5, -0.1, 0.723, 0.02, 4., -2.89, -0.793});
  maiV3.setBoundaries(7,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);
//lc3er1 Nadel-Deciduous-Tropical
  maiV3.setCoef(8, {0.8, 0., 0., 0., 0.75, 2.75, -20., 0.595, 0.01, 5., 0.0137, 1.23, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.1, 2.26, 0.03, 4., -1.5, -1.08});
  maiV3.setBoundaries(8,8.3 + 9.8 - 3,28.5 + 1. + BITT,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
//lc3er2 Nadel-Deciduous-Subtropical
  maiV3.setCoef(9, {0.8, 0., 0., 0., 0.75, 0.43, -63.7, 0.722, 0.00454, 4.78, 0.0199, 3.36, 0.003, 50., 0.1, 0.25, -0.1, 0.581, -0.1, 0.5, 0.02, 2., -3.91, -1.03});
  maiV3.setBoundaries(9,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);
//lc3er3 Nadel-Deciduous-Temperate
  maiV3.setCoef(10, {0.8, 0., 0., 0., 0.75, 0.811, -25.3, 0.729, 0.00343, -0.728, 0.0206, 4.01, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.1, 0.648, 0.03, 2., -2.98, -0.778});
  maiV3.setBoundaries(10,-9.0 + 9.1,15.4 - 2.8,100. + 150,6900. + 4000.,-32.6 + 0,27.9 - 0.,1,1300 + 2000,0.19);
//lc3er4 Nadel-Deciduous-Boreal
  maiV3.setCoef(11, {0.8, 0., 0., 0., 0.75, 0.876, -22.6, 0.663, 0.00383, -0.947, 0.0279, 5.29, 0.003, 50., 0.1, 0.25, -0.4, 0.5, -0.4, 0.5, 0.02, 2., -3.81, -0.939});
  maiV3.setBoundaries(11,-10.3 - 8,6.6 - 6.,215. - 0,6900. + 4000.,-39.9 - 8.,19.6 + 2.,2-2,1300 + 2000,1.17);
//lc4er1 Laub-Deciduous-Tropical
  maiV3.setCoef(12, {0.8, 0., 0., 0., 0.75, 1.13, -20., 0.618, 0.00461, 2.56, 0.0199, 3.54, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.4, 1.57, 0.02, 2., -3.54, -1.16});
  maiV3.setBoundaries(12,8.3 + 9.8 - 3,28.5 + 1. + BITT,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
//lc4er2 Laub-Deciduous-Subtropical
  maiV3.setCoef(13, {0.8, 0., 0., 0., 0.75, 1.22, -44.7, 0.659, 0.0027, 0.631, 0.0204, 4.32, 0.003, 50., 0.1, 0.276, -0.1, 0.5, -0.263, 2.81, 0.02, 2.93, -3.75, -0.723});
  maiV3.setBoundaries(13,1.9 + 10.5,23.8 - 5.1,190. - 0,6900. + 4000.,-10.5 + 11.7,29.9 - 0.,1,1300 + 2000,0.15);
//lc4er3 Laub-Deciduosu-Temperate
  maiV3.setCoef(14, {0.8, 0., 0., 0., 0.75, 0.876, -23.4, 0.753, 0.00307, -2.24, 0.0215, 4.46, 0.003, 50., 0.1, 0.48, -0.195, 0.817, -0.4, 3.62, 0.02, 2., -1.5, -0.546});
  maiV3.setBoundaries(14,-4.9 + 5,14.6 - 2. + 3,305. - 0,6900. + 4000.,-29.7 + 0.,27.1 + 2.5,2,1300 + 2000,1.74);
  //lc4er4 Laub-Deciduous-Boreal
  maiV3.setCoef(15, {0.8, 0., 0., 0., 0.75, 1.11, -21.4, 0.574, 0.00433, -1.71, 0.0292, 6.16, 0.003, 50., 0.1, 0.276, -0.315, 0.612, -0.1, 0.5, 0.03, 2., -1.5, -0.5});
  maiV3.setBoundaries(15,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);
//lc8er1 woody savannas-Tropical
  maiV3.setCoef(16, {0.8, 0., 0., 0., 0.75, 1.78, -20., 0.439, 0.00517, 5., 0.0195, 3.21, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.1, 0.5, 0.02, 2., -2.63, -0.647});
  maiV3.setBoundaries(16, 18., 33. + BITT, 170., 11000., 9., 37., 0., 4000., .2);
//lc8er2 woody savannas-Subtropical
  maiV3.setCoef(17, {0.8, 0., 0., 0., 0.75, 2.05, -53.4, 0.476, 0.0053, -0.578, 0.0177, 3.01, 0.003, 50., 0.1, 0.25, -0.16, 0.767, -0.1, 0.5, 0.03, 2., -3.74, -0.943});
  maiV3.setBoundaries(17, 12., 19., 170., 11000., 1., 32., 0., 4000., .2);
//lc8er3 woody savannas-Temperate
  maiV3.setCoef(18, {0.8, 0., 0., 0., 0.75, 1.05, -26.5, 0.698, 0.00311, -1.12, 0.0183, 3.63, 0.003, 50., 0.1, 0.303, -0.146, 0.5, -0.1, 0.5, 0.02, 2.34, -3.53, -0.868});
  maiV3.setBoundaries(18, 0., 13., 170., 11000., -33., 30., 0., 4000., .2);
//lc8er4 woody savannas-Boreal
  maiV3.setCoef(19, {0.8, 0., 0., 0., 0.75, 1.02, -20., 0.597, 0.00427, -1.52, 0.0276, 5.46, 0.003, 50., 0.1, 0.25, -0.383, 0.5, -0.1, 0.615, 0.03, 2., -3.52, -0.719});
  maiV3.setBoundaries(19, -19., 1., 170., 11000., -50., 23., 0., 4000, .2);


  
  g4m::mai maiV2({1.95866e-11,16.6774,5.30899,300,7.00127,0.0828,0.180158,50,0.002,0.85,20, 2, 0.5, 0.841, -22.131},{1.054684,1.099922,1.075196,0.980570,1.002155,1.044522,1.134524,1.073864,1.000548,1.070339,1.068615,1.086483,1.054495,1.036821,1.095323,1.008207,1.094867,1.031270,0.987843,1.035130,0.950606,1.074587,1.008381},std::valarray<double>(10., 24),std::valarray<double>(70., 24),std::valarray<double>(180., 24),100,0,0.038,0,0,0,std::valarray<double>({0.35}),std::valarray<double>({0.}),std::valarray<double>({std::numeric_limits<double>::infinity()}),std::valarray<double>({0.}),std::valarray<double>({std::numeric_limits<double>::infinity()}),std::valarray<double>({0.}),std::valarray<double>({std::numeric_limits<double>::infinity()}),std::valarray<double>({0.}),std::valarray<double>({std::numeric_limits<double>::infinity()}),std::valarray<double>({0.}),false);
  maiV2.setSwr(0.8);
  maiV2.setcNpp2mai({1./3.});

  //lc1er1 Nadel-Evergreen-Tropical
  maiV2.setCoef(0, {6.94357e-06,8,1.15427,300,2.38,0.025,0.8,50,0.002,0.85,20,0,0,0,0});
  maiV2.setCoefC0(0, {1.167273,1.181957,1.148051,1.144072,1.057347,1.183415,0.839456,0.998758,1.066749,0.998820,0.888920,1.132417,1.070195,1.146386,1.444292,1.338450,0.926202,1.164307,1.482320,1.161192,1.119969,1.010927,1.126816}, 0.7, 1.3);
  maiV2.setBoundaries(0,4.3 + 13.8,29.1 + 2.2,200. - 0,6900. + 4000.,-3.3 + 14.6,33.5 + 2.3,0,1300 + 2000,0.64);

  //lc2er1 Laub-Evergreen-Tropical
maiV2.setCoef(1, {6.97374e-07,8,2.08263,300,2.36,0.025,0.6,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(1, {1.027236,0.930810,1.026614,1.058107,1.044301,1.088828,1.284784,0.997171,1.030403,0.930521,0.917611,1.071603,1.167654,1.063758,1.260789,1.139207,0.999577,0.966556,1.161625,0.990053,1.105658,1.056113,1.107417}, 0.7, 1.3);
maiV2.setBoundaries(1,10.1 + 8.,28.3 + 3.,600. - 0,6900. + 4000.,7.2 + 4.1,33.6 + 2.2,0,1300 + 2000,4.6);

  //lc3er1 Nadel-Deciduous-Tropical
maiV2.setCoef(2, {4.98896e-09,4,3.3326,300,3,0.04,0.8,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(2, {1.25067,1.30666,1.42140,1.37749,1.19130,1.15079,0.42130,1.41935,1.35999,1.15866,1.22622,1.17439,1.46707,1.43959,1.89361,1.66058,1.01811,1.55586,1.65774,1.17218,1.36042,1.39463,1.66056}, 0.7, 1.3);
 maiV2.setBoundaries(2,8.3 + 9.8 - 3,28.5 + 1.,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
 
  //lc4er1 Laub-Deciduous-Tropical
maiV2.setCoef(3, {3.17454e-07,8,1.98803,300,1.82,0.025,0.42,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(3, {1.014203,0.655291,0.932896,1.681138,0.909431,0.724058,1.170861,0.730898,0.584047,0.946718,0.868518,0.866705,0.973624,1.075178,2.262815,0.867673,0.700617,0.794007,1.981404,1.001099,1.352024,1.284358,1.225052}, 0.7, 1.3);
 maiV2.setBoundaries(3,8.3 + 9.8 - 3,28.5 + 1.,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
  
  //lc1er2 Nadel-Evergreen-Subtropical
maiV2.setCoef(4, {2.98529e-07,8,2.52942,300,5,0.025,0.8,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(4, {1.084565,1.258374,1.544269,1.208804,1.048047,1.152335,0.798060,1.058165,1.172564,1.078057,1.483471,1.160240,1.351295,1.353240,1.784506,1.224317,1.034672,1.308062,1.714973,1.010345,1.210056,1.215814,0.994749}, 0.7, 1.3);
maiV2.setBoundaries(4,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);

  //lc2er2 Laub-Evergreen-Subtropical
maiV2.setCoef(5, {.5*3.68661e-07,8,2.4796,300,3.4,0.025,0.6,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(5, {1.0392105,0.9697758,1.1182245,1.0074064,0.9719184,0.9856556,0.8268068,0.9808688,1.0001164,0.9635926,1.0681479,1.0059678,1.2294432,1.0322276,1.0641367,0.9296392,1.0646439,1.1399712,1.0061634,1.1424225,0.9144326,1.1622985,1.0073817}, 0.7, 1.3);
maiV2.setBoundaries(5,3.4 + 9,23.7 - 5. + 2.5,290. - 0,6900. + 4000.,-5.8 + 7.,29.9 + 2.,0,1300 + 2000,0.85);

  //lc3er2 Nadel-Deciduous-Subtropical
maiV2.setCoef(6, {2.78094e-06,4,1.46148,300,3,0.04,0.8,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(6, {1.33731,1.21174,2.09552,1.45462,1.11848,1.78982,0.44737,1.14455,1.33598,1.16703,1.59218,1.89613,1.54779,1.33450,1.61385,1.23386,1.15095,1.48598,1.50806,1.13044,1.04776,1.27060,1.15392}, 0.7, 1.3);
maiV2.setBoundaries(6,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);
 
  //lc4er2 Laub-Deciduous-Subtropical
maiV2.setCoef(7, {5.22354e-08,3.81171,2.94545,300,3.18175,0.0232828,0.809172,50,0.002,0.851749,20,0,0,0,0});
maiV2.setCoefC0(7, {0.983435,1.216336,1.208377,1.164035,1.127213,1.427934,0.427887,1.041255,0.899436,0.951187,1.505357,1.606139,1.512237,1.196615,1.284658,1.063652,1.121445,1.179302,1.375756,0.836196,1.106857,1.427041,1.055856}, 0.7, 1.3);
maiV2.setBoundaries(7,1.9 + 10.5,23.8 - 5.1,190. - 0,6900. + 4000.,-10.5 + 11.7,29.9 - 0.,1,1300 + 2000,0.15);

  //lc1er3 Nadel-Evergreen-Temperate
maiV2.setCoef(8, {1.95866e-11,16.6774,5.30899,300,7.00127,0.0828,0.180158,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(8, {1.054684,1.099922,1.075196,0.980570,1.002155,1.044522,1.134524,1.073864,1.000548,1.070339,1.068615,1.086483,1.054495,1.036821,1.095323,1.008207,1.094867,1.031270,0.987843,1.035130,0.950606,1.074587,1.008381}, 0.7, 1.3);
 maiV2.setBoundaries(8,-4.9 + 5,14.6 - 2. + 3,220. - 0,6900. + 4000.,-27.2 + 0.,25.1 + 4.5,0,1300 + 2000,1.18);

  //lc2er3 Laub-Evergreen-Temperate
maiV2.setCoef(9, {3.15e-09,9.35918,3.66485,300,4.64592,0.06,0.26,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(9, {1.0118956,1.0615462,1.0779899,1.0120382,0.9627487,0.9898199,1.0791497,1.0310722,0.9754276,0.8438813,1.1233655,1.1068356,1.0901310,1.0101304,1.0565290,1.0779955,1.0621784,1.0007144,0.9295641,1.0472222,1.0161400,1.0173837,1.0171552}, 0.7, 1.3);
maiV2.setBoundaries(9,-5.8 + 5.9,15.9 - 3.3 + 2.,330. - 0,6900. + 4000.,-25.7 + 0.,25.1 - 0.,1,1300 + 2000,3.18);

  //lc3er3 Nadel-Deciduous-Temperate
maiV2.setCoef(10, {1.50349e-07,7.07274,2.54163,300,4.00133,0.06,0.25,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(10, {0.862190,1.145883,1.135764,0.950486,1.102457,1.030473,0.956814,0.975896,1.170444,1.003162,1.169161,1.053181,1.126635,1.232963,0.864202,1.179097,0.774995,0.995233,1.321486,1.032954,1.041349,1.029158,1.056834}, 0.7, 1.3);
maiV2.setBoundaries(10,-9.0 + 9.1,15.4 - 2.8,100. + 150,6900. + 4000.,-32.6 + 0,27.9 - 0.,1,1300 + 2000,0.19);

  //lc4er3 Laub-Deciduous-Temperate
maiV2.setCoef(11, {6.30082e-09,9.35918,3.66485,300,4.64592,0.06,0.26,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(11, {0.8426849,1.1336508,0.9219486,1.0428545,1.1223109,0.8339236,1.0641686,0.5452793,1.0681361,1.0280533,1.0013517,0.6224940,1.2335694,1.1491972,1.0465842,0.9879109,1.1978397,0.9430111,1.0909037,1.1027924,1.0124663,1.2082930,1.0756028}, 0.7, 1.3);
maiV2.setBoundaries(11,-4.9 + 5,14.6 - 2. + 3,305. - 0,6900. + 4000.,-29.7 + 0.,27.1 + 2.5,2,1300 + 2000,1.74);

  //lc1er4 Nadel-Evergreen-Boreal
maiV2.setCoef(12, {1.36116e-07,6.7432,2.82955,300,4.57307,0.06,0.50,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(12, {1.035657,1.036384,1.032846,1.030142,1.055238,1.052846,1.045243,1.024852,1.002326,0.998242,1.081536,1.108310,1.013428,1.036258,1.054235,1.041855,1.031564,1.027304,1.115472,1.011239,1.016438,1.006963,1.013799}, 0.7, 1.3);
maiV2.setBoundaries(12,-10.3 - 8,6.6 - 6.,175. - 0,6900. + 4000.,-39.9 - 8.,19.0 + 2.,2-2,1300 + 2000,0.53);
 
  //lc2er4 Laub-Evergreen-Boreal
maiV2.setCoef(13, {6.94485e-11,10.3926,5.18179,300,8.21249,0.06,0.2,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(13, {0.93566,1.04938,0.99955,1.17027,0.92923,0.90336,1.00938,1.02759,0.95376,0.90328,1.00736,0.99617,1.14578,1.09325,1.25368,1.11511,0.92586,0.99376,1.10367,0.90914,0.96348,0.94270,1.41604}, 0.7, 1.3);
maiV2.setBoundaries(13,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);
 
  //lc3er4 Nadel-Deciduous-Boreal
maiV2.setCoef(14, {7.54653e-07,3.39758,2.39893,300,5.10519,0.06,0.59,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(14, {0.997382,1.164807,1.137406,1.157535,1.025785,1.016309,0.922417,1.078434,1.033414,0.960146,1.018151,1.064526,1.083873,1.061930,1.108677,1.053499,1.025711,0.942647,1.277620,1.014716,1.114794,1.009307,1.115872}, 0.7, 1.3);
maiV2.setBoundaries(14,-10.3 - 8,6.6 - 6.,215. - 0,6900. + 4000.,-39.9 - 8.,19.6 + 2.,2-2,1300 + 2000,1.17);

  //lc3er4 Laub-Deciduous-Boreal
maiV2.setCoef(15, {1.38897e-10,10.3926,5.18179,300,8.21249,0.06,0.2,50,0.002,0.85,20,0,0,0,0});
maiV2.setCoefC0(15, {0.816807,1.171890,1.106970,1.007987,0.960848,0.923770,1.029693,1.114420,1.059554,1.006887,1.168652,0.913977,1.082629,1.532929,1.053006,1.072970,0.989127,0.994642,1.297751,0.959316,1.233419,0.953433,1.041302}, 0.7, 1.3);
maiV2.setBoundaries(15,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);


 
 g4m::mai maiV1({2, 0.5, 0.841, -22.131},{1.29460152,-0.09012495,0.17332495, 35, -1, 0.66170523, 2.8, 0.4138984, -54.4741443, -1.4, 1.155907e+00, 2.154028e-04, -3.733458e-01, 2.335792e-05},{0.06179233,0.06448844,0.07000044,0.07867775,0.06506758,0.08137664,0.06192571,0.07169721,0.07110523,0.06381677,0.05441309,0.06347873,0.07584091,0.07330926,0.05766713,0.07205265,0.05055277,0.06077571,0.07759581,0.05685617,0.06527024,0.05558023,0.06699292},std::valarray<double>(10., 24),std::valarray<double>(70., 24),100,0,0.038,0,0.,0.,0.8,0);
  maiV1.setSwr(0.8);
  maiV1.setcNpp2mai({1./3.});

  //Forest
  maiV1.setCoef(0, {1.29460152,-0.09012495,0.17332495, 35, -1, 0.66170523, 2.8, 0.4138984, -54.4741443, -1.4, 1.155907e+00, 2.154028e-04, -3.733458e-01, 2.335792e-05});
  maiV1.setCoefC0(0, {0.06179233,0.06448844,0.07000044,0.07867775,0.06506758,0.08137664,0.06192571,0.07169721,0.07110523,0.06381677,0.05441309,0.06347873,0.07584091,0.07330926,0.05766713,0.07205265,0.05055277,0.06077571,0.07759581,0.05685617,0.06527024,0.05558023,0.06699292});
  maiV1.setBoundaries(0, -12.0 - 2, 28.0 + 2., 203., 5900. + 4000., -43.1 - 4, 32.5 + 5., 0, 1045 + 1100, 1.86);
  //evergreen needleleaf forest
  maiV1.setCoef(1, {1.473547e+00,-9.469511e-02,1.855580e-01,3.500000e+01,-1.000000e+00,1.018910e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.349661e-01,1.698924e-04,-4.095052e-02,-1.261350e-04});
  maiV1.setCoefC0(1, {0.07200521,0.11960568,0.09408846,0.10690904,0.08503689,0.09664446,0.08917135,0.12555602,0.10778141,0.08308198,0.11212895,0.11297067,0.11626721,0.10273018,0.12039956,0.11695393,0.06977949,0.10038106,0.14010804,0.08497696,0.12136300,0.10823336,0.10055720});
  maiV1.setBoundaries(1, -10.1 - 4, 28.0 + 2., 176., 4100. + 4000., -39.1 - 7, 29.2 + 5., 1, 562 + 2000, 0.79);
  //evergreen broadleaf forest
  maiV1.setCoef(2, {1.529964e+00,-9.626790e-02,1.892103e-01,3.500000e+01,-1.000000e+00,1.065803e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,2.006863e+00,-2.252521e-05,-1.190847e+00,3.509442e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(2, {0.06278559,0.05451733,0.07155252,0.07154880,0.06331560,0.07405309,0.05890702,0.06341198,0.06168992,0.06258061,0.04919143,0.05810440,0.08030827,0.07632525,0.05871569,0.06240980,0.05162692,0.05707812,0.07945510,0.05807941,0.06134032,0.05378656,0.06523997});
  maiV1.setBoundaries(2, 4.1 + 7,28.0 + 2.5,480. - 100,6900. + 4000.,-3.6 + 3.,33.5 + 2.5,0,1300 + 2000,2.98);
  //deciduous needleleaf forest
  maiV1.setCoef(3, {1.575326e+00,-9.890705e-02,1.966154e-01,3.500000e+01,-1.000000e+00,1.361797e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.640303e-01,2.381869e-04,-5.982080e-02,-5.153793e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(3, {0.1226366,0.1394199,0.1373671,0.1393298,0.1215172,0.1293482,0.1245032,0.1393599,0.1337010,0.1171449,0.1229379,0.1227410,0.1484251,0.1228124,0.1469167,0.1251016,0.1210288,0.1285934,0.1523116,0.1243452,0.1535025,0.1284694,0.1011582});
  maiV1.setBoundaries(3,-14.1 - 2.3,18.6 + 2.5,200. - 0,6900. + 4000.,-45. - 3.,28. + 5.5,2,1300 + 2000,1.19);
  //deciduous broadleaved forest
  maiV1.setCoef(4, {1.494767e+00,-9.632663e-02,1.886172e-01,3.500000e+01,-1.000000e+00,3.584477e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.107488e+00,1.957089e-04,-5.395287e-01,1.181403e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(4, {0.08172423,0.12588981,0.07748284,0.11167975,0.07765499,0.10551991,0.08170487,0.06883372,0.09889102,0.09630474,0.08749524,0.07827634,0.15207574,0.08388147,0.12978135,0.09060370,0.08727573,0.10390721,0.14516723,0.09127422,0.07513953,0.10896841,0.11420354});
  maiV1.setBoundaries(4, -8.2 - 2.3,28.3 + 1.,235. - 0,6900. + 4000.,-32.9 - 9.,28. + 5.5,0,1300 + 2000,0.66);
  //Tropical evergreen broadleaf
  maiV1.setCoef(5, {1.619920e+00,-9.848997e-02,1.970912e-01,3.500000e+01,-1.000000e+00,5.963311e-01,2.800000e+00,0.4138984,-54.4741443,-1.4,1.941317e+00,-9.634048e-05,-1.100476e+00,4.401370e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(5, {0.06136063,0.05394037,0.06884074,0.06795948,0.06069805,0.07048979,0.05777229,0.06212813,0.05874720,0.06193419,0.04811436,0.05803750,0.07435434,0.07529665,0.05663168,0.06045755,0.04999882,0.05534675,0.07479968,0.05449085,0.05932120,0.05244837,0.06286373});
  maiV1.setBoundaries(5,10.1 + 8.,28.3 + 3.,600. - 0,6900. + 4000.,7.2 + 4.1,33.6 + 2.2,0,1300 + 2000,4.6);
  //Tropical deciduous broadleaf
  maiV1.setCoef(6, {1.491055e+00,-9.734815e-02,1.951930e-01,3.500000e+01,-1.000000e+00,2.144010e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.018970e+00,5.690775e-04,-2.743858e-01,-3.154196e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(6, {0.06836858,0.06813469,0.05934084,0.07794042,0.05072694,0.06129334,0.06565496,0.04773324,0.07122714,0.08419983,0.05885219,0.06289404,0.09646647,0.06666715,0.09231248,0.03816949,0.04651217,0.07239042,0.10244473,0.07365863,0.05660622,0.08033533,0.07028939});
  maiV1.setBoundaries(6,8.3 + 9.8 - 3,28.5 + 1.,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
   //Subtropical evergreen broadleaf
    maiV1.setCoef(7, {1.538287e+00,-9.722637e-02,1.894521e-01,3.500000e+01,-1.000000e+00,1.249044e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.656060e+00,2.328051e-04,-1.522926e+00,1.101988e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(7, {0.11606259,0.09230848,0.11152179,0.07736606,0.10174771,0.10232312,0.09956373,0.09869678,0.12651927,0.11027624,0.09064032,0.14575220,0.09281512,0.09487696,0.13310920,0.08328069,0.10088982,0.12739049,0.14853313,0.12082426,0.10963328,0.16620110,0.10230552});
  maiV1.setBoundaries(7,3.4 + 9,23.7 - 5. + 2.5,290. - 0,6900. + 4000.,-5.8 + 7.,29.9 + 2.,0,1300 + 2000,0.85);
  //Temperate evergreen needleleaf
  maiV1.setCoef(8, {1.570108e+00,-9.829125e-02,1.945205e-01,3.500000e+01,-1.000000e+00,1.416748e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.220346e-01,1.322653e-04,1.418027e-01,-1.572138e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(8, {0.1177171,0.1308500,0.1070914,0.1288103,0.1519428,0.1280058,0.1521247,0.1562641,0.1294695,0.1486603,0.1318565,0.1337459,0.1723263,0.1428401,0.1618081,0.1389939,0.1357501,0.1216093,0.1503008,0.1142445,0.1521230,0.1520602,0.1590204});
  maiV1.setBoundaries(8,-4.9 + 5,14.6 - 2. + 3,220. - 0,6900. + 4000.,-27.2 + 0.,25.1 + 4.5,0,1300 + 2000,1.18);
  //Temperate deciduous broadleaf
  maiV1.setCoef(9, {1.574017e+00,-9.893621e-02,1.965476e-01,3.500000e+01,-1.000000e+00,1.949731e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.597373e-01,3.182178e-04,-5.993809e-01,-2.135231e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(9, {0.12156712,0.12492062,0.13087684,0.11666581,0.11019982,0.12730928,0.12320618,0.07018051,0.11312888,0.09925496,0.10062111,0.12766330,0.15216701,0.11568652,0.13923021,0.09930708,0.14071709,0.11279223
	,0.11843090,0.12642849,0.08836875,0.12722117,0.10910369});
  maiV1.setBoundaries(9,-4.9 + 5,14.6 - 2. + 3,305. - 0,6900. + 4000.,-29.7 + 0.,27.1 + 2.5,2,1300 + 2000,1.74);
  //Boreal evergreen needleleaf
  maiV1.setCoef(10, {1.591897e+00,-9.941671e-02,1.978756e-01,3.500000e+01,-1.000000e+00,1.454123e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.807388e-01,2.934434e-04,-9.318789e-02,1.280086e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(10, {0.1218631,0.1295781,0.1280500,0.1356488,0.1362415,0.1322454,0.1237829,0.1503324,0.1172669,0.1551760,0.1293965,0.1360983,0.1388701,0.1233824,0.1500347,0.1331320,0.1210412,0.1245973,0.1598617,0.1205537,0.1492726,0.1203824,0.1453270});
  maiV1.setBoundaries(10,-10.3 - 8,6.6 - 6.,175. - 0,6900. + 4000.,-39.9 - 8.,19.0 + 2.,2-2,1300 + 2000,0.53);
  //Boreal deciduous needleleaf
  maiV1.setCoef(11, {1.588311e+00,-9.953446e-02,1.982941e-01,3.500000e+01,-1.000000e+00,1.535554e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,7.624086e-01,3.815986e-04,-2.984578e-01,3.871615e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(11, {0.1346323,0.1450399,0.1434999,0.1450294,0.1312006,0.1314684,0.1292051,0.1472063,0.1373973,0.1556409,0.1264541,0.1303976,0.1576328,0.1361593,0.1508253,0.1275356,0.1355128,0.1356315,0.1637595,0.1318753,0.1610309,0.1345712,0.1568916});
  maiV1.setBoundaries(11,-10.3 - 8,6.6 - 6.,215. - 0,6900. + 4000.,-39.9 - 8.,19.6 + 2.,2-2,1300 + 2000,1.17);
  //Tropical evergreen needleleaf
  maiV1.setCoef(12, {1.197497e+00,-8.738827e-02,1.742595e-01,3.500000e+01,-1.000000e+00,6.502581e-01,2.800000e+00,0.4138984,-54.4741443,-1.4,1.223739e+00,-3.062217e-05,-3.844974e-01,2.294290e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(12, {0.03992456,0.03344327,0.04377474,0.03088140,0.02940730,0.05420901,0.04138151,0.03681954,0.03061374,0.03588355,0.03004470,0.03692928,0.04303532,0.04515249,0.03687710,0.04236888,0.02213864,0.06108095,0.03835771,0.02997647,0.03281500,0.03696203,0.04425476});
  maiV1.setBoundaries(12,4.3 + 13.8,29.1 + 2.2,200. - 0,6900. + 4000.,-3.3 + 14.6,33.5 + 2.3,0,1300 + 2000,0.64);
  //Subtropical evergreen needleleaf
  maiV1.setCoef(13, {1.528454e+00,-9.328062e-02,1.778416e-01,3.500000e+01,-1.000000e+00,1.416668e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.225557e+00,1.253244e-04,-9.790134e-01,1.069189e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(13, {0.08771526,0.06546871,0.14893432,0.08151210,0.05439819,0.08062922,0.06700593,0.06470260,0.09436382,0.06005037,0.08061899,0.10209852,0.06744991,0.11864577,0.10419921,0.07418190,0.05797453,0.10102862,0.21474297,0.05752559,0.08622142,0.10598824,0.05646660});
  maiV1.setBoundaries(13,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);
  //Subtropical deciduous broadleaf forest
  maiV1.setCoef(14, {1.583149e+00,-9.801282e-02,1.927008e-01,3.500000e+01,-1.000000e+00,1.152546e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.414094e+00,1.465791e-05,-1.312110e+00,5.351233e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(14, {0.07556472,0.05721778,0.08186703,0.06351753,0.07113803,0.08661813,0.06621690,0.06202732,0.09156333,0.05902771,0.07786552,0.10107472,0.09343677,0.06269478,0.08833595,0.06189770,0.08025677,0.09015629,0.12211850,0.06349837,0.07095126,0.08909001,0.06878965});
  maiV1.setBoundaries(14,1.9 + 10.5,23.8 - 5.1,190. - 0,6900. + 4000.,-10.5 + 11.7,29.9 - 0.,1,1300 + 2000,0.15);
  //Temperate evergreen broadleaf
  maiV1.setCoef(15, {1.580856e+00,-9.891061e-02,1.960383e-01,3.500000e+01,-1.000000e+00,2.521807e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.793388e-01,4.410353e-04,-6.640318e-02,-6.262073e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(15, {0.1084333,0.1069007,0.10839835,0.096417,0.1008877,0.1177522,0.1262071,0.09862685,0.11904175,0.0982001,0.0988278,0.1212168,0.12735525,0.11331315,0.11384885,0.0930662,0.1060646,0.10694685,0.1309429,0.1054626,0.12388035,0.11623495,0.1098665});
  maiV1.setBoundaries(15,-5.8 + 5.9,15.9 - 3.3 + 2.,330. - 0,6900. + 4000.,-25.7 + 0.,25.1 - 0.,1,1300 + 2000,3.18);
  //Temperate deciduous needleleaf
  maiV1.setCoef(16, {1.542234e+00,-9.740093e-02,1.922796e-01,3.500000e+01,-1.000000e+00,1.721792e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.450042e-01,2.795868e-04,-2.644673e-01,-9.114047e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(16, {0.11462020,0.14112211,0.14168577,0.09944337,0.09829958,0.12095412,0.10770765,0.08238588,0.11642098,0.08694502,0.09464438,0.11746427,0.15485947,0.08529750,0.14770957,0.10658592,0.12643711,0.09626534,0.11686801,0.11183171,0.09877526,0.12544831,0.09643012});
  maiV1.setBoundaries(16,-9.0 + 9.1,15.4 - 2.8,100. + 150,6900. + 4000.,-32.6 + 0,27.9 - 0.,1,1300 + 2000,0.19);
  //Boreal deciduous broadleaf
  maiV1.setCoef(17, {1.580021e+00,-9.891136e-02,1.961251e-01,3.500000e+01,-1.000000e+00,3.816509e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.219060e-01,1.584236e-04,-3.908123e-01,7.472023e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(17, {0.152224375,0.161903495,0.162378475,0.14314884,0.134649945,0.13786099,0.13044117,0.13519165,0.1512354,0.16078957,0.13138093,0.122985055,0.208138565,0.16509652,0.161235395,0.123882995,0.14395243,0.135572025,0.204168385,0.148326445,0.16027056,0.130733315,0.172622675});
  maiV1.setBoundaries(17,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);
  //euPinusSylvestris
  maiV1.setCoef(18, {1.594495e+00,-9.970772e-02,1.989457e-01,3.500000e+01,-1.000000e+00,2.900761e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.494779e-01,3.309943e-04,-3.836232e-01,7.078381e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(18, {0.1207741,0.1284495,0.1119152,0.1268292,0.1589158,0.1265268,0.1503300,0.1175510,0.1229715,0.1239201,0.1397111,0.1482649,0.1876775,0.1165897,0.1837231,0.1170574,0.1364137,0.1222335,0.1846275,0.1198684,0.1397678,0.1418467,0.1367707});
  maiV1.setBoundaries(18,-2.1,16.1,422,3000.,-15.8,25.3,5,400,2.36);
  //euPiceaAbies
  maiV1.setCoef(19, {1.592195e+00,-9.952615e-02,1.984377e-01,3.500000e+01,-1.000000e+00,5.058505e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,6.799446e-01,3.591072e-04,-6.739802e-01,8.385973e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(19, {0.1794001,0.1891808,0.1533205,0.1762452,0.1999378,0.2355349,0.1821450,0.1958970,0.1793914,0.1797653,0.2065453,0.1827384,0.2898600,0.1948900,0.2261093,0.1593018,0.1913847,0.2079411,0.2694490,0.1856048,0.2042948,0.1803902,0.2540367});
  maiV1.setBoundaries(19,-1.6,16.8,438,3000.,-15.6,25.3,25,400,1.39);
  //euFagusSylvatice
  maiV1.setCoef(20, {1.594232e+00,-9.968908e-02,1.990604e-01,3.500000e+01,-1.000000e+00,2.648344e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,5.331431e-01,4.369507e-04,-1.211302e+00,4.866968e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(20, {0.1272625,0.1370621,0.1375981,0.1502073,0.1370428,0.1392780,0.1517152,0.1561900,0.1478397,0.1598463,0.1519745,0.1569613,0.1974999,0.1381542,0.1919486,0.1416385,0.1572946,0.1669360,0.1977402,0.1500649,0.1572065,0.1579615,0.1594094});
  maiV1.setBoundaries(20,1.6 + 4.,16.7,389,3000.,-8.5 + 1,26.2,20,400,3.72);
  //euPinusPinaster
  maiV1.setCoef(21, {1.594612e+00,-9.972969e-02,1.988237e-01,3.500000e+01,-1.000000e+00,1.469745e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.379329e-01,8.155537e-04,-1.486089e-01,-3.983407e-03,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(21, {0.09728270,0.10831447,0.12124217,0.09071147,0.08161405,0.09897297,0.10858003,0.07446026,0.09442338,0.08711192,0.07474872,0.08281703,0.13107504,0.10532173,0.10599763,0.08877993,0.08999591,0.08280230,0.13986681,0.07536053,0.09880739,0.09069319,0.09606415});
  maiV1.setBoundaries(21,5.5 + 5,17.3,373,3000.,-1.3 + 3,26.2,3,400,2.79);
  //euAbiesAlba
  maiV1.setCoef(22, {1.555171e+00,-9.932687e-02,1.979182e-01,3.500000e+01,-1.000000e+00,2.040056e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.108940e-01,2.491341e-04,-1.103119e-01,-4.295811e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(22, {0.14777350,0.15048855,0.20410069,0.10166814,0.21977505,0.11141147,0.08796520,0.08977260,0.12067930,0.14464023,0.14432563,0.21735400,0.17989385,0.12323347,0.14592808,0.08893861,0.16532699,0.17645573,0.17876554,0.18757352,0.15062610,0.14638657,0.14874606});
  maiV1.setBoundaries(22,1.6,16.8,379,3000.,-7.5,25.8,20,400,2.51);
  //euOak
  maiV1.setCoef(23, {1.597726e+00,-9.988829e-02,1.996781e-01,3.500000e+01,-1.000000e+00,2.830689e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,7.620456e-01,3.334448e-04,-1.357640e+00,1.191982e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(23, {0.1165066,0.1204457,0.1220312,0.1196470,0.1390695,0.1480787,0.1184926,0.1046163,0.1438569,0.1256989,0.1210813,0.1365279,0.1677675,0.1241299,0.1747276,0.1287212,0.1163627,0.1416925,0.1892422,0.1099974,0.1368449,0.1479013,0.1333442});
  maiV1.setBoundaries(23,4.2,16.7,391-30,3000.,-8.4,25.7,15,400,3.77);
  //euBetulaPendula
  maiV1.setCoef(24, {1.595563e+00,-9.971459e-02,1.991162e-01,3.500000e+01,-1.000000e+00,4.408279e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.095339e-01,1.480798e-04,-4.059949e-02,-2.817819e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiV1.setCoefC0(24, {0.1585352,0.1791702,0.1456907,0.1627288,0.1767568,0.1793059,0.1678409,0.1450082,0.1665453,0.2132190,0.1415539,0.1917198,0.2829671,0.2034887,0.1929536,0.1500426,0.1630552,0.1636004,0.2562288,0.2461736,0.1877926,0.1706541,0.1949842});
  maiV1.setBoundaries(24,-2.9,16.2,378,3000.,-16.6,25.9,15,400,1.67);

 
  std::valarray<double> t = {-5.5, -5.1, -1., 4, 9.4, 14.1, 16.1, 15.1, 11., 6., 5., -3.5};
  t += +0.5;
  std::valarray<double> p = {52,43,45,47,57,71,76,75,66,63,64,60};
  p *= 1.2;
  std::valarray<double> r = std::valarray<double>(180., 12);

  maiV3.setTemperature(t);
  maiV3.setPrecipitation(p);
  maiV3.setRadiation(r);
  maiV3.setCo2(380);
  maiV3.setSwr(0.);
  maiV3.setWhc(150.);
  maiV3.setAltitude(400.);
  maiV3.setN(50.);
  maiV3.setP(30.);
  maiV3.setS(0.);
  maiV3.setpH(7.);

  {
    std::valarray<double> rmai = maiV3.getMai(maiV3.testBoundaries(), true);
    for(double m : rmai) {cout << m << " ";}
    cout << endl;
  }
  
  maiV2.setTemperature(t);
  maiV2.setPrecipitation(p);
  maiV2.setRadiation(r);
  maiV2.setCo2(0.038);
  maiV2.setSwr(0.);
  maiV2.setWhc(150.);
  maiV2.setAltitude(400.);
  maiV2.setSoilType(3);

  {
    std::valarray<double> rmai = maiV2.getMai(maiV2.testBoundaries(), true);
    for(double m : rmai) {cout << m << " ";}
    cout << endl;
  }

  maiV1.setTemperature(t);
  maiV1.setPrecipitation(p);
  maiV1.setCo2(0.038);
  maiV1.setSwr(0.);
  maiV1.setWhc(150.);
  maiV1.setAltitude(400.);
  maiV1.setSoilType(3);
  maiV1.setLatitude(45.*M_PI/180.);

  {
    std::valarray<double> rmai = maiV1.getMai(maiV1.testBoundaries(), true);
    for(double m : rmai) {cout << m << " ";}
    cout << endl;
  }
 
  return(0);
}
