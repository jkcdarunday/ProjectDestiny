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
    regexes->append(new LexemeRegex('Y', "Boolean Operation", "ALL\\s+OF\\s*"));

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

    regexes->append(new LexemeRegex('=', "Comparator", "BOTH\\s+SAEM\\s*"));
    regexes->append(new LexemeRegex('z', "Comparator", "DIFFRINT\\s*"));

    regexes->append(new LexemeRegex('?', "If/Then Delimiter", "O\\s*RLY\\?\\s+|YA\\s*RLY\\s+|MEBBE\\s+|NO\\s*WAI\\s+|OIC\\s*"));
    regexes->append(new LexemeRegex('[', "Case Delimiter", "WTF\\?\\s+|OMG\\s+|GTFO\\s*"));

    regexes->append(new LexemeRegex('{', "Begin Loop Delimeter", "IM\\s+IN\\s+YR\\s*"));
//    regexes->append(new LexemeRegex('!', "Loop Function", "(UPPIN|NERFIN)\\s+YR\\s*"));
    regexes->append(new LexemeRegex('@', "Loop Condition", "(TIL|WILE)\\s*"));
    regexes->append(new LexemeRegex('}', "End Loop Delimiter", "IM\\s+OUTTA\\s+YR\\s*"));

    regexes->append(new LexemeRegex('t', "Data Type", "" + typeR + "\\s*"));

    regexes->append(new LexemeRegex('Q', "Invalid Use of Keyword", "(SUM|DIFF|PRODUKT|QUOSHUNT|MOD|BIGGR|SMALLR|BOTH|EITHER|WON|NOT|ANY|I|NOW|I|HAS|A|DIFFRINT|BOTH|SAEM|AND|BIGGR|SMALLR|O|YA|RLY|MEBBE|NO|WAI|IM|OUTTA|YR)\\s*"));

    regexes->append(new LexemeRegex('f', "Float Literal", floatR + "\\s*"));
    regexes->append(new LexemeRegex('0', "Integer Literal", numR +"\\s*"));
    regexes->append(new LexemeRegex('"', "String Literal", stringR + "\\s*"));
    regexes->append(new LexemeRegex('1', "Boolean Literal", booleanR + "\\s*"));
    regexes->append(new LexemeRegex('v', "Variable Name", varR + "\\s*"));
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
        this->parse(lineIterator.next().trimmed());
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
    if(this->lexemes->isEmpty()) return;
    int lineNumber = 0;
    do{
        lineNumber++;
        //i has a var
        if(syntaxCheck(lastN,"iv\n"))
            if(!this->symbols->contains(lexemes->at(lastN+1).token))
                this->symbols->insert(lexemes->at(lastN+1).token, new VariableData('x',""));
            else qDebug() << "Syntax Error : Declaring already-existing variable :(";


        //i has a var itz something
        else if(syntaxCheck(lastN,"iv#t\n"))
            if(!this->symbols->contains(lexemes->at(lastN+1).token))
                this->symbols->insert(lexemes->at(lastN+1).token, new VariableData(lexemes->at(lastN+3).type,lexemes->at(lastN+3).token));
            else qDebug() << "Syntax Error : Declaring already-existing variable to string :(";

        //i has a var itz expression
        else if(syntaxCheck(lastN,"iv#o")){
            int nn=lastN;
            do nn++; while(nn+1!=lexemes->size()-1 && this->lexemes->at(nn+1).type != '\n');
            VariableData *tmp = this->processExpression(lastN+3,nn);
            if(tmp==NULL)
                qDebug() << "Syntax Error : Problem executing expression";
            else if(!this->symbols->contains(lexemes->at(lastN+1).token))
                this->symbols->insert(lexemes->at(lastN+1).token, tmp);
        }

        //var r loli
        else if(syntaxCheck(lastN,"v:t\n"))
            if(this->symbols->contains(lexemes->at(lastN).token)){
                this->symbols->value(lexemes->at(lastN).token)->value=lexemes->at(lastN+2).token;
                this->symbols->value(lexemes->at(lastN).token)->type=lexemes->at(lastN+2).type;
            }else qDebug() << "Syntax Error : Assigning string to non-existant variable :(";


        //var r expression
        else if(syntaxCheck(lastN,"v:o")){
            int nn=lastN;
            do nn++; while(nn+1!=lexemes->size()-1 && this->lexemes->at(nn+1).type != '\n');
            VariableData *tmp = this->processExpression(lastN+2,nn);
            if(tmp==NULL)
                qDebug() << "Syntax Error : Problem executing expression";
            else if(this->symbols->contains(lexemes->at(lastN).token))
                this->symbols->insert(lexemes->at(lastN).token, tmp);
            else
                qDebug() << "Syntax Error : Variable " + this->lexemes->at(lastN).token + " does not exist";
        }

        //visible var
        else if(syntaxCheck(lastN, ".v\n"))
            if(this->symbols->contains(this->lexemes->at(lastN+1).token) && QString("f01\"").contains(this->symbols->value(this->lexemes->at(lastN+1).token)->type))
                emit output(this->symbols->value(this->lexemes->at(lastN+1).token)->value);
            else qDebug() << "Syntax Error : Printed a variable that does not exist or is noob:(";


        //visible "loli"
        else if(syntaxCheck(lastN, ".t\n"))
            emit output(this->lexemes->at(lastN+1).token);

        //blank space
        else if(syntaxCheck(lastN,"\n"));

        else
            qDebug() << "Syntax Error near : " << this->lexemes->at(lastN).token;

        //go to the next newline
        do lastN++; while(lastN!=lexemes->size()-1 && this->lexemes->at(lastN-1).type != '\n');
    }while(lastN<=lexemes->size()-1);
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
        if(s.at(i) != lexemes->at(si+i).type
                && !(s.at(i) == 't' && QString("01f\"").contains(lexemes->at(si+i).type))
                && !(s.at(i) == 'o' && QString("+-*/%><&^|!yY=z").contains(lexemes->at(si+i).type))
                )
            return false;
    }
    return true;
}

