#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <memory>

using namespace ci;
using namespace ci::app;
using namespace std;

class Planet {
public:
    Planet(gl::BatchRef batch) : mBatch(batch) {}
    
    Planet &setRadius(float radius) { mRadius = radius; return *this; }
    Planet &setOrbitingRadius(float radius) { mOrbitingRadius = radius; return *this; }
    Planet &setOrbiting(Planet *orbiting) { mOrbiting = orbiting; return *this; }
    Planet &setOrbitingAngle(float angle) { mAngle = angle; return *this; }
    Planet &addOrbitingAngle(float angle) { mAngle += angle; return *this; }
    Planet &setTexture(gl::Texture2dRef tex) { mTex = tex; return *this; }
    
    mat4 getModelMatrix();
    void draw(vec3 lightCoord);
    void draw();
private:
    gl::BatchRef mBatch;
    gl::Texture2dRef mTex {nullptr};
    mat4 mOrbitingPos;
    mat4 mModel;
    Planet *mOrbiting {nullptr};
    
    float mRadius {1};
    float mOrbitingRadius {3};
    float mAngle {0};
    
    void maybeBindTexture();
};

void Planet::maybeBindTexture() {
    if (mTex != nullptr) {
        mTex->bind(0);
        mBatch->getGlslProg()->uniform("uTex0", 0);
    }
}

mat4 Planet::getModelMatrix() {
    mat4 modelMatrix;
    
    if (mOrbiting != nullptr)
        modelMatrix *= mOrbiting->getModelMatrix();
    modelMatrix *= glm::rotate(mAngle, vec3(0, 1, 0));
    modelMatrix *= glm::translate(vec3(mOrbitingRadius, 0, 0));
    modelMatrix *= glm::scale(vec3(mRadius));
    
    return modelMatrix;
}

void Planet::draw(vec3 lightCoord) {
    gl::ScopedModelMatrix scpMtx;
    gl::multModelMatrix(getModelMatrix());
    auto invModelMatrix = glm::inverse(gl::getModelMatrix());
    mBatch->getGlslProg()->uniform("uLightCoord",
                                    glm::normalize(vec3(invModelMatrix * vec4(lightCoord, 1))));
    
    maybeBindTexture();
    mBatch->draw();
}

void Planet::draw() {
    gl::ScopedModelMatrix scpMtx;
    gl::multModelMatrix(getModelMatrix());
    maybeBindTexture();
    mBatch->draw();
}

class PlanetsApp : public App {
public:
    PlanetsApp() : cRotationPerFrame(1 / getFrameRate()) {}
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
private:
//    gl::BatchRef mSun;
    
    CameraPersp mCam;
    vec3 cInitialEyeLocation {4.0f * vec3(4, 3, 4)};
    const float cRotationPerFrame;
    quat mRotation {0, vec3(0, 1, 0)};
    
    vec3 mLightCoord {1, 0, 0};
    
    std::unique_ptr<Planet> mSun;
    std::unique_ptr<Planet> mEarth;
    std::unique_ptr<Planet> mMoon;
    
    gl::Texture2dRef mTex;
};

void PlanetsApp::setup()
{
    auto lambert = gl::GlslProg::create(gl::GlslProg::Format()
                                     .vertex(loadAsset("shaders/vertex.glsl"))
                                     .fragment(loadAsset("shaders/lambert.glsl")));
    
    auto textureShader = gl::GlslProg::create(gl::GlslProg::Format()
                                        .vertex(loadAsset("shaders/vertex.glsl"))
                                        .fragment(loadAsset("shaders/texture.glsl")));
    
    auto geomSphere = geom::Sphere().subdivisions(50);
    auto planetBatch = gl::Batch::create(geomSphere, textureShader);
    
    mSun = std::make_unique<Planet>(planetBatch);
    mSun->setOrbitingRadius(0)
        .setTexture(gl::Texture2d::create(loadImage(loadAsset("textures/sun.jpg"))));
    mEarth = std::make_unique<Planet>(planetBatch);
    mEarth->setOrbiting(mSun.get()).setOrbitingRadius(5)
        .setTexture(gl::Texture2d::create(loadImage(loadAsset("textures/earth.jpg"))));;
    mMoon = std::make_unique<Planet>(planetBatch);
    mMoon->setOrbiting(mEarth.get()).setOrbitingRadius(2).setRadius(0.4)
        .setTexture(gl::Texture2d::create(loadImage(loadAsset("textures/moon.jpg"))));;
    
    gl::enableDepth();
}

void PlanetsApp::draw()
{
    gl::clear(Color(0, 0, 0));
    
//    mRotation *= angleAxis(cRotationPerFrame, vec3(0, 1, 0));
    auto eyeLocation = mRotation * cInitialEyeLocation;
    mCam.lookAt(eyeLocation, vec3(0));
    gl::setMatrices(mCam);
    
    auto angle = static_cast<float>(0.5 * M_PI * getElapsedSeconds());
    gl::color(0.8, 0.8, 0.8);
    
    mEarth->draw();
    mEarth->setOrbitingAngle(angle);
    mMoon->draw();
    mMoon->setOrbitingAngle(2 * angle);
    mSun->draw();
}

void PlanetsApp::mouseDown(MouseEvent event)
{
}

void PlanetsApp::update()
{
}


CINDER_APP( PlanetsApp, RendererGl(RendererGl::Options().msaa(16)) )
