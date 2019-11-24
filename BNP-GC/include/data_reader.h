#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <limits>
using namespace std;

#define INFINITE std::numeric_limits<double>::infinity();

class Data
{
public:
    Data(int, char *);
    ~Data();

    void readData();
    void printMatrixDist();
    inline int getDimension() { return dimension; };
    inline double getDistance(int i, int j) { return distMatrix[i][j]; };
    inline double **getMatrixCost() { return distMatrix; }
    inline double getXCoord(int i) { return xCoord[i]; }
    inline double getYCoord(int i) { return yCoord[i]; }
    inline bool getExplicitCoord() { return explicitCoord; };

    string getInstanceName(); //Get instance's name

private:
    string instaceName;
    int nbOfPar;

    int dimension;

    double **distMatrix;
    double *xCoord, *yCoord;

    //Computing Distances
    static double CalcDistEuc(double *, double *, int, int);
    static double CalcDistAtt(double *, double *, int, int);
    static double CalcDistGeo(double *, double *, int, int);
    static void CalcLatLong(double *, double *, int, double *, double *);

    bool explicitCoord;
};

#endif // DATA_H
