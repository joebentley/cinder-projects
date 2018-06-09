#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/audio.h"
#include "cinder/Easing.h"

#include <deque>

//#define DEBUG___

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
    audio::MonitorNodeRef mMonitor;
    
    const float cGraphPower {0.7/*1*/};
    const int cNumFftBins {60};
    const float cBarWidth {0.04f};
    const float cBarSpacing {0.01f};
    const float cMinimumBarHeight {0.04f};
    float mMaxMagnitudeSoFar {0.00001f};
    
    const vec2 cMaxScaleFactor {0.3f, 1.6f};
    
    const float cInitialRotationDelay {0.5f};
    const float cCameraZoomAmount {0.1f};
    
    const vec3 cInitialCamPos {0, 1.7, 4};
    const vec3 cLookingAtPos {0, 0.2, 0};
    
    const size_t cMovingAverageLength {3};
    std::deque<std::vector<float>> mMovingAverageValues;
    
    float calculateMovingAverageValue(int i) const;
    
    const size_t cVolumeMovAvgValues {5};
    std::deque<float> mVolumeMovAvgValues;
    float mMaxVolumeSoFar {0.00001f};
    float mMaxVolumeDifferenceSoFar {0.00001f};
    
    const size_t cMovAvgTransientTimesLength {1000};
    std::deque<float> mMovAvgTransientTimes;
    
    gl::GlslProgRef mGlsl;
    vec3 mLightCoord {1.5, 1, -1};
};

void SpectrumApp::setup()
{
    mCam.lookAt(cInitialCamPos, cLookingAtPos);
    mCam.setPerspective(30, getWindowAspectRatio(), 0.1, 1000);
    
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
    const auto &devices = audio::Device::getDevices();
    
    audio::InputDeviceNodeRef input = nullptr;
    
    if (std::find_if(devices.cbegin(), devices.cend(),
                  [] (const audio::DeviceRef &device) { return device->getName() == "Loopback Audio"; }) != devices.cend())
    {
        input = ctx->createInputDeviceNode(audio::Device::findDeviceByName("Loopback Audio"));
        input >> ctx->getOutput();
    } else {
        input = ctx->createInputDeviceNode();
    }
    
    input->enable();
    
    mSpectral = ctx->makeNode<audio::MonitorSpectralNode>(audio::MonitorSpectralNode::Format()
                                                          .fftSize(cNumFftBins).windowSize(1024));
    input >> mSpectral;
    
    mMonitor = ctx->makeNode<audio::MonitorNode>();
    input >> mMonitor;
}

void SpectrumApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCam);
    
    const float anim = fmod(0.3f * getElapsedSeconds() - cInitialRotationDelay, 2.0f);
    
    const auto angle = static_cast<float>(M_PI * (easeInOutAtan(fmod(anim, 1.0f), 5.5f) + (anim > 1.0f ? 1.0f : 0.0f)));
    auto eyePosition = vec3(glm::rotate(angle, vec3(0, 1, 0)) * vec4(cInitialCamPos, 1));
    
    const auto volume = mMonitor->getVolume();
    mVolumeMovAvgValues.push_front(volume);
    if (mVolumeMovAvgValues.size() > cVolumeMovAvgValues)
        mVolumeMovAvgValues.pop_back();
    
    float averageVolume = 0;
    for (const auto vol : mVolumeMovAvgValues)
        averageVolume += vol;
    
    averageVolume /= cVolumeMovAvgValues;
    if (averageVolume > mMaxVolumeSoFar)
        mMaxVolumeSoFar = averageVolume;
    const auto normalizedVolume = averageVolume / mMaxVolumeSoFar;
    
    const auto zoom = 1 - cCameraZoomAmount * normalizedVolume;
    eyePosition = zoom * eyePosition;
    
    mCam.lookAt(eyePosition, cLookingAtPos);
    
    mGlsl->uniform("uLightCoord", glm::normalize(mLightCoord));
    
#ifdef DEBUG___
    gl::pushMatrices();
    gl::translate(mLightCoord);
    mDebugLight->draw();
    gl::popMatrices();
    
    gl::pushMatrices();
    const auto vol = 2 * normalizedVolume;
    gl::translate(vec3(0.5, vol, 0.5));
    mDebugLight->draw();
    gl::popMatrices();
#endif
    
    const auto &magSpectrum = mSpectral->getMagSpectrum();

    mMovingAverageValues.push_front(magSpectrum);
    if (mMovingAverageValues.size() > cMovingAverageLength)
        mMovingAverageValues.pop_back();
    
    // TODO: Sort out transient stuff
//    float averageBassMagnitude = 0;
    for (int i = 0; i < cNumFftBins; ++i) {
        const auto mag = calculateMovingAverageValue(i);
        
        if (mag > mMaxMagnitudeSoFar)
            mMaxMagnitudeSoFar = mag;
        
        const auto magNormalized = cMinimumBarHeight
                                 + (1 - cMinimumBarHeight) * pow(mag / mMaxMagnitudeSoFar, cGraphPower);
        
        gl::ScopedModelMatrix scpMtx;
        gl::ScopedColor scpCol;
        gl::translate(vec3((i - cNumFftBins / 2 + 1) * (cBarWidth + cBarSpacing), /*0*/ cMaxScaleFactor.x * magNormalized, 0));
        gl::scale(vec3(cBarWidth, cMaxScaleFactor * magNormalized));
        gl::color(Color(CM_HSV, (float)i / cNumFftBins, 1, 1));
        mSegment->draw();
        
//        if (i < 6)
//            averageBassMagnitude += magNormalized;
    }
    
//    averageBassMagnitude /= 6;
//
//    if (averageBassMagnitude > 0.4) {
//        // transient kind-of detected (maybe?)
//
//        mMovAvgTransientTimes.push_back((float)getElapsedSeconds());
//        if (mMovAvgTransientTimes.size() > cMovAvgTransientTimesLength)
//            mMovAvgTransientTimes.pop_front();
//
//        float avgTimeBetweenTransient = 0;
//        float prevTime = 0;
//        for (auto val : mMovAvgTransientTimes) {
//            avgTimeBetweenTransient = val - prevTime;
//            prevTime = avgTimeBetweenTransient;
//        }
//
//        avgTimeBetweenTransient /= (mMovAvgTransientTimes.size() - 1);
//
//        const auto avgBPM = 60.0f / avgTimeBetweenTransient;
//
//        console() << avgTimeBetweenTransient << " " << avgBPM << std::endl;
//    }
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
    settings->setWindowSize(ivec2(800, 400));
//    settings->setResizable(false);
}

CINDER_APP( SpectrumApp, RendererGl(RendererGl::Options().msaa(16)), settingsFn )
