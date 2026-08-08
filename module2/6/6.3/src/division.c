#include <math.h>

typedef struct
{
    const char *name;
    double (*func)(double, double);
} FuncInfo;

double opDiv(double a, double b)
{

    if (!b)
    {
        return NAN;
    }
    return a / b;
}

FuncInfo getInfo(void)
{
    FuncInfo info = {"Division", opDiv};
    return info;
}