#ifndef IMPLEMENTACIA_H
#define IMPLEMENTACIJA_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;

class Symbol {
  string name;
  int section;

  bool isLocal; // globalni ili lokalni
  int id;
  bool isDefined;
  
  int size; // za sekciju, size=LC na kraju sekcije
  string flags;
  bool nastaoSaEqu;

public:
  int value;
  int type; // Data=0, function=1, section=2, src file

  Symbol(){};

  Symbol(string name, int section, int value, int type, int id, bool isLocal,
         bool isDefined, string flags) { // za sekcije
    this->name = name;
    this->section = section;
    this->value = value;
    this->type = type;
    this->id = id;
    this->isLocal = isLocal;
    this->isDefined = isDefined;
    this->flags = flags;
  }

  Symbol(string name, int section, int value, int type, int id, bool isLocal,
         bool isDefined) { // za equ
    this->name = name;
    this->section = section;
    this->value = value;
    this->type = type;
    this->id = id;
    this->isLocal = isLocal;
    this->isDefined = isDefined;
  }

  Symbol(string name, int type, int id, bool isLocal,
         bool isDefined) { // za simbole u .global

    this->name = name;
    this->type = type;
    this->id = id;
    this->isLocal = isLocal;
    this->isDefined = isDefined;
  }
  Symbol(string name, int type, int id) { // za simbole u .extern
    this->name = name;
    this->section = 0;
    this->id = id;
    this->type = type;
  }

  Symbol(string name, int type, bool isLocal, bool isDefined,
         int value) { // za equ sa vrednoscu
    this->name = name;
    this->type = type;
    this->isLocal = isLocal;
    this->isDefined = isDefined;
    this->value = value;
  }

  Symbol(string name, int section, int type, int id, bool isLocal,
         bool isDefined) { // za simbole u instrukciji koji nisu def
    this->section = section;
    this->name = name;
    this->type = type;
    this->id = id;
    this->isLocal = isLocal;
    this->isDefined = isDefined;
  }

  void setName(string name) { this->name = name; }
  string getName() { return this->name; }

  void setSection(int section) { this->section = section; }

  int getSection() { return this->section; }

  void setLocal(bool isLocal) { this->isLocal = isLocal; }
  bool getLocal() { return this->isLocal; }

  void sedId(int id) { this->id = id; }
  int getId() { return this->id; }

  void setIsDefined(bool isDefined) { this->isDefined = isDefined; }

  bool getDefined() { return this->isDefined; }

  void setSize(int size) { this->size = size; }
  int getSize() { return this->size; }

  void setValue(int value) { this->value = value; }
  int getValue() { return this->value; }

  string getFlags() { return this->flags; }

  void setNastaoSaEqu(bool nastaoSaEqu) { this->nastaoSaEqu = nastaoSaEqu; }
  bool getNastaoSaEqu() { return this->nastaoSaEqu; }
};

struct SymbolTableElem {
  SymbolTableElem *next;
  Symbol *symbol;
  SymbolTableElem(Symbol *symbol) {
    this->symbol = symbol;
    this->next = nullptr;
  }
};

class SymbolTableList {

public:
  SymbolTableElem *head, *tail;
  int size;

  SymbolTableList() {
    this->head = nullptr;
    this->tail = nullptr;
    size = 0;
  }
  ~SymbolTableList() {
    SymbolTableElem *pom;
    while (head != nullptr) {
      pom = head;
      head = pom->next;
      delete pom;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
  }

  void addElem(Symbol *symbol) {
    SymbolTableElem *pom = new SymbolTableElem(symbol);
    if (head == nullptr) {
      head = pom;
    } else {
      tail->next = pom;
    }
    tail = pom;
    size += 1;
  }

  Symbol *getSymbolByName(string name) {
    SymbolTableElem *pom = head;
    while (pom != nullptr) {
      if (pom->symbol->getName() == name) {
        return pom->symbol;
      } else {
        pom = pom->next;
      }
    }
    return nullptr;
  }
};
//--------------------------------------------------------------------------------------------------------------------------------------
class RelocationRecord {
  long offset;
  string type; // zapisi ovde literal ili nepoznati simbol i to pa posle kad
               // izbacujes u izlazni odradi ono sa njihovim siframa
  int vr;
  int section;

public:
  RelocationRecord(){};
  RelocationRecord(long offset, string type, int vr, int section) {
    this->offset = offset;
    this->type = type;
    this->vr = vr;
    this->section = section;
  }
  void setOffset(long offset) { this->offset = offset; }

