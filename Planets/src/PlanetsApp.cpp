#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PlanetsApp : public App {
public:
    PlanetsApp() : cRotationPerFrame(1 / getFrameRate()) {}
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
private:
    gl::BatchRef mSphere;
    
    CameraPersp mCam;
    vec3 cInitialEyeLocation {2.0f * vec3(4, 3, 4)};
    const float cRotationPerFrame;
    quat mRotation {0, vec3(0, 1, 0)};
};

void PlanetsApp::setup()
{
    auto glsl = gl::GlslProg::create(gl::GlslProg::Format()
                                     .vertex(loadAsset("vertex.glsl"))
                                     .fragment(loadAsset("fragment.glsl")));
    
    auto geomSphere = geom::Sphere().subdivisions(50);
    mSphere = gl::Batch::create(geomSphere, glsl);
    gl::enableDepth();
}

void PlanetsApp::draw()
{
    gl::clear(Color(0, 0, 0));
    
    mRotation *= angleAxis(cRotationPerFrame, vec3(0, 1, 0));
    auto eyeLocation = mRotation * cInitialEyeLocation;
    mCam.lookAt(eyeLocation, vec3(0));
    gl::setMatrices(mCam);
    mSphere->draw();
}

void PlanetsApp::mouseDown(MouseEvent event)
{
}

void PlanetsApp::update()
{
}


CINDER_APP( PlanetsApp, RendererGl(RendererGl::Options().msaa(16)) )
