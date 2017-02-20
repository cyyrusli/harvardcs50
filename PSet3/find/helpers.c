/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

bool binary(int value, int values[], int n);
void selection(int values[], int n);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    // binary searching algorithm
    if (value < 0)
    {
        return false;
    }
    else
    {
        return binary(value, values, n);
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    selection(values, n);
    return;
}

// binary search function
bool binary(int value, int values[], int n)
{
    int start = 0;
    int end = n-1;
    
    while (end >= start)
    {
        int middle = (start + end) / 2;
        if (values[middle] == value)
        {
            return true;
        }
        else if (values[middle] > value)
        {
            end = middle - 1;
        }
        else 
        {
            start = middle + 1;
        }
    }
    return false;
}

// selection sort function
void selection(int values[], int n)
{
    int x;
    for (int i = 0; i < n; i++)
    {
        int smallest = i;
        for (int j = i+1; j < n; j++)
        {
            if(values[smallest] > values[j])
            {
                smallest = j;
            }
            
            x = values[smallest];
            values[smallest] = values[i];
            values[i] = x;
        }
    }
}
