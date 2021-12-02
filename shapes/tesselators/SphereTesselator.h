#ifndef SPHERETESSSLATOR_H
#define SPHERETESSSLATOR_H

#include "BaseTesselator.h"

class SphereTesselator : public BaseTesselator
{
public:
    SphereTesselator();

    void setStackNum(int stackNum);
    void setSliceNum(int sliceNum);

    void tesselate() override;

protected:
    int verticesNum() override;

private:
    int m_stackNum;
    int m_sliceNum;
};

#endif // SPHERETESSSLATOR_H
