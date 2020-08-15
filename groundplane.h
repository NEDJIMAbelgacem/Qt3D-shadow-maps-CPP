#ifndef GROUNDPLANE_H
#define GROUNDPLANE_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

class GroundPlane : public Qt3DCore::QEntity
{
  public:
    GroundPlane(const QVector3D &position, Qt3DCore::QEntity *parent = nullptr);

    Qt3DCore::QTransform *transform() { return mTransform; }
    void setMaterial(Qt3DRender::QMaterial *material);
  private:
    Qt3DRender::QMaterial *mMaterial = nullptr;
    Qt3DCore::QTransform *mTransform = nullptr;

};

#endif // GROUNDPLANE_H
