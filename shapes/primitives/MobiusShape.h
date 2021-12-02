#ifndef MOBIUSSHAPE_H
#define MOBIUSSHAPE_H

#include "BaseShape.h"

class MobiusTesselator;

class MobiusShape : public BaseShape
{
public:
    MobiusShape(int stackNum, int sliceNum);
    ~MobiusShape();

    void update(int param1, int param2, float param3, bool force) override;

protected:
    void construct() override;

private:
    int m_stackNum;
    int m_sliceNum;

    std::unique_ptr<MobiusTesselator> m_mobiusTess;
};

#endif // MOBIUSSHAPE_H
