#ifndef WRAPPINGQVECTOR_H
#define WRAPPINGQVECTOR_H

#include <QVector>

template <typename T> class WrappingQVector
        : public QVector<T>
{
public:
    WrappingQVector(int maxSize);

    void push(QVector<T> in);
    void push(T in);
    void insert(QVector<T> in);
    void clear();
    int maxSize() const;
    void setMaxSize(int maxSize);
private:
    int m_maxSize;
    int pos;
};

template<typename T>
WrappingQVector<T>::WrappingQVector(int maxSize)
{
    m_maxSize = maxSize;
    pos = 0;
}

template<typename T>
void WrappingQVector<T>::push(QVector<T> in)
{
    for (int i = 0; i < in.size(); i++)
    {
        if (size() < maxSize() && pos == 0)
        {
            push_back(in[i]);
        }
        else if (size() <= maxSize())
        {
            (*this)[pos] = in[i];
            pos++;
            if (pos >= size())
            {
                pos = 0;
            }
        }
        else
        {
            (*this)[pos] = in[i];
            pos++;
            if (pos + size() - maxSize() <= size())
            {
                erase(begin() + pos, begin() + pos + size() - maxSize());
            }
            else
            {
                erase(begin() + pos, end());
            }
            if (pos >= size())
            {
                pos = 0;
            }
        }
    }
}

template<typename T>
void WrappingQVector<T>::push(T in)
{
    if (size() < maxSize() && pos == 0)
    {
        push_back(in);
    }
    else if (size() <= maxSize())
    {
        (*this)[pos] = in;
        pos++;
        if (pos >= size())
        {
            pos = 0;
        }
    }
    else
    {
        (*this)[pos] = in;
        pos++;
        if (pos + size() - maxSize() <= size())
        {
            erase(begin() + pos, begin() + pos + size() - maxSize());
        }
        else
        {
            erase(begin() + pos, end());
        }
        if (pos >= size())
        {
            pos = 0;
        }
    }
}

template<typename T>
void WrappingQVector<T>::clear()
{
    QVector<T>::clear();
    pos = 0;
}

template<typename T>
int WrappingQVector<T>::maxSize() const
{
    return m_maxSize;
}

template<typename T>
void WrappingQVector<T>::setMaxSize(int maxSize)
{
    m_maxSize = maxSize;
}

#endif // WRAPPINGQVECTOR_H
