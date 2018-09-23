#include "olcPixelGameEngine.h"
#include <sstream>

using namespace std;

class GridRay : public olc::PixelGameEngine
{
public:
	GridRay()
	{
		sAppName = "GridRay";
	}

private:
    float stepX = 10.0f;
    float stepY = 10.0f;

    olc::Pixel gridPix = olc::Pixel(127, 127, 127);

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
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

        ostringstream ostr;
        //ostr << "fElapsedTime: " << fElapsedTime;
        DrawString(10, 10, ostr.str());
		return true;
	}
};


int main()
{
	GridRay gridRay;
	if (gridRay.Construct(256, 240, 4, 4))
		gridRay.Start();

	return 0;
}
