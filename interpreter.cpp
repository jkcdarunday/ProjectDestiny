#include "interpreter.h"

Interpreter::Interpreter(QObject *parent) :
    QObject(parent)
{
    lexemes = new QList<Lexeme>();
    regexes = new QList<LexemeRegex*>();
    symbols = new QHash<QString, VariableData*>();

    QString typeR = "(NOOB|TROOF|NUMBAR|NUMBR|YARN)";
    QString booleanR = "(WIN|FAIL)";
    QString varR = "[a-zA-Z][a-zA-Z0-9_]*";
    QString numR = "-?[0-9]+";
    QString floatR = numR + "\\.[0-9]+";
    QString stringR = "\"([^\":]|:\"|::|:\\)|:>|:o|:\\([A-F0-9a-f]+\\)|:\\{[a-zA-Z][a-zA-Z0-9_]*\\}|:\\[[a-zA-Z]+\\])*\"";
    //qDebug() << stringR;

    regexes->append(new LexemeRegex('+',"Arithmetic Operation", "SUM\\s+OF\\s*"));
    regexes->append(new LexemeRegex('-',"Arithmetic Operation", "DIFF\\s+OF\\s*"));
    regexes->append(new LexemeRegex('*',"Arithmetic Operation", "PRODUKT\\s+OF\\s*"));
    regexes->append(new LexemeRegex('/',"Arithmetic Operation", "QUOSHUNT\\s+OF\\s*"));
    regexes->append(new LexemeRegex('%',"Arithmetic Operation", "MOD\\s+OF\\s*"));
    regexes->append(new LexemeRegex('>',"Arithmetic Operation", "BIGGR\\s+OF\\s*"));
    regexes->append(new LexemeRegex('<',"Arithmetic Operation", "SMALLR\\s+OF\\s*"));

    regexes->append(new LexemeRegex('&', "Boolean Operation", "BOTH\\s+OF\\s*"));
    regexes->append(new LexemeRegex('|', "Boolean Operation", "EITHER\\s+OF\\s*"));
    regexes->append(new LexemeRegex('^', "Boolean Operation", "WON\\s+OF\\s*"));
    regexes->append(new LexemeRegex('!', "Boolean Operation", "NOT\\s+OF\\s*"));
    regexes->append(new LexemeRegex('y', "Boolean Operation", "ANY\\s+OF\\s*"));

    regexes->append(new LexemeRegex('s', "String Operation", "SMOOSH\\s*"));

    regexes->append(new LexemeRegex('n', "Delimiter", "AN\\s*"));
    regexes->append(new LexemeRegex('_', "Delimiter", "(HAI\\s+(\\d+(\\.\\d+)?)?\\s*|KTHNXBYE)\\s*"));
    regexes->append(new LexemeRegex(']', "Infinite Arity Terminator", "MKAY\\s*"));

    regexes->append(new LexemeRegex('#', "Initialization", "ITZ\\s*"));

    regexes->append(new LexemeRegex('c', "Comment", "BTW.*"));

    regexes->append(new LexemeRegex('$', "Typecast Operator", "MAEK\\s*"));
    regexes->append(new LexemeRegex('4', "Typecast Operator", "IS\\s+NOW\\s+A\\s*"));

    regexes->append(new LexemeRegex('.', "Output", "VISIBLE\\s*"));
    regexes->append(new LexemeRegex(',', "Input", "GIMMEH\\s*"));

    regexes->append(new LexemeRegex('i', "Variable Declaration", "I\\s+HAS\\s+A\\s*"));

    regexes->append(new LexemeRegex(':', "Assignment Operator", "R\\s*"));

    regexes->append(new LexemeRegex('e', "Comparator", "(DIFFRINT|BOTH\\s+SAEM)\\s+|AND\\s+(BIGGR|SMALLR)\\s+OF\\s*"));

    regexes->append(new LexemeRegex('?', "If/Then Delimiter", "O\\s*RLY\\?\\s+|YA\\s*RLY\\s+|MEBBE\\s+|NO\\s*WAI\\s+|OIC\\s*"));
    regexes->append(new LexemeRegex('[', "Case Delimiter", "WTF\\?\\s+|OMG\\s+|GTFO\\s*"));

    regexes->append(new LexemeRegex('{', "Begin Loop Delimeter", "IM\\s+IN\\s+YR\\s*"));
    regexes->append(new LexemeRegex('!', "Loop Function", "(UPPIN|NERFIN)\\s+YR\\s*"));
    regexes->append(new LexemeRegex('@', "Loop Condition", "(TIL|WILE)\\s*"));
    regexes->append(new LexemeRegex('}', "End Loop Delimiter", "IM\\s+OUTTA\\s+YR\\s*"));

    regexes->append(new LexemeRegex('t', "Data Type", "" + typeR + "\\s*"));

    regexes->append(new LexemeRegex('Q', "Invalid Use of Keyword", "(SUM|DIFF|PRODUKT|QUOSHUNT|MOD|BIGGR|SMALLR|BOTH|EITHER|WON|NOT|ANY|I|NOW|I|HAS|A|DIFFRINT|BOTH|SAEM|AND|BIGGR|SMALLR|O|YA|RLY|MEBBE|NO|WAI|IM|OUTTA|YR)\\s*"));

    regexes->append(new LexemeRegex('v', "Variable Name", varR + "\\s*"));
    regexes->append(new LexemeRegex('f', "Float Literal", floatR + "\\s*"));
    regexes->append(new LexemeRegex('0', "Integer Literal", numR +"\\s*"));
    regexes->append(new LexemeRegex('"', "String Literal", stringR + "\\s*"));
    regexes->append(new LexemeRegex('1', "Boolean Literal", booleanR + "\\s*"));
}

