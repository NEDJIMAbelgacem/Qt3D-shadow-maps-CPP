#include "postprocessquad.h"

PostprocessQuad::PostprocessQuad(ShadowMapFrameGraph *frameGraph, const QString& vertexShaderPath, const QString& fragmentShaderPath, Qt3DCore::QEntity *parent)
: Qt3DCore::QEntity( parent )
{
  materialContainer = new PostprocessQuadMaterial(frameGraph, vertexShaderPath, fragmentShaderPath);
  setObjectName("Postprocess Quad");
  Qt3DRender::QGeometry *geom = new Qt3DRender::QGeometry;
  Qt3DRender::QAttribute *positionAttribute = new Qt3DRender::QAttribute;
  QVector<float> vert = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f
  };

  QByteArray vertexArr( (const char *) vert.constData(), vert.size() * sizeof ( float ));
  Qt3DRender::QBuffer *vertexBuffer = new Qt3DRender::QBuffer;
  vertexBuffer->setData(vertexArr);

  positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
  positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
  positionAttribute->setVertexSize(3);
  positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  positionAttribute->setBuffer(vertexBuffer);
  positionAttribute->setByteOffset(0);
  positionAttribute->setByteStride( 3 * sizeof (float));
  positionAttribute->setCount(6);

  geom->addAttribute(positionAttribute);

  Qt3DRender::QGeometryRenderer *renderer = new Qt3DRender::QGeometryRenderer;
  renderer->setPrimitiveType( Qt3DRender::QGeometryRenderer::PrimitiveType::Triangles );
  renderer->setGeometry(geom);

  addComponent(renderer);
  addComponent(materialContainer->material());
}

PostprocessQuadMaterial::PostprocessQuadMaterial(ShadowMapFrameGraph *frameGraph, const QString& vertexShaderPath, const QString& fragmentShaderPath)
{
  mMaterial = new Qt3DRender::QMaterial;
  mColorTexturePrameter = new Qt3DRender::QParameter("colorTexture", frameGraph->viewColorTexture());
  mDepthTexturePrameter = new Qt3DRender::QParameter("depthTexture", frameGraph->viewDepthTexture());
  mShadowTextureParameter = new Qt3DRender::QParameter("shadowTexture", frameGraph->shadowDepthTexture());
  mViewCamera = frameGraph->viewCamera();
  mLightCamera = frameGraph->lightCamera();
  QMatrix4x4 cameraProj = mViewCamera->projectionMatrix();
  QMatrix4x4 cameraView = mViewCamera->viewMatrix();
  QMatrix4x4 lightProj = mLightCamera->projectionMatrix();
  QMatrix4x4 lightView = mLightCamera->viewMatrix();

  lightProjParameter = new Qt3DRender::QParameter("lightProj", lightProj);
  lightViewParameter = new Qt3DRender::QParameter("lightView", lightView);

  cameraProjParameter = new Qt3DRender::QParameter("cameraProj", cameraProj);
  cameraViewParameter = new Qt3DRender::QParameter("cameraView", cameraView);

  QObject::connect(mViewCamera, &Qt3DRender::QCamera::projectionMatrixChanged, [&](const QMatrix4x4 &proj) {
    cameraProjParameter->setValue(proj);
  });
  QObject::connect(mViewCamera, &Qt3DRender::QCamera::viewMatrixChanged, [&]() {
    cameraViewParameter->setValue(mViewCamera->viewMatrix());
  });

  mMaterial->addParameter(mColorTexturePrameter);
  mMaterial->addParameter(mDepthTexturePrameter);
  mMaterial->addParameter(mShadowTextureParameter);
  mMaterial->addParameter(cameraProjParameter);
  mMaterial->addParameter(cameraViewParameter);
  mMaterial->addParameter(lightProjParameter);
  mMaterial->addParameter(lightViewParameter);

  Qt3DRender::QEffect *effect = new Qt3DRender::QEffect;

  Qt3DRender::QTechnique *technique = new Qt3DRender::QTechnique;

  Qt3DRender::QGraphicsApiFilter *graphicsApiFilter = technique->graphicsApiFilter();
  graphicsApiFilter->setApi(Qt3DRender::QGraphicsApiFilter::Api::OpenGL);
  graphicsApiFilter->setProfile( Qt3DRender::QGraphicsApiFilter::OpenGLProfile::CoreProfile );
  graphicsApiFilter->setMajorVersion(3);
  graphicsApiFilter->setMinorVersion(5);

//  Qt3DRender::QFilterKey *filterKey = new Qt3DRender::QFilterKey;
//  filterKey->setName("renderingStyle");
//  filterKey->setName("forward");

  Qt3DRender::QRenderPass *renderPass = new Qt3DRender::QRenderPass;
//  renderPass->addFilterKey(filterKey);

  Qt3DRender::QShaderProgram *shader = new Qt3DRender::QShaderProgram;
  shader->setVertexShaderCode( Qt3DRender::QShaderProgram::loadSource( QUrl(vertexShaderPath) ) );
  shader->setFragmentShaderCode( Qt3DRender::QShaderProgram::loadSource( QUrl(fragmentShaderPath) ) );
  renderPass->setShaderProgram(shader);

  technique->addRenderPass(renderPass);

  effect->addTechnique(technique);
  mMaterial->setEffect(effect);
}

