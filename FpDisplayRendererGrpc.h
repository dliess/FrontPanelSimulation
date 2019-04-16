#ifndef FP_DISPLAY_RENDERER_GRPC_H
#define FP_DISPLAY_RENDERER_GRPC_H

#include "FpRenderBase.h"
#include <string>

class GrpcClient;

class IdToLabelMapper;

namespace fp
{

class DisplayRendererGrpc : public RenderBase
{
public:
    DisplayRendererGrpc(GrpcClient& rGrpcClient,
                        const std::string& label,
                        const gfx::Resolution& resolution);
    ~DisplayRendererGrpc();
    virtual void drawPixel( const gfx::Coord&    pixPos,
                            const ColorRGB& colorRGB) override;
    virtual void streamToSubWindow(const gfx::Rectangle& subWindow,
                                   const ColorRGB*  pPixelStream) override;
    virtual void streamFromFramebuf(const gfx::Rectangle& subWindow,
                                    const Vector2d<uint32_t>&      resolution,
                                    const ColorRGB*  pFrameBuf) override;
    virtual void drawLine(  const gfx::Coord&    p1,
                            const gfx::Coord&    p2,
                            const ColorRGB& colorRGB) override;
    virtual void drawHLine( const gfx::Coord&    p1,
                            gfx::Pixel           width,
                            const ColorRGB& colorRGB) override;
    virtual void drawVLine( const gfx::Coord&    p1,
                            gfx::Pixel           height,
                            const ColorRGB& colorRGB) override;
    virtual void drawRectangle( const gfx::Rectangle& rect,
                                const ColorRGB&  colorRGB,
                                bool             filled) override;
    virtual uint32_t getDiscretePixelOverFrameStreamPercentage();

    virtual gfx::Resolution getResolution() const override;

private:
    GrpcClient&     m_rGrpcClient;
    std::string     m_label;
    gfx::Resolution m_resolution;
};

} // namespace fp
#endif