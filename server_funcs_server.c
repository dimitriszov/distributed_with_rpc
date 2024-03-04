/************************************************************/
/*         Εργαστήριο Κατανεμημένα Συστήματα                */
/* Ονοματεπώνυμο: Ζωβοϊλης Δημήτριος-Μάριος                 */
/* ΑΜ: 19390064                                             */
/* Το αρχείο αυτό αποτελεί την υλοποίηση του RPC server     */
/************************************************************/
#include "server_funcs.h"


int* findMaxMin(int arr[], int N);
double* multArrDouble(int arr[], int N, double a);


/**********************************/
/*         RPC functions          */
/**********************************/

/*
 * RPC function for calculating max
 * and min of an int array
 */
double *
avg_1_svc(int_array *argp, struct svc_req *rqstp)
{
    static double  result;
    int len = argp->arr.arr_len;
    int sum = 0;

    /*
     * find the sum of the elements
     */
    for(int i=0;i<len;i++)
    {
        sum += argp->arr.arr_val[i];
    }
    result = (double)sum/len; // calculate avg
    return &result;
}


/*
 * RPC function for calculating max
 * and min of an int array
 */
intpair *
max_min_1_svc(int_array *argp, struct svc_req *rqstp)
{
    static intpair result;
    int *max_min;
    int len = argp->arr.arr_len;

    /*
     * calculate max and min calling findMaxMin
     */
    max_min = findMaxMin(argp->arr.arr_val, len);
    result.num1 = max_min[0];
    result.num2 = max_min[1];

    return &result;
}


/*
 * RPC function for calculating the product
 * of an array multiplied by a double
 */
double_array *
mult_1_svc(mult_array *argp, struct svc_req *rqstp)
{
    static double_array  result;
    int len = argp->arr.arr_len;
    double a = argp->num;
    
    result.arr.arr_len = len;
    result.arr.arr_val = multArrDouble(argp->arr.arr_val, len, a);

    return &result;
}


/**********************************/
/*        useful functions        */
/**********************************/

/*
 * Function to find the minimum and
 * maximum element of an array
 */
int* findMaxMin(int arr[], int N)
{
    int i;
    int* max_min;
    // variable to store the minimum
    // and maximum element
    int min = arr[0], max = arr[0];
    // Traverse the given array
    for (i = 0; i < N; i++)
    {
        // If current element is smaller
        // than min then update it
        if (arr[i] < min)
        {
            min = arr[i];
        }
        // If current element is greater
        // than max then update it
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    
    max_min = malloc(2 * sizeof(int));
    max_min[0] = max;
    max_min[1] = min;
    return max_min;
}


/*
 * Function to find the product of
 * an array multiplied with a double
 */
double* multArrDouble(int arr[], int N, double a)
{
    int i;
    double* result;
    result = (double *) malloc(N * sizeof(double));
    for (i = 0; i < N; i++)
    {
        result[i] = arr[i] * a;
    }
    
    return result;
    
}
