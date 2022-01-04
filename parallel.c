#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <omp.h>

#define COLUMNS 10
#define ROWS 50000

void generate_data(double ptr[COLUMNS][ROWS]);
void print_data(double ptr[COLUMNS][ROWS]);
double column_mean(double column[]);
void print_mean(double data[COLUMNS][ROWS]);
double column_std(double column[]);
void print_std(double data[COLUMNS][ROWS]);
void *normalize_column(void *arg);
void normalize_all(double data[COLUMNS][ROWS]);

int main()
{
    // #pragma omp parallel shared (argv, argc ) private (i,id)
    // {
    //     tid=omp_get_thread_num();

    // }
    double data[COLUMNS][ROWS];
    generate_data(data);
    pthread_t thread_id[COLUMNS];
    for (int i = 0; i < COLUMNS; i++)
    {
        pthread_create(thread_id + i, NULL, normalize_column, (void *)data[i]);
    }
    for (int i = 0; i < COLUMNS; i++)
    {
        pthread_join(*(thread_id + i), NULL);
    }
    print_data(data);

    return 0;
}

void generate_data(double ptr[COLUMNS][ROWS])
{
    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            ptr[i][j] = i * ROWS * 3.14159 + j;
        }
    }
}

void print_data(double ptr[COLUMNS][ROWS])
{
    printf("first twenty rows: \n\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            printf("%lf\t", ptr[j][i]);
        }
        printf("\n");
    }
    printf("....\n");
}

double column_mean(double column[])
{
    double sum = 0;
    double temp = 77;
    for (int i = 0; i < ROWS; i++)
    {
        sum += column[i];
    }
    return (sum / ROWS);
}
double column_std(double column[])
{
    double sum = 0;
    double temp = 77;
    for (int i = 0; i < ROWS; i++)
    {
        sum += sqrt((column[i] - column_mean(column)) * (column[i] - column_mean(column)));
    }
    return (sum / ROWS);
}
void print_mean(double data[COLUMNS][ROWS])
{
    for (int i = 0; i < COLUMNS; i++)
    {
        printf("%lf \t", column_mean(*data + i));
    }
    printf("\n");
}
void print_std(double data[COLUMNS][ROWS])
{
    for (int i = 0; i < COLUMNS; i++)
    {
        printf("%lf \t", column_std(*data + i));
    }
    printf("\n");
}

void *normalize_column(void *arg)
{
    double *column = (double *)arg;
    double mean = column_mean(column);
    double std = column_std(column);
    for (int i = 0; i < ROWS; i++)
    {
        column[i] -= mean;
        column[i] /= std;
    }
}

void normalize_all(double data[COLUMNS][ROWS])
{
    for (int i = 0; i < COLUMNS; i++)
    {
        normalize_column(data[i]);
    }
}