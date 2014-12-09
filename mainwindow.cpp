#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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


void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionLoad_triggered()
{
    
}

void MainWindow::on_actionAuthors_triggered()
{
}

void MainWindow::on_inputLine_returnPressed()
{
    this->interpreter->input(ui->inputLine->text().trimmed());
    ui->inputLine->setEnabled(false);
}

void MainWindow::needInput()
{
    ui->inputLine->setEnabled(true);
    ui->inputLine->setFocus();
}
