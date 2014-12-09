#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->inputLine->setEnabled(false);
    this->interpreter = new Interpreter(this);
    connect(this->interpreter, SIGNAL(output(QString)), ui->outputBox, SLOT(appendPlainText(QString)));
    connect(this->interpreter, SIGNAL(askInput()), this, SLOT(needInput()));
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

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->inputBox->clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->inputBox->append(line);
    }
}

void MainWindow::on_actionAuthors_triggered()
{
    QMessageBox::about(this, QString("About the Authors"),QString("Ferriel Lisandro B. Melarpis and Jan Keith C. Darunday\n"\
                    "are junior BS Computer Science students in University of the Philippines Los Banos. They were the champions for 2 consecutive years in Get 1/4 Pen & Paper Programming Challenge which was sponsored by AZEUS.\n"\
                        "Keith is interested in anime, sci-fi movies, programming, 3D modeling, MMORPG, hacking ang especially LOLIs.\n"\
                        "Ferriel likes playing strategic games, basketball, watching anime, movies, programming, hacking, and cute girls with glasses."));
}

void MainWindow::on_inputLine_returnPressed()
{
    this->interpreter->input(ui->inputLine->text().trimmed());
    ui->inputLine->setEnabled(false);
    ui->inputLine->clear();
    ui->analyzeButton->setEnabled(true);
}

void MainWindow::needInput()
{
    ui->analyzeButton->setEnabled(false);
    ui->inputLine->setEnabled(true);
    ui->inputLine->setFocus();
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "",
                                                    tr("Files (*.*)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << ui->inputBox->toPlainText();
}

void MainWindow::on_actionAnalyze_triggered()
{
    ui->analyzeButton->click();
}

void MainWindow::on_actionAbout_LOLi_triggered()
{
    QMessageBox::about(this, QString("About LOLi"),QString("LOLi is short for LOLCODE Interpreter. It is a program made in C++ that interprets LOLCODE."));
}
