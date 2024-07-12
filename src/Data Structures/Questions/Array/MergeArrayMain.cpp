// There are two sorted arrays. First one is of size m+n containing only m elements. Another one is of size n and contains n elements.
// Merge these two arrays into the first array of size m+n such that the output is sorted. 
// Input: array with m+n elements (mPlusN[]).

//MPlusN[] = {1,5,8,None,None,None}
//N[] = {2,3,7}
//Result: MPlusN[] = {1,2,3,5,7,8}


// mPlusN[] initially contains [1, 5, 8] with 3 empty slots.
// N[] contains [2, 3, 7]
// We need to merge the elements of N[] into mPlusN[] while maintaining the sorted order.

#include <iostream>
//merged branch into master
int main()
{
    int m = 4, n = 4;
    int** FirstArr = new int*[m + n];
    int** SecArr = new int*[n];
    
    for (int i = 0; i < m+n; ++i)
        FirstArr[i] = nullptr;
    
    // FirstArr[0] = new int(1);
    // FirstArr[1] = new int(5);
    // FirstArr[2] = new int(8);

    for (int i = 0; i <= 3; ++i)
    {
        
    }

    SecArr[0] = new int(2);
    SecArr[1] = new int(3);
    SecArr[2] = new int(7);

    //merge but let's make this more complicated. Randomly assign any random value to the random index for both and then somehow merge and sort
    
    
    delete[] FirstArr;
    delete[] SecArr;
}
