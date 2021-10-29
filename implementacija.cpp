#include "implementacija.h"
#include <bits/stdc++.h>
#include <cctype>

void OperationCodeTable::inic() {
  OpCode *opCode = new OpCode("halt", "00000", 0);
  this->addElem(opCode);
  opCode = new OpCode("iret", "00001", 0);
  this->addElem(opCode);
  opCode = new OpCode("ret", "00010", 0);
  this->addElem(opCode);
  opCode = new OpCode("int", "00011", 1);
  this->addElem(opCode);
  opCode = new OpCode("call", "00100", 1);
  this->addElem(opCode);
  opCode = new OpCode("jmp", "00101", 1);
  this->addElem(opCode);
  opCode = new OpCode("jeq", "00110", 1);
  this->addElem(opCode);
  opCode = new OpCode("jne", "00111", 1);
  this->addElem(opCode);
  opCode = new OpCode("jgt", "01000", 1);
  this->addElem(opCode);
  opCode = new OpCode("push", "01001", 1);
  this->addElem(opCode);
  opCode = new OpCode("pop", "01010", 1);
  this->addElem(opCode);
  opCode = new OpCode("xchg", "01011", 2);
  this->addElem(opCode);
  opCode = new OpCode("mov", "01100", 2);
  this->addElem(opCode);
  opCode = new OpCode("add", "01101", 2);
  this->addElem(opCode);
  opCode = new OpCode("sub", "01110", 2);
  this->addElem(opCode);
  opCode = new OpCode("mul", "01111", 2);
  this->addElem(opCode);
  opCode = new OpCode("div", "10000", 2);
  this->addElem(opCode);
  opCode = new OpCode("cmp", "10001", 2);
  this->addElem(opCode);
  opCode = new OpCode("not", "10010", 2);
  this->addElem(opCode);
  opCode = new OpCode("and", "10011", 2);
  this->addElem(opCode);
  opCode = new OpCode("or", "10100", 2);
  this->addElem(opCode);
  opCode = new OpCode("xor", "10101", 2);
  this->addElem(opCode);
  opCode = new OpCode("test", "10110", 2);
  this->addElem(opCode);
  opCode = new OpCode("shl", "10111", 2);
  this->addElem(opCode);
  opCode = new OpCode("shr", "11000", 2);
  this->addElem(opCode);
};

string getWord(string &line) {
  string word = "";

  while (line[0] == ' ' || line[0] == ',' || line[0] == '\t') {
    line.erase(0, 1);
  }

  for (char &c : line) {
    if (c == ' ' || c == ',' || line[0] == '\t') {
      if (word != "") {
        int pom = line.find(word);
        line.erase(pom, word.size());
        return word;
      }
    } else {
      word += c;
    }
  }
  if (line.size() == 1)
    line.erase(0, 1);
  return word;
}

void sortRelocationRecordTable(RelocationTable *relocationTable) {
  RelocationListElem *pom1, *pom2;

  if (relocationTable->head == nullptr)
    return;

  pom1 = relocationTable->head;
  while (pom1 != nullptr) {
    pom2 = pom1->next;
    while (pom2 != nullptr) {
      if (pom1->relocationRecord->getOffset() >
          pom2->relocationRecord->getOffset()) {
        int tempOffsetPom1 = pom1->relocationRecord->getOffset();
        int tempVrPom1 = pom1->relocationRecord->getVr();
        string tempTypePom1 = pom1->relocationRecord->getType();
        int tempSectionPom1 = pom1->relocationRecord->getSection();

        pom1->relocationRecord->setVr(pom2->relocationRecord->getVr());
        pom1->relocationRecord->setType(pom2->relocationRecord->getType());
        pom1->relocationRecord->setSection(
            pom2->relocationRecord->getSection());
        pom1->relocationRecord->setOffset(pom2->relocationRecord->getOffset());

        pom2->relocationRecord->setVr(tempVrPom1);
        pom2->relocationRecord->setType(tempTypePom1);
        pom2->relocationRecord->setSection(tempSectionPom1);
        pom2->relocationRecord->setOffset(tempOffsetPom1);
      }
      pom2 = pom2->next;
    }
    pom1 = pom1->next;
  }
}

void izbaciZarez(string &word) {
  if (word.find(',') != std::string::npos) {
    int pom = word.find(',');
    word.erase(pom, 1);
  }
}

bool postojiOperacija(string word) {
  if (word.find('+') != std::string::npos ||
      word.find('-') != std::string::npos) {
    return true;
  }
  return false;
}

bool is_number(std::string s) {
  if (s.at(0) == '-')
    s.erase(0, 1);
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it))
    ++it;
  return !s.empty() && it == s.end();
}

vector<string> split(string strToSplit, char delimeter) {
  std::stringstream ss(strToSplit);
  std::string item;
  std::vector<std::string> splittedStrings;
  while (std::getline(ss, item, delimeter)) {
    splittedStrings.push_back(item);
  }
  return splittedStrings;
}

string intToHex(int i) {
  stringstream sstream;
  sstream << hex << i;
  string res = sstream.str();

  return res;
}

string intToBinary(int n) {
  std::string r;
  while (n != 0) {
    r = (n % 2 == 0 ? "0" : "1") + r;
    n /= 2;
  }
  if ((r.size() % 4) != 0) {
    while ((r.size() % 4) != 0) {
      r = "0" + r;
    }
  }
  return r;
}

string intToBinary(int n, int bits) {

  int bin[bits];

  for (int i = 0; i < bits; i++)
    bin[i] = 0;

  bool negative;

  if (n < 0) {
    negative = true;
    n = n * (-1);
  }

  int k = 0;
  while (n > 0) {
    bin[k] = n % 2;
    n = n / 2;
    k++;
  }

  string binary = "";

  for (int i = bits - 1; i >= 0; i--) {
    binary += to_string(bin[i]);
  }

  if (!negative)
    return binary;
  else {
    return convertNegativeBinary(binary);
  }
}

string convertNegativeBinary(string binary) {

  string pom = "";
  int i;
  for (i = binary.size() - 1; i >= 0; i--) {
    if (binary.at(i) == '1') {
      pom += "1";
      i--;
      break;
    } else {
      pom += binary.at(i);
    }
  }

  while (i >= 0) {
    if (binary.at(i) == '1') {
      pom += "0";
    } else {
      pom += "1";
    }
    i--;
  }
  string ret = "";
  for (i = pom.size() - 1; i >= 0; i--) {
    ret += pom.at(i);
  }
  return ret;
}

string byteToHex(string byte) {

  if (byte.size() < 8)
    return "";

  string word2 = byte.substr(0, 4);
  string word1 = byte.substr(4, 4);

  string hex = "";

  if (word2 == "0000")
    hex.append("0");
  if (word2 == "0001")
    hex.append("1");
  if (word2 == "0010")
    hex.append("2");
  if (word2 == "0011")
    hex.append("3");
  if (word2 == "0100")
    hex.append("4");
  if (word2 == "0101")
    hex.append("5");
  if (word2 == "0110")
    hex.append("6");
  if (word2 == "0111")
    hex.append("7");
  if (word2 == "1000")
    hex.append("8");
  if (word2 == "1001")
    hex.append("9");
  if (word2 == "1010")
    hex.append("A");
  if (word2 == "1011")
    hex.append("B");
  if (word2 == "1100")
    hex.append("C");
  if (word2 == "1101")
    hex.append("D");
  if (word2 == "1110")
    hex.append("E");
  if (word2 == "1111")
    hex.append("F");

  if (word1 == "0000")
    hex.append("0");
  if (word1 == "0001")
    hex.append("1");
  if (word1 == "0010")
    hex.append("2");
  if (word1 == "0011")
    hex.append("3");
  if (word1 == "0100")
    hex.append("4");
  if (word1 == "0101")
    hex.append("5");
  if (word1 == "0110")
    hex.append("6");
  if (word1 == "0111")
    hex.append("7");
  if (word1 == "1000")
    hex.append("8");
  if (word1 == "1001")
    hex.append("9");
  if (word1 == "1010")
    hex.append("A");
  if (word1 == "1011")
    hex.append("B");
  if (word1 == "1100")
    hex.append("C");
  if (word1 == "1101")
    hex.append("D");
  if (word1 == "1110")
    hex.append("E");
  if (word1 == "1111")
    hex.append("F");

  return hex;
}

string fourBitsToHex(string bits) {
  if (bits.size() != 4)
    bits = "00" + bits;
  if (bits == "0000")
    return "0";
  if (bits == "0001")
    return "1";
  if (bits == "0010")
    return "2";
  if (bits == "0011")
    return "3";
  if (bits == "0100")
    return "4";
  if (bits == "0101")
    return "5";
  if (bits == "0110")
    return "6";
  if (bits == "0111")
    return "7";
  if (bits == "1000")
    return "8";
  if (bits == "1001")
    return "9";
  if (bits == "1010")
    return "a";
  if (bits == "1011")
    return "b";
  if (bits == "1100")
    return "c";
  if (bits == "1101")
    return "d";
  if (bits == "1110")
    return "e";
  if (bits == "1111")
    return "f";
  return "-1";
}

