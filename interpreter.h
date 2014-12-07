#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QStack>
#include <QRegExp>
#include <QDebug>
#include <QTableWidget>

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
            this->regex = new QRegExp(regex);
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
    };

public:
    explicit Interpreter(QObject *parent = 0);

signals:
    void output(QString s);
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
    QList<Lexeme> *lexemes;
    QList<LexemeRegex*> *regexes;
    QHash<QString,VariableData*> *symbols;
    bool syntaxCheck(int si,QString s);
    VariableData *processExpression(int start, int end);
};

#endif // INTERPRETER_H
