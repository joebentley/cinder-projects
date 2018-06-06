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
    gl::BatchRef mSun;
    
    CameraPersp mCam;
    vec3 cInitialEyeLocation {2.0f * vec3(4, 3, 4)};
    const float cRotationPerFrame;
    quat mRotation {0, vec3(0, 1, 0)};
    
    vec3 mLightCoord {1, 0, 0};
};

void PlanetsApp::setup()
{
    auto lambert = gl::GlslProg::create(gl::GlslProg::Format()
                                     .vertex(loadAsset("vertex.glsl"))
                                     .fragment(loadAsset("lambert.glsl")));
    
    auto color = gl::GlslProg::create(gl::GlslProg::Format()
                                        .vertex(loadAsset("vertex.glsl"))
                                        .fragment(loadAsset("color.glsl")));
    
    auto geomSphere = geom::Sphere().subdivisions(50);
    mSphere = gl::Batch::create(geomSphere, lambert);
    mSun = gl::Batch::create(geomSphere, color);
    
    gl::enableDepth();
}

void PlanetsApp::draw()
{
    gl::clear(Color(0, 0, 0));
    
//    mRotation *= angleAxis(cRotationPerFrame, vec3(0, 1, 0));
    auto eyeLocation = mRotation * cInitialEyeLocation;
    mCam.lookAt(eyeLocation, vec3(0));
    gl::setMatrices(mCam);
    
    gl::pushMatrices();
    
    auto anim = static_cast<double>(getElapsedSeconds());
    gl::rotate(M_PI * anim, vec3(0, 1, 0));
    gl::translate(3, 0, 0);
    
    gl::color(0.8, 0.8, 0.8);
    
    auto invModelMatrix = glm::inverse(gl::getModelMatrix());
    mSphere->getGlslProg()->uniform("uLightCoord",
        glm::normalize(vec3(invModelMatrix * vec4(mLightCoord, 1))));
    
    mSphere->draw();
    
    gl::popMatrices();
    
    gl::color(1, 0.5, 0);
    
    mSun->draw();
}

void PlanetsApp::mouseDown(MouseEvent event)
{
}

void PlanetsApp::update()
{
}


CINDER_APP( PlanetsApp, RendererGl(RendererGl::Options().msaa(16)) )
