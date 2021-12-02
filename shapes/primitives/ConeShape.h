#ifndef CONESHAPE_H
#define CONESHAPE_H

#include "BaseShape.h"

class CircleTesselator;
class ConeHatTesselator;

class ConeShape : public BaseShape
{
public:
    ConeShape(int stackNum, int sliceNum);
    ~ConeShape();

    void update(int param1, int param2, float param3, bool force) override;

protected:
    void construct() override;

private:
    int m_stackNum;
    int m_sliceNum;

    std::unique_ptr<ConeHatTesselator> m_hatTess;
    std::unique_ptr<CircleTesselator> m_circleTess;
};

#endif // CONESHAPE_H
