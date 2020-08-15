#ifndef Q3DWINDOW_H
#define Q3DWINDOW_H

#include <Qt3DExtras/qt3dextras_global.h>
#include <Qt3DRender/qrenderapi.h>
#include <QtGui/QWindow>
#include <QWindow>
#include <Qt3DRender>

namespace Qt3DCore {
class QAspectEngine;
class QAbstractAspect;
class QEntity;
}

namespace Qt3DRender {
class QCamera;
class QFrameGraphNode;
class QRenderAspect;
class QRenderSettings;
}

namespace Qt3DExtras {
class QForwardRenderer;
}

namespace Qt3DInput {
class QInputAspect;
class QInputSettings;
}

namespace Qt3DLogic {
class QLogicAspect;
}

class Q3DWindow : public QWindow
{
    Q_OBJECT
  public:
    Q3DWindow(QScreen *screen = nullptr, Qt3DRender::API = Qt3DRender::API::OpenGL);
    ~Q3DWindow();

    void registerAspect(Qt3DCore::QAbstractAspect *aspect);
    void registerAspect(const QString &name);

    void setRootEntity(Qt3DCore::QEntity *root);

    void setActiveFrameGraph(Qt3DRender::QFrameGraphNode *activeFrameGraph);
    Qt3DRender::QFrameGraphNode *activeFrameGraph() const;
    Qt3DExtras::QForwardRenderer *defaultFrameGraph() const;

    Qt3DRender::QCamera *camera() const;
    Qt3DRender::QRenderSettings *renderSettings() const;

  public Q_SLOTS:

  Q_SIGNALS:

  protected:
    void showEvent(QShowEvent *e) override;
    void resizeEvent(QResizeEvent *) override;
    bool event(QEvent *e) override;

};

#endif // Q3DWINDOW_H
