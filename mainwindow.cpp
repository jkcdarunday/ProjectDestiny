#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->interpreter = new Interpreter(this);
    connect(this->interpreter, SIGNAL(output(QString)), ui->outputBox, SLOT(append(QString )));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_analyzeButton_clicked()
{
    ui->outputBox->clear();
    ui->symbolTable->clear();
    ui->lexemeTable->clear();
    this->interpreter->reset();
    this->interpreter->multiParse(ui->inputBox->toPlainText());
    this->interpreter->execute();

    this->interpreter->populateLexemeTable(ui->lexemeTable);
    this->interpreter->populateSymbolTable(ui->symbolTable);
}

