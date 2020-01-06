/*
 * Author: Yunho Cho
 * Date: 2019-05-14
 * Project:
 * Purpose: Functions that manages table. Create, copy, delete, and modify.
 * Notes:
 */

#ifndef BPLUSTREE_FUNCTIONS_H
#define BPLUSTREE_FUNCTIONS_H

#include <iostream>
#include <iomanip>
using namespace std;

/*
 * =====================================================================================================================
 * PROTOCOLS
 * =====================================================================================================================
 */

template<class T>
int get_idx(const T *arr, int numberOfData, T entry);                          //Get index of the entry.

template<class T>
void copy_arr(const T* src, T* dest, int size);                                //Copy the given array.



/*
 * =====================================================================================================================
 * DEFINITIONS
 * =====================================================================================================================
 */

template<class T>
int get_idx(const T *arr, int numberOfData, T entry)
{

    for(int i=0; i<=numberOfData; i++)
    {
        if((arr[i] >= entry)||(i==numberOfData))
        {
            return i;
        }
    }
}

template<class T>
void copy_arr(const T* src, T* dest, int size)
{
    for(int i=0; i<size; i++)
    {
        dest[i] = src[i];
    }
}



#endif //BPLUSTREE_FUNCTIONS_H
