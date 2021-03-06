#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QStack>
#include <QRegExp>
#include <QDebug>
#include <QTableWidget>
#include <QEventLoop>

class Interpreter : public QObject
{
    Q_OBJECT
public:
    class Lexeme{
    public:
        QString token;
        char type;
        QString label;
        explicit Lexeme(char type, QString token, QString label){
            this->type = type;
            this->token = token;
            this->label = label;
        }
    };

    class LexemeRegex{
    public:
        char type;
        QString label;
        QRegExp *regex;
        explicit LexemeRegex(char type, QString label, QString regex){
            this->type=type;
            this->regex = new QRegExp(regex,Qt::CaseInsensitive);
            this->label=label;
        }
    };

    class VariableData{
    public:
        char type;
        QString value;
        explicit VariableData(char type, QString value){
            this->type=type;
            this->value=value;
        }

        void copy(VariableData* src){
            this->value = src->value;
            this->type = src->type;
        }
    };

public:
    explicit Interpreter(QObject *parent = 0);

signals:
    void output(QString s);
    void error(QString m);
    void askInput();

public slots:
    void input(QString s);
    void parse(QString s);
    void multiParse(QString s);
    void populateLexemeTable(QTableWidget *w);
    void populateSymbolTable(QTableWidget *w);
    void execute();
    void reset();

private:
    QEventLoop scanner;
    QList<Lexeme> *lexemes;
    QList<LexemeRegex*> *regexes;
    QStack<QString> *inputBuffer;
    QStack<QHash<QString,VariableData*>*> *symbols;
    VariableData *findVariable(QString var);
    bool syntaxCheck(int si,QString s);
    QString cleanString(QString s);
    VariableData *processExpression(int start, int end);
    QRegExp *stringCleaner;
};

#endif // INTERPRETER_H
