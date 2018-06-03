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
    gl::BatchRef mRect;
    gl::GlslProgRef mGlsl;
};

void BasicApp::setup() {
    // Make sure that window initially on top
    getWindow()->setAlwaysOnTop(true);

    mCam.lookAt(vec3(4, 3, 4), vec3(0));

    mGlsl = gl::GlslProg::create(gl::GlslProg::Format()
            .vertex(loadAsset("vertex.glsl"))
            .fragment(loadAsset("fragment.glsl")));

    auto plane = geom::Plane().subdivisions(ivec2(100)).size(vec2(3, 3));
    mRect = gl::Batch::create(plane, mGlsl);

    gl::enableDepthRead();
    gl::enableDepthWrite();
}

void BasicApp::draw() {
    getWindow()->setAlwaysOnTop(false);

    gl::clear(Color(0.2f, 0.2f, 0.2f));
    gl::setMatrices(mCam);

    mGlsl->uniform("uAnim", getElapsedFrames() / 60.0f);
    mRect->draw();
}

CINDER_APP( BasicApp, RendererGl(RendererGl::Options().msaa(16)) )
