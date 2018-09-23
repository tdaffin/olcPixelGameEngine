#pragma once

#include "Vec.h"

class GridRay {
private:
    Vec normal;
    Vec an; // Absolute normal
    Vec sn; // Sign of normal

public:
    GridRay(const Vec& normal){
        this->normal = normal;
        // Split normal into abs and sign
        an = normal.abs();
        sn = normal.sign();
    }

    Vec nearest(const Vec& pos) const{
        Vec bnd = bndry(pos, sn);
        auto dist = (bnd - pos).abs();
        auto t = dist / an;
        auto insct = pos;
        if (t[0] < t[1])
            insct += normal * t[0];
        else
            insct += normal * t[1];
        return insct;
    }

    static Vec bndry(const Vec& pos, const Vec& sign){
        int n = pos.size();
        Vec bnd(n);
        for(int i=0; i<n; ++i)
            bnd[i] = bndry(pos[i], sign[i]);
        return bnd;
    }

private:
    static float bndry(float pos, float sgn) {
        float bndry;
        if (sgn == -1.0f)
            bndry = floor(pos);
        else
            bndry = ceil(pos);
        if (bndry == pos)
            bndry += sgn;
        return bndry;
    }
};
