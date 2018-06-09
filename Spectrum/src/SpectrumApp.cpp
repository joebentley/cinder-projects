#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/audio.h"

#include <deque>

#define DEBUG___

using namespace ci;
using namespace ci::app;
using namespace std;

class SpectrumApp : public App {
public:
	void setup() override;
	void draw() override;
    void keyDown(KeyEvent event) override;
private:
#ifdef DEBUG___
    gl::BatchRef mDebugLight;
#endif
    
    gl::BatchRef mSegment;
    CameraPersp mCam;
    audio::MonitorSpectralNodeRef mSpectral;
    
    const float cGraphPower {0.7/*1*/};
    const int cNumFftBins {60};
    const float cBarWidth {0.04f};
    const float cBarSpacing {0.01f};
    float mMaxMagnitudeSoFar {0};
    
    const size_t cMovingAverageLength {3};
    std::deque<std::vector<float>> mMovingAverageValues;
    
    float calculateMovingAverageValue(int i) const;
    
    gl::GlslProgRef mGlsl;
    vec3 mLightCoord {1, 1, -1};
};

void SpectrumApp::setup()
{
    mCam.lookAt(vec3(0, 1.5, 4), vec3(0, 0, 0));
    mCam.setFovHorizontal(60);
    
    const auto geom = geom::Cube();
    
#ifdef DEBUG___
    auto shader = gl::ShaderDef().color();
    mDebugLight = gl::Batch::create(geom >> geom::Scale(0.2), gl::getStockShader(shader));
#endif
    
    mGlsl = gl::GlslProg::create(gl::GlslProg::Format()
                                 .vertex(loadAsset("vertex.glsl"))
                                 .fragment(loadAsset("fragment.glsl")));
    
    mSegment = gl::Batch::create(geom, mGlsl);
    
    gl::enableDepth();
    
    const auto ctx = audio::Context::master();
    ctx->enable();
    const auto device = audio::Device::findDeviceByName("Loopback Audio");
    const auto input = ctx->createInputDeviceNode(device);
    input >> ctx->getOutput();
    input->enable();
    
    mSpectral = ctx->makeNode<audio::MonitorSpectralNode>(audio::MonitorSpectralNode::Format()
                                                          .fftSize(cNumFftBins).windowSize(1024));
    input >> mSpectral;
}

void SpectrumApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCam);
    
    mGlsl->uniform("uLightCoord", glm::normalize(mLightCoord));
    
#ifdef DEBUG___
    {
        gl::ScopedModelMatrix scpMtx;
        gl::translate(mLightCoord);
        mDebugLight->draw();
    }
#endif
    
    const auto &magSpectrum = mSpectral->getMagSpectrum();

    mMovingAverageValues.push_front(magSpectrum);
    if (mMovingAverageValues.size() > cMovingAverageLength)
        mMovingAverageValues.pop_back();
    
    for (int i = 0; i < cNumFftBins; ++i) {
        const auto mag = calculateMovingAverageValue(i);
        if (mag > mMaxMagnitudeSoFar)
            mMaxMagnitudeSoFar = mag;
        
        const auto magNormalized = pow(mag / mMaxMagnitudeSoFar, cGraphPower);
        
        gl::ScopedModelMatrix scpMtx;
        gl::ScopedColor scpCol;
        gl::translate(vec3((i - cNumFftBins / 2 + 1) * (cBarWidth + cBarSpacing), magNormalized / 2, 0));
        gl::scale(vec3(cBarWidth, 0.5 * magNormalized, 3 * magNormalized));
        gl::color(Color(CM_HSV, (float)i / cNumFftBins, 1, 1));
        mSegment->draw();
    }
}

float SpectrumApp::calculateMovingAverageValue(int i) const {
    if (mMovingAverageValues.empty())
        return 0;
    
    float sum = 0;
    
    for (const auto &vec : mMovingAverageValues) {
        auto val = vec[i];
        sum += val;
    }
    
    return sum / cMovingAverageLength;
}

void SpectrumApp::keyDown(cinder::app::KeyEvent event) {
#ifdef DEBUG___
    switch (event.getCode()) {
        case event.KEY_UP:
            mLightCoord += vec3(0, 0, -0.1);
            break;
        case event.KEY_DOWN:
            mLightCoord += vec3(0, 0, 0.1);
            break;
        case event.KEY_LEFT:
            mLightCoord += vec3(-0.1, 0, 0);
            break;
        case event.KEY_RIGHT:
            mLightCoord += vec3(0.1, 0, 0);
            break;
    }
#endif
}

void settingsFn(SpectrumApp::Settings *settings) {
    settings->setHighDensityDisplayEnabled(true);
    settings->setWindowSize(ivec2(600, 300));
//    settings->setResizable(false);
}

CINDER_APP( SpectrumApp, RendererGl(RendererGl::Options().msaa(16)), settingsFn )
