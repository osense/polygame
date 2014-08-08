#ifndef CIRCULAR_BUFFER_H_INCLUDED
#define CIRCULAR_BUFFER_H_INCLUDED


template <typename T>
class circular_buffer
{
public:
    circular_buffer (unsigned int size)
    {
        Size = size;
        Array = new T[Size];
        Index = Size - 1;
    }

    circular_buffer<T> operator= (const circular_buffer<T>& other)
    {
        memcpy(Array, other.Array, Size * sizeof(T));

        setIndex(other.getIndex());

        return *this;
    }

    ~circular_buffer ()
    {
        delete[] Array;
    }

    void push_back (T val)
    {
        Index++;
        Index = Index % Size;
        Array[Index] = val;
    }

    const unsigned int getSize () const
    {
        return Size;
    }

    const unsigned int getIndex () const
    {
        return Index;
    }

    void setIndex (const unsigned int newIdx)
    {
        Index = newIdx;
    }

    T operator[] (const unsigned int idx) const
    {
        return Array[idx % Size];
    }

private:

    T* Array;
    unsigned int Size;
    unsigned int Index;
};

#endif // CIRCULAR_BUFFER_H_INCLUDED
