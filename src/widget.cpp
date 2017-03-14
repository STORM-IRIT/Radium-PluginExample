#include "widget.hpp"
#include "ui_widget.h"

#include <QComboBox>


namespace MyPluginExample {
    MyWidget::MyWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MyWidget)
    {
        ui->setupUi(this);

        // signal to signal connection not possible in QtDesigner
        connect(ui->_paramModelName, &QComboBox::currentTextChanged,
                this,                &MyWidget::selectedModelChanged);
    }

    MyWidget::~MyWidget()
    {
        delete ui;
    }

    void MyWidget::addInputModel(const QString &name)
    {
        ui->_paramModelName->addItem(name);
    }

    void MyWidget::on__runButton_clicked()
    {
        Param p;
        p.entityName = ui->_paramModelName->currentText();
        emit computationRequested(p);
    }
} // namespace MyPlugin