  long getOffset() { return this->offset; }

  void setType(string type) { this->type = type; }

  string getType() { return this->type; }

  void setVr(int vr) { this->vr = vr; }

  int getVr() { return this->vr; }

  void setSection(int section) { this->section = section; }

  int getSection() { return this->section; }
};

struct RelocationListElem {
  RelocationRecord *relocationRecord;
  RelocationListElem *next;
  RelocationListElem(RelocationRecord *relocationRecord) {
    this->relocationRecord = relocationRecord;
    this->next = nullptr;
  }
};

class RelocationTable {

public:
  RelocationListElem *head, *tail;
  int size;

  void addElem(RelocationListElem *novi) {
    if (head == nullptr)
      head = novi;
    else
      tail->next = novi;
    tail = novi;
    size += 1;
  }
};
//--------------------------------------------------------------------------------------------------------------------------------------
class ForwardReference { //**insturkcije ciji masinski kod treba prepraviti**
  // **za instrukcije koje traze 2 B zapisao si da l su 2B ili nisu, zbog prepoznavanja**
  Symbol *symbol;
  int address;
  // **bajt na kom je koriscen nepoznati simbol, to je lc linije za prvi operand i jmp a tacan lc
  // za drugi**
  long value; // **ako je globalni simbol treba njegov id ako je lokalni onda
              // treba vr sekcije**
  string action; // **poruka koju na kraju razresavamo**
  int section;

public:
  ForwardReference(){};
  ForwardReference(Symbol *symbol, int address, long value, string action,
                   int section) {
    this->symbol = symbol;
    this->address = address;
    this->value = value;
    this->action = action;
    this->section = section;
  }
  void setSymbol(Symbol *symbol) { this->symbol = symbol; }
  Symbol *getSymbol() { return this->symbol; }
  void setAction(string action) { this->action = action; }
  string getAction() { return this->action; }

  int getSection() { return this->section; }

  int getAddress() { return this->address; }
};

struct ForwardRefElem {
  ForwardReference *forwardRef;
  ForwardRefElem *next;
  ForwardRefElem(ForwardReference *forwardRef) {
    this->forwardRef = forwardRef;
    this->next = nullptr;
  }
};

class ForwardReferenceList {

  int size = 0;

public:
  ForwardRefElem *head, *tail;

  ForwardReferenceList() {
    this->head = nullptr;
    this->tail = nullptr;
    size = 0;
  }
  ~ForwardReferenceList() {
    ForwardRefElem *pom;
    while (head != nullptr) {
      pom = head;
      head = pom->next;
      delete pom;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
  }
  void addElem(ForwardRefElem *pom) {
    if (head == nullptr) {
      this->head = pom;

    } else {
      tail->next = pom;
    }
    this->tail = pom;
    size += 1;
  }
};
//----------------------------------------------------------------------------------------------------------------------------------------

class OpCode {
  string mnemonic;
  string oc;
  int numOfOperands;

public:
  OpCode(){};
  OpCode(string mnemonic, string oc, int numOfOperands) {
    this->mnemonic = mnemonic;
    this->oc = oc;
    this->numOfOperands = numOfOperands;
  }

  void setMnemonic(string mnemonic) { this->mnemonic = mnemonic; }
  string getMnemonic() { return this->mnemonic; }

  void setOc(string oc) { this->oc = oc; }
  string getOc() { return this->oc; }

  void setSize(int numOfOperands) { this->numOfOperands = numOfOperands; }
  int getSize() { return this->numOfOperands; }

  int getNumOfOperands() { return this->numOfOperands; }
};

struct OpCodeTableElem {
  OpCode *opCode;
  OpCodeTableElem *next;

