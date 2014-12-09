#ifndef AUTHORS_H
#define AUTHORS_H

#include <QDialog>

namespace Ui {
class Authors;
}

class Authors : public QDialog
{
    Q_OBJECT

public:
    explicit Authors(QWidget *parent = 0);
    ~Authors();

private:
    Ui::Authors *ui;
};

#endif // AUTHORS_H
