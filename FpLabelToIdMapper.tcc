#include "FpLabelToIdMapper.h"
#include <cassert>

using namespace fp;

inline
std::tuple<std::string, Vector2d<uint8_t>> extractNameAndCoordinates(const std::string& label)
{
    auto firstOpenBracket = label.find('[');
    if(std::string::npos == firstOpenBracket)
    {
        return  std::make_tuple(label, Vector2d<uint8_t>(0, 0));
    }
    std::string labelName = label.substr(0, firstOpenBracket);
    auto firstCloseBracket = label.find(']', firstOpenBracket);
    if(std::string::npos == firstCloseBracket)
    {
        return  std::make_tuple(labelName, Vector2d<uint8_t>(0, 0));
    }
    auto index1 = std::stoi(label.substr(firstOpenBracket + 1, firstCloseBracket));
    auto secondOpenBracket = label.find('[', firstCloseBracket);
    if(secondOpenBracket != firstCloseBracket + 1)
    {
        return  std::make_tuple(labelName, Vector2d<uint8_t>(index1, 0));
    }
    auto secondCloseBracket = label.find(']', secondOpenBracket);
    if(std::string::npos == secondCloseBracket)
    {
        return  std::make_tuple(labelName, Vector2d<uint8_t>(index1, 0));
    }
    auto index2 = std::stoi(label.substr(secondOpenBracket + 1, secondCloseBracket));
    return  std::make_tuple(labelName, Vector2d<uint8_t>(index1, index2));
}

template<class WTopology>
LabelToIdMapper<WTopology>::LabelToIdMapper()
{
    for(unsigned int i = 0; i <= WTopology::Id::eLast; ++i)
    {
        m_label2IdMap[WTopology::id2Str(static_cast<typename WTopology::Id>(i))] = i;
    }
}

template<class WTopology>
Widget LabelToIdMapper<WTopology>::mapLabelToId(const std::string& label)
{
    std::string idLabel;
    Vector2d<uint8_t> coord;
    std::tie(idLabel, coord) = extractNameAndCoordinates(label);
    auto iter = m_label2IdMap.find(idLabel);
    if(iter == m_label2IdMap.end())
    {
        return Widget(WTopology::Id::eLast, Vector2d<uint8_t>(0, 0));
    }
    auto id = iter->second;
    return Widget(id, coord);
}
