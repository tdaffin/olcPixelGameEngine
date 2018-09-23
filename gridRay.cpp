#include "olcPixelGameEngine.h"
#include <sstream>

using namespace std;

class Vec : public vector<float> {

public:
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
        int n = size();
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

class GridRay {
private:
    Vec normal = Vec(2);
    Vec an = Vec(2); // Absolute normal
    Vec sn = Vec(2); // Sign of normal

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
        //pos += 0.5f;
        if (sgn == -1.0f)
            bndry = floor(pos);
        else
            bndry = ceil(pos);
        if (bndry == pos)
            bndry += sgn;
        return bndry;// - 0.5f;
    }
};

class GridRayEngine : public olc::PixelGameEngine
{
public:
	GridRayEngine()
	{
		sAppName = "GridRay";
	}

private:
    float stepX = 10.0f;
    float stepY = 10.0f;

    olc::Pixel gridPix = olc::VERY_DARK_GREY;

    Vec pos = Vec(50, 50);
    Vec dir = Vec(2, 1);

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
        dir.normalize();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame

        //FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));
        Clear(olc::Pixel(0, 0, 0));

        // Draw grid
		for (int x = 0; x < ScreenWidth(); x+=stepX){
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, gridPix);
        }
        for (int y = 0; y < ScreenHeight(); y+=stepY){
            for (int x = 0; x < ScreenWidth(); x++)
                Draw(x, y, gridPix);
        }
        //olc::Pixel(255, 255, 255)

        Draw(pos[0], pos[1], olc::RED);

        GridRay gridRay(dir);
        Vec p(pos[0]/stepX, pos[1]/stepY);
        for(auto np = gridRay.nearest(p); isOnScreen(np); np = gridRay.nearest(np)){
            Vec npos(np[0]* stepX, np[1] * stepY);
            Draw(npos[0], npos[1], olc::WHITE);
        }


        ostringstream ostr;
        //ostr << "fElapsedTime: " << fElapsedTime;
        DrawString(10, 10, ostr.str());
		return true;
	}

private:
    bool isOnScreen(const Vec& v) {
        auto x = v[0];
        auto y = v[1];
        if (x < 0 || x > ScreenWidth())
            return false;
        if (y < 0 || y > ScreenHeight())
            return false;
        return true;
    }    
};


int main()
{
	GridRayEngine gridRayEngine;
	if (gridRayEngine.Construct(128, 120, 8, 8))
		gridRayEngine.Start();

	return 0;
}
