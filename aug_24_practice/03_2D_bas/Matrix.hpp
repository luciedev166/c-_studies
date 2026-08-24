class Matrix{
    public:
    virtual void set(int r, int c, int val) = 0;
    virtual void displayRowByRow() = 0;
    virtual void displayColByCol() = 0;
    virtual void displayReverseRows() = 0;
    virtual void displayRowsReversed() = 0;
    virtual int sum() = 0;
    virtual int maxValue() = 0;
    virtual int minValue() = 0;
    virtual int countEven() = 0;
    virtual int countOdd() = 0;
    virtual void displayRowSums() = 0;
    virtual void displayColSums() = 0;
    virtual bool isSquare() = 0;
    virtual void swapRows(int x, int y) = 0;

    virtual void displayMainDiagonal() = 0;
    virtual void displayAntiDiagonal() = 0;
    virtual int sumMainDiagonal() = 0;
    virtual int sumAntiDiagonal() = 0;
};