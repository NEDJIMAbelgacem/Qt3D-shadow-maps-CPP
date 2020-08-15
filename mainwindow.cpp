#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "shadowmaplight.h"
#include "shadowmapframegraph.h"
#include "toyplane.h"
#include "trefoil.h"
#include "groundplane.h"
#include "previewquad.h"
#include "postprocessquad.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  Qt3DExtras::Qt3DWindow *window = new Qt3DExtras::Qt3DWindow;
  ui->mainLayout->addWidget( QWidget::createWindowContainer(window) );

  Qt3DCore::QEntity *root = new Qt3DCore::QEntity;
  root->setObjectName("rootEntity");

  Qt3DRender::QCamera* viewCamera = new Qt3DRender::QCamera;
  viewCamera->setProjectionType(Qt3DRender::QCameraLens::ProjectionType::PerspectiveProjection);
  viewCamera->lens()->setPerspectiveProjection(45.0f, window->width() / window->height(), 0.1f, 100.0f);
  viewCamera->setPosition(QVector3D(0.0, 10.0, 20.0));
  viewCamera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
  viewCamera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));

  Qt3DExtras::QFirstPersonCameraController *cameraController = new Qt3DExtras::QFirstPersonCameraController(root);
  cameraController->setCamera(viewCamera);


  QVector3D lightPos(-10.0, 20.0, -10.0);
  ShadowMapLight *light = new ShadowMapLight(lightPos, root);
  light->setObjectName("light");

  ShadowMapFrameGraph *framegraph = new ShadowMapFrameGraph(window, viewCamera, light);
  root->addComponent(framegraph->renderSettings());
  window->setActiveFrameGraph(framegraph->frameGraph());

  Qt3DRender::QTexture2D *shadowTexture = framegraph->shadowDepthTexture();

  Qt3DRender::QTextureImage *textureImage = new Qt3DRender::QTextureImage;
  textureImage->setSource( QUrl( "qrc:/thisisfine.jpg" ) );
  Qt3DRender::QTexture2D *texture = new Qt3DRender::QTexture2D;
  texture->addTextureImage(textureImage);

  Trefoil *trefoil = new Trefoil(QVector3D(0.0f, 5.0f, 0.0f), root);
  trefoil->setObjectName("trefoil");
  Qt3DExtras::QPhongMaterial* trefoilMaterial = new Qt3DExtras::QPhongMaterial;
  trefoilMaterial->setAmbient(QColor(Qt::blue));
  trefoil->setMaterial(trefoilMaterial);

  ToyPlane *toy = new ToyPlane(QVector3D(10.0f, 5.0f, 10.0f), root);
  toy->setObjectName("toyplane");
  Qt3DExtras::QPhongMaterial *toyMaterial = new Qt3DExtras::QPhongMaterial;
  toyMaterial->setAmbient(QColor(Qt::red));
  toy->setMaterial(toyMaterial);

  GroundPlane *ground = new GroundPlane(QVector3D(0.0f, -5.0f, 0.0f), root);
  ground->setObjectName("ground");
  Qt3DExtras::QPhongMaterial *groundMaterial = new Qt3DExtras::QPhongMaterial;
  groundMaterial->setAmbient(QColor(Qt::yellow));
  ground->setMaterial(groundMaterial);

//  GroundPlane *wall = new GroundPlane(QVector3D(0.0f, -5.0f, 0.0f), root);
//  wall->setObjectName("wall");
//  Qt3DExtras::QPhongMaterial *wallMaterial = new Qt3DExtras::QPhongMaterial;
//  wallMaterial->setAmbient(QColor(Qt::green));
//  wall->setMaterial(wallMaterial);
//  wall->transform()->setRotationX(90.0f);
//  wall->transform()->setScale(0.1);

//  GroundPlane *wall2 = new GroundPlane(QVector3D(0.0f, -5.0f, 0.0f), root);
//  wall2->setObjectName("wall");
//  Qt3DExtras::QPhongMaterial *wall2Material = new Qt3DExtras::QPhongMaterial;
//  wall2Material->setAmbient(QColor(Qt::green));
//  wall2->setMaterial(wall2Material);
//  wall2->transform()->setRotationX(-90.0f);
//  wall2->transform()->setScale(0.1);


  PostprocessQuad *postprocessingQuad = new PostprocessQuad(framegraph, "qrc:/shaders/postprocess.vert", "qrc:/shaders/postprocess.frag", root);
  postprocessingQuad->addComponent(framegraph->postprocessingLayer());

  PreviewQuad *previewQuad = new PreviewQuad(shadowTexture, QVector<Qt3DRender::QParameter *>(), root);
  previewQuad->addComponent(framegraph->previewLayer());

  window->setRootEntity(root);
}

MainWindow::~MainWindow()
{
  delete ui;
}

