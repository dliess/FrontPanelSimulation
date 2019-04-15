#include "FpDisplayRendererGrpc.h"
#include "GrpcClient.h"

using namespace fp;

DisplayRendererGrpc::DisplayRendererGrpc(GrpcClient& rGrpcClient,
                                         const std::string& m_label,
                                         const gfx::Resolution& resolution) :
    m_rGrpcClient(rGrpcClient),
    m_label(m_label),
    m_resolution(resolution)
{
}

DisplayRendererGrpc::~DisplayRendererGrpc()
{
}


void DisplayRendererGrpc::drawPixel( const gfx::Coord& pixPos,
                                     const ColorRGB& colorRGB)
{
    DisplaySetPixelsRequest_ARGB32 req;
    Duration answer;
    req.set_objlabel(m_label);
    PixelDataList_ARGB32 pixelDataList;
    PixelPos pixelPos;
    pixelPos.set_x(pixPos.x);
    pixelPos.set_y(pixPos.y); 
    auto pPixelData = pixelDataList.add_pixeldata();
    pPixelData->set_allocated_pos(&pixelPos);
    pPixelData->set_rgba(colorRGB.toUint32_0xffRRGGBB());
    req.set_allocated_pixeldatalist(&pixelDataList);
    if(!m_rGrpcClient.display_setPixels_ARGB32(req, answer))
    {
        std::cout << "display_setPixels_ARGB32 failed for m_label: '" << m_label << "'" << std::endl;
    }
    req.release_pixeldatalist();
    pPixelData->release_pos();
}

void DisplayRendererGrpc::streamToSubWindow(const gfx::Rectangle& subWindow,
                                            const ColorRGB*  pPixelStream)
{
    DisplaySubWindowPixelsRequest_ARGB32 req;
    Duration answer;
    req.set_objlabel(m_label);
    DisplaySubWindowData_ARGB32 data;
    PixelPos upperLeft;
    upperLeft.set_x(subWindow.upperLeft.x);
    upperLeft.set_y(subWindow.upperLeft.y);
    PixelPos lowerRight;
    lowerRight.set_x(subWindow.lowerRight.x);
    lowerRight.set_y(subWindow.lowerRight.y);
    Color pixColor;
    data.set_allocated_p1(&upperLeft);
    data.set_allocated_p2(&lowerRight);
    data.add_rgbastream(0);
    for(gfx::Pixel subY = 0; subY < subWindow.height(); ++subY)
    {
        for(gfx::Pixel subX = 0; subX < subWindow.width(); ++subX)
        {
            data.add_rgbastream(pPixelStream[subX + (subY * subWindow.width())].toUint32_0xffRRGGBB());
        }
    }
    req.set_allocated_data(&data);
    if(!m_rGrpcClient.display_setSubWindowPixels_ARGB32(req, answer))
    {
        std::cout << "display_setSubWindowPixels_ARGB32 failed for m_label: '" << m_label << "'" << std::endl;
    }
    req.release_data();
    data.release_p2();
    data.release_p1();
}

void DisplayRendererGrpc::streamFromFramebuf(const gfx::Rectangle& subWindow,
                                             const Vector2d<uint32_t>&      resolution,
                                             const ColorRGB*  pFrameBuf)
{
    DisplaySubWindowPixelsRequest_ARGB32 req;
    Duration answer;
    req.set_objlabel(m_label);
    DisplaySubWindowData_ARGB32 data;
    PixelPos upperLeft;
    upperLeft.set_x(subWindow.upperLeft.x);
    upperLeft.set_y(subWindow.upperLeft.y);
    PixelPos lowerRight;
    lowerRight.set_x(subWindow.lowerRight.x);
    lowerRight.set_y(subWindow.lowerRight.y);
    Color pixColor;
    data.set_allocated_p1(&upperLeft);
    data.set_allocated_p2(&lowerRight);
    for(gfx::Pixel y = subWindow.upperLeft.y; y <= subWindow.lowerRight.y; ++y)
    {
        for(gfx::Pixel x = subWindow.upperLeft.x; x <= subWindow.lowerRight.x; ++x)
        {
            data.add_rgbastream(pFrameBuf[x + (y * resolution.x)].toUint32_0xffRRGGBB());
        }
    }       
    req.set_allocated_data(&data);
    if(!m_rGrpcClient.display_setSubWindowPixels_ARGB32(req, answer))
    {
        std::cout << "display_setSubWindowPixels_ARGB32 failed for m_label: '" << m_label << "'" << std::endl;
    }
    req.release_data();
    data.release_p2();
    data.release_p1();
}

