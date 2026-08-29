class List{
    public:
    virtual void insert(int pos, int n) = 0;
    virtual void addFront(int n) = 0;
    virtual void addBack(int n) = 0;
    virtual void removeOccurrences(int n) = 0;
    virtual void clear() = 0;
    virtual int getSize() = 0;
    virtual bool isEmpty() = 0;
    virtual int search(int n) = 0;
    virtual bool contains(int n) = 0;
    virtual int count(int n) = 0;
    virtual int sum() = 0;
    virtual int maxValue() = 0;
    virtual int minValue() = 0;
    virtual void reverse() = 0;
    virtual int replace(int n, int val) = 0;
};