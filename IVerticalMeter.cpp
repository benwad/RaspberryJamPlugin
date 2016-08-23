//
//  IVerticalMeter.cpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 23/08/2016.
//
//

#include "IVerticalMeter.hpp"

IVerticalMeter::IVerticalMeter(IPlugBase* pPlug, IRECT pR)
: IControl(pPlug, pR)
{
    mColor = COLOR_BLUE;
}


bool IVerticalMeter::Draw(IGraphics* pGraphics)
{
    pGraphics->FillIRect(&COLOR_GREEN, &mRECT);
    
    IRECT filledBit = IRECT(mRECT.L, mRECT.T, mRECT.R , mRECT.B - (mValue * mRECT.H()));
    pGraphics->FillIRect(&mColor, &filledBit);
    return true;
}
