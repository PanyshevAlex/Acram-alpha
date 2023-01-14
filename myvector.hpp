//
//  myvector.hpp
//  Acram alpha
//
//  Created by Панышев Александр Сергеевич on 12.05.2021.
//

#ifndef myvector_hpp
#define myvector_hpp
#pragma once
#include <stdio.h>

#endif /* myvector_hpp */



template <class T>
class  Vector
{
public:

    typedef T * iterator;

    Vector();
    ~Vector();

    unsigned int size() const;
        
    void push_back(const T & value);
    void pop_back();



    T & operator[](unsigned int index);
    Vector<T> & operator=(const Vector<T> &);
private:
    unsigned int my_size;
    unsigned int my_capacity;
    T * buffer;
};

template<class T>
Vector<T>::Vector()
{
    my_capacity = 0;
    my_size = 0;
    buffer = 0;
}

template<class T>
void Vector<T>::push_back(const T & v)
{

    buffer [my_size++] = v;
}

template<class T>
void Vector<T>::pop_back()
{
    my_size--;
}


template<class T>
unsigned int Vector<T>::size()const//
{
    return my_size;
}



template<class T>
T& Vector<T>::operator[](unsigned int index)
{
    return buffer[index];
}

template<class T>
Vector<T>::~Vector()
{
    delete[ ] buffer;
}

