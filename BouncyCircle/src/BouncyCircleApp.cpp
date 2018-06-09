#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BouncyCircleApp : public App {
public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
    gl::BatchRef mCircle;
    gl::GlslProgRef mGlsl;
    CameraPersp mCam;
};

void BouncyCircleApp::setup()
{
    mCam.lookAt(vec3(4, 3, 4), vec3(0));
    mGlsl = gl::GlslProg::create(gl::GlslProg::Format()
                                 .vertex(loadAsset("vertex.glsl"))
                                 .fragment(loadAsset("fragment.glsl")));
    
    auto geom = geom::Torus().radius(1, 1.03).subdivisionsAxis(100).subdivisionsHeight(20);
    mCircle = gl::Batch::create(geom, mGlsl);
    
    gl::enableDepth();
}

void BouncyCircleApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCam);

    gl::color(Color(1, 0, 0));
//    mGlsl->uniform("uRadius", 1.0f);
    mGlsl->uniform("uAnim", (float)getElapsedSeconds());
    mGlsl->uniform("uLightCoord", glm::normalize(vec3(-2, 3, -4)));
    mCircle->draw();
}


void BouncyCircleApp::mouseDown( MouseEvent event )
{
}

void BouncyCircleApp::update()
{
}

CINDER_APP( BouncyCircleApp, RendererGl(RendererGl::Options().msaa(16)) )
