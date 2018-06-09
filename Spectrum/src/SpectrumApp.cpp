#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/audio.h"

#include <deque>

using namespace ci;
using namespace ci::app;
using namespace std;

class SpectrumApp : public App {
public:
	void setup() override;
	void draw() override;
private:
    gl::BatchRef mSegment;
    CameraPersp mCam;
    audio::MonitorSpectralNodeRef mSpectral;
    
    const int cNumFftBins {50};
    const float cBarWidth {0.05f};
    const float cBarSpacing {0.01f};
    float mMaxMagnitudeSoFar {0};
    
    const size_t cMovingAverageLength {3};
    std::deque<std::vector<float>> mMovingAverageValues;
    
    float calculateMovingAverageValue(int i) const;
};

void SpectrumApp::setup()
{
    mCam.lookAt(vec3(0, 2, 4), vec3(0, 0.8, 0));
    mCam.setFovHorizontal(60);
    
    const auto shaderDef = gl::ShaderDef().color().lambert();
    const auto glsl = gl::getStockShader(shaderDef);
    const auto geom = geom::Cube();
    mSegment = gl::Batch::create(geom, glsl);
    
    gl::enableDepth();
    
    const auto ctx = audio::Context::master();
    ctx->enable();
    const auto device = audio::Device::findDeviceByName("Loopback Audio");
    const auto input = ctx->createInputDeviceNode(device);
    input >> ctx->getOutput();
    input->enable();
    
    mSpectral = ctx->makeNode<audio::MonitorSpectralNode>(audio::MonitorSpectralNode::Format().fftSize(cNumFftBins).windowSize(1024));
    input >> mSpectral;
}

void SpectrumApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCam);
    
    const auto &magSpectrum = mSpectral->getMagSpectrum();

    mMovingAverageValues.push_front(magSpectrum);
    if (mMovingAverageValues.size() > cMovingAverageLength)
        mMovingAverageValues.pop_back();
    
    for (int i = 0; i < cNumFftBins; ++i) {
        const auto mag = calculateMovingAverageValue(i);
        if (mag > mMaxMagnitudeSoFar)
            mMaxMagnitudeSoFar = mag;
        
        const auto magNormalized = mag / mMaxMagnitudeSoFar;
        
        gl::ScopedModelMatrix scpMtx;
        gl::translate(vec3((i - cNumFftBins / 2) * (cBarWidth + cBarSpacing), magNormalized / 2, 0));
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


CINDER_APP( SpectrumApp, RendererGl(RendererGl::Options().msaa(16)) )