void DisplayRendererGrpc::drawLine( const gfx::Coord&    p1,
                                    const gfx::Coord&    p2,
                                    const ColorRGB& colorRGB)
{
    DisplayDrawLineRequest req;
    req.set_objlabel(m_label);
    PixelPos pixPos1;
    pixPos1.set_x(p1.x);
    pixPos1.set_y(p1.y);
    PixelPos pixPos2;
    pixPos2.set_x(p2.x);
    pixPos2.set_y(p2.y);
    Color pixColor;
    pixColor.set_r(colorRGB.r());
    pixColor.set_g(colorRGB.g());
    pixColor.set_b(colorRGB.b());
    pixColor.set_a(255);
    LineData lineData;
    lineData.set_allocated_p1(&pixPos1);
    lineData.set_allocated_p2(&pixPos2);
    lineData.set_allocated_pixelcolor(&pixColor);
    req.set_allocated_data(&lineData);
    if(!m_rGrpcClient.display_drawLine(req))
    {
        std::cout << "display_drawLine failed for m_label: '" << m_label << "'" << std::endl;
    }
    req.release_data();
    lineData.release_pixelcolor();
    lineData.release_p2();
    lineData.release_p1();
}

void DisplayRendererGrpc::drawHLine(const gfx::Coord&    p1,
                                    gfx::Pixel           width,
                                    const ColorRGB& colorRGB)
{
    gfx::Coord p2(p1);
    p2.x += width - 1;
    drawLine(p1, p2, colorRGB);
}

void DisplayRendererGrpc::drawVLine(const gfx::Coord&    p1,
                                    gfx::Pixel           height,
                                    const ColorRGB& colorRGB)
{
    gfx::Coord p2(p1);
    p2.y += height - 1;
    drawLine(p1, p2, colorRGB);
}

void DisplayRendererGrpc::drawRectangle(const gfx::Rectangle& rect,
                                        const ColorRGB&  colorRGB,
                                        bool                           filled)
{
    DisplayDrawRectangleRequest req;
    req.set_objlabel(m_label);
    PixelPos pixPos1;
    pixPos1.set_x(rect.upperLeft.x); 
    pixPos1.set_y(rect.upperLeft.y); 
    PixelPos pixPos2;
    pixPos2.set_x(rect.lowerRight.x); 
    pixPos2.set_y(rect.lowerRight.y); 
    Color pixColor;
    pixColor.set_r(colorRGB.r());
    pixColor.set_g(colorRGB.g());
    pixColor.set_b(colorRGB.b());
    pixColor.set_a(255);
    RectangleData rectangleData;
    rectangleData.set_allocated_p1(&pixPos1);
    rectangleData.set_allocated_p2(&pixPos2);
    rectangleData.set_allocated_pixelcolor(&pixColor);
    rectangleData.set_filled(filled);
    req.set_allocated_data(&rectangleData);
    if(!m_rGrpcClient.display_drawRectangle(req))
    {
        std::cout << "display_drawRectangle failed for m_label: '" << m_label << "'" << std::endl;
    }
    req.release_data();
    rectangleData.release_pixelcolor();
    rectangleData.release_p2();
    rectangleData.release_p1();
}

uint32_t DisplayRendererGrpc::getDiscretePixelOverFrameStreamPercentage()
{
    return 5;
}

gfx::Resolution DisplayRendererGrpc::getResolution() const
{
    return m_resolution;
}
