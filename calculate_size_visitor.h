#ifndef CALCULATE_SIZE_VISITOR_H
#define CALCULATE_SIZE_VISITOR_H

class CalculateSizeVisitor : public TreeVisitor<DirectoryEntry>
{
public:
    CalculateSizeVisitor() : total_size(0) {}

    virtual void visit(DirectoryEntry* data)
    {
        total_size += data->getSize();
    }

    void printResults()
    {
        if (total_size > 1000*1024)
            printf("%lu MB\n", total_size / (1024*1024));
        else if (total_size > 10*1024)
            printf("%lu kB\n", total_size / 1024);
        else
            printf("%lu B\n", total_size);
    }

    size_t getCalculatedSize() const { return total_size; }

private:
    size_t total_size;
};



#endif // CALCULATE_SIZE_VISITOR_H
