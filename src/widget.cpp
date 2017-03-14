#include "widget.hpp"
#include "ui_widget.h"

#include <QComboBox>


namespace MyPluginExample {
    MyWidget::MyWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MyWidget)
    {
        ui->setupUi(this);
    }

    MyWidget::~MyWidget()
    {
        delete ui;
    }

    void MyWidget::addInputModel(const QString &name)
    {
        ui->_paramModelName->addItem(QString::fromStdString(name));
    }

    void MyWidget::on__runButton_clicked()
    {
        Param p;
        p.entityName = ui->_paramModelName->currentText();
        emit computationRequested(p);
    }
} // namespace MyPlugin

