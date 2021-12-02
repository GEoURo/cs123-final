#ifndef TORUSSHAPE_H
#define TORUSSHAPE_H

#include "BaseShape.h"
#include <memory>

class TorusTesselator;

class TorusShape : public BaseShape
{
public:
    TorusShape(int ringSlice, int barrelSlice, float radiusPercent);
    ~TorusShape();

    void update(int param1, int param2, float param3, bool force) override;

protected:
    void construct() override;

private:
    int m_ringSlice;
    int m_barrelSlice;
    float m_radiusPercent;

    std::unique_ptr<TorusTesselator> m_torusTess;
};

#endif // TORUSSHAPE_H
