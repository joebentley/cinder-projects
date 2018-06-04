#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Easing.h"

using namespace ci;
using namespace ci::app;

class BasicApp : public App {
public:
    void setup() override;
    void draw() override;

private:
    CameraPersp mCam;
    gl::BatchRef mPlane;
    gl::GlslProgRef mGlsl;
    gl::BatchRef mCube;
};

void BasicApp::setup() {
    // Make sure that window initially on top
    getWindow()->setAlwaysOnTop(true);

    mCam.lookAt(vec3(4, 3, 4), vec3(0));

    mGlsl = gl::GlslProg::create(gl::GlslProg::Format()
            .vertex(loadAsset("vertex.glsl"))
            .geometry(loadAsset("geometry.glsl"))
            .fragment(loadAsset("fragment.glsl")));

    auto plane = geom::Plane().subdivisions(ivec2(10)).size(vec2(3, 3));
    mPlane = gl::Batch::create(plane, mGlsl);

    auto cube = geom::Cube().size(vec3(0.2));
    mCube = gl::Batch::create(cube, gl::getStockShader(gl::ShaderDef().lambert()));

    gl::enableDepthRead();
    gl::enableDepthWrite();
}

void BasicApp::draw() {
    getWindow()->setAlwaysOnTop(false);

    gl::clear(Color(0.2f, 0.2f, 0.2f));
    gl::setMatrices(mCam);

    const float anim = getElapsedFrames() / 60.0f;
    mGlsl->uniform("uAnim", anim);
    const vec3 lightSource = vec3(sin(M_PI * anim), 1.0f, cos(M_PI * anim));
    mGlsl->uniform("uLightCoord", glm::normalize(lightSource));
    mPlane->draw();

    gl::translate(lightSource);
    mCube->draw();
}

CINDER_APP( BasicApp, RendererGl(RendererGl::Options().msaa(16)) )
