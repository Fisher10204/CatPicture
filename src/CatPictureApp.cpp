#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	private:
		float currentColor;
};

void CatPictureApp::setup()
{
	currentColor=0.0f;
}

void CatPictureApp::mouseDown( MouseEvent event )
{
}

void CatPictureApp::update()
{
	currentColor= currentColor+.005f;
	if(currentColor>1.0)
	{
		currentColor=0.0f;
	}
}

void CatPictureApp::draw()
{
	// clear out the window with black
	gl::clear( Color( currentColor, 0, currentColor ) ); 
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
