#pragma once

//#include <QtMath>
//#include <QtCore>


namespace Editor
{

template<typename U, typename T, T N>
class AccessArray
{
    using ThisType = AccessArray<U, T, N>;

    static const size_t SIZE = static_cast<size_t>(N);
    U m_array[SIZE];
    //bool m_mask[SIZE];

public:

    AccessArray()
    {
        Reset();
    }

    U& operator[](T index)
    {
        size_t n = static_cast<size_t>(index);
        return m_array[n];
    }

/*    ThisType& operator|(T v)
    {
        (*this)[v] = true;
        return *this;
    }

    ThisType& operator^(T v)
    {
        (*this)[v] = false;
        return *this;
    }*/

    U Sum()
    {
        U sum = 0;

        for(size_t i = 0; i < SIZE; i++)
        {
            //if(m_mask[i])
                sum += m_array[i];
        }

        return sum;
    }

    void Reset(U initvalue = 0, bool initflag = false)
    {
        for(size_t i = 0; i < SIZE; i++)
        {
            m_array[i] = initvalue;
           // m_mask[i] = initflag;
        }
    }
};

}//namespace Editor
