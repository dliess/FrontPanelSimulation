#include "FpIdToLabelMapper.h"

using namespace fp;

template<class WTopology>
std::string IdToLabelMapper<WTopology>::makeLabelFrom(const Widget& w)
{
    if(w.id > WTopology::Id::eLast)
    {
        return std::string();
    }
    auto wId = static_cast<typename WTopology::Id>(w.id);
    const auto dim = WTopology::getDim(wId);
    std::string label = WTopology::id2Str(wId);
    if( dim.x > 1 )
    {
        label.append("[");
        label.append(std::to_string(w.coord.x));
        label.append("]");
    }
    if( dim.y > 1 )
    {
        label.append("[");
        label.append(std::to_string(w.coord.y));
        label.append("]");
    }
    return label;
}