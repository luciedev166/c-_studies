class List{
    virtual void insert(int pos, int n) = 0;
    virtual void display() = 0;
    virtual void removeDuplicates() = 0;
    virtual void printDuplicates() = 0;
    virtual void rotateLeft(int n) = 0;
    virtual void rotateRight(int n) = 0;
    virtual void moveZerosToEnd() = 0;
    virtual int kthLargest(int n) = 0;
    virtual int firstLargest() = 0;
    virtual int secondLargest() = 0;
    virtual int thirdLargest() = 0;
};