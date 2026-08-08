typedef struct
{
    const char *name;
    double (*func)(double, double);
} FuncInfo;

double opSub(double a, double b) { return a - b; }

FuncInfo getInfo(void)
{
    FuncInfo info = {"Substraction", opSub};
    return info;
}