class UnionSet
{
    int *parent, *rnk;
    int n;

    public:
    UnionSet(int n);
    
    int find(int u);
    void Union(int x, int y);
};