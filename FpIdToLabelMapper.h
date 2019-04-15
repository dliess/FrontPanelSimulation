#ifndef FP_ID_TO_LABEL_MAPPER_H
#define FP_ID_TO_LABEL_MAPPER_H

#include "FpWidget.h"
#include <string>

namespace fp
{

template<class WTopology>
class IdToLabelMapper
{
public:
    static std::string makeLabelFrom(const Widget& w);
};

} // namespace fp
#include "FpIdToLabelMapper.tcc"
#endif