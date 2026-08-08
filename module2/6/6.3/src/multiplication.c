typedef struct
{
    const char *name;
    double (*func)(double, double);
} FuncInfo;

double opMult(double a, double b) { return a * b; }

FuncInfo getInfo(void)
{
    FuncInfo info = {"Multiplication", opMult};
    return info;
}