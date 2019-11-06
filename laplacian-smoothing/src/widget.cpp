#include "widget.hpp"
#include "ui_widget.h"

#include <QComboBox>

namespace MyPluginExample {
static const QString separator = "_";

MyWidget::MyWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::MyWidget ) {
    ui->setupUi( this );

    // signal to signal connection not possible in QtDesigner
    connect( ui->_paramModelName,
             &QComboBox::currentTextChanged,
             this,
             &MyWidget::selectedModelChanged );
}

MyWidget::~MyWidget() {
    delete ui;
}

void MyWidget::addInputModel( const QString& entityName, const QString& componentName ) {
    ui->_paramModelName->addItem( entityName + separator + componentName,
                                  QVariant( QStringList( {entityName, componentName} ) ) );
}

void MyWidget::on__runButton_clicked() {
    QVariant data  = ui->_paramModelName->currentData();
    QStringList sl = data.toStringList();

    Param p;
    p.entityName = sl[0];
    p.dataId     = sl[1];

    emit computationRequested( p );
}
} // namespace MyPluginExample
