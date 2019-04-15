#ifndef FP_LABEL_TO_ID_MAPPER_H
#define FP_LABEL_TO_ID_MAPPER_H

#include "FpVector2D.h"
#include <string>
#include <map>
#include <cstdint>
#include <tuple>

namespace fp
{

template<class WTopology>
class LabelToIdMapper
{
public:
    LabelToIdMapper();
    Widget mapLabelToId(const std::string& label);
private:
    std::map<std::string, uint8_t> m_label2IdMap;
};

} // namespace fp
#include "FpLabelToIdMapper.tcc"
#endif