#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "BaseShape.h"

class SphereTesselator;

class SphereShape : public BaseShape
{
public:
    SphereShape(int stackNum, int sliceNum);
    ~SphereShape();

    void update(int param1, int param2, float param3, bool force) override;

protected:
    void construct() override;

private:
    int m_stackNum;
    int m_sliceNum;

    std::unique_ptr<SphereTesselator> m_sphereTess;
};

#endif // SPHERESHAPE_H
