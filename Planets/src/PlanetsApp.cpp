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
    Planet &setTexture(gl::Texture2dRef tex) { mTex = tex; return *this; }
    
    Planet &setOrbitingAngleFrequency(float omega) { mOrbitingAngleFrequency = omega; return *this; }
    Planet &setAngularFrequency(float omega) { mAngularFreq = omega; return *this; }
    
    void animate(float timeInSeconds);
    
    mat4 getModelMatrix();
    mat4 getModelMatrixWithScaling() {
        return getModelMatrix() * glm::scale(vec3(mRadius));
    }
    
    void draw(vec3 lightCoord);
    void draw();
private:
    gl::BatchRef mBatch;
    gl::Texture2dRef mTex {nullptr};
    mat4 mOrbitingPos;
    Planet *mOrbiting {nullptr};
    
    float mRadius {1};
    float mOrbitingRadius {3};
    float mOrbitingAngle {0};
    float mOrbitingAngleFrequency {0};
    float mAngle {0};
    float mAngularFreq {0};
    
    void maybeBindTexture();
    
    // Cache model matrix
    bool mDirtyModelMatrix {true};
    mat4 mModelMatrix;
};

void Planet::maybeBindTexture() {
    if (mTex != nullptr) {
        mTex->bind(0);
        mBatch->getGlslProg()->uniform("uTex0", 0);
    }
}

mat4 Planet::getModelMatrix() {
    if (!mDirtyModelMatrix)
        return mModelMatrix;
    
    mat4 modelMatrix;
    
    if (mOrbiting != nullptr)
        modelMatrix *= mOrbiting->getModelMatrix();
    modelMatrix *= glm::rotate(mOrbitingAngle, vec3(0, 1, 0));
    modelMatrix *= glm::translate(vec3(mOrbitingRadius, 0, 0));
    modelMatrix *= glm::rotate(mAngle, vec3(0, 1, 0));
    
    mDirtyModelMatrix = false;
    mModelMatrix = modelMatrix;
    return mModelMatrix;
}

void Planet::draw(vec3 lightCoord) {
    gl::ScopedModelMatrix scpMtx;
    gl::multModelMatrix(getModelMatrixWithScaling());
    auto invModelMatrix = glm::inverse(getModelMatrix());
    mBatch->getGlslProg()->uniform("uLightCoord",
                                    glm::normalize(vec3(invModelMatrix * vec4(lightCoord, 1))));
    
    maybeBindTexture();
    mBatch->draw();
}

void Planet::draw() {
    gl::ScopedModelMatrix scpMtx;
    gl::multModelMatrix(getModelMatrixWithScaling());
    maybeBindTexture();
    mBatch->draw();
}

void Planet::animate(float timeInSeconds) {
    mOrbitingAngle = mOrbitingAngleFrequency * timeInSeconds;
    mAngle = mAngularFreq * timeInSeconds;
    mDirtyModelMatrix = true;
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
    vec3 cInitialEyeLocation {3.0f * vec3(4, 3, 4)};
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
    auto glslNoLight = gl::GlslProg::create(gl::GlslProg::Format()
                                     .vertex(loadAsset("shaders/vertex.glsl"))
                                     .fragment(loadAsset("shaders/texture.glsl")));
    auto glsl = gl::GlslProg::create(gl::GlslProg::Format()
                                    .vertex(loadAsset("shaders/vertex.glsl"))
                                    .fragment(loadAsset("shaders/textureLambert.glsl")));
    
    auto geomSphere = geom::Sphere().subdivisions(50);
    auto planetBatch = gl::Batch::create(geomSphere, glsl);
    auto sunBatch = gl::Batch::create(geomSphere, glslNoLight);
    
    mSun = std::make_unique<Planet>(sunBatch);
    mSun->setOrbitingRadius(0).setRadius(1.4f)
        .setAngularFrequency(2 * M_PI * 0.01f)
        .setTexture(gl::Texture2d::create(loadImage(loadAsset("textures/sun.jpg"))));
    mEarth = std::make_unique<Planet>(planetBatch);
    mEarth->setOrbiting(mSun.get()).setOrbitingRadius(5)
        .setOrbitingAngleFrequency(2 * M_PI * 0.1f)
        .setAngularFrequency(2 * M_PI * 0.05f)
        .setTexture(gl::Texture2d::create(loadImage(loadAsset("textures/earth.jpg"))));;
    mMoon = std::make_unique<Planet>(planetBatch);
    mMoon->setOrbiting(mEarth.get()).setOrbitingRadius(2).setRadius(0.4)
        .setOrbitingAngleFrequency(2 * M_PI * 0.1f)
        .setAngularFrequency(2 * M_PI * 0.1f)
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
    
    gl::color(0.8, 0.8, 0.8);
    
    mEarth->draw(mLightCoord);
    mEarth->animate(getElapsedSeconds());
    mMoon->draw(mLightCoord);
    mMoon->animate(getElapsedSeconds());
    mSun->draw();
    mSun->animate(getElapsedSeconds());
}

void PlanetsApp::mouseDown(MouseEvent event)
{
}

void PlanetsApp::update()
{
}


CINDER_APP( PlanetsApp, RendererGl(RendererGl::Options().msaa(16)) )
