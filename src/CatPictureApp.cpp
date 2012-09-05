//Jacob McCanty
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	

	private:
		//float currentColor;
		void createLine(uint8_t* pixles, int startX, int startY, int segLength);
		void makeRect(uint8_t* pixles, int startX, int startY, int width, int height);
		void makeCopy(uint8_t* pixles, int startX, int startY, int width, int height, int x2, int y2);
		void makeCircle(uint8_t* pixles, int x, int y, int radius);
		void blur(uint8_t* pixles);
		Surface* mySurface;
		static const int windowWidth=800;
		static const int windowHeight=600;
		static const int surfaceSize=1024;
		int updateCount;
};

void CatPictureApp::prepareSettings(Settings* settings)
{
	(*settings).setWindowSize(windowWidth,windowHeight);
	(*settings).setResizable(false);
}

/*
Blurs the image for requirement B 1
*/
void CatPictureApp::blur(uint8_t* pixles)
{
	int offset;
	float a=9.0;
	for (int y1 = 1; y1 < windowHeight+1; y1++) {
		for (int x1 = 1; x1 < windowWidth+1; x1++) {
			int totalR = 0;
			int totalG = 0;
			int totalB = 0;
			for (int y2 = y1-1; y2 <= y1+1; y2++) {
				for (int x2 = x1-1; x2 <= x1+1; x2++) {
					//calculate offset for y2 and x2
					offset = 3*(y2+x2*surfaceSize);
					//add offset to holding variables
					totalR += pixles [offset];
					totalG += pixles [offset + 1];
					totalB += pixles [offset + 2];
				}
			}
			//store in offset for x1 and y1 but divide by 9.0
			offset = 3*(y1+x1*surfaceSize);
			pixles [offset] = totalR/a;
			pixles [offset + 1] = totalG/a;
			pixles [offset + 2] = totalB/a;
			}
	}
}


/*
Draws a circle for requirement A 2
*/
void CatPictureApp::makeCircle(uint8_t* pixles, int x, int y, int radius)
{
		if (radius <= 0) return;

		for(int newY = y-radius; newY <= y+radius; newY++) {
			for (int newX=x-radius; newX <= x+radius; newX++) {

				int newR = (int)sqrt((double)((newX-x)*(newX-x) + (newY-y)*(newY-y)));

				if(newR <= radius) {
				int offset = 3*(newX+newY*surfaceSize);
				pixles [offset] = 50;
				pixles [offset+1] = 100;
				pixles [offset+2] =255;
				}
			}
		}
}


/*
Copies an area of pixles for requirement A 5
*/
void CatPictureApp::makeCopy(uint8_t* pixles, int startX, int startY, int width, int height, int x2, int y2)
{
	int x1=startX;
	int y1=startY;
	int offset;
	int origX2=x2;
	for(int i=startY; i<height+startY; ++i)
	{
		for(int j=startX; j<width+startX; ++j)
		{
			offset=3*(j+i*surfaceSize);
			pixles[x2+y2*surfaceSize]=pixles[offset];
			pixles[x2+y2*surfaceSize+1]=pixles[offset+1];
			pixles[x2+y2*surfaceSize+2]=pixles[offset+2];
			++x2;
		}
		++y2;
		x2=origX2;
	}
}

/*
Draws a rectangle for requirement A 1, like the steak sauce
*/
void CatPictureApp::makeRect(uint8_t* pixles, int startX, int startY, int width, int height)
{
	int x=startX;
	int y=startY;
	int offset;
	for(int i=startY; i<height+startY; ++i)
	{
		for(int j=startX; j<width+startX; ++j)
		{
			offset=3*(j+i*surfaceSize);
			pixles[offset]=100;
			pixles[offset+1]=150;
			pixles[offset+2]=175;
		}
	}
}
/*
Draws a line that looks kinda like a green glowstick for requirement A 3
*/
void CatPictureApp::createLine(uint8_t* pixles, int startX, int startY, int segLength)
{
	int x=startX;
	int y=startY;
	for(int i=0; i< segLength; ++i)
	{
		pixles[3*(x+y*surfaceSize)]=0;
		pixles[3*(x+y*surfaceSize)+1]=200;
		pixles[3*(x+y*surfaceSize)+2]=0;
		++x;
		++y;
	}
}

void CatPictureApp::setup()
{
	//currentColor=0.0f;
	updateCount=0;
	mySurface=new Surface(surfaceSize, surfaceSize, false);
}

void CatPictureApp::mouseDown( MouseEvent event )
{
}

void CatPictureApp::update()
{
	++updateCount;
	uint8_t* pixels = (*mySurface).getData();
	if(updateCount>0&&updateCount<25)
	{
		blur(pixels);
	}
	/*currentColor= currentColor+.005f;
	if(currentColor>1.0)
	{
		currentColor=0.0f;
	}*/
	createLine(pixels, 20, 200, 100);
	makeRect(pixels, 10, 40, 100, 150);
	makeCopy(pixels, 10, 40, 100, 150, 200, 40);
	makeCircle(pixels, 120, 220, 25);
	if(updateCount==26)
	{
		writeImage("works.png", *mySurface);// kinda looks like a mouse
	}
}

void CatPictureApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::draw(*mySurface);
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
