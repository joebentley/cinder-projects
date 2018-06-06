#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PlanetsApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
private:
    gl::BatchRef mSphere;
    CameraPersp mCam;
};

void PlanetsApp::setup()
{
    mCam.lookAt(vec3(4, 3, 4), vec3(0));
    
    auto glsl = gl::GlslProg::create(gl::GlslProg::Format()
                                     .vertex(loadAsset("vertex.glsl"))
                                     .fragment(loadAsset("fragment.glsl")));
    
    auto geom = geom::Sphere().subdivisions(30);
    mSphere = gl::Batch::create(geom, glsl);
    
    gl::enableDepth();
}

void PlanetsApp::mouseDown(MouseEvent event)
{
}

void PlanetsApp::update()
{
}

void PlanetsApp::draw()
{
    gl::clear(Color(0, 0, 0));
    gl::setMatrices(mCam);
    mSphere->draw();
}

CINDER_APP( PlanetsApp, RendererGl )
