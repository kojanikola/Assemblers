#include "implementacija.h"
#include <cctype>
#include <fstream>
#include <list>
#include <new>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

int main(int argc, char *args[]) {

  int lc = 0;
  int symbolID = 1;
  bool firstSection = false;

  Symbol *activeSection = nullptr;

  OperationCodeTable *operationCodeTable = new OperationCodeTable();
  operationCodeTable->inic();

  ForwardReferenceList *forwardReferenceList = new ForwardReferenceList();

  RelocationTable *relocationTable = new RelocationTable();

  CodeList *codeList = new CodeList();

  SymbolTableList *symbolTableList = new SymbolTableList();
  SymbolTableList *allSections = new SymbolTableList();
  SymbolTableList *externSimbols = new SymbolTableList();

  LiteralPool *literalPool = new LiteralPool();

  TabelaNeizracunljivihSimbola *tabelaNeizracunljivihSimbola =
      new TabelaNeizracunljivihSimbola();

  string instrDescr;
  string op1Descr, im_Di_Ad11, im_Di_Ad12;
  string op2Descr, im_Di_Ad21, im_Di_Ad22;

  if (argc != 3) {
    printf("Neispravan broj argumenata, unesite ponovo\n");
    exit(EXIT_FAILURE);
  }

  ifstream inputFile;
  inputFile.open(args[1]);

  ofstream outputFile;
  outputFile.open(args[2]);

  if (!inputFile.is_open() || !outputFile.is_open()) {
    printf("Neispravani argumeti, unesite ponovo\n");
    exit(EXIT_FAILURE);
  }

  string line;
  bool end = false;
  bool pocelaSekcija = false;

  while (getline(inputFile, line) || !end) {
    bool instrukcijaIlabela = false;
    string operacijaNakonLabele;
    istringstream allWords(line);
    string word;
    allWords >> word;

    //-------------------------------------------------------------za labelu
    if (word.find(':') != string::npos) {
      if (activeSection == nullptr) {
        printf("Labela nije unutar sekcije");
        return -1;
      }
      word.erase(word.size() - 1, 1);
      Symbol *labela = symbolTableList->getSymbolByName(word);
      if (labela != nullptr) {
        if (labela->getDefined() == true) {
          printf("Labela definisana vise puta");
          return -1;
        } else {
          labela->setIsDefined(true);
          labela->setSection(activeSection->getId());
          labela->setValue(lc);
        }
      } else {
        labela = new Symbol(word, activeSection->getId(), lc, 0, symbolID, true,
                            true);
        symbolID++;
        symbolTableList->addElem(labela);
      }

      //**zbog operacije i labele u istom redu ovde proverimo i sacuvamo**
      //**nepotrebne 2 promenljive**
      allWords >> operacijaNakonLabele;
      if (operacijaNakonLabele != "") {
        instrukcijaIlabela = true;
      }
    }

    //-------------------------------------------------------------za sekciju
    if (word.compare(".section") == 0) {
      string sectionName;
      allWords >> sectionName;

      if (sectionName == "") {
        printf("Neispravan kod\n");
        return -1;
      }

      string flags = ""; // r-read section || w-writable section || x-executable
      // section || p-has content in file

      if (sectionName.compare(".text") == 0 ||
          sectionName.compare("text") == 0) {
        flags = "rwxp";
      }

      if (sectionName.compare(".data") == 0 ||
          sectionName.compare("data") == 0) {
        flags = "rwxp";
      }

      if (sectionName.compare(".bss") == 0 || sectionName.compare("bss") == 0) {
        flags = "rw";
      }

      if (sectionName.compare(".rodata") == 0 ||
          sectionName.compare("rodata") == 0) {
        flags = "re";
      }

      if (allSections->getSymbolByName(sectionName) != nullptr) {
        printf("Sekcija otvorena 2 puta\n");
        return -1;
      }

      Symbol *newSection;

      if (symbolTableList->getSymbolByName(sectionName) == nullptr) {
        newSection = new Symbol(sectionName, symbolID, 0, 2, symbolID, true,
                                true, flags);
        symbolID++;
        allSections->addElem(newSection);
        symbolTableList->addElem(newSection);
      } else {
        newSection = symbolTableList->getSymbolByName(sectionName);
        allSections->addElem(newSection);
        newSection->setIsDefined(true);
        newSection->setLocal(true);
        newSection->setSection(newSection->getId());
        newSection->type=2;
        newSection->value=0;
      }

      if (activeSection != nullptr) {
        activeSection->setSize(lc);
        lc = 0;
        activeSection = newSection;
      }
      if (activeSection == nullptr) {
        activeSection = newSection;
        pocelaSekcija = true;
      }
    }

    if (word.compare(".text") == 0 || word.compare(".data") == 0 ||
        word.compare(".bss") == 0 || word.compare(".rodata") == 0 ||
        word.compare("text") == 0 || word.compare("data") == 0 ||
        word.compare("bss") == 0 || word.compare("rodata") == 0) {
      if (allSections->getSymbolByName(word) != nullptr) {
        printf("Sekcija otvorena 2 puta\n");
        return -1;
      }

      string flags; // r-read section || w-writable section || x-executable
                    // section || p-has content in file

      if (word.compare(".text") == 0 || word.compare("text") == 0) {
        flags = "rwxp";
      }

      if (word.compare(".data") == 0 || word.compare("data") == 0) {
        flags = "rwxp";
      }

      if (word.compare(".bss") == 0 || word.compare("bss") == 0) {
        flags = "rw";
      }

      if (word.compare(".rodata") == 0 || word.compare("rodata") == 0) {
        flags = "re";
      }

      Symbol *newSection;
      if (symbolTableList->getSymbolByName(word) == nullptr) {
        newSection = new Symbol(word, symbolID, 0, 2, symbolID, true,
                                true, flags);
        symbolID++;
        allSections->addElem(newSection);
        symbolTableList->addElem(newSection);
      } else {
        newSection = symbolTableList->getSymbolByName(word);
        allSections->addElem(newSection);
        newSection->setIsDefined(true);
        newSection->setLocal(true);
        newSection->type=2;
        newSection->value=0;
      }

      if (activeSection != nullptr) {
        activeSection->setSize(lc);
        lc = 0;
        activeSection = newSection;
      }
      if (activeSection == nullptr) {
        activeSection = newSection;
        pocelaSekcija = true;
      }
    }

    //-------------------------------------------------------------za end
    if (word.compare(".end") == 0 ||
        operacijaNakonLabele.compare(".end") == 0) {
      end = true;
      if (activeSection != nullptr) {
        activeSection->setSize(lc);
        break;
      }
    }

    //-------------------------------------------------------------za global
    if (word.compare(".global") == 0) {
      if (pocelaSekcija == true) {
        printf("Globalni simboli se definisu pre pocetka sekcije\n");
        return -1;
      }

      string simbol;

      while (allWords >> simbol) {
        izbaciZarez(simbol);
        //**ukoliko se simbol nalazi kao literal**
        if (literalPool->getLiteral(simbol) != nullptr) {
          printf("Simbol je vec definisan kao literal");
          return -1;
        }

        Symbol *pom = symbolTableList->getSymbolByName(simbol);

        if (pom != nullptr)
          pom->setLocal(false);
        else {
          pom = new Symbol(simbol, 0, symbolID, false, false);
          symbolID++;
          symbolTableList->addElem(pom);
        }
      }
    }
    //-------------------------------------------------------------za extern
    if (word.compare(".extern") == 0) {
      if (pocelaSekcija == true) {
        printf("Eksterni simboli se definisu pre pocetka sekcije\n");
        return -1;
      }

      string simbol;

      while (allWords >> simbol) {
        izbaciZarez(simbol);

        //**ukoliko se simbol nalazi kao literal**
        if (literalPool->getLiteral(simbol) != nullptr) {
          printf("Simbol je vec definisan kao literal");
          return -1;
        }

        Symbol *pom = symbolTableList->getSymbolByName(simbol);
        if (pom != nullptr) {
          pom->setSection(0);
          pom->setLocal(false);
          pom->setIsDefined(true);
        } else {
          pom = new Symbol(simbol, 0, symbolID);
          pom->setSection(0);
          pom->setIsDefined(true);
          symbolID++;
          symbolTableList->addElem(pom);
          externSimbols->addElem(pom);
        }
      }
    }
    //-------------------------------------------------------------za equ
    if (word.compare(".equ") == 0 ||
        operacijaNakonLabele.compare(".equ") == 0) {
      string simbol;
      allWords >> simbol;
      izbaciZarez(simbol);

      // **proveri da li je vec definisan u tabeli simbola**
      Symbol *postoji = symbolTableList->getSymbolByName(simbol);

      if (postoji != nullptr && postoji->getDefined()) {

        printf("Ovaj simbol je vec definisan\n");
        return -1;

      } else if (postoji != nullptr && !postoji->getDefined()) {

        string value;
        allWords >> value;

        if (externSimbols->getSymbolByName(value) != nullptr) {
          printf("Nije moguce inicijalizovati simbol externim");
          return -1;
        }

        if (tabelaNeizracunljivihSimbola->getLeftSymbol(postoji->getName()) !=
            nullptr) {
          printf("Isti simbol se dva puta nasao u equ direktivi\n");
          return -1;
        }

        equSimbolVecPostojeci(postoji, value, symbolTableList, symbolID,
                              activeSection, tabelaNeizracunljivihSimbola,
                              literalPool);

      } else {
        /* **jedan slucaj je da ga izracunas onda ide u tabelu simbola drugi je
        da ne mozes da ga izracunas onda ide u tabelu neizracunljivih** */
        string value;
        allWords >> value;

        if (externSimbols->getSymbolByName(value) != nullptr) {
          printf("Nije moguce inicijalizovati simbol externim");
          return -1;
        }

        if (value.find("/") != string::npos ||
            value.find("*") != string::npos) {
          printf("Nedozvoljene operacije kod equ direktive");
          return -1;
        }

        if (postojiOperacija(value)) {

          bool ubacen = false;

          vector<string> operands;

          if (value.find('+') != string::npos) {
            operands = split(value, '+');
          } else {
            operands = split(value, '-');
          }

          //**ako su oba operanda brojevi dodajemo odmah novi simbol**
          if (is_number(operands[0]) && is_number(operands[1])) {
            if (value.find('+') != string::npos) {
              int sectionId;
              if (activeSection != nullptr) {
                sectionId = activeSection->getId();
              } else {
                sectionId = 0;
              }
              Symbol *noviSimbol = new Symbol(
                  simbol, sectionId, stoi(operands[0]) + stoi(operands[1]), 0,
                  symbolID, true, true);

              symbolTableList->addElem(noviSimbol);
              symbolID++;
              noviSimbol->setNastaoSaEqu(true);
            } else {
              int sectionId;
              if (activeSection != nullptr) {
                sectionId = activeSection->getId();
              } else {
                sectionId = 0;
              }
              Symbol *noviSimbol = new Symbol(
                  simbol, sectionId, stoi(operands[0]) - stoi(operands[1]), 0,
                  symbolID, true, true);
              symbolTableList->addElem(noviSimbol);
              symbolID++;
              noviSimbol->setNastaoSaEqu(true);
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

          //**ako je prvi ili drugi operand  nedefinisan ili null odmah dodajemo
          // u tabelu neizracunljivih**
          if (!is_number(operands[0])) {

            Symbol *operand1 = symbolTableList->getSymbolByName(operands[0]);

            if (((operand1 != nullptr && !operand1->getDefined()) ||
                 operand1 == nullptr) &&
                !ubacen) {
              Symbol *neizracunljiviSimbol =
                  new Symbol(simbol, 0, symbolID, true, false);
              symbolTableList->addElem(neizracunljiviSimbol);
              symbolID++;

              TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                  new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                       value, activeSection);
              tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
              ubacen = true;
            }
            if (operand1 == nullptr) {
              int sectionId;
              if (activeSection != nullptr) {
                sectionId = activeSection->getId();
              } else {
                sectionId = 0;
              }
              operand1 =
                  new Symbol(operands[0], sectionId, 0, symbolID, true, false);
              symbolID++;
              symbolTableList->addElem(operand1);
            }
          }

          if (!is_number(operands[1])) {

            Symbol *operand2 = symbolTableList->getSymbolByName(operands[1]);

            if (((operand2 != nullptr && !operand2->getDefined()) ||
                 operand2 == nullptr) &&
                !ubacen) {

              Symbol *neizracunljiviSimbol =
                  new Symbol(simbol, 0, symbolID, true, false);
              symbolTableList->addElem(neizracunljiviSimbol);
              symbolID++;

              TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                  new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                       value, activeSection);
              tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
              ubacen = true;
            }
            if (operand2 == nullptr) {
              int sectionId;
              if (activeSection != nullptr) {
                sectionId = activeSection->getId();
              } else {
                sectionId = 0;
              }
              operand2 =
                  new Symbol(operands[1], sectionId, 0, symbolID, true, false);
              symbolID++;
              symbolTableList->addElem(operand2);
            }
          }

          //**ako je prvi operand literal a drugi operand simbol**
          if (is_number(operands[0]) && !is_number(operands[1]) && !ubacen) {

            Symbol *operand2 = symbolTableList->getSymbolByName(operands[1]);

            if (operand2 != nullptr) {
              if (operand2->getDefined()) {
                int vr = 0;
                if (value.find('+') != string::npos) {
                  vr = operand2->getValue() + stoi(operands[0]);
                } else {
                  vr = operand2->getValue() - stoi(operands[0]);
                }
                int sectionId;
                if (activeSection != nullptr) {
                  sectionId = activeSection->getId();
                } else {
                  sectionId = 0;
                }
                Symbol *noviSimbol =
                    new Symbol(simbol, sectionId, vr, 0, symbolID, true, true);
                symbolTableList->addElem(noviSimbol);
                symbolID++;
                noviSimbol->setNastaoSaEqu(true);
              } else {

                Symbol *neizracunljiviSimbol =
                    new Symbol(simbol, 0, symbolID, true, false);
                symbolTableList->addElem(neizracunljiviSimbol);
                symbolID++;

                TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                    new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                         value, activeSection);
                tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
                ubacen = true;
              }
            } else {
              Symbol *neizracunljiviSimbol =
                  new Symbol(simbol, 0, symbolID, true, false);
              symbolTableList->addElem(neizracunljiviSimbol);
              symbolID++;

              TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                  new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                       value, activeSection);

              tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
              ubacen = true;

              int sectionId;
              if (activeSection != nullptr) {
                sectionId = activeSection->getId();
              } else {
                sectionId = 0;
              }

              operand2 =
                  new Symbol(operands[1], sectionId, 0, symbolID, true, false);
              symbolID++;
              symbolTableList->addElem(operand2);
            }
            if (literalPool->getLiteral(operands[0]) == nullptr) {
              int sectionId;
              if (activeSection != nullptr) {
                sectionId = activeSection->getId();
              } else {
                sectionId = 0;
              }
              Literal *noviLiteral =
                  new Literal(stoi(operands[0]), sectionId, operands[0]);
              literalPool->addElem(noviLiteral);
            }
          }

          //**ako je prvi operand simbol a drugi operand literal**
          if (!is_number(operands[0]) && is_number(operands[1]) && !ubacen) {

            Symbol *operand1 = symbolTableList->getSymbolByName(operands[0]);

            if (operand1 != nullptr) {
              if (operand1->getDefined()) {
                int vr = 0;
                if (value.find('+') != string::npos) {
                  vr = operand1->getValue() + stoi(operands[1]);
                } else {
                  vr = operand1->getValue() - stoi(operands[1]);
                }
                int sectionId;
                if (activeSection != nullptr) {
                  sectionId = activeSection->getId();
                } else {
                  sectionId = 0;
                }
                Symbol *noviSimbol =
                    new Symbol(simbol, sectionId, vr, 0, symbolID, true, true);
                symbolTableList->addElem(noviSimbol);
                symbolID++;
                noviSimbol->setNastaoSaEqu(true);
              } else {

                Symbol *neizracunljiviSimbol =
                    new Symbol(simbol, 0, symbolID, true, false);
                symbolTableList->addElem(neizracunljiviSimbol);
                symbolID++;

                TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                    new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                         value, activeSection);
                tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
                ubacen = true;
              }
            } else {
              Symbol *neizracunljiviSimbol =
                  new Symbol(simbol, 0, symbolID, true, false);
              symbolTableList->addElem(neizracunljiviSimbol);
              symbolID++;
              TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                  new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                       value, activeSection);
              tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
              ubacen = true;

              int sectionId;
              if (activeSection != nullptr) {
                sectionId = activeSection->getId();
              } else {
                sectionId = 0;
              }
              operand1 =
                  new Symbol(operands[0], sectionId, 0, symbolID, true, false);
              symbolID++;
              symbolTableList->addElem(operand1);

              if (literalPool->getLiteral(operands[1]) == nullptr) {
                int sectionId;
                if (activeSection != nullptr) {
                  sectionId = activeSection->getId();
                } else {
                  sectionId = 0;
                }
                Literal *noviLiteral =
                    new Literal(stoi(operands[1]), sectionId, operands[1]);
                literalPool->addElem(noviLiteral);
              }
            }
          }

          //**ako su oba simboli**
          if (!is_number(operands[0]) && !is_number(operands[1]) && !ubacen) {

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
                int sectionId;
                if (activeSection != nullptr) {
                  sectionId = activeSection->getId();
                } else {
                  sectionId = 0;
                }
                Symbol *noviSimbol =
                    new Symbol(simbol, sectionId, vr, 0, symbolID, true, true);
                symbolTableList->addElem(noviSimbol);
                symbolID++;
                noviSimbol->setNastaoSaEqu(true);
              } else {
                Symbol *neizracunljiviSimbol =
                    new Symbol(simbol, 0, symbolID, true, false);
                symbolTableList->addElem(neizracunljiviSimbol);
                symbolID++;
                TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                    new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                         value, activeSection);
                tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
                ubacen = true;
              }
            } else {
              Symbol *neizracunljiviSimbol =
                  new Symbol(simbol, 0, symbolID, true, false);
              symbolTableList->addElem(neizracunljiviSimbol);
              symbolID++;
              TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                  new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                       value, activeSection);
              tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
              ubacen = true;

              if (operand1 == nullptr) {
                int sectionId;
                if (activeSection != nullptr) {
                  sectionId = activeSection->getId();
                } else {
                  sectionId = 0;
                }
                operand1 = new Symbol(operands[0], sectionId, 0, symbolID, true,
                                      false);
                symbolID++;
                symbolTableList->addElem(operand1);
              }
              if (operand2 == nullptr) {
                int sectionId;
                if (activeSection != nullptr) {
                  sectionId = activeSection->getId();
                } else {
                  sectionId = 0;
                }
                operand2 = new Symbol(operands[1], sectionId, 0, symbolID, true,
                                      false);
                symbolID++;
                symbolTableList->addElem(operand2);
              }
            }
          }
        } else {
          if (is_number(value)) {
            //**ukoliko je vrednost samo literal**
            int i = stoi(value);
            Symbol *newSymbol = new Symbol(simbol, activeSection->getId(), i, 0,
                                           symbolID, true, true);

            symbolID++;
            symbolTableList->addElem(newSymbol);
            newSymbol->setNastaoSaEqu(true);

            if (literalPool->getLiteral(value) == nullptr) {
              Literal *noviLiteral =
                  new Literal(stoi(value), activeSection->getId(), value);
              literalPool->addElem(noviLiteral);
            }

            //**ukoliko je vrednost simbol**
          } else {
            Symbol *symbolValue = symbolTableList->getSymbolByName(value);

            //**ukoliko je simbol odmah definisan, imamo odmah vrednost**
            if (symbolValue != nullptr && symbolValue->getDefined() == true) {
              Symbol *newSymbol =
                  new Symbol(simbol, activeSection->getId(),
                             symbolValue->getValue(), 0, symbolID, true, true);
              symbolID++;
              symbolTableList->addElem(newSymbol);
              newSymbol->setNastaoSaEqu(true);

              //**ukoliko simbol nije definisan, ostavljamo TNS ulaz**
            } else {
              Symbol *neizracunljiviSimbol =
                  new Symbol(simbol, 0, symbolID, true, false);
              symbolTableList->addElem(neizracunljiviSimbol);
              symbolID++;

              TabelaNeizracunljivihSimbolaElem *noviNeizracunljiviElem =
                  new TabelaNeizracunljivihSimbolaElem(neizracunljiviSimbol,
                                                       value, activeSection);
              tabelaNeizracunljivihSimbola->addElem(noviNeizracunljiviElem);
              if (symbolValue == nullptr) {
                symbolValue = new Symbol(value, activeSection->getId(), 0,
                                         symbolID, true, false);
                symbolID++;
                symbolTableList->addElem(symbolValue);
              }
            }
          }
        }
      }
    }

    //-------------------------------------------------------------za byte
    if (word.compare(".byte") == 0 ||
        operacijaNakonLabele.compare(".byte") == 0) {
      if (activeSection != nullptr && activeSection->getName() == "bss") {
        printf("Byte instrukcija nije dozvoljena za bss sekciju");
        return -1;
      }

      string nizBajtova = "";
      string slByte;
      while (allWords >> slByte) {
        izbaciZarez(slByte);
        nizBajtova.append(slByte);
        nizBajtova.append(" ");
      }
      if (nizBajtova != "")
        nizBajtova.pop_back();

      stringstream pomNizByte(nizBajtova);

      while (pomNizByte >> slByte) {

        if (postojiOperacija(slByte) && !(slByte.at(0) == '-')) {
          printf("U .byte direktivi mogu da stoje samo izraz ili simbol");
          return -1;
        }

        Symbol *pomSimbol;

        //**ukoliko je literal odmah dodajemo u izlazni kod**
        if (is_number(slByte)) {
          if (literalPool->getLiteral(slByte) == nullptr) {
            Literal *noviLiteral =
                new Literal(stoi(slByte), activeSection->getId(), slByte);
            literalPool->addElem(noviLiteral);
          }

          string binarno = intToBinary(stoi(slByte), 8);

          if (binarno.size() < 8)
            binarno = zeroFill(binarno, 8 - binarno.size());

          //**da li je ovde potrebna provera?**
          if (binarno.size() > 8) {
            printf("Preveliki broj za byte polje");
            return -1;
          }

          binarno = binaryToHex(binarno);

          CodeForSection *code =
              new CodeForSection(activeSection->getId(), lc, 1, binarno + " ");
          CodeTableElem *newCodeElem = new CodeTableElem(code);
          codeList->addElem(newCodeElem);
          //**za adresu ide relokacioni i ugradi se 00**

        } else {

          pomSimbol = symbolTableList->getSymbolByName(slByte);

          if (pomSimbol != nullptr) {

            if (pomSimbol->getDefined()) {
              string binarno = intToBinary(pomSimbol->getValue(), 8);
              if (!pomSimbol->getLocal()) {
                binarno = "00000000";

                RelocationRecord *relRecord = new RelocationRecord(
                    lc, "globalni simbol ", pomSimbol->getId(),
                    activeSection->getId());
                relocationTable->addElem(new RelocationListElem(relRecord));
              } else {
                RelocationRecord *relRecord = new RelocationRecord(
                    lc, "lokalni simbol ", pomSimbol->getSection(),
                    activeSection->getId());
                relocationTable->addElem(new RelocationListElem(relRecord));
              }
              if (binarno.size() < 8)
                binarno = zeroFill(binarno, 8 - binarno.size());
              if (binarno.size() > 8) {
                printf("Preveliki broj za byte polje");
                return -1;
              }

              binarno = binaryToHex(binarno);

              CodeForSection *code = new CodeForSection(activeSection->getId(),
                                                        lc, 1, binarno + " ");
              CodeTableElem *newCodeElem = new CodeTableElem(code);
              codeList->addElem(newCodeElem);
            } else {

              ForwardReference *forwardRef = new ForwardReference(
                  pomSimbol, lc, pomSimbol->getId(), "nedefinisan .byte",
                  activeSection->getId());
              forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

              CodeForSection *code =
                  new CodeForSection(activeSection->getId(), lc, 1, "? ");
              CodeTableElem *newCodeElem = new CodeTableElem(code);
              codeList->addElem(newCodeElem);
            }
          } else {
            Symbol *noviSimbol = new Symbol(slByte, activeSection->getId(), 0,
                                            symbolID, true, false);
            symbolID++;
            symbolTableList->addElem(noviSimbol);

            CodeForSection *code =
                new CodeForSection(activeSection->getId(), lc, 1, "! ");
            CodeTableElem *newCodeElem = new CodeTableElem(code);
            codeList->addElem(newCodeElem);

            ForwardReference *forwardRef = new ForwardReference(
                noviSimbol, lc, noviSimbol->getId(), "nepostojeci .byte",
                activeSection->getId());
            forwardReferenceList->addElem(new ForwardRefElem(forwardRef));
          }
        }
        lc++;
      }
    }
    //-------------------------------------------------------------za word
    if (word.compare(".word") == 0 ||
        operacijaNakonLabele.compare(".word") == 0) {
      if (activeSection != nullptr && activeSection->getName() == "bss") {
        printf("Word instrukcija nije dozvoljena za bss sekciju");
        return -1;
      }
      string nizWord = "";
      string slWord;
      while (allWords >> slWord) {
        izbaciZarez(slWord);
        nizWord.append(slWord);
        nizWord.append(" ");
      }
      if (nizWord != "")
        nizWord.pop_back();

      stringstream pomNizByte(nizWord);

      while (pomNizByte >> slWord) {
        if (postojiOperacija(slWord) && !(slWord.at(0) == '-')) {
          printf("U .word direktivi mogu da stoje samo izraz ili simbol");
          return -1;
        }

        Symbol *pomSimbol;

        if (is_number(slWord)) {
          if (literalPool->getLiteral(slWord) == nullptr) {
            Literal *noviLiteral =
                new Literal(stoi(slWord), activeSection->getId(), slWord);
            literalPool->addElem(noviLiteral);
          }

          string binarno = intToBinary(stoi(slWord), 16);

          if (binarno.size() < 16)
            binarno = zeroFill(binarno, 16 - binarno.size());

          //**da li je ovde potrebna provera?**
          if (binarno.size() > 16) {
            printf("Preveliki broj za word polje");
            return -1;
          }

          string addressL = binarno.substr(8, 8);
          string addressH = binarno.substr(0, 8);

          addressL = binaryToHex(addressL);
          addressH = binaryToHex(addressH);

          CodeForSection *code = new CodeForSection(
              activeSection->getId(), lc, 1, addressL + " " + addressH + " ");
          CodeTableElem *newCodeElem = new CodeTableElem(code);
          codeList->addElem(newCodeElem);
          //**za adresu ide relokacioni i ugradi se 00**

        } else {

          pomSimbol = symbolTableList->getSymbolByName(slWord);

          if (pomSimbol != nullptr) {
            if (pomSimbol->getDefined()) {
              string binarno = intToBinary(pomSimbol->getValue(), 16);
              if (!pomSimbol->getLocal()) {
                binarno = "00000000";
                RelocationRecord *relRecord = new RelocationRecord(
                    lc, "globalni simbol ", pomSimbol->getId(),
                    activeSection->getId());
                relocationTable->addElem(new RelocationListElem(relRecord));
              } else {
                RelocationRecord *relRecord = new RelocationRecord(
                    lc, "lokalni simbol ", pomSimbol->getSection(),
                    activeSection->getId());
                relocationTable->addElem(new RelocationListElem(relRecord));
              }
              if (binarno.size() < 16)
                binarno = zeroFill(binarno, 16 - binarno.size());
              if (binarno.size() > 16) {
                printf("Preveliki broj za word polje");
                return -1;
              }

              string addressL = binarno.substr(8, 8);
              addressL = binaryToHex(addressL);
              string addressH = binarno.substr(0, 8);
              addressH = binaryToHex(addressH);

              CodeForSection *code =
                  new CodeForSection(activeSection->getId(), lc, 1,
                                     addressL + " " + addressH + " ");
              CodeTableElem *newCodeElem = new CodeTableElem(code);
              codeList->addElem(newCodeElem);
            } else {

              ForwardReference *forwardRef = new ForwardReference(
                  pomSimbol, lc, pomSimbol->getId(), "nedefinisan .word",
                  activeSection->getId());
              forwardReferenceList->addElem(new ForwardRefElem(forwardRef));

              CodeForSection *code =
                  new CodeForSection(activeSection->getId(), lc, 1, "? ? ");
              CodeTableElem *newCodeElem = new CodeTableElem(code);
              codeList->addElem(newCodeElem);
            }
          } else {
            Symbol *noviSimbol = new Symbol(slWord, activeSection->getId(), 0,
                                            symbolID, true, false);
            symbolID++;
            symbolTableList->addElem(noviSimbol);

            CodeForSection *code =
                new CodeForSection(activeSection->getId(), lc, 1, "! ! ");
            CodeTableElem *newCodeElem = new CodeTableElem(code);
            codeList->addElem(newCodeElem);

            ForwardReference *forwardRef = new ForwardReference(
                noviSimbol, lc, noviSimbol->getId(), "nepostojeci .word",
                activeSection->getId());
            forwardReferenceList->addElem(new ForwardRefElem(forwardRef));
          }
        }
        lc += 2;
      }
    }

    //-------------------------------------------------------------za skip

    if (word.compare(".skip") == 0 ||
        operacijaNakonLabele.compare(".skip") == 0) {

      if (activeSection == nullptr) {
        printf("Ne postoji sekcija za koju proizvodimo masinski kod");
        return -1;
      }

      string number;
      allWords >> number;

      int brBajtova = stoi(number);
      string code = "";

      for (int i = 0; i < brBajtova; i++) {
        code.append("00 ");
      }

      CodeForSection *skipBites =
          new CodeForSection(activeSection->getId(), lc, brBajtova, code);
      CodeTableElem *skipCodeElem = new CodeTableElem(skipBites);
      codeList->addElem(skipCodeElem);

      lc += brBajtova;
    }

    //-------------------------------------------------------------za komentare

    if (word.length() >= 1) {
      if (word.at(0) == '#') {
        continue;
      }
    }

    //-------------------------------------------------------------za
    // instrukcije
    char c = 'w';
    if ((word.back() == 'w' || word.back() == 'b')) {
      c = word.back();
      word.erase(word.size() - 1, 1);
    }

    OpCode *opCode = operationCodeTable->getElemByName(word);

    if (instrukcijaIlabela == true) {

      string instr = operacijaNakonLabele;

      if (instr.back() == 'w' || instr.back() == 'b') {
        c = instr.back();
        instr.erase(instr.size() - 1, 1);
      }

      opCode = operationCodeTable->getElemByName(instr);
    }

    if (opCode != nullptr) {
      instrDescr = "";
      op1Descr = "";
      op2Descr = "";
      im_Di_Ad11 = "";
      im_Di_Ad12 = "";
      im_Di_Ad21 = "";
      im_Di_Ad22 = "";

      if (activeSection == nullptr) {
        printf("Kod mora da bude unutar text sekcije");
        return -1;
      }
      //------------------------------------------------------------------za
      // skokove

      if (isJump(opCode)) {

        string dst;
        allWords >> dst;

        string tipAdresiranja;

        string jmpAddressPom = jmpAddress(
            dst, symbolTableList, forwardReferenceList, relocationTable,
            activeSection, literalPool, tipAdresiranja, lc, symbolID);

        //**vraca se adresa u bitima ako je int zove se intToBinary

        instrDescr = "";
        instrDescr.append(opCode->getOc()); // dodaj 5 bajtova za operacioni kod
        instrDescr.append("100");

        //**neposredno, skokovi 1,2**
        if (tipAdresiranja.compare("immediate") == 0) {

          op1Descr = "00000000";

          im_Di_Ad11 = jmpAddressPom;

          if (instrDescr.size() < 8)
            instrDescr = zeroFill(instrDescr, 8 - instrDescr.size());
          if (op1Descr.size() < 8)
            op1Descr = zeroFill(op1Descr, 8 - op1Descr.size());

          instrDescr = binaryToHex(instrDescr);
          op1Descr = binaryToHex(op1Descr);

          if (im_Di_Ad11 != "!" && im_Di_Ad11 != "?") {
            if (im_Di_Ad11.size() < 16)
              im_Di_Ad11 = zeroFill(im_Di_Ad11, 16 - im_Di_Ad11.size());

            string addressL = im_Di_Ad11.substr(8, 8);
            string addressH = im_Di_Ad11.substr(0, 8);

            im_Di_Ad11 = byteToHex(addressL);
            im_Di_Ad12 = byteToHex(addressH);
          } else {
            if (im_Di_Ad11 == "!") {
              im_Di_Ad11 = "!";
              im_Di_Ad12 = "!";
            } else {
              im_Di_Ad11 = "?";
              im_Di_Ad12 = "?";
            }
          }
          string fullInstr = instrDescr + " " + op1Descr + " " + im_Di_Ad11 +
                             " " + im_Di_Ad12 + " ";
          CodeForSection *code =
              new CodeForSection(activeSection->getId(), lc, 4, fullInstr);
          CodeTableElem *newCodeElem = new CodeTableElem(code);
          codeList->addElem(newCodeElem);
          lc = lc + 4;
        }

        //**registarsko direktno bez pomeraja, skok 3**
        if (tipAdresiranja.compare("regdir") == 0) {

          op1Descr = "001";
          jmpAddressPom = intToBinary(stoi(jmpAddressPom), 4);
          //**vracen je broj registra ne adresa**
          op1Descr.append(jmpAddressPom + "0");

          if (instrDescr.size() < 8)
            zeroFill(instrDescr, 8 - instrDescr.size());
          if (op1Descr.size() < 8)
            zeroFill(op1Descr, 8 - op1Descr.size());

          instrDescr = binaryToHex(instrDescr);
          op1Descr = binaryToHex(op1Descr);

          string fullInstr = instrDescr + " " + op1Descr + " ";
          CodeForSection *code =
              new CodeForSection(activeSection->getId(), lc, 2, fullInstr);
          CodeTableElem *newCodeElem = new CodeTableElem(code);
          codeList->addElem(newCodeElem);
          lc += 2;
        }

        //**registarsko indirektno bez pomeraja, skok 4**
        if (tipAdresiranja.compare("regindir") == 0) {

          op1Descr = "010" + intToBinary(stoi(jmpAddressPom), 4) + "0";

          if (instrDescr.size() < 8)
            zeroFill(instrDescr, 8 - instrDescr.size());
          if (op1Descr.size() < 8)
            zeroFill(op1Descr, 8 - op1Descr.size());

          instrDescr = binaryToHex(instrDescr);
          op1Descr = binaryToHex(op1Descr);

          string fullInstr = instrDescr + " " + op1Descr + " ";
          CodeForSection *code =
              new CodeForSection(activeSection->getId(), lc, 2, fullInstr);
          CodeTableElem *newCodeElem = new CodeTableElem(code);
          codeList->addElem(newCodeElem);
          lc += 2;
        }

        //**memorijsko direktno, skokovi 8,9**
        if (tipAdresiranja.compare("memdir") == 0) {
          op1Descr = "10000000";
          im_Di_Ad11 = jmpAddressPom; // adresa gde se skace; oba koristis
          if (instrDescr.size() < 8)
            instrDescr = zeroFill(instrDescr, 8 - instrDescr.size());
          if (op1Descr.size() < 8)
            op1Descr = zeroFill(op1Descr, 8 - op1Descr.size());

          instrDescr = binaryToHex(instrDescr);
          op1Descr = binaryToHex(op1Descr);

          if (im_Di_Ad11 != "!" && im_Di_Ad11 != "?") {
            if (im_Di_Ad11.size() < 16)
              im_Di_Ad11 = zeroFill(im_Di_Ad11, 16 - im_Di_Ad11.size());
            string addressL = im_Di_Ad11.substr(8, 8);
            string addressH = im_Di_Ad11.substr(0, 8);

            im_Di_Ad11 = byteToHex(addressL);
            im_Di_Ad12 = byteToHex(addressH);
          } else {
            if (im_Di_Ad11 == "!") {
              im_Di_Ad11 = "!";
              im_Di_Ad12 = "!";
            } else {
              im_Di_Ad11 = "?";
              im_Di_Ad12 = "?";
            }
          }

          string fullInstr = instrDescr + " " + op1Descr + " " + im_Di_Ad11 +
                             " " + im_Di_Ad12 + " ";
          CodeForSection *code =
              new CodeForSection(activeSection->getId(), lc, 4, fullInstr);
          CodeTableElem *newCodeElem = new CodeTableElem(code);
          codeList->addElem(newCodeElem);
          lc = lc + 4;
        }

        //**registarsko idnirektno sa pomerajem, skokovi 5,6,7**
        if (tipAdresiranja.compare("regindiroffset") == 0) {

          //**vracen pomeraj kao adresa, a broj registra se trazi ovde**
          op1Descr = "011";
          string regPom = getRegName(dst);
          int i = stoi(findRegId(regPom));
          string regId = intToBinary(i, 4);
          op1Descr += regId + "0";

          if (instrDescr.size() < 8)
            instrDescr = zeroFill(instrDescr, 8 - instrDescr.size());
          if (op1Descr.size() < 8)
            op1Descr = zeroFill(op1Descr, 8 - op1Descr.size());

          im_Di_Ad11 = jmpAddressPom;

          instrDescr = binaryToHex(instrDescr);
          op1Descr = binaryToHex(op1Descr);

          if (im_Di_Ad11 != "?" && im_Di_Ad11 != "-2") {
            if (im_Di_Ad11.size() < 16)
              im_Di_Ad11 = zeroFill(im_Di_Ad11, 16 - im_Di_Ad11.size());
            string addressL = im_Di_Ad11.substr(8, 8);
            string addressH = im_Di_Ad11.substr(0, 8);

            im_Di_Ad11 = byteToHex(addressL);
            im_Di_Ad12 = byteToHex(addressH);
          } else {
            if (im_Di_Ad11 == "-2") {
              im_Di_Ad11 = "-2";
              im_Di_Ad12 = "-2";
            } else {
              im_Di_Ad11 = "?";
              im_Di_Ad12 = "?";
            }
          }

          string fullInstr = instrDescr + " " + op1Descr + " " + im_Di_Ad11 +
                             " " + im_Di_Ad12 + " ";

          CodeForSection *code =
              new CodeForSection(activeSection->getId(), lc, 4, fullInstr);
          CodeTableElem *newCodeElem = new CodeTableElem(code);
          codeList->addElem(newCodeElem);
          lc = lc + 4;
        }
      } else { //--------------------------------------------------------------------operacije

        string operand1;
        string operand2;
        string tipAdresiranja;

        // proveri broj operanada ako ih ima vise nego sto treba error
        if (opCode->getSize() == 0) {
          if (allWords.rdbuf()->in_avail() != 0) {
            printf("Instrukciji sa jednim operandom prosledjeno je dva \n");
            return -1;
          }
        } else if (opCode->getSize() == 1) {
          allWords >> operand1;
        } else {
          allWords >> operand1;
          allWords >> operand2;
        }

        izbaciZarez(operand1);
        izbaciZarez(operand2);

        instrDescr = "";
        instrDescr.append(opCode->getOc()); // dodaj 5 bajtova za operacioni kod
        if (c == 'b' || opCode->getNumOfOperands() == 0) {
          instrDescr.append("000");
        } else {
          instrDescr.append("100");
        }

        if (opCode->getSize() == 1) {
          string operand1Address =
              operandAddress(operand1, symbolTableList, forwardReferenceList,
                             relocationTable, activeSection, literalPool,
                             tipAdresiranja, lc, symbolID, c, false, lc);
          popuniBajtoveOperandaInstr(operand1Address, op1Descr, im_Di_Ad11,
                                     im_Di_Ad12, tipAdresiranja, lc, c);

          if (tipAdresiranja.compare("regdir") == 0) {
            if (c == 'b') {
              if (operand1.at(operand1.size() - 1) == 'l') {
                op1Descr.append("0");
              } else {
                op1Descr.append("1");
              }
            } else {
              op1Descr.append("0");
            }
          }
        }

        bool operand1RegDir = false, operand2RegDir = false,
             operand1RegIndir = false, operand2RegIndir = false,
             op1RegIndirOffset = false;
        //**pomoc za proveru**

        int pozicijaFR = 0;
        bool drugiOperand = false;

        if (opCode->getSize() == 2) {
          string operand1Address =
              operandAddress(operand1, symbolTableList, forwardReferenceList,
                             relocationTable, activeSection, literalPool,
                             tipAdresiranja, lc, symbolID, c, drugiOperand, lc);
          popuniBajtoveOperandaInstr(operand1Address, op1Descr, im_Di_Ad11,
                                     im_Di_Ad12, tipAdresiranja, lc, c);

          if (tipAdresiranja.compare("regdir") == 0) {
            operand1RegDir = true;
          } else {
            operand1RegDir = false;
          }

          if (tipAdresiranja.compare("regindir") == 0) {
            operand1RegIndir = true;
          } else {
            operand1RegIndir = false;
          }

          if (tipAdresiranja.compare("regdir") == 0 && c == 'b') {
            if (operand1.at(operand1.size() - 1) == 'l') {
              op1Descr.append("0");
            } else {
              op1Descr.append("1");
            }
          }

          if (tipAdresiranja.compare("regIndiroffset") == 0) {

            op1RegIndirOffset = true;
            //**potencijalno treba da prepravimo vrednost zbog drugog operanda**

            string offset = "";
            int i = 0;
            while (operand1.at(i) != '(') {
              offset += operand1.at(i);
              i++;
            }

            string reg = operand1.substr(i + 2, operand1.size() - i - 3);

            string regId = findRegId(reg);

            op1Descr.append(intToBinary(stoi(regId), 4));
            op1Descr.append("0");
          }

          pozicijaFR = 3;

          drugiOperand = true;
          if (im_Di_Ad11 != "")
            pozicijaFR++;
          if (im_Di_Ad12 != "")
            pozicijaFR++;

          tipAdresiranja = "";

          string operand2Address = operandAddress(
              operand2, symbolTableList, forwardReferenceList, relocationTable,
              activeSection, literalPool, tipAdresiranja, lc, symbolID, c,
              drugiOperand, pozicijaFR + lc);

          popuniBajtoveOperandaInstr(operand2Address, op2Descr, im_Di_Ad21,
                                     im_Di_Ad22, tipAdresiranja, lc, c);

          //**ukoliko je drugi operand adresiran regDir ili regIndir bez
          // pomeraja pomeraj za pc se uvecava za 3, u svim ostalim slucajevima
          // za 5**
          if (tipAdresiranja.compare("regdir") == 0 && c == 'b') {
            if (im_Di_Ad11.compare("-2") == 0) {
              im_Di_Ad11 = "-3";
              if (im_Di_Ad12 != "")
                im_Di_Ad12 = "-3";
            }

            operand2RegDir = true;

            if (operand2.at(operand2.size() - 1) == 'l') {
              op2Descr.append("0");
            } else {
              op2Descr.append("1");
            }
          }

          if (tipAdresiranja.compare("regindir") == 0) {
            if (im_Di_Ad11.compare("-2") == 0) {
              im_Di_Ad11 = "-3";
              if (im_Di_Ad12 != "")
                im_Di_Ad12 = "-3";
            }
            operand2RegIndir = true;
          } else {
            operand2RegIndir = false;
          }

          if (im_Di_Ad11.compare("-2") == 0) {
            im_Di_Ad11 = "-5";
            if (im_Di_Ad12 != "")
              im_Di_Ad12 = "-5";
          }

          int regId;

          if (op1RegIndirOffset == true) {

            int i = 0;
            while (operand1.at(i) != '(') {
              i++;
            }

            string reg = operand1.substr(i + 2, operand1.size() - i - 3);
            regId = stoi(findRegId(reg));
          }

          if (op1RegIndirOffset == true &&
              im_Di_Ad11.find('-') == std::string::npos && regId == 7) {

            int pcRelConst;
            //**konstanta koju oduzimamo zbog PC relativnogadr**

            string addrPom;

            if (operand2RegDir || operand2RegIndir) {
              pcRelConst = 3;
            } else {
              pcRelConst = 5;
            }

            string offset = "";
            int i = 0;
            while (operand1.at(i) != '(') {
              offset += operand1.at(i);
              i++;
            }

            if (is_number(offset)) {

              int pom = stoi(offset) - pcRelConst;

              if (c == 'b') {
                addrPom = intToBinary(pom, 8);
                addrPom = binaryToHex(addrPom);
                im_Di_Ad11 = addrPom;
              } else {
                addrPom = intToBinary(pom, 16);
                im_Di_Ad11 = addrPom.substr(8, 8);
                im_Di_Ad12 = addrPom.substr(0, 8);
                im_Di_Ad11 = byteToHex(im_Di_Ad11);
                im_Di_Ad12 = byteToHex(im_Di_Ad12);
              }
            } else {
              Symbol *symbol = symbolTableList->getSymbolByName(offset);
              int pom = symbol->getValue() - pcRelConst;

              if (c == 'b') {
                addrPom = intToBinary(pom, 8);
                addrPom = binaryToHex(addrPom);
                im_Di_Ad11 = addrPom;
              } else {
                addrPom = intToBinary(pom, 16);
                im_Di_Ad11 = addrPom.substr(8, 8);
                im_Di_Ad12 = addrPom.substr(0, 8);
                im_Di_Ad11 = byteToHex(im_Di_Ad11);
                im_Di_Ad12 = byteToHex(im_Di_Ad12);
              }
            }
          }

          if (tipAdresiranja.compare("regIndiroffset") == 0) {
            string offset = "";
            int i = 0;
            while (operand2.at(i) != '(') {
              offset += operand2.at(i);
              i++;
            }

            string reg = operand2.substr(i + 2, operand2.size() - i - 3);

            string regId = findRegId(reg);

            op2Descr.append(intToBinary(stoi(regId), 4));
            op2Descr.append("0");
          }
        }

        if (instrDescr.size() < 8)
          instrDescr = zeroFill(instrDescr, 8 - instrDescr.size());
        if (op1Descr.size() < 8)
          op1Descr = zeroFill(op1Descr, 8 - op1Descr.size());
        if (op2Descr != "" && op2Descr.size() < 8)
          op2Descr = zeroFill(op2Descr, 8 - op2Descr.size());

        instrDescr = binaryToHex(instrDescr);
        op1Descr = binaryToHex(op1Descr);

        string fullInstr = instrDescr + " " + op1Descr;

        int size = 2;
        //**obavezni deo svake instrukcije, nakon toga provera za ostale
        // bajtove**

        //**moramo rucno da prebrojimo bajtove kako bismo povecali lc**

        if (opCode->getSize() == 1) {
          if (tipAdresiranja.compare("regdir") != 0 &&
              tipAdresiranja.compare("regindir") != 0) {

            if (c == 'b') {
              fullInstr += " " + im_Di_Ad11;
              size++;
            } else {
              fullInstr += " " + im_Di_Ad11 + " " + im_Di_Ad12;
              size += 2;
            }
          }
        } else if (opCode->getSize() == 2) {
          if (!operand1RegDir && !operand1RegIndir) {
            if (c == 'b') {
              fullInstr += " " + im_Di_Ad11;
              size++;
            } else {
              fullInstr += " " + im_Di_Ad11 + " " + im_Di_Ad12;
              size += 2;
            }
          }

          op2Descr = binaryToHex(op2Descr);

          fullInstr += " " + op2Descr;
          size++;

          if (!operand2RegDir && !operand2RegIndir) {
            if (c == 'b') {
              fullInstr += " " + im_Di_Ad21;
              size++;
            } else {
              fullInstr += " " + im_Di_Ad21 + " " + im_Di_Ad22;
              size += 2;
            }
          }
        } else {
          op1Descr = "";
          fullInstr = instrDescr;
          size = 1;
        }

        fullInstr += " ";

        CodeForSection *code =
            new CodeForSection(activeSection->getId(), lc, size, fullInstr);
        CodeTableElem *newCodeElem = new CodeTableElem(code);
        codeList->addElem(newCodeElem);

        lc += size;
      }
    }
  }

  //**backpatching**

  //**razresavanje tabele neizracunljivih simbola(nastali equ direktivom)**
  int rezultatRazresavanjaTNS = razresiTNS(
      lc, symbolTableList, tabelaNeizracunljivihSimbola, allSections);
  if (rezultatRazresavanjaTNS == -1)
    return -1;

  //**razresvanje forward reference, nedefinisanog dela u kodu**
  int rezultatForwardReference =
      razresiFR(lc, symbolTableList, tabelaNeizracunljivihSimbola, allSections,
                forwardReferenceList, codeList, externSimbols, relocationTable);

  //----------------------za ispis
  outputFile << "Tabela simbola:" << '\n';
  string undSekcija = "ime: UND sekcija: 0              ";
  outputFile << undSekcija << '\n';
  SymbolTableElem *sekcija = allSections->head;
  while (sekcija != nullptr) {
    string flegovi;
    if (sekcija->symbol->getFlags() == "") {
      flegovi = "sekcija";
    } else {
      flegovi = sekcija->symbol->getFlags();
    }
    string simbol =
        "ime: " + sekcija->symbol->getName() + "|  " +
        " sekcija: " + std::to_string(sekcija->symbol->getSection()) + "|  " +
        " vrednost: " + std::to_string(sekcija->symbol->getValue()) + "|  " +
        " fleg: " + flegovi + "|  " +
        " id: " + std::to_string(sekcija->symbol->getId()) + "|  " +
        " size: " + std::to_string(sekcija->symbol->getSize());
    outputFile << simbol << '\n';
    sekcija = sekcija->next;
  }

  sekcija = symbolTableList->head;
  while (sekcija != nullptr) {
    if (allSections->getSymbolByName(sekcija->symbol->getName()) == nullptr) {
      string local;
      if (sekcija->symbol->getLocal())
        local = "l";
      else
        local = "g";
      string simbol =
          "ime: " + sekcija->symbol->getName() + "|  " +
          " sekcija: " + std::to_string(sekcija->symbol->getSection()) + "|  " +
          " vrednost: " + std::to_string(sekcija->symbol->getValue()) + "|  " +
          " fleg: simbol" + "|  " +
          " id: " + std::to_string(sekcija->symbol->getId()) + "|  " +
          " global/local " + local;
      outputFile << simbol << '\n';
    }
    sekcija = sekcija->next;
  }

  outputFile << '\n';

  sekcija = allSections->head;
  CodeTableElem *codeElem;

  while (sekcija != nullptr) {

    outputFile << "code " << sekcija->symbol->getName() << '\n';
    codeElem = codeList->head;

    while (codeElem != nullptr) {
      if (codeElem->code->section == sekcija->symbol->getId()) {
        outputFile << codeElem->code->machineCode << '\n';
      }
      codeElem = codeElem->next;
    }
    outputFile << '\n';
    sekcija = sekcija->next;
  }

  sekcija = allSections->head;
  sortRelocationRecordTable(relocationTable);
  RelocationListElem *relRecord;

  while (sekcija != nullptr) {
    outputFile << "rel. " << sekcija->symbol->getName() << '\n';
    relRecord = relocationTable->head;
    while (relRecord != nullptr) {
      if (relRecord->relocationRecord->getSection() ==
          sekcija->symbol->getId()) {
        string tip = "";
        if (relRecord->relocationRecord->getType().find("pcRel") !=
            std::string::npos) {
          tip = "R_386_PC32 ";
        } else {
          tip = "R_386_32 ";
        }
        int value;
        value = relRecord->relocationRecord->getVr();

        outputFile << relRecord->relocationRecord->getOffset() << " " << tip
                   << value;
        outputFile << '\n';
      }
      relRecord = relRecord->next;
    }
    outputFile << '\n';
    sekcija = sekcija->next;
  }
}
