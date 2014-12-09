#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "interpreter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_analyzeButton_clicked();

    void on_actionQuit_triggered();

    void on_actionAuthors_triggered();

    void on_actionLoad_triggered();

    void on_inputLine_returnPressed();

    void needInput();
private:
    Ui::MainWindow *ui;
    Interpreter *interpreter;
};

#endif // MAINWINDOW_H
