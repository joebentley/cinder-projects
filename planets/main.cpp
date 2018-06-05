#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

class BasicApp : public App {
public:
    void setup() override;
    void draw() override;

private:
    CameraPersp mCam;
    gl::BatchRef mCube;
};

void BasicApp::setup() {
    getWindow()->setTitle("Planets");

    // Make sure that window initially on top
    getWindow()->setAlwaysOnTop(true);

    mCam.lookAt(vec3(4, 3, 4), vec3(0));

    auto cube = geom::Cube().size(vec3(0.2));
    mCube = gl::Batch::create(cube, gl::getStockShader(gl::ShaderDef().lambert()));

    gl::enableDepthRead();
    gl::enableDepthWrite();
}

void BasicApp::draw() {
    getWindow()->setAlwaysOnTop(false);

    gl::clear(Color(0.2f, 0.2f, 0.2f));
    gl::setMatrices(mCam);

    mCube->draw();
}

CINDER_APP( BasicApp, RendererGl(RendererGl::Options().msaa(16)) )
