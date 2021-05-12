//
//  mystring.cpp
//  Acram alpha
//
//  Created by Панышев Александр Сергеевич on 12.05.2021.
//  Copyright © 2021 Панышев Александр Сергеевич. All rights reserved.
//

#include "mystring.hpp"

void swap(char*& str1, char*& str2) {
    char* temp = std::move(str1);
    str1 = std::move(str2);
    str2 = std::move(temp);
}
int __strlen(const char* str) {
    if (str == nullptr)
        return 0;
    else {
        int len = 0;
        while (str[len] != '\0')
            len++;
        return len;
    }
}

std::ostream& operator << (std::ostream& out, const mystring& string1) {
    return out << string1.data_;
}




mystring::mystring() :
    data_(new char[1]),
    length_(0) {
    data_[0] = '\0';
}

mystring::mystring(const char* str) :
    data_(nullptr),
    length_(__strlen(str)) {
    
    if (str != nullptr) {
        data_ = new char[length_ + 1];
        memcpy(data_, str, length_);
        data_[length_] = '\0';
    }
}

mystring::mystring(const std::string str):
    data_(nullptr),
    length_(int(str.size())) {
        
    data_ = new char[length_ + 1];
    memcpy(data_, &(str[0]), length_);
    data_[length_] = '\0';
}

mystring::mystring(const mystring& other):
    data_(nullptr),
    length_(other.length()) {
        
    data_ = new char[length_ + 1];
    memcpy(&((*this)[0]), &(other[0]), length_ + 1);
}

mystring::mystring(mystring&& other):
    data_(nullptr),
    length_(other.length()) {
        
    swap(data_, other.data_);
}

mystring::~mystring() {
    delete[] data_;
}


int mystring::length() const {
    return length_;
}

bool mystring::is_empty() const {
    if (length_ == 0)
        return true;
    return false;
}

char* mystring::c_str() const {
    return data_;
}


void mystring::push_back(const char& new_elem) {
    if (new_elem == '\0')
        return;

    length_++;
    char* new_data = new char[length_ + 1];
    memcpy(new_data, data_, length_);
    new_data[length_ - 1] = new_elem;
    new_data[length_] = '\0';

    delete[] data_;
    data_ = new_data;
}

char mystring::pop_back() {
    if (length_ != 0) {
        length_--;
        char output_data = data_[length_];
        data_[length_] = '\0';
        return output_data;
    }

    return '\0';
}

char& mystring::operator [] (int num) const {
    if (num > length_ || num < 0) {
        throw std::runtime_error("Overflow.");
    }
    return data_[num];
}


mystring& mystring::operator = (mystring&& str) {
    length_ = str.length();
    swap(data_, str.data_);
    return (*this);
}

mystring& mystring::operator = (const char* str) {
    length_ = __strlen(str);
    delete [] data_;
    data_ = new char[length_ + 1];
    memcpy(data_, str, length_ + 1);
    return (*this);
}

mystring& mystring::operator = (char*&& str) {
    length_ = __strlen(str);
    swap(data_, str);
    return (*this);
}

mystring& mystring::operator = (const mystring& other) {
    length_ = other.length();
    delete [] data_;
    data_ = new char[length_ + 1];
    memcpy(data_, &(other[0]), length_ + 1);
    return (*this);
}

mystring& mystring::operator = (const std::string& str) {
    length_ = int(str.size());
    delete [] data_;
    data_ = new char[length_ + 1];
    memcpy(data_, &(str[0]), length_ + 1);
    return (*this);
}

mystring mystring::operator + (const std::string& str) const & {
    
    int lengh_str = int(str.length());
    char *new_data = new char[length_ + lengh_str + 1];
    memcpy(new_data, data_, length_);
    memcpy(new_data + length_, &(str[0]), lengh_str + 1);
    mystring outstring = mystring(new_data);
    return outstring;
}

mystring mystring::operator + (const std::string& str) && {
    int lengh_str = int(str.length());
    char *new_data = new char[length_ + lengh_str + 1];
    memcpy(new_data, data_, length_);
    memcpy(new_data + length_, &(str[0]), lengh_str + 1);
    mystring outstring = mystring(new_data);
    return outstring;
}

mystring mystring::operator + (const mystring& str) const & {
    mystring out_string = (*this) + str.c_str();
    return out_string;
}
mystring mystring::operator + (const mystring& str) && {
    mystring out_string = (*this) + str.c_str();
    return out_string;
}


mystring mystring::operator + (const char* str) const & {
    int lengh_str = __strlen(str);
    char *new_data = new char[length_ + lengh_str + 1];
    memcpy(new_data, data_, length_);
    memcpy(new_data + length_, &(str[0]), lengh_str);
    new_data[length_ + lengh_str] = '\0';
    mystring outstring = mystring(new_data);
    return outstring;
}

mystring mystring::operator + (const char* str) && {
    int lengh_str = __strlen(str);
    char *new_data = new char[length_ + lengh_str + 1];
    memcpy(new_data, data_, length_);
    memcpy(new_data + length_, &(str[0]), lengh_str);
    new_data[length_ + lengh_str] = '\0';
    mystring outstring = mystring(new_data);
    return outstring;
}


bool mystring::operator == (const std::string& str) const {
    if ((size_t)this->length_ != str.size())
        return false;
    
    const char* start_str = &(str[0]);
    for (int i = 0; i < length_; ++i) {
        if (data_[i] != *(start_str + i))
            return false;
    }
    return true;
}
bool mystring::operator == (const mystring& str) const {
    std::string new_string = str.data_;
    return (*this == new_string);
}
bool mystring::operator == (const char* str) const {
    std::string new_string = str;
    return (*this == new_string);
}


