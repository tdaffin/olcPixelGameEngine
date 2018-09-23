#include "olcPixelGameEngine.h"
#include "Vec.h"
#include "GridRay.h"

#include <sstream>

using namespace std;

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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
        const auto& but0 = GetMouse(0);
        const auto& but1 = GetMouse(1);
        Vec m(GetMouseX(), GetMouseY());
        if (but0.bPressed || but0.bHeld){
            pos = m;
        }
        if (but1.bPressed || but1.bHeld){
            dir = m - pos;
        }

        dir.normalize();

        //FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));
        Clear(olc::Pixel(0, 0, 0));

        // Draw grid
		for (int x = 0; x < ScreenWidth(); x+=stepX)
            DrawLine(x, 0, x, ScreenHeight(), gridPix);
        for (int y = 0; y < ScreenHeight(); y+=stepY)
            DrawLine(0, y, ScreenWidth(), y, gridPix);

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
