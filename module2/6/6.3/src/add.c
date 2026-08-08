
typedef struct
{
    const char *name;
    double (*func)(double, double);
} FuncInfo;

double opSum(double a, double b) { return a + b; }

FuncInfo getInfo(void)
{
    FuncInfo info = {"Add", opSum};
    return info;
}