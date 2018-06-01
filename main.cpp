#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

class BasicApp : public App {
public:
    void draw() override;
};

void BasicApp::draw()
{
    gl::clear();
    // reset the matrices
    gl::setMatricesWindow( getWindowSize() );

    // move to the horizontal window center, down 75
    gl::translate( getWindowCenter().x, 75 );
    gl::color( Color( 1, 0, 0 ) );
    gl::drawSolidCircle( vec2( 0 ), 70 );

    // move down 150 pixels
    gl::translate( 0, 150 );
    gl::color( Color( 1, 1, 0 ) );
    gl::drawSolidCircle( vec2( 0 ), 70 );

    // move down another 150 pixels
    gl::translate( 0, 150 );
    gl::color( Color( 0, 1, 0 ) );
    gl::drawSolidCircle( vec2( 0 ), 70 );
}

CINDER_APP( BasicApp, RendererGl )
