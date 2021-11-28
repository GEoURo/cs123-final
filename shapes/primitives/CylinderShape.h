#ifndef CYLINDERSHAPE_H
#define CYLINDERSHAPE_H

#include <memory>
#include "BaseShape.h"

class BarrelTesselator;
class CircleTesselator;

class CylinderShape : public BaseShape
{
public:
    CylinderShape(int stackNum, int sliceNum);
    ~CylinderShape();

    void update(int param1, int param2, float param3, bool force) override;

protected:
    void construct() override;

private:
    int m_stackNum;
    int m_sliceNum;

    std::unique_ptr<BarrelTesselator> m_barrelTess;
    std::unique_ptr<CircleTesselator> m_circleTess;

    std::vector<glm::vec3> m_capCenters;
    std::vector<glm::vec3> m_capNormals;
};

#endif // CYLINDERSHAPE_H
