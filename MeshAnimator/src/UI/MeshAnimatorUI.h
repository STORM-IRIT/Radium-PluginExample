#ifndef MESHANIMATORUI_H
#define MESHANIMATORUI_H

#include <GuiBase/Utils/PickingManager.hpp>
#include <QFrame>

namespace UI {
class MeshAnimatorUi;
}

namespace MeshAnimatorPlugin {
class MeshAnimatorPluginC;
}

namespace Ui {
class MeshAnimatorUI;
}

class MeshAnimatorUI : public QFrame
{
    Q_OBJECT

    friend class MeshAnimatorPlugin::MeshAnimatorPluginC;

  public:
    explicit MeshAnimatorUI( QWidget* parent = nullptr );
    ~MeshAnimatorUI();

  signals:
    void indexChanged( int );
    void animatePosition( bool );
    void animateNormal( bool );

  private slots:
    void on_m_vertexIdx_valueChanged( int arg1 );
    void on_m_position_toggled( bool arg1 );
    void on_m_normal_toggled( bool arg1 );

  private:
    Ui::MeshAnimatorUI* ui;
};

#endif // MESHANIMATORUI_H
