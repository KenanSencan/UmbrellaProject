//This is simple linked list. Nodes can be inserted at the front and behind
//I've used Ottoman king names as example. From Predecessor to successor nodes are following "Yavuz -> Suleiman -> YellowSelim -> ThirdMurad
// SecondBayezit inserted front of the Yavuz.
// Imaginary 2. Suleiman inserted back/behind of the Suleiman
// Imaginary 2. yavuz inserted behind of the yavuz

#include <iostream>

class Node
{
public:
    std::string Value;
    Node* Next;

    ~Node();
};


void printList(Node* n)
{
    while (n != nullptr)
    {
        std::cout << n->Value << std::endl;
        n = n->Next;
    }
}

void InsertAtTheFront(Node** head, std::string&& newValue)
{
    Node* NewNode = new Node();
    NewNode->Value = newValue;
    NewNode->Next = *head; //Don't get confuse I just dereferencing pointer pointer -> Node Pointer .
    *head = NewNode;
}

void InsertBehind(Node** last, std::string&& NewValue)
{
    Node* LastNode = new Node();
    LastNode->Value = NewValue;

    //If fist element is empty, make it newly created node
    if (*last == nullptr)
    {
        *last = LastNode;
        return;
    }

    //If Suleiman's next is already given
    if ((*last)->Next != nullptr)
    {
        Node* TempShift = (*last)->Next; //Suleiman->Next = SariSelim. Variable is SariSelim
        (*last)->Next = LastNode; //Suleiman->Next = Hayali 2. Suleiman. Now the variable has already changed. I have to shift everything 

        //hayali 2. Suleiman->Next    must be YellowSelim
        LastNode->Next = TempShift;
    }
    else //Already at the end of the chain
    {
        (*last)->Next = LastNode;
    }
}

int main()
{
    Node* Yavuz = new Node();
    Node* Suleiman = new Node();
    Node* SariSelim = new Node();
    Node* UcuncuMurat = new Node();

    Yavuz->Value = "Yavuz";
    Yavuz->Next = Suleiman;
    Suleiman->Value = "Suleiman";
    Suleiman->Next = SariSelim;
    SariSelim->Value = "YellowSelim";
    SariSelim->Next = UcuncuMurat;
    UcuncuMurat->Value = "ThirdMurat";

    InsertAtTheFront(&Yavuz, "SecondBayezit");
    InsertBehind(&Suleiman, "Imaginary 2. Suleiman ");
    InsertBehind(&Yavuz, "Imaginary 2. yavuz ");
    printList(Yavuz);
    return 0;
}
