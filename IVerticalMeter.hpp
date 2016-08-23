//
//  IVerticalMeter.hpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 23/08/2016.
//
//

#ifndef IVerticalMeter_hpp
#define IVerticalMeter_hpp

#include <stdio.h>

#include "RaspberryJammer.h"

class IVerticalMeter : public IControl
{
public:
    IVerticalMeter(IPlugBase* pPlug, IRECT pR);
    ~IVerticalMeter() {}
    bool Draw(IGraphics* pGraphics);
    bool IsDirty() { return true; }
protected:
    IColor mColor;
};

#endif /* IVerticalMeter_hpp */
