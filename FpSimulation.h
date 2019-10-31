#ifndef FP_SIMULATION_H
#define FP_SIMULATION_H

#include "FpButton.h"
#include "FpButton3d.h"
#include "FpButton5d.h"
#include "FpEncoder.h"
#include "FpPotentiometer.h"
#include "FpTouchSurface.h"
#include "FpLed.h"
#include "FpPositioner.h"

#include "FpInputCallbackLayer.h"
#include "FpLedSceneHandler.h"

// todo
#include "FpTopologyContainer.h"
#include "FpIRenderMedium.h"
#include "FpDoubleBufHandler.h"

#include <string>
#include <memory>

class GrpcClient;

namespace fp
{
class DisplayRendererGrpc;
}

namespace fp
{

template<class DeviceTopology>
class Simulation : public Button::IProvider,
                   public Button3d::IProvider,
                   public Button5d::IProvider,
                   public Encoder::IProvider,
                   public Potentiometer::IProvider,
                   public TouchSurface::IProvider,
                   public Led::ISetter,
                   public fp::LedSceneHandler<Push2Device, DeviceTopology::Led, 3>
{
public:
    Simulation(const std::string& serverAddress);
    Simulation(const Simulation&) = delete;

    void updateInputs();

    virtual void registerCB(typename Button::CallbackIf& cb, const Widget& w) override;
    virtual void registerCB(typename Button3d::CallbackIf& cb, const Widget& w) override;
    virtual void registerCB(typename Button5d::CallbackIf& cb, const Widget& w) override;
    virtual void registerCB(typename Encoder::CallbackIf& cb, const Widget& w) override;
    virtual void registerCB(typename Potentiometer::CallbackIf& cb, const Widget& w) override;
    virtual void registerCB(typename TouchSurface::CallbackIf& cb, const Widget& w) override;

    virtual void unRegisterCB(typename Button::CallbackIf& cb, const Widget& w) override;
    virtual void unRegisterCB(typename Button3d::CallbackIf& cb, const Widget& w) override;
    virtual void unRegisterCB(typename Button5d::CallbackIf& cb, const Widget& w) override;
    virtual void unRegisterCB(typename Encoder::CallbackIf& cb, const Widget& w) override;
    virtual void unRegisterCB(typename Potentiometer::CallbackIf& cb, const Widget& w) override;
    virtual void unRegisterCB(typename TouchSurface::CallbackIf& cb, const Widget& w) override;

    fp::Button::PressState getButtonState(const fp::Widget& w) const;

    void setLed(const Widget& w, const Led::ColorRGB& colorRGB) override;
    void movePositioner(const Widget& w, const Positioner::Position& pos);

    IRenderMedium* getRenderMedium(const Widget& w);

    using DisplayTopology = typename DeviceTopology::Display;
private:
   std::unique_ptr<GrpcClient> m_pGrpcClient;

   using ButtonLayer   = InputCallbackLayer<typename DeviceTopology::Button>;
   using Button3dLayer = InputCallbackLayer<typename DeviceTopology::Button3d>;
   using Button5dLayer = InputCallbackLayer<typename DeviceTopology::Button5d>;
   using EncoderLayer  = InputCallbackLayer<typename DeviceTopology::Encoder>;
   using PotentiometerLayer = InputCallbackLayer<typename DeviceTopology::Potentiometer>;
   using TouchSurfaceLayer = InputCallbackLayer<typename DeviceTopology::TouchSurface>;

   void update(ButtonLayer& layer);
   void update(Button3dLayer& layer);
   void update(Button5dLayer& layer);
   void update(EncoderLayer& layer);
   void update(PotentiometerLayer& layer);
   void update(TouchSurfaceLayer& layer);

   ButtonLayer        m_buttonCbLayer;
   Button3dLayer      m_button3dCbLayer;
   Button5dLayer      m_button5dCbLayer;
   EncoderLayer       m_encoderCbLayer;
   PotentiometerLayer m_potentiometerCbLayer;
   TouchSurfaceLayer  m_touchSurfaceCbLayer;

   struct RenderData
   {
       std::unique_ptr<IRenderMedium>                         pMedium;
       std::unique_ptr<DisplayRendererGrpc>                   pGrpcRenderer;
       std::unique_ptr<DoubleBufHandler<DisplayRendererGrpc>> pDoubleBuf;
   };
   TopologyContainer<RenderData, DisplayTopology> m_renderMediumContainer;
};

} // namespace fp

#include "FpSimulation.tcc"

#endif