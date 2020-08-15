#include "groundplane.h"

GroundPlane::GroundPlane(const QVector3D &position, Qt3DCore::QEntity *parent)
: Qt3DCore::QEntity( parent )
{
  Qt3DExtras::QPlaneMesh *plane = new Qt3DExtras::QPlaneMesh;
  plane->setWidth(50.0f);
  plane->setHeight(50.0f);
  plane->setMeshResolution(QSize(2, 2));

  mTransform = new Qt3DCore::QTransform;
  mTransform->setTranslation(position);

  addComponent(plane);
  addComponent(mTransform);
}

//RenderPass {
//filterKeys: [ FilterKey { name: "pass"; value: "shadowmap" } ]

//    shaderProgram: ShaderProgram {
//    vertexShaderCode:   loadSource("qrc:/shaders/shadowmap.vert")
//    fragmentShaderCode: loadSource("qrc:/shaders/shadowmap.frag")
//  }

//    renderStates: [
//    PolygonOffset { scaleFactor: 4; depthSteps: 4 },
//      DepthTest { depthFunction: DepthTest.Less }
//      ]
//    }

void GroundPlane::setMaterial(Qt3DRender::QMaterial *material)
{
  mMaterial = material;
  addComponent(material);
}
