#include "MeshAnimatorUI.h"

#include <iostream>

#include "ui_MeshAnimatorUI.h"

MeshAnimatorUI::MeshAnimatorUI( QWidget* parent ) : QFrame( parent ), ui( new Ui::MeshAnimatorUI ) {
    ui->setupUi( this );
}

MeshAnimatorUI::~MeshAnimatorUI() {
    delete ui;
}

void MeshAnimatorUI::on_m_vertexIdx_valueChanged( int arg1 ) {
    emit indexChanged( arg1 );
}

void MeshAnimatorUI::on_m_position_toggled( bool arg1 ) {
    emit animatePosition( arg1 );
}

void MeshAnimatorUI::on_m_normal_toggled( bool arg1 ) {
    emit animateNormal( arg1 );
}
