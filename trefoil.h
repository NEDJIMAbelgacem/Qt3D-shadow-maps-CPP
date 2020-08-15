#ifndef TREFOIL_H
#define TREFOIL_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

class Trefoil : public Qt3DCore::QEntity
{
  public:
    Trefoil(const QVector3D &position, Qt3DCore::QEntity *parent = nullptr);
    void setMaterial(Qt3DRender::QMaterial *material );

  private:
    Qt3DRender::QMaterial *mMaterial = nullptr;
};

#endif // TREFOIL_H
