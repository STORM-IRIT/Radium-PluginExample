#ifndef MyWIDGET_H
#define MyWIDGET_H

#include "plugin.hpp"
#include <QWidget>

namespace Ui {
class MyWidget;
}

namespace MyPluginExample {
class MyWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit MyWidget( QWidget* parent = 0 );
    ~MyWidget();

  public slots:
    void addInputModel( const QString& entityName, const QString& componentName );

  signals:
    //! Connected _paramModelName::currentIndexChanged in the ui file
    void selectedModelChanged( const QString& name );
    void computationRequested( Param p );

  private slots:
    void on__runButton_clicked();

  private:
    Ui::MyWidget* ui;
};

} // namespace MyPluginExample

#endif // MyWIDGET_H
