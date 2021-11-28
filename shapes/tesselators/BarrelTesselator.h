#ifndef BARRELTESSELATOR_H
#define BARRELTESSELATOR_H

#include "BaseTesselator.h"

class BarrelTesselator : public BaseTesselator
{
public:
    BarrelTesselator();

    void setStackNum(int stackNum);
    void setSliceNum(int sliceNum);

    void tesselate() override;

protected:
    int verticesNum() override;

private:
    int m_stackNum;
    int m_sliceNum;
};

#endif // BARRELTESSELATOR_H
