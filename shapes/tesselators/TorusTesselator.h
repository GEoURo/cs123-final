#ifndef TORUSTESSELATOR_H
#define TORUSTESSELATOR_H

#include "BaseTesselator.h"

class TorusTesselator : public BaseTesselator
{
public:
    TorusTesselator();

    void setRingSlice(int ringSlice);
    void setBarrelSlice(int barrelSlice);
    void setRadiusPercent(float radiusPercent);

    void tesselate() override;

protected:
    int verticesNum() override;

private:
    int m_ringSlice;
    int m_barrelSlice;
    int m_radiusPercent;
};

#endif // TORUSTESSELATOR_H
