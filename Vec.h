#pragma once

#include <vector>
#include <math.h>

class Vec : public std::vector<float> {

public:
    Vec(){
        
    }

    Vec(size_t n){
        resize(n, 0);
    }

    Vec(const Vec& o){
        assign(o.begin(), o.end());
    }

    Vec(float x, float y){
        resize(2);
        at(0) = x;
        at(1) = y;
    }

    inline Vec& operator+=(const Vec& o) {
        int n = size();
        for(int i=0; i<n; ++i)
            at(i) += o[i];
        return *this;
    }

    inline Vec& operator*=(float t) {
        int n = size();
        for(int i=0; i<n; ++i)
            at(i) *= t;
        return *this;
    }

    inline Vec operator*(float t) const{
        auto a = Vec(*this);
        a *= t;
        return a;
    }

    float sqrLength() const {
        float sqLen = 0;
        int n = size();
        for(int i=0; i<n; ++i){
            auto& v = at(i);
            sqLen += v*v;
        }
        return sqLen;
    }

    float length() const {
        return sqrt(sqrLength());
    }

    Vec abs() const{
        auto a = Vec(*this);
        for(int i=0, n=size(); i<n; ++i)
            a[i] = fabs(at(i));
        return a;
    }

    Vec sign() const{
        auto s = Vec(*this);
        for(int i=0, n=size(); i<n; ++i)
            s[i] = at(i) < 0 ? -1 : 1;
        return s;
    }

    void normalize() {
        float rlen = 1.0f/length();
        (*this) *= rlen;
    }

};

inline Vec operator-(const Vec& lhs, const Vec& rhs){
    int n = lhs.size();
    auto a = Vec(n);
    for(int i=0; i<n; ++i)
        a[i] = lhs[i] - rhs[i];
    return a;
}

inline Vec operator/(const Vec& lhs, const Vec& rhs){
    int n = lhs.size();
    auto a = Vec(n);
    for(int i=0; i<n; ++i)
        a[i] = lhs[i] / rhs[i];
    return a;
}
