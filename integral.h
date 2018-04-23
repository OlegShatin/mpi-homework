#ifndef INTEGRAL_H
#define INTEGRAL_H

/*
Интегрируемая функция */
double f (double x)
{
    x=cos(x*x)*cos(x*x);
    return x;
}




/*
Вычислить интеграл по отрезку [а, Ь] с числом точек
разбиения n методом трапеций. */
double integrate (double a, double b, int n)
{
    double res;/* результат */

    double h;//I* шаг интегрирования *I
    int i;

    h = (b - a)/ n;
    res = 0.5 * (f (a) + f (b)) * h;
    for (i = 1; i < n; i++)
    res += f(a + i*h) * h;

    return res;
}
#endif // INTEGRAL_H