  OpCodeTableElem(OpCode *opCode) {
    this->opCode = opCode;
    this->next = nullptr;
  }
};

class OperationCodeTable {
  OpCodeTableElem *head, *tail;
  int size;

public:
  OperationCodeTable() {
    this->head = nullptr;
    this->tail = nullptr;
    size = 0;
  }
  ~OperationCodeTable() {
    OpCodeTableElem *pom;
    while (head != nullptr) {
      pom = head;
      head = pom->next;
      delete pom;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
  }

  void addElem(OpCode *opCode) {
    OpCodeTableElem *pom = new OpCodeTableElem(opCode);
    if (head == nullptr) {
      this->head = pom;

    } else {
      tail->next = pom;
    }
    this->tail = pom;
    size += 1;
  }

  OpCode *getElemByName(string name) {
    OpCodeTableElem *pom = this->head;
    while (pom != nullptr) {
      if (name.compare(pom->opCode->getMnemonic()) ==
          0) // izdebaguj ovo vidi da l je ok
        return pom->opCode;
      else
        pom = pom->next;
    }
    return nullptr;
  }

  void inic();
};
//------------------------------------------------------------------------------------------------------------------------------------
class Literal {
  int value;
  int section;
  string duzina;
  string name;
  long adresa;

public:
  Literal(int value, int section, string name) {
    this->value = value;
    this->section = section;
    this->name = name;
  };
  void setAdresa(long adresa) { this->adresa = adresa; }
  long getAdresa() { return this->adresa; }
  string getName() { return this->name; }
  void setName(string name) { this->name = name; }
  void setValue(int value) { this->value = value; }
  int getValue() { return this->value; }
};

struct LiteralPoolElem {
  Literal *literal;
  LiteralPoolElem *next;
  LiteralPoolElem(Literal *literal) {
    this->literal = literal;
    this->next = nullptr;
  }
};

class LiteralPool {
  LiteralPoolElem *head, *tail;
  int size;

public:
  Literal *getLiteral(string name) {
    LiteralPoolElem *pom = head;
    while (pom != nullptr) {
      if (pom->literal->getName() == name) {
        return pom->literal;
      } else {
        pom = pom->next;
      }
    }
    return nullptr;
  }

  void addElem(Literal *literal) {
    LiteralPoolElem *pom = new LiteralPoolElem(literal);
    if (head == nullptr)
      head = pom;
    else
      tail->next = pom;
    tail = pom;
    size += 1;
  }
};
//----------------------------------------------------------------------------------------------------------------------------------------
struct TabelaNeizracunljivihSimbolaElem {
  Symbol *leviSimbol;
  Symbol *section;
  // sekcija u kojoj je simbol trebalo da bude definisan da bi posle u
  // backpachingu mogla da se postavi sekicja simbola
  string izraz;
  TabelaNeizracunljivihSimbolaElem *next;
  TabelaNeizracunljivihSimbolaElem(Symbol *leviSimbol, string izraz,
                                   Symbol *section) {
    this->leviSimbol = leviSimbol;
    this->izraz = izraz;
    this->next = nullptr;
    this->section = section;
  }
};

class TabelaNeizracunljivihSimbola {

  int size;

public:
  TabelaNeizracunljivihSimbolaElem *head, *tail;

  TabelaNeizracunljivihSimbola() {
    this->head = nullptr;
    this->tail = nullptr;
    this->size = 0;
  }

  void addElem(TabelaNeizracunljivihSimbolaElem *pom) {
    if (head == nullptr)
      head = pom;
    else
      tail->next = pom;
    tail = pom;
    size += 1;
  }

  Symbol *getLeftSymbol(string name) {
    TabelaNeizracunljivihSimbolaElem *pom = head;
    while (pom != nullptr) {
      if (pom->leviSimbol->getName() == name) {
        return pom->leviSimbol;
      } else {
        pom = pom->next;
      }
    }
    return nullptr;
  }

