#ifndef WRAPPINGQVECTOR_H
#define WRAPPINGQVECTOR_H

#include <QVector>

template <typename T> class WrappingQVector
        : public QVector<T>
{
public:
    WrappingQVector(size_t maxSize);

    void push(QVector<T> in);
    void push(T in);
    void insert(QVector<T> in);
    void clear();
    size_t maxSize() const;
    void setMaxSize(size_t maxSize);
private:
    size_t m_maxSize;
    size_t pos;
};

template<typename T>
WrappingQVector<T>::WrappingQVector(size_t maxSize)
{
    m_maxSize = maxSize;
    pos = 0;
}

template<typename T>
void WrappingQVector<T>::push(QVector<T> in)
{
    for (size_t i = 0; i < in.size(); i++)
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
size_t WrappingQVector<T>::maxSize() const
{
    return m_maxSize;
}

template<typename T>
void WrappingQVector<T>::setMaxSize(size_t maxSize)
{
    m_maxSize = maxSize;
}

#endif // WRAPPINGQVECTOR_H