void Interpreter::parse(QString s)
{
    if(s.trimmed().length()<=0){
        this->lexemes->append(Lexeme('\n',"\n","Line Delimiter"));
        return;
    }
    for(int i =0; i<regexes->size();i++){
        if(regexes->at(i)->regex->indexIn(s) == 0){
            //qDebug() << "MatchedLength:" << regexes->at(i)->regex->matchedLength();
            //qDebug() << "Added to Lexemes: " << regexes->at(i)->regex->cap() << regexes->at(i)->label <<  regexes->at(i)->type;
            this->lexemes->append(Lexeme(regexes->at(i)->type,regexes->at(i)->regex->cap().trimmed(),regexes->at(i)->label));
            this->parse(s.remove(0,regexes->at(i)->regex->matchedLength()));
            break;
        }
    }
}

void Interpreter::multiParse(QString s)
{
    QStringList lines = s.split("\n");
    QString line;
    QStringListIterator lineIterator(lines);
    while (lineIterator.hasNext())
        this->parse(lineIterator.next());
}

void Interpreter::populateLexemeTable(QTableWidget *w)
{
    int size = this->lexemes->size();
    w->setRowCount(size);
    for(int i=0;i<size;i++){
        w->setItem(i,0,new QTableWidgetItem(this->lexemes->at(i).label));
        w->setItem(i,1,new QTableWidgetItem(this->lexemes->at(i).token));
    }
}

void Interpreter::populateSymbolTable(QTableWidget *w)
{
    int size = this->symbols->size();
    w->setRowCount(size);
    int count=0;
    QHash<QString, Interpreter::VariableData*>::iterator i = this->symbols->begin();
    while (i != this->symbols->end()) {
        w->setItem(count,0,new QTableWidgetItem(QString(i.value()->type)));
        w->setItem(count,1,new QTableWidgetItem(i.key()));
        w->setItem(count,2,new QTableWidgetItem(i.value()->value));
        ++i;
        count++;
    }
}

void Interpreter::execute()
{
    int lastN = 0;
    do{
        //i has a var
        if(syntaxCheck(lastN,"iv\n")){
            if(!this->symbols->contains(lexemes->at(lastN+1).token))
                this->symbols->insert(lexemes->at(lastN+1).token, new VariableData('x',""));
            else
                qDebug() << "Syntax Error : Declaring already-existing variable :(";
        }
        else if(syntaxCheck(lastN,"iv#t\n")){
            if(!this->symbols->contains(lexemes->at(lastN+1).token))
                this->symbols->insert(lexemes->at(lastN+1).token, new VariableData(lexemes->at(lastN+3).type,lexemes->at(lastN+3).token));
            else
                qDebug() << "Syntax Error : Declaring already-existing variable to string :(";
        }
        else if(syntaxCheck(lastN,"v:t\n")){
            if(this->symbols->contains(lexemes->at(lastN).token)){
                this->symbols->value(lexemes->at(lastN).token)->value=lexemes->at(lastN+2).token;
                this->symbols->value(lexemes->at(lastN).token)->type=lexemes->at(lastN+2).type;
            }else
                qDebug() << "Syntax Error : Assigning string to non-existant variable :(";
        }

        else if(syntaxCheck(lastN, ".v\n")){
            if(this->symbols->contains(this->lexemes->at(lastN+1).token) && QString("f01\"").contains(this->symbols->value(this->lexemes->at(lastN+1).token)->type))
                emit output(this->symbols->value(this->lexemes->at(lastN+1).token)->value);
            else
                qDebug() << "Syntax Error : Printed a variable that does not exist or is noob:(";
        }else {
            qDebug() << "Syntax Error near : " << this->lexemes->at(lastN).token;
        }

        do{
            lastN++;
        }while(lastN!=lexemes->size()-1 && this->lexemes->at(lastN-1).type != '\n');
    }while(lastN!=lexemes->size()-1);
}

void Interpreter::reset()
{
    this->lexemes->clear();
    this->symbols->clear();
}

bool Interpreter::syntaxCheck(int si, QString s)
{
    int ss = s.size();
    if(si+ss > lexemes->size())
        return false;
    for(int i=0;i<ss;i++){
        if(s.at(i) != lexemes->at(si+i).type && !(s.at(i) == 't' && QString("01f\"").contains(lexemes->at(si+i).type)))
            return false;
    }
    return true;
}

void Interpreter::input(QString s)
{

}