  void izbaciSimbol(TabelaNeizracunljivihSimbolaElem *tnsElem) {
    TabelaNeizracunljivihSimbolaElem *pom = head, *back = nullptr;
    while (pom != nullptr) {
      if (pom->leviSimbol->getName().compare(tnsElem->leviSimbol->getName()) ==
          0) {

        if (back == nullptr) {
          head = head->next;
          size--;
        } else {
          back->next = pom->next;
          if (pom->leviSimbol->getName().compare(tail->leviSimbol->getName())) {
            tail = back;
          }
          size--;
        }
        return;
      } else {
        pom = pom->next;
      }
    }
  }

  int getSize() { return this->size; }
};
//------------------------------------------------------------------------------------------------------------------------------------------
class CodeForSection {

public:
  string machineCode;
  int section;
  int lc;
  int size;
  // number of bytes
  CodeForSection(int section, int lc, int size, string machineCode) {
    this->section = section;
    this->lc = lc;
    this->size = size;
    this->machineCode = machineCode;
  }
};

struct CodeTableElem {
  CodeForSection *code;
  CodeTableElem *next;
  CodeTableElem(CodeForSection *code) {
    this->code = code;
    this->next = nullptr;
  }
};

class CodeList {

public:
  CodeTableElem *head, *tail;
  int size;

  void addElem(CodeTableElem *newElem) {

    if (head == nullptr) {
      head = newElem;
    } else {
      tail->next = newElem;
    }
    tail = newElem;
    size += 1;
  }

  CodeForSection *getCode(int section, int lc) {
    CodeTableElem *pom = head;
    while (pom != nullptr) {
      if (pom->code->section == section) {
        if (lc == pom->code->lc)
          return pom->code;
        if (lc < (pom->code->lc + pom->code->size) && lc > pom->code->lc)
          return pom->code;
      }
      pom = pom->next;
    }
    return nullptr;
  }
};
//----------------------------------------------------------------------------------------------------------------------------------------

string getWord(string &line);

void izbaciZarez(string &word);

bool postojiOperacija(string word);

bool is_number(string s);

string intToHex(int i);

string intToBinary(int i);

string intToBinary(int i, int bits);

string binaryToHex(string i);

string byteToHex(string byte);

string fourBitsToHex(string bits);

string findRegId(string pom);

string zeroFill(string binary, int size);

string getRegName(string dst);

bool isJump(OpCode *opCode);

void sortRelocationRecordTable(RelocationTable *relocationTable);

string convertBits(string binary);

string convertNegativeBinary(string binary);

std::vector<string> split(std::string strToSplit, char delimeter);

int razresiTNS(int lc, SymbolTableList *symbolTable,
               TabelaNeizracunljivihSimbola *tabelaNeizracunljivihSimbola,
               SymbolTableList *allSections);

int razresiFR(int lc, SymbolTableList *symbolTable,
              TabelaNeizracunljivihSimbola *tabelaNeizracunljivihSimbola,
              SymbolTableList *allSections,
              ForwardReferenceList *forwardReferenceList, CodeList *codeList,
              SymbolTableList *externSimbols, RelocationTable *relocationTable);

void equSimbolVecPostojeci(
    Symbol *postojeci, string value, SymbolTableList *symbolTableList,
    int &symbolID, Symbol *activeSection,
    TabelaNeizracunljivihSimbola *tabelaNeizracunljivihSimbola,
    LiteralPool *literalPool);

void popuniBajtoveOperandaInstr(string operandAddress, string &opDescr,
                                string &im_Di_Ad1, string &im_Di_Ad2,
                                string &tipAdresiranja, int &lc, char c);

string jmpAddress(string word, SymbolTableList *symbolTable,
                  ForwardReferenceList *forwardReferenceList,
                  RelocationTable *relocationTable, Symbol *activeSection,
                  LiteralPool *literalPool, string &tip, int lc, int &symbolID);
// vracam vr adrese na koju se skace u bitima, ili vr operanda za neposredno

string operandAddress(string word, SymbolTableList *symbolTable,
                      ForwardReferenceList *forwardReferenceList,
                      RelocationTable *relocationTable, Symbol *activeSection,
                      LiteralPool *literalPool, string &tip, int lc,
                      int &symbolID, char c, bool drugiOperand, int pozicijaFR);
// ako je drugi operand ugradimo tacnu vrednost FR to posle proverimo
// adresa operanda ili vrednost samog operanda za neposredno
#endif