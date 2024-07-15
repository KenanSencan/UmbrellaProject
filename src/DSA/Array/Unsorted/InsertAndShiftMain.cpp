#include <iostream>
using namespace std;

void insertElement(int* arr, int size, int value, int pos)
{
    // shift elements to the right which are on the right side of pos
    for (int i = size - 1; i >= pos; i--)
        arr[i + 1] = arr[i];

    arr[pos] = value;

}
// Driver's code
int main()
{
    int arr[15] = { 2, 4, 1, 8, 5 };
    int size = 5;

    cout<<"Before insertion : ";
    for (int i = 0; i < size; i++)
        cout<<arr[i]<<" ";

    cout<<endl;

    int value = 10, pos = 2;

    // Function call
    insertElement(arr, size, value, pos);
    size++;

    cout<<"After insertion : ";
    for (int i = 0; i < size; i++)
        cout<<arr[i]<<" ";

    return 0;
}