Interpreter::VariableData *Interpreter::processExpression(int start, int end)
{
    QStack<VariableData*> s;
    int mkays = 0;
    for(int i=end;i>=start;i--){
        char type=this->lexemes->at(i).type;
        QString token=this->lexemes->at(i).token;
        if(QString("f01\"").contains(type))
            s.push(new VariableData(type, token));
        else if(type == 'v'){
            if(this->symbols->contains(token)){
                VariableData *tmp = this->symbols->value(token);
                s.push(new VariableData(tmp->type,tmp->value));
            } else return NULL;
        }

        else if(QString("+-*/%><").contains(type)){
            VariableData *arg2=s.pop();
            VariableData *arg1=s.pop();
            if(!QString("0f").contains(arg1->type))return NULL;
            if(!QString("0f").contains(arg2->type))return NULL;
            double v1 = arg1->value.toDouble();
            double v2 = arg2->value.toDouble();
            double result;
            switch(type){
            case '+': result = v1+v2; break;
            case '-': result = v1-v2;  break;
            case '*': result = v1*v2;  break;
            case '/': result = v1/v2;  break;
            case '%': result = (int)v1 % (int)v2;  break;
            case '>': result = (v1>v2)?v1:v2; break;
            case '<': result = (v1<v2)?v1:v2; break;
            }
            if(arg1->type == 'f' || arg2->type == 'f')
                s.push(new VariableData('f',QString::number((double)result)));
            else
                s.push(new VariableData('0',QString::number((int)result)));
            delete arg1;
            delete arg2;
        }

        else if(QString("&^|!").contains(type)){
            VariableData *arg2=NULL;
            bool b2=false;
            if(type!='!'){
                arg2=s.pop();
                b2 = QRegExp("WIN",Qt::CaseInsensitive).exactMatch(arg2->value);
            }
            VariableData *arg1=s.pop();
            bool b1 = QRegExp("WIN",Qt::CaseInsensitive).exactMatch(arg1->value);
            bool res;
            switch(type){
            case '&': res = b1 && b2; break;
            case '^': res = b1 ^ b2; break;
            case '|': res = b1 || b2; break;
            case '!': res = !b1; break;
            }
            s.push(new VariableData('1', QString(res?"WIN":"FAIL")));
        }

        else if(type == ']'){
            mkays++;
            s.push(new VariableData(type,token));
        }

        else if(QString("yY").contains(type)){
            if(mkays <= 0)
                return NULL;
            mkays--;
            if(s.size() == 0) return NULL;
            VariableData *arg1=s.pop();

            bool hasResult=false;
            bool resultIs=false;
            while(arg1->type != ']'){
                if(arg1->type!='1') return NULL;
                bool b1 = QRegExp("WIN",Qt::CaseInsensitive).exactMatch(arg1->value);
                if(!hasResult && ((type=='y' && b1==true) || (type == 'Y' && b1==false))){
                    resultIs=b1;
                    hasResult=true;
                }
                delete arg1;
                arg1=s.pop();
            }
            if(!hasResult){
                s.push(new VariableData('1',QString(type=='y'?"FAIL":"WIN")));
            } else {
                s.push(new VariableData('1',QString(resultIs?"WIN":"FAIL")));
            }

        }

        else if(QString("=z").contains(type)){
            VariableData *arg2=s.pop();
            VariableData *arg1=s.pop();
            bool result=false;
            if(QString("0f").contains(arg1->type) && QString("0f").contains(arg2->type)){
                double v1 = arg1->value.toDouble();
                double v2 = arg2->value.toDouble();
                if((type=='=' && v1==v2) || (type=='z' && v1!=v2)) result=true;
            }

            else if(arg1->type=='"' && arg2->type=='"'){
                result = arg1->value.trimmed().compare(arg2->value.trimmed())==0;
                if(type=='z') result=!result;
            }

            else if(arg1->type=='1' && arg2->type=='1'){
                result = arg1->value.trimmed().compare(arg2->value.trimmed(),Qt::CaseInsensitive)==0;
                if(type=='z') result=!result;
            }
            delete arg1;
            delete arg2;
            s.push(new VariableData('1',QString(result?"WIN":"FAIL")));
        }

        else if(type=='n');

        else return NULL;
    }
    if(s.size()==1) return s.at(0);
    return NULL;
}

void Interpreter::input(QString s)
{

}
