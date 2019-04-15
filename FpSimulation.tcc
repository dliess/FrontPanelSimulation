#include "FpSimulation.h"
#include "GrpcClient.h"
#include "FpLabelToIdMapper.h"
#include "FpIdToLabelMapper.h"
#include "FpDisplayRendererGrpc.h"
#include <vector>

// todo
#include "FpRenderSwitch.h"

using namespace fp;

template<class DeviceTopology>
Simulation<DeviceTopology>::Simulation(const std::string& serverAddress) :
    m_pGrpcClient(new GrpcClient(serverAddress))
{
   m_renderMediumContainer.forEach([this](RenderData& rRenderData, const Widget& w){
       const auto resolution = DeviceTopology::Display::getResolution(static_cast<typename DeviceTopology::Display::Id>(w.id));
       rRenderData.pGrpcRenderer.reset(new DisplayRendererGrpc(
           *m_pGrpcClient, 
           IdToLabelMapper<typename DeviceTopology::Display>::makeLabelFrom(w),
           resolution
        ));
       rRenderData.pDoubleBuf.reset(new DoubleBufHandler<DisplayRendererGrpc>(
           *rRenderData.pGrpcRenderer
        ));
       rRenderData.pMedium.reset(new RenderSwitch< DisplayRendererGrpc, DoubleBufHandler<DisplayRendererGrpc> >(
           *rRenderData.pGrpcRenderer,
           *rRenderData.pDoubleBuf
       ));
   });
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::updateInputs()
{
    update(m_buttonCbLayer);
    m_buttonCbLayer.checkValuesAndInvokeCallbacks();
    update(m_button3dCbLayer);
    m_button3dCbLayer.checkValuesAndInvokeCallbacks();
    update(m_button5dCbLayer);
    m_button5dCbLayer.checkValuesAndInvokeCallbacks();
    update(m_encoderCbLayer);
    m_encoderCbLayer.checkValuesAndInvokeCallbacks();
    update(m_potentiometerCbLayer);
    m_potentiometerCbLayer.checkValuesAndInvokeCallbacks();
    update(m_touchSurfaceCbLayer);
    m_touchSurfaceCbLayer.checkValuesAndInvokeCallbacks();
}


template<class DeviceTopology>
void Simulation<DeviceTopology>::registerCB(typename Button::CallbackIf& cb, const Widget& w)
{
    m_buttonCbLayer.registerCB(cb, w);
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::registerCB(typename Button3d::CallbackIf& cb, const Widget& w)
{
    m_button3dCbLayer.registerCB(cb, w);
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::registerCB(typename Button5d::CallbackIf& cb, const Widget& w)
{
    m_button5dCbLayer.registerCB(cb, w);
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::registerCB(typename Encoder::CallbackIf& cb, const Widget& w)
{
    m_encoderCbLayer.registerCB(cb, w);
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::registerCB(typename Potentiometer::CallbackIf& cb, const Widget& w)
{
    m_potentiometerCbLayer.registerCB(cb, w);
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::registerCB(typename TouchSurface::CallbackIf& cb, const Widget& w)
{
    m_touchSurfaceCbLayer.registerCB(cb, w);
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::update(ButtonLayer& layer)
{
    std::vector<GetButtonStateAnswer> changedBtnStates;
    if(!m_pGrpcClient->getButtonStates(changedBtnStates))
    {
        return;
    }
    for(auto it = begin(changedBtnStates); it != end(changedBtnStates); ++it)
    {
        static LabelToIdMapper<typename DeviceTopology::Button> labelToIdMapper;
        auto widgetId = labelToIdMapper.mapLabelToId(it->objlabel());
        auto pValueHandler = layer.get(widgetId);
        if(pValueHandler)
        {
            switch(it->state())
            {
                case ButtonState::BUTTON_PRESSED:
                    pValueHandler->pressState.set(Button::Pressed);
                break;
                case ButtonState::BUTTON_RELEASED:
                default:
                    pValueHandler->pressState.set(Button::Released);
                break;
            }
        }
    }
}


template<class DeviceTopology>
void Simulation<DeviceTopology>::update(Button3dLayer& layer)
{
    std::vector<GetButton3dStateAnswer> changedBtnStates;
    if(!m_pGrpcClient->getButton3dStates(changedBtnStates))
    {
        return;
    }
    for(auto it = begin(changedBtnStates); it != end(changedBtnStates); ++it)
    {
        static LabelToIdMapper<typename DeviceTopology::Button3d> labelToIdMapper;
        auto widgetId = labelToIdMapper.mapLabelToId(it->objlabel());
        auto pValueHandler = layer.get(widgetId);
        if(pValueHandler)
        {
            switch(it->state())
            {
                case ButtonState::BUTTON_PRESSED:
                    pValueHandler->pressState.set(Button3d::Pressed);
                break;
                case ButtonState::BUTTON_RELEASED:
                default:
                    pValueHandler->pressState.set(Button3d::Released);
                break;
            }
        }
    }
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::update(Button5dLayer& layer)
{
    std::vector<GetButton5dStateAnswer> changedBtnStates;
    if(!m_pGrpcClient->getButton5dStates(changedBtnStates))
    {
        return;
    }
    for(auto it = begin(changedBtnStates); it != end(changedBtnStates); ++it)
    {
        static LabelToIdMapper<typename DeviceTopology::Button5d> labelToIdMapper;
        auto widgetId = labelToIdMapper.mapLabelToId(it->objlabel());
        auto pValueHandler = layer.get(widgetId);
        if(pValueHandler)
        {
            switch(it->state())
            {
                case ButtonState::BUTTON_PRESSED:
                    pValueHandler->pressState.set(Button5d::Pressed);
                break;
                case ButtonState::BUTTON_RELEASED:
                default:
                    pValueHandler->pressState.set(Button5d::Released);
                break;
            }
        }
    }
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::update(EncoderLayer& layer)
{
    std::vector<GetEncoderIncrementsAnswer> encoderIncrements;
    if(!m_pGrpcClient->getEncoderIncrements(encoderIncrements))
    {
        return;
    }
    for(auto it = begin(encoderIncrements); it != end(encoderIncrements); ++it)
    {
        static LabelToIdMapper<typename DeviceTopology::Encoder> labelToIdMapper;
        auto widgetId = labelToIdMapper.mapLabelToId(it->objlabel());
        auto pIncrHolder = layer.get(widgetId);
        if(pIncrHolder)
        {
            pIncrHolder->increments.set(it->increments());
        }
    }
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::update(PotentiometerLayer& layer)
{
    std::vector<GetPotentiometerValuesAnswer> potentiometerVals;
    if(!m_pGrpcClient->getPotentiometerValues(potentiometerVals))
    {
        return;
    }
    for(auto it = begin(potentiometerVals); it != end(potentiometerVals); ++it)
    {
        static LabelToIdMapper<typename DeviceTopology::Potentiometer> labelToIdMapper;
        auto widgetId = labelToIdMapper.mapLabelToId(it->objlabel());
        auto pValueHandler = layer.get(widgetId);
        if(pValueHandler)
        {
            pValueHandler->value.set(it->value());
        }
    }
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::update(TouchSurfaceLayer& layer)
{
    std::vector<GetTouchValueAnswer> touchVals;
    if(!m_pGrpcClient->getTouchValue(touchVals))
    {
        return;
    }
    for(auto it = begin(touchVals); it != end(touchVals); ++it)
    {
        static LabelToIdMapper<typename DeviceTopology::TouchSurface> labelToIdMapper;
        auto widgetId = labelToIdMapper.mapLabelToId(it->objlabel());
        auto pValueHandler = layer.get(widgetId);
        if(pValueHandler)
        {
            switch(it->touchstate())
            {
                case TouchState::TOUCHED:
                    pValueHandler->touchState.set( TouchSurface::Touched );
                break;
                case TouchState::RELEASED:
                default:
                    pValueHandler->touchState.set( TouchSurface::Released );
                break;
            }
            pValueHandler->pos.set( {it->pos().x(), it->pos().y()} );
        }
    }
}

template<class DeviceTopology>
void Simulation<DeviceTopology>::setLed(const Widget& w, const Led::ColorRGB& colorRGB)
{
    forWidget<typename DeviceTopology::Led>(w, [this, &colorRGB](const Widget& w){
        LedSetColorRequest req;
        auto label = IdToLabelMapper<typename DeviceTopology::Led>::makeLabelFrom(w);
        req.set_objlabel(label);
        Color pixColor;
        pixColor.set_r(colorRGB.r);
        pixColor.set_g(colorRGB.g);
        pixColor.set_b(colorRGB.b);
        pixColor.set_a(255);
        req.set_allocated_color(&pixColor);
        if(!m_pGrpcClient->led_setColor(req))
        {
            std::cout << "led_setColor failed for label: '" << label << "'" << std::endl;
        }
        req.release_color();
    });
}
    
template<class DeviceTopology>
void Simulation<DeviceTopology>::movePositioner(const Widget& w, const Positioner::Position& pos)
{
    forWidget<typename DeviceTopology::Positioner>(w, [this, &pos](const Widget& w){
        MovePotentiometerRequest req;
        auto label = IdToLabelMapper<typename DeviceTopology::Positioner>::makeLabelFrom(w);
        req.set_objlabel(label);
        req.set_value(pos);
        if(!m_pGrpcClient->movePotentiometerToValue(req))
        {
            std::cout << "movePotentiometerToValue failed for label: '" << label << "'" << std::endl;
        }
    });
}

template<class DeviceTopology>
IRenderMedium* Simulation<DeviceTopology>::getRenderMedium(const Widget& w)
{
    auto pRenderData = m_renderMediumContainer.get(w);
    if(pRenderData)
    {
        return pRenderData->pMedium.get(); 
    }
    else
    {
        return nullptr;
    }
}