string binaryToHex(string s) {
  int fill = 4 - (s.size() % 4);
  while (fill > 0 && s.size() % 4 != 0) {
    s = "0" + s;
    fill--;
  }

  string res = "";
  int cnt = 1;
  while (s.size() > 0) {
    res += fourBitsToHex(s.substr(0, 4));
    s.erase(0, 4);
  }

  for (auto &c : res)
    c = toupper(c);

  return res;
}

string findRegId(string pom) {
  if (pom.size() == 2) {
    return pom.substr(1, 1);
  } else {
    return pom.substr(1, 2);
  }
}

string zeroFill(string binary, int size) {
  string pom = "";
  for (int i = 0; i < size; i++)
    pom.append("0");
  pom.append(binary);
  return pom;
}

string getRegName(string dst) {
  int i = dst.find('(');
  string pom = "";

  int j = i + 2;

  while (dst.at(j) != ')') {
    pom += dst.at(j);
    j++;
  }
  return pom;
}

bool isJump(OpCode *opCode) {
  if (opCode->getMnemonic().compare("jmp") == 0 ||
      opCode->getMnemonic().compare("jeq") == 0 ||
      opCode->getMnemonic().compare("jne") == 0 ||
      opCode->getMnemonic().compare("jgt") == 0 ||
      opCode->getMnemonic().compare("int") == 0 ||
      opCode->getMnemonic().compare("call") == 0) {
    return true;
  }
  return false;
}

void equSimbolVecPostojeci(
    Symbol *postojeci, string value, SymbolTableList *symbolTableList,
    int &symbolID, Symbol *activeSection,
    TabelaNeizracunljivihSimbola *tabelaNeizracunljivihSimbola,
    LiteralPool *literalPool) {

  if (postojiOperacija(value)) {

    vector<string> operands;

    if (value.find('+') != string::npos) {
      operands = split(value, '+');
    } else {
      operands = split(value, '-');
    }

    //**ako su oba operanda brojevi dodajemo odmah novi simbol**
    if (is_number(operands[0]) && is_number(operands[1])) {
      if (value.find('+') != string::npos) {
        postojeci->setIsDefined(true);
        postojeci->setSection(activeSection->getId());
        postojeci->setValue(stoi(operands[0]) + stoi(operands[1]));
        postojeci->setNastaoSaEqu(true);
      } else {
        postojeci->setIsDefined(true);
        postojeci->setSection(activeSection->getId());
        postojeci->setValue(stoi(operands[0]) - stoi(operands[1]));
        postojeci->setNastaoSaEqu(true);
      }

      Literal *o1 = literalPool->getLiteral(operands[0]);
      if (o1 == nullptr) {
        o1 =
            new Literal(stoi(operands[0]), activeSection->getId(), operands[0]);
        literalPool->addElem(o1);
      }

      Literal *o2 = literalPool->getLiteral(operands[0]);
      if (o2 == nullptr) {
        o2 =
            new Literal(stoi(operands[0]), activeSection->getId(), operands[0]);
        literalPool->addElem(o2);
      }
    }

    //**ako je prvi ili drugi operand  nedefinisan ili null odmah dodajemo
    // u tabelu neizracunljivih**
    if (!is_number(operands[0])) {

      Symbol *operand1 = symbolTableList->getSymbolByName(operands[0]);

      if ((operand1 != nullptr && !operand1->getDefined()) ||
          operand1 == nullptr) {

        if (tabelaNeizracunljivihSimbola->getLeftSymbol(postojeci->getName()) ==
            nullptr) {
          TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
              new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                   activeSection);
          tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
        }
      }
      if (operand1 == nullptr) {
        operand1 = new Symbol(operands[0], activeSection->getId(), 0, symbolID,
                              true, false);
        symbolID++;
        symbolTableList->addElem(operand1);
      }
    }

    if (!is_number(operands[1])) {

      Symbol *operand2 = symbolTableList->getSymbolByName(operands[1]);

      if ((operand2 != nullptr && !operand2->getDefined()) ||
          operand2 == nullptr) {

        if (tabelaNeizracunljivihSimbola->getLeftSymbol(postojeci->getName()) ==
            nullptr) {
          TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
              new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                   activeSection);
          tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
        }

        if (operand2 == nullptr) {
          operand2 = new Symbol(operands[1], activeSection->getId(), 0,
                                symbolID, true, false);
          symbolID++;
          symbolTableList->addElem(operand2);
        }
      }
    }

    //**ako je prvi operand literal a drugi operand simbol**
    if (is_number(operands[0]) && !is_number(operands[1])) {

      Symbol *operand2 = symbolTableList->getSymbolByName(operands[1]);

      if (operand2 != nullptr) {
        if (operand2->getDefined()) {
          int vr = 0;
          if (value.find('+') != string::npos) {
            vr = operand2->getValue() + stoi(operands[0]);
          } else {
            vr = operand2->getValue() - stoi(operands[0]);
          }
          postojeci->setIsDefined(true);
          postojeci->setSection(activeSection->getId());
          postojeci->setValue(vr);
          postojeci->setNastaoSaEqu(true);
        } else {

          if (tabelaNeizracunljivihSimbola->getLeftSymbol(
                  postojeci->getName())) {
            TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                     activeSection);
            tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
          }
        }
      } else {
        if (tabelaNeizracunljivihSimbola->getLeftSymbol(postojeci->getName()) ==
            nullptr) {
          TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
              new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                   activeSection);
          tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
        }

        operand2 = new Symbol(operands[1], activeSection->getId(), 0, symbolID,
                              true, false);
        symbolID++;
        symbolTableList->addElem(operand2);
      }

      Literal *o1 = literalPool->getLiteral(operands[0]);
      if (o1 == nullptr) {
        int sectionId;
        if (activeSection != nullptr) {
          sectionId = activeSection->getId();
        } else {
          sectionId = 0;
        }
        o1 = new Literal(stoi(operands[0]), sectionId, operands[0]);
        literalPool->addElem(o1);
      }
    }

    //**ako je prvi operand simbol a drugi operand literal**
    if (!is_number(operands[0]) && is_number(operands[1])) {

      Symbol *operand1 = symbolTableList->getSymbolByName(operands[0]);

      if (operand1 != nullptr) {
        if (operand1->getDefined()) {
          int vr = 0;
          if (value.find('+') != string::npos) {
            vr = operand1->getValue() + stoi(operands[1]);
          } else {
            vr = operand1->getValue() - stoi(operands[1]);
          }
          postojeci->setIsDefined(true);
          postojeci->setSection(activeSection->getId());
          postojeci->setValue(vr);
          postojeci->setNastaoSaEqu(true);
        } else {

          if (tabelaNeizracunljivihSimbola->getLeftSymbol(
                  postojeci->getName()) == nullptr) {
            TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                     activeSection);
            tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
          }
        }
      } else {
        if (tabelaNeizracunljivihSimbola->getLeftSymbol(postojeci->getName()) ==
            nullptr) {
          TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
              new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                   activeSection);
          tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
        }

        operand1 = new Symbol(operands[0], activeSection->getId(), 0, symbolID,
                              true, false);
        symbolID++;
        symbolTableList->addElem(operand1);
      }

      Literal *o2 = literalPool->getLiteral(operands[0]);
      if (o2 == nullptr) {
        int sectionId;
        if (activeSection != nullptr) {
          sectionId = activeSection->getId();
        } else {
          sectionId = 0;
        }
        o2 = new Literal(stoi(operands[0]), sectionId, operands[0]);
        literalPool->addElem(o2);
      }
    }

    //**ako su oba simboli**
    if (!is_number(operands[0]) && !is_number(operands[1])) {

      Symbol *operand1 = symbolTableList->getSymbolByName(operands[0]);
      Symbol *operand2 = symbolTableList->getSymbolByName(operands[1]);

      if (operand1 != nullptr && operand2 != nullptr) {
        if (operand1->getDefined() && operand2->getDefined()) {
          int vr = 0;
          if (value.find('+') != string::npos) {
            vr = operand1->getValue() + operand2->getValue();
          } else {
            vr = operand1->getValue() - operand2->getValue();
          }
          postojeci->setIsDefined(true);
          postojeci->setSection(activeSection->getId());
          postojeci->setValue(vr);
          postojeci->setNastaoSaEqu(true);
        } else {
          if (tabelaNeizracunljivihSimbola->getLeftSymbol(
                  postojeci->getName()) == nullptr) {
            TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                     activeSection);
            tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
          }
        }
      } else {
        if (tabelaNeizracunljivihSimbola->getLeftSymbol(postojeci->getName()) ==
            nullptr) {
          TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
              new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                   activeSection);
          tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
        }

        if (operand1 == nullptr) {
          operand1 = new Symbol(operands[0], activeSection->getId(), 0,
                                symbolID, true, false);
          symbolID++;
          symbolTableList->addElem(operand1);
        }
        if (operand2 == nullptr) {
          operand2 = new Symbol(operands[1], activeSection->getId(), 0,
                                symbolID, true, false);
          symbolID++;
          symbolTableList->addElem(operand2);
        }
      }
    }
  } else {
    if (is_number(value)) {
      //**ukoliko je vrednost samo literal**

      postojeci->setIsDefined(true);
      postojeci->setSection(activeSection->getId());
      postojeci->setValue(stoi(value));
      postojeci->setNastaoSaEqu(true);

      Literal *postoji = literalPool->getLiteral(value);

      if (postoji == nullptr) {
        Literal *noviLiteral =
            new Literal(stoi(value), activeSection->getId(), value);
        literalPool->addElem(noviLiteral);
      }

      //**ukoliko je vrednost simbol**
    } else {
      Symbol *symbolValue = symbolTableList->getSymbolByName(value);

      //**ukoliko je simbol odmah definisan, imamo odmah vrednost**
      if (symbolValue != nullptr && symbolValue->getDefined() == true) {
        postojeci->setIsDefined(true);
        postojeci->setSection(activeSection->getId());
        postojeci->setValue(symbolValue->getValue());
        postojeci->setNastaoSaEqu(true);

      } else {
        //**ukoliko simbol nije definisan, ostavljamo TNS ulaz**
        if (tabelaNeizracunljivihSimbola->getLeftSymbol(postojeci->getName()) ==
            nullptr) {
          TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
              new TabelaNeizracunljivihSimbolaElem(postojeci, value,
                                                   activeSection);
          tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
        }
      }
    }
  }
}

