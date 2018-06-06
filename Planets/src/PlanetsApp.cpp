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
    vec3 mEyeLocation {4, 3, 4};
    const float cRotationPerFrame;
};

void PlanetsApp::setup()
{
    
    auto glsl = gl::GlslProg::create(gl::GlslProg::Format()
                                     .vertex(loadAsset("vertex.glsl"))
                                     .fragment(loadAsset("fragment.glsl")));
    
    auto geom = geom::Sphere().subdivisions(50);
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
    mEyeLocation = vec3(glm::rotate(cRotationPerFrame, vec3(0, 1, 0)) * vec4(mEyeLocation, 0));
    
    mCam.lookAt(mEyeLocation, vec3(0));
    gl::setMatrices(mCam);
    mSphere->draw();
}

CINDER_APP( PlanetsApp, RendererGl(RendererGl::Options().msaa(16)) )
