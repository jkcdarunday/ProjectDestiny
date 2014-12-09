#include "authors.h"
#include "ui_authors.h"

Authors::Authors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authors)
{
    ui->setupUi(this);
}

Authors::~Authors()
{
    delete ui;
}