string jmpAddress(string word, SymbolTableList *symbolTable,
                  ForwardReferenceList *forwardReferenceList,
                  RelocationTable *relocationTable, Symbol *activeSecion,
                  LiteralPool *literalPool, string &tip, int lc,
                  int &symbolID) {

  //-----------------------------------------------------indirektno i direktno
  if (word.at(0) == '*') {

    //**registarsko direktno, skok 3 na listi**
    if (word.at(1) == '%') {
      int wordSize = word.size();
      string pom = word.substr(2, wordSize - 2);
      string binary = findRegId(pom);
      tip = "regdir";
      return binary;
      //**ovde je vracen registar ne adresa**
    }

    // **registarsko indirektno, skok 4 na listi**
    if (word.at(1) == '(' and word.at(2) == '%') {
      string pom = word.erase(word.size() - 1, 1);
      pom.erase(0, 3);
      string binary = findRegId(pom);
      tip = "regindir";
      return binary;
    }

    //**memorijsko direktno,skokovi 8 i 9 na listi**
    if (word.find('(') == string::npos) {

      string pom = word.erase(0, 1);

      //**provera da li je adresa literal ili simobl**
      if (is_number(pom)) {
        Literal *literal = literalPool->getLiteral(pom);
        if (literal == nullptr) {
          literal = new Literal(stoi(pom), activeSecion->getId(), pom);
          literalPool->addElem(literal);
        }
        tip = "memdir";
        return intToBinary(stoi(pom), 16);

        //-----------------------------------------------------simbol
      } else {

        Symbol *simbol = symbolTable->getSymbolByName(pom);
        tip = "memdir";
        if (simbol != nullptr && simbol->getDefined() == true) {
          if (activeSecion->getId() == simbol->getSection()) {

            //**skok je unutar sekcije i simbol je definisan
            //=>nema potrebe ostavljati relokacioni zapis**

            return intToBinary(simbol->getValue(), 16);

          } else {
            //**ako simbol nije u istoj sekciji ugradimo vrednost, 0 za globalni
            // vr za lokalni, i ostavimo relokacioni zapis**

            if (!simbol->getNastaoSaEqu()) {
              if (simbol->getLocal() == true) {
                RelocationRecord *relocationRec = new RelocationRecord(
                    lc + 2, "jmp razlicita sekcija lokalni",
                    simbol->getSection(), activeSecion->getId());
                relocationTable->addElem(new RelocationListElem(relocationRec));
              }

              else {
                RelocationRecord *relocationRec = new RelocationRecord(
                    lc + 2, "jmp razlicita sekcija globalni", simbol->getId(),
                    activeSecion->getId());
                relocationTable->addElem(new RelocationListElem(relocationRec));
              }
            }

            if (simbol->getLocal() == true)
              return intToBinary(simbol->getValue(), 16);
            else
              return intToBinary(0, 16);
          }
        } else if (simbol != nullptr && simbol->getDefined() == false) {

          ForwardReference *forwardRef = new ForwardReference(
              simbol, lc, simbol->getId(), "lokalni nedefinisan jmp",
              activeSecion->getId()); // vec postojao
          forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

          //**posle pravi relokacioni**

          return "?"; //**vraca se nesto sto mozes da prepoznas**

        } else { /* **ako simbol nije definisan treba da samo ostavis forward
                 ref i da stavis da nije definisan pa da posle proveris i
                 napravis rel zapis po potrebi** */
          Symbol *simbol =
              new Symbol(pom, activeSecion->getId(), 0, symbolID, true, false);
          symbolID++;
          symbolTable->addElem(simbol);

          ForwardReference *forwardRef = new ForwardReference(
              simbol, lc, simbol->getId(), "novi nedefinisan jmp",
              activeSecion->getId());
          forwardReferenceList->addElem(new ForwardRefElem(forwardRef));
          tip = "memdir";
          return "!";
        }
      }
      //**registarsko indirekto sa pomerajem, skokovi 5,6,7**
    } else {

      tip = "regindiroffset";

      int i = 1;

      string offset = "";
      for (; i < word.size(); i++) {
        if (word.at(i) != '(')
          offset += word.at(i);
        else
          break;
      }

      i += 2;
      string reg = "";
      for (; i < word.size(); i++) {
        if (word.at(i) != ')')
          reg += word.at(i);
        else
          break;
      }

      if (is_number(offset)) {

        if (stoi(findRegId(reg)) == 7) {
          int pom = stoi(offset) - 2;
          return intToBinary(pom, 16);
        } else {
          return intToBinary(stoi(offset), 16);
        }

      } else {

        Symbol *symbol = symbolTable->getSymbolByName(offset);

        if (symbol == nullptr) {

          symbol = new Symbol(offset, activeSecion->getId(), 0, symbolID, true,
                              false);
          symbolID++;
          symbolTable->addElem(symbol);

          ForwardReference *forwardRef = new ForwardReference(
              symbol, lc, symbol->getId(), "novi nedefinisan jmp",
              activeSecion->getId());
          forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

          if (stoi(findRegId(reg)) == 7) {
            return "-2";
          } else {
            return "?";
          }
        } else {
          if (symbol->getSection() == activeSecion->getId()) {

            if (symbol->getDefined()) {

              if (!symbol->getNastaoSaEqu()) {

                string pcRel = "";

                if (stoi(findRegId(reg)) == 7) {
                  pcRel = "pcRel";
                }

                if (!symbol->getLocal()) {
                  RelocationRecord *relocationRec = new RelocationRecord(
                      lc + 2, "jmp ista sekcija globalni " + pcRel,
                      symbol->getId(), activeSecion->getId());
                  relocationTable->addElem(
                      new RelocationListElem(relocationRec));
                }

                if (stoi(findRegId(reg)) != 7) {
                  RelocationRecord *relocationRec = new RelocationRecord(
                      lc + 2, "jmp ista sekcija lokalni ", symbol->getSection(),
                      activeSecion->getId());
                  relocationTable->addElem(
                      new RelocationListElem(relocationRec));
                }
              }

              if (symbol->getLocal()) {
                if (stoi(findRegId(reg)) == 7) {

                  return intToBinary(symbol->getValue() - 2, 16);

                } else {

                  return intToBinary(symbol->getValue(), 16);
                }
              } else {
                if (stoi(findRegId(reg)) == 7) {

                  return intToBinary(0 - 2, 16);

                } else {

                  return intToBinary(0, 16);
                }
              }

            } else {
              ForwardReference *forwardRef = new ForwardReference(
                  symbol, lc, symbol->getId(), "lokalni nedefinisan jmp",
                  activeSecion->getId()); // vec postojao
              forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

              //**posle pravi relokacioni**

              if (stoi(findRegId(reg)) == 7) {

                return "-2";
              } else {

                return "?";
              }
            }
          } else {
            if (symbol->getDefined()) {

              if (!symbol->getNastaoSaEqu()) {

                string pcRel = "";
                if (stoi(findRegId(reg)) == 7) {
                  pcRel = "pcRel";
                }

                if (symbol->getLocal() == true) {
                  RelocationRecord *relocationRec = new RelocationRecord(
                      lc + 2, "jmp razlicita sekcija lokalni " + pcRel,
                      symbol->getSection(), activeSecion->getId());
                  relocationTable->addElem(
                      new RelocationListElem(relocationRec));
                }

                else {
                  RelocationRecord *relocationRec = new RelocationRecord(
                      lc + 2, "jmp razlicita sekcija globalni " + pcRel,
                      symbol->getId(), activeSecion->getId());
                  relocationTable->addElem(
                      new RelocationListElem(relocationRec));
                }
              }

              if (stoi(findRegId(reg)) == 7) {

                return intToBinary(symbol->getValue() - 2, 16);

              } else {

                return intToBinary(symbol->getValue(), 16);
              }
            } else {
              ForwardReference *forwardRef = new ForwardReference(
                  symbol, lc, symbol->getId(), "lokalni nedefinisan jmp",
                  activeSecion->getId()); // vec postojao
              forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

              //**posle pravi relokacioni**

              if (stoi(findRegId(reg)) == 7) {

                return "-2";
              } else {

                return "?";
              }
            }
          }
        }
      }
    }
    //**direktni skokovi, skokovi 1,2 na listi**
  } else {
    if (is_number(word)) {

      Literal *literal = literalPool->getLiteral(word);

      if (literal == nullptr) {
        literal = new Literal(stoi(word), activeSecion->getId(), word);
        literalPool->addElem(literal);
      }
      tip = "immediate";
      return intToBinary(stoi(word), 16);

    } else {
      Symbol *simbol = symbolTable->getSymbolByName(word);
      tip = "immediate";

      if (simbol != nullptr && simbol->getDefined() == true) {
        // ako dohvatis simbol i on je definisan vrati tu vr u suprotnom
        // napravi forwardref

        if (activeSecion->getId() == simbol->getSection()) {

          if (!simbol->getNastaoSaEqu()) {
            if (simbol->getLocal() == true) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  lc + 2, "jmp ista sekcija lokalni", simbol->getSection(),
                  activeSecion->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            else {
              RelocationRecord *relocationRec =
                  new RelocationRecord(lc + 2, "jmp ista sekcija globalni",
                                       simbol->getId(), activeSecion->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }
          }
          if (simbol->getLocal() == true)
            return intToBinary(simbol->getValue(), 16);
          else
            return intToBinary(0, 16);

        } else {
          if (!simbol->getNastaoSaEqu()) {
            if (simbol->getLocal() == true) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  lc + 2, "jmp razlicita sekcija lokalni", simbol->getSection(),
                  activeSecion->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            else {
              RelocationRecord *relocationRec =
                  new RelocationRecord(lc + 2, "jmp razlicita sekcija globalni",
                                       simbol->getId(), activeSecion->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }
          }

          if (simbol->getLocal() == true)
            return intToBinary(simbol->getValue(), 16);
          else
            return intToBinary(0, 16);
        }

      } else if (simbol != nullptr && simbol->getDefined() == false) {

        ForwardReference *forwardRef = new ForwardReference(
            simbol, lc, simbol->getId(), "lokalni nedefinisan jmp",
            activeSecion->getId()); // vec postojao
        forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

        return "?"; //**bitno je da prepoznas kad se vratis tamo**

      } else { // **ako simbol nije definisan treba da samo ostavis forward ref
               // i da stavis da nije definisan pa da posle proveris**
        simbol =
            new Symbol(word, activeSecion->getId(), 0, symbolID, true, false);
        symbolID++;
        symbolTable->addElem(simbol);

        ForwardReference *forwardRef =
            new ForwardReference(simbol, lc, simbol->getId(),
                                 "novi nedefinisan jmp", activeSecion->getId());
        forwardReferenceList->addElem(new ForwardRefElem(forwardRef));
        tip = "immediate";
        return "!";
        //**bitno je da prepoznas kad se vratis tamo**
      }
    }
  }
  return "";
}

void popuniBajtoveOperandaInstr(string operandAddress, string &opDescr,
                                string &im_Di_Ad1, string &im_Di_Ad2,
                                string &tipAdresiranja, int &lc, char c) {

  if (c != 'b')
    c = 'w';

  //**neposredno**
  // iz metode operandAddress vraceno je intToBinary(vrSimbola) ili !
  if (tipAdresiranja.compare("immediate") == 0) {
    opDescr = "00000000";

    if (c == 'b') {
      if (operandAddress == "!") {
        im_Di_Ad1 = "!";
        im_Di_Ad2 = "";
      } else {

        im_Di_Ad1 = operandAddress;

        if (im_Di_Ad1.size() < 8)
          im_Di_Ad1 = zeroFill(im_Di_Ad1, 8 - im_Di_Ad1.size());
        im_Di_Ad2 = "";
        im_Di_Ad1 = byteToHex(im_Di_Ad1);
      }
    } else {
      if (operandAddress == "!") {
        im_Di_Ad1 = "!";
        im_Di_Ad2 = "!";
      } else {

        im_Di_Ad1 = operandAddress;

        if (im_Di_Ad1.size() < 16)
          im_Di_Ad1 = zeroFill(im_Di_Ad1, 16 - im_Di_Ad1.size());

        string addressL = im_Di_Ad1.substr(8, 8);
        string addressH = im_Di_Ad1.substr(0, 8);

        im_Di_Ad1 = byteToHex(addressL);
        im_Di_Ad2 = byteToHex(addressH);
      }
    }
    //**ovde je vracen samo broj registra kao string pa treba da se prebaci u
    // binarni oblik**
  } else if (tipAdresiranja.compare("regdir") == 0) {

    opDescr = "001";
    operandAddress = intToBinary(stoi(operandAddress), 4);
    opDescr.append(operandAddress);
    // **treba jos da se doda na kraju ako je L 0 ako je H 1,
    // odradjeno u glavnoj petlji**

  } else if (tipAdresiranja.compare("regindir") == 0) {

    opDescr = "010" + intToBinary(stoi(operandAddress), 4) + "0";

  } else if (tipAdresiranja.compare("memdir") == 0) {

    /*vracena adresa offseta kao intToBinary tako da ovde samo ugradis*/

    opDescr = "10000000";

    if (c == 'b') {
      if (operandAddress == "!") {
        im_Di_Ad1 = "!";
        im_Di_Ad2 = "";
      } else {

        im_Di_Ad1 = operandAddress;

        if (im_Di_Ad1.size() < 8)
          im_Di_Ad1 = zeroFill(im_Di_Ad1, 8 - im_Di_Ad1.size());

        im_Di_Ad2 = "";
        im_Di_Ad1 = byteToHex(im_Di_Ad1);
      }
    } else {
      if (operandAddress == "!") {
        im_Di_Ad1 = "!";
        im_Di_Ad2 = "!";
      } else {

        im_Di_Ad1 = operandAddress;

        if (im_Di_Ad1.size() < 16)
          im_Di_Ad1 = zeroFill(im_Di_Ad1, 16 - im_Di_Ad1.size());

        string addressL = im_Di_Ad1.substr(8, 8);
        string addressH = im_Di_Ad1.substr(0, 8);

        im_Di_Ad1 = byteToHex(addressL);
        im_Di_Ad2 = byteToHex(addressH);
      }
    }
  } else if (tipAdresiranja == "regIndiroffset") {

    opDescr = "011";

    if (c == 'b') {
      if (operandAddress == "!") {
        im_Di_Ad1 = "!";
        im_Di_Ad2 = "";
      } else if (operandAddress == "-2") {
        im_Di_Ad1 = "-2";
        im_Di_Ad2 = "";
      } else {

        im_Di_Ad1 = operandAddress;
        if (im_Di_Ad1.size() < 8)
          im_Di_Ad1 = zeroFill(im_Di_Ad1, 8 - im_Di_Ad1.size());
        im_Di_Ad2 = "";
        im_Di_Ad1 = byteToHex(im_Di_Ad1);
      }
    } else {
      if (operandAddress == "!") {
        im_Di_Ad1 = "!";
        im_Di_Ad2 = "!";
      } else if (operandAddress == "-2") {
        im_Di_Ad1 = "-2";
        im_Di_Ad2 = "-2";
      } else {

        im_Di_Ad1 = operandAddress;

        if (im_Di_Ad1.size() < 16)
          im_Di_Ad1 = zeroFill(im_Di_Ad1, 16 - im_Di_Ad1.size());

        string addressL = im_Di_Ad1.substr(8, 8);
        string addressH = im_Di_Ad1.substr(0, 8);

        im_Di_Ad1 = byteToHex(addressL);
        im_Di_Ad2 = byteToHex(addressH);
      }
    }
  }
}

string operandAddress(string word, SymbolTableList *symbolTable,
                      ForwardReferenceList *forwardReferenceList,
                      RelocationTable *relocationTable, Symbol *activeSection,
                      LiteralPool *literalPool, string &tip, int lc,
                      int &symbolID, char c, bool drugiOperand,
                      int pozicijaFR) {

  //**neposredna vrednost**
  if (word.at(0) == '$') {

    word.erase(0, 1);

    tip = "immediate";

    //**literal**
    if (is_number(word)) {

      Literal *literal = literalPool->getLiteral(word);

      if (literal == nullptr) {
        literal = new Literal(stoi(word), activeSection->getId(), word);
        literalPool->addElem(literal);
      }

      if (c == 'b') {
        return intToBinary(stoi(word), 8);
      } else {
        return intToBinary(stoi(word), 16);
      }

      //**simbol**
    } else {

      Symbol *symbol = symbolTable->getSymbolByName(word);

      if (symbol != nullptr) {
        if (symbol->getDefined() == true &&
            symbol->getSection() == activeSection->getId()) {

          int locationByte;
          if (drugiOperand)
            locationByte = pozicijaFR;
          else
            locationByte = lc + 2;

          if (!symbol->getNastaoSaEqu()) {
            if (symbol->getLocal() == true) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  locationByte, "op ista sekcija lokalni", symbol->getSection(),
                  activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            else {
              RelocationRecord *relocationRec =
                  new RelocationRecord(locationByte, "op ista sekcija globalni",
                                       symbol->getId(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }
          }

          if (symbol->getLocal()) {
            if (c == 'b') {
              return intToBinary(symbol->getValue(), 8);
            } else {
              return intToBinary(symbol->getValue(), 16);
            }
          } else
            return "00000000";

        } else if (symbol->getDefined() == true &&
                   symbol->getSection() != activeSection->getId()) {

          int locationByte;
          if (drugiOperand)
            locationByte = pozicijaFR;
          else
            locationByte = lc + 2;

          if (!symbol->getNastaoSaEqu()) {
            if (symbol->getLocal() == true) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  locationByte, "op razlicita sekcija lokalni",
                  symbol->getSection(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            else {
              RelocationRecord *relocationRec = new RelocationRecord(
                  locationByte, "op razlicita sekcija globalni",
                  symbol->getId(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }
          }

          if (c == 'b') {
            return intToBinary(symbol->getValue(), 8);
          } else {
            return intToBinary(symbol->getValue(), 16);
          }

        } else if (symbol->getDefined() == false) {

          string action;
          if (c == 'b') {
            action = "lokalni nedefinisan B";
          } else {
            action = "lokalni nedefinisan 2B";
          }

          int positionFR;
          if (drugiOperand) {
            positionFR = pozicijaFR;
            action += " drugi operand";
          } else
            positionFR = lc;

          ForwardReference *forwardRef =
              new ForwardReference(symbol, positionFR, symbol->getId(), action,
                                   activeSection->getId());
          //**kasnije dodajes relocationRecord po potrebi**
          forwardReferenceList->addElem(new ForwardRefElem(forwardRef));
          return "!";
        }
      } else {

        symbol =
            new Symbol(word, activeSection->getId(), 0, symbolID, true, false);
        symbolID++;
        symbolTable->addElem(symbol);

        string action;
        if (c == 'b') {
          action = "novi nedefinisan B";
        } else {
          action = "novi nedefinisan 2B";
        }

        int positionFR;
        if (drugiOperand) {
          positionFR = pozicijaFR;
          action += " drugi operand";
        } else
          positionFR = lc;

        ForwardReference *forwardRef =
            new ForwardReference(symbol, positionFR, symbol->getId(), action,
                                 activeSection->getId());
        //**kasnije dodajes relocationRecord po potrebi**
        forwardReferenceList->addElem(new ForwardRefElem(forwardRef));
        return "!";
      }
    }
    //**regdir**
  } else if (word.at(0) == '%') {

    tip = "regdir";

    word.erase(0, 2);
    string id = word;

    return id;

    //**regIndir**
  } else if (word.at(0) == '(' && word.at(1) == '%') {

    tip = "regindir";

    string reg = word.substr(2, word.size() - 3);
    string id = findRegId(reg);

    return id;

    //**memdrijsko direktno**
  } else if (word.find('$') == std::string::npos &&
             word.find('%') == std::string::npos) {

    tip = "memdir";

    if (is_number(word)) {

      Literal *literal = literalPool->getLiteral(word);

      if (literal == nullptr) {
        literal = new Literal(stoi(word), activeSection->getId(), word);
        literalPool->addElem(literal);
      }

      if (c == 'b') {
        return intToBinary(stoi(word), 8);
      } else {
        return intToBinary(stoi(word), 16);
      }

      //**simbol**
    } else {

      Symbol *symbol = symbolTable->getSymbolByName(word);

      if (symbol != nullptr) {
        if (symbol->getDefined() == true &&
            symbol->getSection() == activeSection->getId()) {

          int relocationByte;
          if (drugiOperand) {
            relocationByte = pozicijaFR;
          } else {
            relocationByte = lc + 2;
          }
          if (symbol->getLocal() == true) {
            if (!symbol->getNastaoSaEqu()) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  relocationByte, "op ista sekcija lokalni",
                  symbol->getSection(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            if (c == 'b') {
              return intToBinary(symbol->getValue(), 8);
            } else {
              return intToBinary(symbol->getValue(), 16);
            }
          } else {
            if (!symbol->getNastaoSaEqu()) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  relocationByte, "op ista sekcija globalni", symbol->getId(),
                  activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }
            return "00000000";
          }

        } else if (symbol->getDefined() == true &&
                   symbol->getSection() != activeSection->getId()) {

          int relocationByte;
          if (drugiOperand) {
            relocationByte = pozicijaFR;
          } else {
            relocationByte = lc + 2;
          }
          if (!symbol->getNastaoSaEqu()) {

            if (symbol->getLocal() == true) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  relocationByte, "op razlicita sekcija lokalni",
                  symbol->getSection(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            else {
              RelocationRecord *relocationRec = new RelocationRecord(
                  relocationByte, "op razlicita sekcija globalni",
                  symbol->getId(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }
          }
          if (symbol->getLocal()) {
            if (c == 'b') {
              return intToBinary(symbol->getValue(), 8);
            } else {
              return intToBinary(symbol->getValue(), 16);
            }
          } else {
            if (c == 'b') {
              return intToBinary(0, 8);
            } else {
              return intToBinary(0, 16);
            }
          }

        } else if (symbol->getDefined() == false) {
          string action;
          if (c == 'b') {
            action = "lokalni nedefinisan B";
          } else {
            action = "lokalni nedefinisan 2B";
          }

          int positionFR;
          if (drugiOperand) {
            positionFR = pozicijaFR;
            action += " drugi operand";
          } else
            positionFR = lc;

          ForwardReference *forwardRef =
              new ForwardReference(symbol, positionFR, symbol->getId(), action,
                                   activeSection->getId());
          //**kasnije dodajes relocationRecord po potrebi**
          forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

          return "!";
        }
      } else {

        symbol =
            new Symbol(word, activeSection->getId(), 0, symbolID, true, false);
        symbolID++;
        symbolTable->addElem(symbol);

        string action;
        if (c == 'b') {
          action = "novi nedefinisan B";
        } else {
          action = "novi nedefinisan 2B";
        }

        int positionFR;
        if (drugiOperand) {
          positionFR = pozicijaFR;
          action += " drugi operand";
        } else
          positionFR = lc;

        ForwardReference *forwardRef =
            new ForwardReference(symbol, positionFR, symbol->getId(), action,
                                 activeSection->getId());
        //**kasnije dodajes relocationRecord po potrebi**
        forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

        return "!";
      }
    }
  } else {
    tip = "regIndiroffset";

    string offset = "";
    int i = 0;
    while (word.at(i) != '(') {
      offset += word.at(i);
      i++;
    }

    string reg = word.substr(i + 2, word.size() - i - 3);
    int regId = stoi(findRegId(reg));

    if (is_number(offset)) {

      Literal *literal = literalPool->getLiteral(offset);

      if (literal == nullptr) {
        literal = new Literal(stoi(offset), activeSection->getId(), offset);
        literalPool->addElem(literal);
      }

      if (regId == 7) {

        int pom = stoi(offset) - 2;

        if (c == 'b') {
          return intToBinary(pom, 8);
        } else {
          return intToBinary(pom, 16);
        }

      } else {

        if (c == 'b') {
          return intToBinary(stoi(offset), 8);
        } else {
          return intToBinary(stoi(offset), 16);
        }
      }

      //**simbol**
    } else {
      Symbol *symbol = symbolTable->getSymbolByName(offset);
      if (symbol != nullptr) {
        if (symbol->getDefined() == true &&
            symbol->getSection() == activeSection->getId()) {

          int locationByte;
          if (drugiOperand)
            locationByte = pozicijaFR;
          else
            locationByte = lc + 2;

          if (!symbol->getNastaoSaEqu()) {

            string pcRel = "";

            if (regId == 7) {
              pcRel = "pcRel";
            }

            if (symbol->getLocal() == true && regId != 7) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  locationByte, "op ista sekcija lokalni", symbol->getSection(),
                  activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            //**ako je globalni u svakom slucaju ostaje relokacioni zapis
            // ako je lokalni ostaje samo ako nije PC relativno jer je ista
            // sekcija**

            if (!symbol->getLocal()) {
              RelocationRecord *relocationRec =
                  new RelocationRecord(locationByte, "op ista sekcija globalni "+pcRel,
                                       symbol->getId(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }
          }

          if (regId == 7) {
            if (symbol->getLocal() == true)
              if (c == 'b') {
                return intToBinary(symbol->getValue() - 2, 8);
              } else {
                return intToBinary(symbol->getValue() - 2, 16);
              }
            else {
              if (c == 'b') {
                return intToBinary(-2, 8);
              } else {
                return intToBinary(-2, 16);
              }
            }

          } else {
            if (symbol->getLocal() == true)
              if (c == 'b') {
                return intToBinary(symbol->getValue(), 8);
              } else {
                return intToBinary(symbol->getValue(), 16);
              }
            else {
              return "00000000";
            }
          }
        } else if (symbol->getDefined() == true &&
                   symbol->getSection() != activeSection->getId()) {

          //**Razlicita sekcija, relokacioni zapisi ostaju i za lokalni i za
          // globalne**

          int locationByte;
          if (drugiOperand)
            locationByte = pozicijaFR;
          else
            locationByte = lc + 2;

          string pcRel = "";

          if (regId == 7)
            pcRel = "pcRel";

          if (symbol->getLocal() == true) {
            if (!symbol->getNastaoSaEqu()) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  locationByte, "op razlicita sekcija lokalni " + pcRel,
                  symbol->getSection(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            if (regId == 7) {
              if (symbol->getLocal() == true)
                if (c == 'b') {
                  return intToBinary(symbol->getValue() - 2, 8);
                } else {
                  return intToBinary(symbol->getValue() - 2, 16);
                }
              else {
                if (c == 'b') {
                  return intToBinary(-2, 8);
                } else {
                  return intToBinary(-2, 16);
                }
              }
            } else {
              if (symbol->getLocal() == true)
                if (c == 'b') {
                  return intToBinary(symbol->getValue(), 8);
                } else {
                  return intToBinary(symbol->getValue(), 16);
                }
              else
                return "00000000";
            }
          }

          else {
            if (!symbol->getNastaoSaEqu()) {
              RelocationRecord *relocationRec = new RelocationRecord(
                  locationByte, "op razlicita sekcija globalni " + pcRel,
                  symbol->getId(), activeSection->getId());
              relocationTable->addElem(new RelocationListElem(relocationRec));
            }

            if (regId == 7) {
              if (symbol->getLocal() == true)
                if (c == 'b') {
                  return intToBinary(symbol->getValue() - 2, 8);
                } else {
                  return intToBinary(symbol->getValue() - 2, 16);
                }
              else {
                if (c == 'b') {
                  return intToBinary(-2, 8);
                } else {
                  return intToBinary(-2, 16);
                }
              }
            } else {
              if (symbol->getLocal() == true)
                if (c == 'b') {
                  return intToBinary(symbol->getValue(), 8);
                } else {
                  return intToBinary(symbol->getValue(), 16);
                }
              else
                return "00000000";
            }
          }

        } else if (symbol->getDefined() == false) {

          string action;
          if (c == 'b') {
            action = "lokalni nedefinisan B";
          } else {
            action = "lokalni nedefinisan 2B";
          }
          int positionFR;
          if (drugiOperand) {
            positionFR = pozicijaFR;
            action += " drugi operand";
          } else
            positionFR = lc;

          ForwardReference *forwardRef =
              new ForwardReference(symbol, positionFR, symbol->getId(), action,
                                   activeSection->getId());
          forwardReferenceList->addElem(new ForwardRefElem(forwardRef));
          if (regId == 7)
            return "-2";
          else
            return "!";
        }
      } else {

        symbol = new Symbol(offset, activeSection->getId(), 0, symbolID, true,
                            false);
        symbolID++;
        symbolTable->addElem(symbol);

        string action;
        if (c == 'b') {
          action = "novi nedefinisan B";
        } else {
          action = "novi nedefinisan 2B";
        }

        int positionFR;
        if (drugiOperand) {
          positionFR = pozicijaFR;
          action += " drugi operand";
        } else
          positionFR = lc;

        ForwardReference *forwardRef =
            new ForwardReference(symbol, positionFR, symbol->getId(), action,
                                 activeSection->getId());
        forwardReferenceList->addElem(new ForwardRefElem(forwardRef));
        if (regId == 7)
          return "-2";
        else
          return "!";
      }
    }
  }
  return "";
}

int razresiTNS(int lc, SymbolTableList *symbolTable,
               TabelaNeizracunljivihSimbola *tabelaNeizracunljivihSimbola,
               SymbolTableList *allSections) {

  int trenSize = tabelaNeizracunljivihSimbola->getSize();

  TabelaNeizracunljivihSimbolaElem *tnsElem =
      tabelaNeizracunljivihSimbola->head;

  bool nastavi = true;

  while (nastavi) {
    tnsElem = tabelaNeizracunljivihSimbola->head;
    while (tnsElem != nullptr) {

      string izraz = tnsElem->izraz;

      if (postojiOperacija(izraz)) {

        vector<string> operands;

        if (izraz.find('+') != string::npos) {
          operands = split(izraz, '+');
        } else {
          operands = split(izraz, '-');
        }

        int value = 0;

        if (is_number(operands[0]))
          value += stoi(operands[0]);
        else {

          Symbol *operand1 = symbolTable->getSymbolByName(operands[0]);
          if (operand1 == nullptr) {
            tnsElem = tnsElem->next;
            continue;
          } else {
            if (operand1->getDefined() == false) {
              tnsElem = tnsElem->next;
              continue;
            } else {
              value += operand1->getValue();
            }
          }
        }

        if (is_number(operands[1])) {

          if (izraz.find('+') != string::npos) {
            value += stoi(operands[1]);
          } else {
            value -= stoi(operands[1]);
          }

        } else {
          Symbol *operand1 = symbolTable->getSymbolByName(operands[1]);
          if (operand1 == nullptr) {
            tnsElem = tnsElem->next;
            continue;
          } else {
            if (operand1->getDefined() == false) {
              tnsElem = tnsElem->next;
              continue;
            } else {
              if (izraz.find('+') != string::npos) {
                value += operand1->getValue();
              } else {
                value += operand1->getValue();
              }
            }
          }
        }

        tnsElem->leviSimbol->setIsDefined(true);
        tnsElem->leviSimbol->value=value;
        tnsElem->leviSimbol->setNastaoSaEqu(true);
        if (tnsElem->section == nullptr) {
          tnsElem->leviSimbol->setSection(0);
        } else
          tnsElem->leviSimbol->setSection(tnsElem->section->getId());

        tabelaNeizracunljivihSimbola->izbaciSimbol(tnsElem);
      } else {

        int vr = 0;

        if (is_number(izraz))
          vr += stoi(izraz);
        else {
          Symbol *operand1 = symbolTable->getSymbolByName(izraz);
          if (operand1 == nullptr) {
            tnsElem = tnsElem->next;
            continue;
          } else {
            if (operand1->getDefined() == false) {
              tnsElem = tnsElem->next;
              continue;
            } else {
              vr += operand1->getValue();
            }
          }
        }

        tnsElem->leviSimbol->setIsDefined(true);
         tnsElem->leviSimbol->value=vr;
        if (tnsElem->section == nullptr) {
          tnsElem->leviSimbol->setSection(0);
        } else
          tnsElem->leviSimbol->setSection(tnsElem->section->getId());
        tnsElem->leviSimbol->setNastaoSaEqu(true);

        tabelaNeizracunljivihSimbola->izbaciSimbol(tnsElem);
      }
      tnsElem = tnsElem->next;
    }

    if (trenSize == 0)
      return 0;

    if (trenSize == tabelaNeizracunljivihSimbola->getSize()) {
      nastavi = false;
      printf("Postoji kruzna zavisnost simbola, ili simboli nisu definisani \n");
      return -1;
    } else {
      trenSize = tabelaNeizracunljivihSimbola->getSize();
    }
  }
  return 0;
}

int razresiFR(int lc, SymbolTableList *symbolTable,
              TabelaNeizracunljivihSimbola *tabelaNeizracunljivihSimbola,
              SymbolTableList *allSections,
              ForwardReferenceList *forwardReferenceList, CodeList *codeList,
              SymbolTableList *externSimbols,
              RelocationTable *relocationTable) {

  ForwardRefElem *forwardRefIterator = forwardReferenceList->head;

  while (forwardRefIterator != nullptr) {

    int section = forwardRefIterator->forwardRef->getSection();
    int lc = forwardRefIterator->forwardRef->getAddress();

    CodeForSection *codeForSection = codeList->getCode(section, lc);

    if (codeForSection != nullptr) {

      string action = forwardRefIterator->forwardRef->getAction();
      Symbol *symbol = forwardRefIterator->forwardRef->getSymbol();

      string jmpAddress = "";

      if (!symbol->getDefined()) {
        printf("Simbol je idalje nedefinisan");
        return -1;
      }

      if (action.find("jmp") != std::string::npos) {

        if (action.find("novi") != std::string::npos) {

          if (codeForSection->machineCode.find("!") != std::string::npos ||
              codeForSection->machineCode.find("?") != std::string::npos) {

            int pos;

            if (codeForSection->machineCode.find("!") != std::string::npos)
              pos = codeForSection->machineCode.find("!");
            else
              pos = codeForSection->machineCode.find("?");

            //**ukoliko je simbol eksterni dodajemo relokacioni zapis i
            // upisujemo nule**
            if (externSimbols->getSymbolByName(symbol->getName())) {
              jmpAddress = "00 00 ";
              RelocationRecord *relocRecord = new RelocationRecord(
                  codeForSection->lc + 2, "jmp globlani simbol",
                  symbol->getId(), codeForSection->section);
              relocationTable->addElem(new RelocationListElem(relocRecord));
              codeForSection->machineCode.erase(pos, 4);
              codeForSection->machineCode.insert(pos, jmpAddress);

              //**simbol je unutar sekcije, po potrebi dodati relokacioni**
            } else {
              string addr = intToBinary(symbol->getValue(), 16);

              if (!symbol->getNastaoSaEqu()) {

                if (!symbol->getLocal()) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "jmp globlani simbol",
                      symbol->getId(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));

                } else if (symbol->getSection() != codeForSection->section) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "jmp lokalni simbol",
                      symbol->getSection(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                }
              }

              if (!symbol->getLocal())
                addr = "00000000";

              if (addr.size() < 16)
                addr = zeroFill(addr, 16 - addr.size());
              string addressL = addr.substr(8, 8);
              string addressH = addr.substr(0, 8);
              addressL = binaryToHex(addressL);
              addressH = binaryToHex(addressH);
              jmpAddress = addressL + " " + addressH + " ";
              codeForSection->machineCode.erase(pos, 4);
              codeForSection->machineCode.insert(pos, jmpAddress);
            }

          } else {

            int pos = codeForSection->machineCode.find("-2");

            string jmpAddress = "";

            if (externSimbols->getSymbolByName(symbol->getName())) {
              jmpAddress = "FE FF ";
              RelocationRecord *relocRecord = new RelocationRecord(
                  codeForSection->lc + 2, "jmp globlani simbol pcRel",
                  symbol->getId(), codeForSection->section);
              relocationTable->addElem(new RelocationListElem(relocRecord));
              codeForSection->machineCode.erase(pos, 6);
              codeForSection->machineCode.insert(pos, jmpAddress);

              //**simbol je unutar sekcije, po potrebi dodati relokacioni**
            } else {
              string addr;

              if (!symbol->getNastaoSaEqu()) {
                if (!symbol->getLocal()) {

                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "jmp globlani simbol pcRel",
                      symbol->getId(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));

                } else if (symbol->getSection() != codeForSection->section) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "jmp lokalni simbol pcRel",
                      symbol->getSection(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                }
              }

              if (symbol->getValue() == 0 || !symbol->getLocal()) {
                addr = "FE FF ";

                codeForSection->machineCode.erase(pos, 6);
                codeForSection->machineCode.insert(pos, jmpAddress);

              } else {

                addr = intToBinary(symbol->getValue() - 2);

                if (addr.size() < 16)
                  addr = zeroFill(addr, 16 - addr.size());
                string addressL = addr.substr(8, 8);
                string addressH = addr.substr(0, 8);
                addressL = binaryToHex(addressL);
                addressH = binaryToHex(addressH);
                jmpAddress = addressL + " " + addressH + " ";
                codeForSection->machineCode.erase(pos, 6);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }
            }
          }

        } else {
          if (!symbol->getDefined()) {
            printf("Simbol je idalje nedefinisan");
            return -1;
          }
          int pos = 0;
          string jmpAddress = "";

          if (codeForSection->machineCode.find("?") != string::npos) {

            pos = codeForSection->machineCode.find("?");

            //**ukoliko je simbol eksterni dodajemo relokacioni zapis i
            // upisujemo nule**
            if (externSimbols->getSymbolByName(symbol->getName())) {
              jmpAddress = "00 00 ";
              RelocationRecord *relocRecord = new RelocationRecord(
                  codeForSection->lc + 2, "globlani simbol", symbol->getId(),
                  codeForSection->section);
              relocationTable->addElem(new RelocationListElem(relocRecord));
              codeForSection->machineCode.erase(pos, 4);
              codeForSection->machineCode.insert(pos, jmpAddress);

              //**simbol je unutar sekcije, po potrebi dodati relokacioni**
            } else {

              if (!symbol->getNastaoSaEqu()) {
                if (!symbol->getLocal()) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "globalni simbol",
                      symbol->getId(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                } else {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "lokalni simbol",
                      symbol->getSection(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                }
              }

              string addr = intToBinary(symbol->getValue());
              if (!symbol->getLocal())
                addr = "00000000";
              if (addr.size() < 16)
                addr = zeroFill(addr, 16 - addr.size());
              string addressL = addr.substr(8, 8);
              string addressH = addr.substr(0, 8);
              addressL = binaryToHex(addressL);
              addressH = binaryToHex(addressH);
              jmpAddress = addressL + " " + addressH + " ";
              codeForSection->machineCode.erase(pos, 4);
              codeForSection->machineCode.insert(pos, jmpAddress);
            }

          } else {
            pos = codeForSection->machineCode.find("-2");
            string addr = intToBinary(symbol->getValue() - 2);

            if (!symbol->getNastaoSaEqu()) {
              if (!symbol->getLocal()) {

                RelocationRecord *relocRecord = new RelocationRecord(
                    codeForSection->lc + 2, "jmp globlani simbol pcRel",
                    symbol->getId(), codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));

              } else if (symbol->getSection() != codeForSection->section) {
                RelocationRecord *relocRecord = new RelocationRecord(
                    codeForSection->lc + 2, "jmp lokalni simbol pcRel",
                    symbol->getSection(), codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
              }
            }

            if (symbol->getValue() == 0 || !symbol->getLocal())
              jmpAddress = "FE FF";
            else {
              if (symbol->getValue() - 2 < 0) {
                addr = intToBinary(symbol->getValue() - 2, 16);

              } else {
                addr = zeroFill(addr, 16 - addr.size());
              }
              string addressL = addr.substr(8, 8);
              string addressH = addr.substr(0, 8);
              addressL = binaryToHex(addressL);
              addressH = binaryToHex(addressH);
              jmpAddress = addressL + " " + addressH + " ";
            }
            codeForSection->machineCode.erase(pos, 6);
            codeForSection->machineCode.insert(pos, jmpAddress);
          }
        }
      } else if (action.find("B") != string::npos) {

        //**Operacije**
        //--prvi operand--
        if (lc == codeForSection->lc) {

          if (codeForSection->machineCode.at(6) == '!') {
            int pos = codeForSection->machineCode.find("!");
            string jmpAddress = "";

            if (externSimbols->getSymbolByName(symbol->getName())) {
              if (action.find("2B") != std::string::npos) {
                jmpAddress = "00 00 ";
                RelocationRecord *relocRecord = new RelocationRecord(
                    codeForSection->lc + 2, "globlani simbol", symbol->getId(),
                    codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
                codeForSection->machineCode.erase(pos, 4);
                codeForSection->machineCode.insert(pos, jmpAddress);
              } else {
                jmpAddress = "00 ";
                RelocationRecord *relocRecord = new RelocationRecord(
                    codeForSection->lc + 2, "globlani simbol", symbol->getId(),
                    codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
                codeForSection->machineCode.erase(pos, 2);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }

              //**simbol je unutar sekcije, po potrebi dodati relokacioni**
            } else {

              if (!symbol->getNastaoSaEqu()) {
                if (!symbol->getLocal()) {

                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "globlani simbol",
                      symbol->getId(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));

                } else if (symbol->getSection() != codeForSection->section) {

                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "lokalni simbol",
                      symbol->getSection(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                }
              }

              string addr = intToBinary(symbol->getValue());
              if (!symbol->getLocal() && !symbol->getNastaoSaEqu())
                addr = "00000000";

              if (action.find("2B") != std::string::npos) {
                if (addr.size() < 16)
                  addr = zeroFill(addr, 16 - addr.size());
                string addressL = addr.substr(8, 8);
                string addressH = addr.substr(0, 8);
                addressL = binaryToHex(addressL);
                addressH = binaryToHex(addressH);
                jmpAddress = addressL + " " + addressH + " ";
                codeForSection->machineCode.erase(pos, 4);
                codeForSection->machineCode.insert(pos, jmpAddress);
              } else {
                if (addr.size() < 8)
                  addr = zeroFill(addr, 8 - addr.size());
                addr = binaryToHex(addr);
                jmpAddress = addr + " ";
                codeForSection->machineCode.erase(pos, 2);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }
            }
          } else {

            int pos = codeForSection->machineCode.find("-");
            int konstanta = codeForSection->machineCode.at(pos + 1) - '0';
            string jmpAddress = "";

            if (externSimbols->getSymbolByName(symbol->getName())) {
              if (action.find("2B") != std::string::npos) {
                if (konstanta == 5) {
                  jmpAddress = "FB FF ";
                } else {
                  jmpAddress = "FD FF ";
                }

                RelocationRecord *relocRecord = new RelocationRecord(
                    codeForSection->lc + 2, "globlani simbol pcRel",
                    symbol->getId(), codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
                codeForSection->machineCode.erase(pos, 6);
                codeForSection->machineCode.insert(pos, jmpAddress);
              } else {
                if (konstanta == 5) {
                  jmpAddress = "FB ";
                } else {
                  jmpAddress = "FD ";
                }
                RelocationRecord *relocRecord = new RelocationRecord(
                    codeForSection->lc + 2, "globlani simbol pcRel",
                    symbol->getId(), codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
                codeForSection->machineCode.erase(pos, 3);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }

              //**simbol je unutar sekcije, po potrebi dodati relokacioni**
            } else {

              string addr = "";

              if (!symbol->getNastaoSaEqu()) {

                if (!symbol->getLocal()) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "globlani simbol pcRel",
                      symbol->getId(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));

                } else if (symbol->getSection() != codeForSection->section) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      codeForSection->lc + 2, "lokalni simbol pcRel",
                      symbol->getSection(), codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                }
              }

              if (action.find("2B") != std::string::npos)
                addr = intToBinary(symbol->getValue() - konstanta, 16);
              else
                addr = intToBinary(symbol->getValue() - konstanta, 8);

              if (action.find("2B") != std::string::npos) {
                if (symbol->getValue() == 0 ||
                    (!symbol->getLocal() && !symbol->getNastaoSaEqu())) {
                  if (konstanta == 5) {
                    jmpAddress = "FB FF ";
                  } else {
                    jmpAddress = "FD FF ";
                  }
                } else {
                  if (addr.size() < 16)
                    addr = zeroFill(addr, 16 - addr.size());
                  string addressL = addr.substr(8, 8);
                  string addressH = addr.substr(0, 8);
                  addressL = binaryToHex(addressL);
                  addressH = binaryToHex(addressH);
                  jmpAddress = addressL + " " + addressH + " ";
                }
                codeForSection->machineCode.erase(pos, 6);
                codeForSection->machineCode.insert(pos, jmpAddress);
              } else {
                if (symbol->getValue() == 0 || !symbol->getLocal()) {
                  if (konstanta == 5) {
                    jmpAddress = "FB ";
                  } else {
                    jmpAddress = "FD ";
                  }
                } else {
                  if (addr.size() < 8)
                    addr = zeroFill(addr, 8 - addr.size());
                  addr = binaryToHex(addr);
                  jmpAddress = addr + " ";
                }
                codeForSection->machineCode.erase(pos, 3);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }
            }
          }
          //---drugi operand--
        } else {

          int pos = 0;
          int brojac = codeForSection->lc;
          for (int i = 0; i < codeForSection->machineCode.size(); i++) {
            if (codeForSection->machineCode.at(i) == ' ') {
              brojac++;
              pos = i + 1;
              if (brojac == lc)
                break;
            }
          }

          if (codeForSection->machineCode.find("-2") != string::npos) {

            if (externSimbols->getSymbolByName(symbol->getName())) {
              if (action.find("2B") != std::string::npos) {
                jmpAddress = "FE FF ";

                RelocationRecord *relocRecord = new RelocationRecord(
                    brojac, "globlani simbol pcRel", symbol->getId(),
                    codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
                codeForSection->machineCode.erase(pos, 6);
                codeForSection->machineCode.insert(pos, jmpAddress);
              } else {
                jmpAddress = "FE ";
                RelocationRecord *relocRecord = new RelocationRecord(
                    brojac, "globlani simbol pcRel", symbol->getId(),
                    codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
                codeForSection->machineCode.erase(pos, 3);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }

              //**simbol je unutar sekcije, po potrebi dodati relokacioni**
            } else {

              string addr = intToBinary(symbol->getValue() - 2);

              if (!symbol->getNastaoSaEqu()) {
                if (!symbol->getLocal()) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      brojac, "globlani simbol pcRel", symbol->getId(),
                      codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                } else if (symbol->getSection() != codeForSection->section) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      brojac, "lokalni simbol pcRel", symbol->getSection(),
                      codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                }
              }

              if (action.find("2B") != std::string::npos) {
                if (symbol->getValue() == 0 ||
                    (!symbol->getLocal() && !symbol->getNastaoSaEqu())) {
                  jmpAddress = "FE FF ";
                } else {
                  if (addr.size() < 16)
                    addr = zeroFill(addr, 16 - addr.size());
                  string addressL = addr.substr(8, 8);
                  string addressH = addr.substr(0, 8);
                  addressL = binaryToHex(addressL);
                  addressH = binaryToHex(addressH);
                  jmpAddress = addressL + " " + addressH + " ";
                }
                codeForSection->machineCode.erase(pos, 6);
                codeForSection->machineCode.insert(pos, jmpAddress);
              } else {
                if (symbol->getValue() == 0 || !symbol->getLocal()) {
                  jmpAddress = "FE ";
                } else {
                  if (addr.size() < 8)
                    addr = zeroFill(addr, 8 - addr.size());
                  addr = binaryToHex(addr);
                  jmpAddress = addr + " ";
                }
                codeForSection->machineCode.erase(pos, 3);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }
            }
          } else {

            if (externSimbols->getSymbolByName(symbol->getName())) {
              if (action.find("2B") != std::string::npos) {
                jmpAddress = "00 00 ";
                RelocationRecord *relocRecord = new RelocationRecord(
                    brojac, "globlani simbol", symbol->getId(),
                    codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
                codeForSection->machineCode.erase(pos, 4);
                codeForSection->machineCode.insert(pos, jmpAddress);
              } else {
                jmpAddress = "00 ";
                RelocationRecord *relocRecord = new RelocationRecord(
                    brojac, "globlani simbol", symbol->getId(),
                    codeForSection->section);
                relocationTable->addElem(new RelocationListElem(relocRecord));
                codeForSection->machineCode.erase(pos, 2);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }

              //**simbol je unutar sekcije, po potrebi dodati relokacioni**
            } else {

              if (!symbol->getNastaoSaEqu()) {
                if (!symbol->getLocal()) {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      brojac, "globlani simbol", symbol->getId(),
                      codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                } else {
                  RelocationRecord *relocRecord = new RelocationRecord(
                      brojac, "lokalni simbol", symbol->getSection(),
                      codeForSection->section);
                  relocationTable->addElem(new RelocationListElem(relocRecord));
                }
              }

              string addr = intToBinary(symbol->getValue());
              if (!symbol->getLocal() && !symbol->getNastaoSaEqu())
                addr = "00000000";

              if (action.find("2B") != std::string::npos) {
                if (addr.size() < 16)
                  addr = zeroFill(addr, 16 - addr.size());
                string addressL = addr.substr(8, 8);
                string addressH = addr.substr(0, 8);
                addressL = binaryToHex(addressL);
                addressH = binaryToHex(addressH);
                jmpAddress = addressL + " " + addressH + " ";
                codeForSection->machineCode.erase(pos, 4);
                codeForSection->machineCode.insert(pos, jmpAddress);
              } else {
                if (addr.size() < 8)
                  addr = zeroFill(addr, 8 - addr.size());
                addr = binaryToHex(addr);
                jmpAddress = addr + " ";
                codeForSection->machineCode.erase(pos, 2);
                codeForSection->machineCode.insert(pos, jmpAddress);
              }
            }
          }
        }

      } else if (action.find("word") != std::string::npos) {
        int pos;
        if (codeForSection->machineCode.find("!") != string::npos)
          pos = codeForSection->machineCode.find("!");
        else {
          pos = codeForSection->machineCode.find("?");
        }

        string addr = intToBinary(symbol->getValue());

        if (!symbol->getLocal() && !symbol->getNastaoSaEqu())
          addr = "00000000";

        if (externSimbols->getSymbolByName(symbol->getName())) {

          jmpAddress = "00 00 ";

          RelocationRecord *relocRecord =
              new RelocationRecord(codeForSection->lc, "globlani simbol",
                                   symbol->getId(), codeForSection->section);
          relocationTable->addElem(new RelocationListElem(relocRecord));
          codeForSection->machineCode.erase(pos, 4);
          codeForSection->machineCode.insert(pos, jmpAddress);

          //**simbol je unutar sekcije, po potrebi dodati relokacioni**
        } else {

          if (!symbol->getNastaoSaEqu()) {
            if (!symbol->getLocal()) {
              RelocationRecord *relocRecord = new RelocationRecord(
                  codeForSection->lc, "globlani simbol", symbol->getId(),
                  codeForSection->section);
              relocationTable->addElem(new RelocationListElem(relocRecord));
            } else {
              RelocationRecord *relocRecord = new RelocationRecord(
                  codeForSection->lc, "lokalni simbol", symbol->getSection(),
                  codeForSection->section);
              relocationTable->addElem(new RelocationListElem(relocRecord));
            }
          }

          if (addr.size() < 16)
            addr = zeroFill(addr, 16 - addr.size());
          string addressL = addr.substr(8, 8);
          string addressH = addr.substr(0, 8);
          addressL = binaryToHex(addressL);
          addressH = binaryToHex(addressH);
          jmpAddress = addressL + " " + addressH + " ";
          codeForSection->machineCode.erase(pos, 4);
          codeForSection->machineCode.insert(pos, jmpAddress);
        }
      } else if (action.find("byte") != std::string::npos) {

        int pos;
        if (codeForSection->machineCode.find("!") != string::npos)
          pos = codeForSection->machineCode.find("!");
        else {
          pos = codeForSection->machineCode.find("?");
        }

        string addr = intToBinary(symbol->getValue());

        if (!symbol->getNastaoSaEqu()) {
          if (!symbol->getLocal()) {
            RelocationRecord *relocRecord =
                new RelocationRecord(codeForSection->lc, "globlani simbol",
                                     symbol->getId(), codeForSection->section);
            relocationTable->addElem(new RelocationListElem(relocRecord));
          } else {
            RelocationRecord *relocRecord = new RelocationRecord(
                codeForSection->lc, "lokalni simbol", symbol->getSection(),
                codeForSection->section);
            relocationTable->addElem(new RelocationListElem(relocRecord));
          }
        }

        if (externSimbols->getSymbolByName(symbol->getName())) {

          jmpAddress = "00 ";

          RelocationRecord *relocRecord =
              new RelocationRecord(codeForSection->lc, "globlani simbol",
                                   symbol->getId(), codeForSection->section);
          relocationTable->addElem(new RelocationListElem(relocRecord));
          codeForSection->machineCode.erase(pos, 3);
          codeForSection->machineCode.insert(pos, jmpAddress);

          //**simbol je unutar sekcije, po potrebi dodati relokacioni**
        } else {
          if (addr.size() < 8)
            addr = zeroFill(addr, 8 - addr.size());
          addr = binaryToHex(addr);
          jmpAddress = addr + " ";
          codeForSection->machineCode.erase(pos, 2);
          codeForSection->machineCode.insert(pos, jmpAddress);
        }
      }
    }

    forwardRefIterator = forwardRefIterator->next;
  }
  return 0;
}
