#ifndef CONEHATTESSELATOR_H
#define CONEHATTESSELATOR_H

#include "BaseTesselator.h"

class ConeHatTesselator : public BaseTesselator
{
public:
    ConeHatTesselator();

    void setStackNum(int stackNum);
    void setSliceNum(int sliceNum);

    void tesselate() override;

protected:
    int verticesNum() override;

private:
    int m_stackNum;
    int m_sliceNum;
};

#endif // CONEHATTESSELATOR_H
