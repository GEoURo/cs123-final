#ifndef MOBIUSTESSELATOR_H
#define MOBIUSTESSELATOR_H

#include "BaseTesselator.h"

class MobiusTesselator : public BaseTesselator
{
public:
    MobiusTesselator();

    void setStackNum(int stackNum);
    void setSliceNum(int sliceNum);

    void tesselate() override;

protected:
    int verticesNum() override;

private:
    int m_stackNum;
    int m_sliceNum;
};

#endif // MOBIUSTESSELATOR_H
