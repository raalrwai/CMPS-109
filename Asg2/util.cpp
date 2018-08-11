// $Id: util.cpp,v 1.11 2016-01-13 16:21:53-08 - - $
// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include <cstdlib>
#include <unistd.h>

using namespace std;

#include "util.h"
#include "debug.h"

int exit_status::status = EXIT_SUCCESS;
static string execname_string;

void exit_status::set (int new_status) {
   status = new_status;
}

int exit_status::get() {
   return status;
}

void execname (const string& name) {
   execname_string =  name.substr (name.rfind ('/') + 1);
   DEBUGF ('u', execname_string);
}

string& execname() {
   return execname_string;
}

bool want_echo() {
   constexpr int CIN_FD {0};
   constexpr int COUT_FD {1};
   bool cin_is_not_a_tty = not isatty (CIN_FD);
   bool cout_is_not_a_tty = not isatty (COUT_FD);
   DEBUGF ('u', "cin_is_not_a_tty = " << cin_is_not_a_tty
          << ", cout_is_not_a_tty = " << cout_is_not_a_tty);
   return cin_is_not_a_tty or cout_is_not_a_tty;
}


wordvec split (const string& line, const string& delimiters) {
   wordvec words;
   size_t end = 0;

   // Loop over the string, splitting out words, and for each word
   // thus found, append it to the output wordvec.
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   DEBUGF ('u', words);
   return words;
}

ostream& complain() {
   exit_status::set (EXIT_FAILURE);
   cerr << execname() << ": ";
   return cerr;
}

ostream& function_complain() {
   exit_status::set (EXIT_FAILURE);
   cerr << "";
   return cerr;
}

string swap_case(const string& dirname) {
   string directory_name = dirname;
   string aaa = "a";
   string bbb = "b";
   string ccc = "c";
   string ddd = "d";
   string eee = "e";
   string fff = "f";
   string ggg = "g";
   string hhh = "h";
   string iii = "i";
   string jjj = "j";
   string kkk = "k";
   string lll = "l";
   string mmm = "m";
   string nnn = "n";
   string ooo = "o";
   string ppp = "p";
   string qqq = "q";
   string rrr = "r";
   string sss = "s";
   string ttt = "t";
   string uuu = "u";
   string vvv = "v";
   string www = "w";
   string xxx = "x";
   string yyy = "y";
   string zzz = "z";

   string AAA = "A";
   string BBB = "B";
   string CCC = "C";
   string DDD = "D";
   string EEE = "E";
   string FFF = "F";
   string GGG = "G";
   string HHH = "H";
   string III = "I";
   string JJJ = "J";
   string KKK = "K";
   string LLL = "L";
   string MMM = "M";
   string NNN = "N";
   string OOO = "O";
   string PPP = "P";
   string QQQ = "Q";
   string RRR = "R";
   string SSS = "S";
   string TTT = "T";
   string UUU = "U";
   string VVV = "V";
   string WWW = "W";
   string XXX = "X";
   string YYY = "Y";
   string ZZZ = "Z";

   if (directory_name.compare(0, aaa.size(), aaa) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "A");
    } else if (directory_name.compare(0, bbb.size(), bbb) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "B");
    } else if (directory_name.compare(0, ccc.size(), ccc) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "C");
    } else if (directory_name.compare(0, ddd.size(), ddd) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "D");
    } else if (directory_name.compare(0, eee.size(), eee) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "E");
    } else if (directory_name.compare(0, fff.size(), fff) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "F");
    } else if (directory_name.compare(0, ggg.size(), ggg) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "G");
    } else if (directory_name.compare(0, hhh.size(), hhh) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "H");
    } else if (directory_name.compare(0, iii.size(), iii) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "I");
    } else if (directory_name.compare(0, jjj.size(), jjj) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "J");
    } else if (directory_name.compare(0, kkk.size(), kkk) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "K");
    } else if (directory_name.compare(0, lll.size(), lll) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "L");
    } else if (directory_name.compare(0, mmm.size(), mmm) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "M");
    } else if (directory_name.compare(0, nnn.size(), nnn) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "N");
    } else if (directory_name.compare(0, ooo.size(), ooo) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "O");
    } else if (directory_name.compare(0, ppp.size(), ppp) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "P");
    } else if (directory_name.compare(0, qqq.size(), qqq) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "Q");
    } else if (directory_name.compare(0, rrr.size(), rrr) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "R");
    } else if (directory_name.compare(0, sss.size(), sss) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "S");
    } else if (directory_name.compare(0, ttt.size(), ttt) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "T");
    } else if (directory_name.compare(0, uuu.size(), uuu) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "U");
    } else if (directory_name.compare(0, vvv.size(), vvv) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "V");
    } else if (directory_name.compare(0, www.size(), www) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "W");
    } else if (directory_name.compare(0, xxx.size(), xxx) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "X");
    } else if (directory_name.compare(0, yyy.size(), yyy) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "Y");
    } else if (directory_name.compare(0, zzz.size(), zzz) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "Z");
    } else if (directory_name.compare(0, AAA.size(), AAA) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "a");
    } else if (directory_name.compare(0, BBB.size(), BBB) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "b");
    } else if (directory_name.compare(0, CCC.size(), CCC) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "c");
    } else if (directory_name.compare(0, DDD.size(), DDD) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "d");
    } else if (directory_name.compare(0, EEE.size(), EEE) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "e");
    } else if (directory_name.compare(0, FFF.size(), FFF) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "f");
    } else if (directory_name.compare(0, GGG.size(), GGG) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "g");
    } else if (directory_name.compare(0, HHH.size(), HHH) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "h");
    } else if (directory_name.compare(0, III.size(), III) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "i");
    } else if (directory_name.compare(0, JJJ.size(), JJJ) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "j");
    } else if (directory_name.compare(0, KKK.size(), KKK) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "k");
    } else if (directory_name.compare(0, LLL.size(), LLL) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "l");
    } else if (directory_name.compare(0, MMM.size(), MMM) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "m");
    } else if (directory_name.compare(0, NNN.size(), NNN) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "n");
    } else if (directory_name.compare(0, OOO.size(), OOO) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "o");
    } else if (directory_name.compare(0, PPP.size(), PPP) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "p");
    } else if (directory_name.compare(0, QQQ.size(), QQQ) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "q");
    } else if (directory_name.compare(0, RRR.size(), RRR) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "r");
    } else if (directory_name.compare(0, SSS.size(), SSS) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "s");
    } else if (directory_name.compare(0, TTT.size(), TTT) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "t");
    } else if (directory_name.compare(0, UUU.size(), UUU) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "u");
    } else if (directory_name.compare(0, VVV.size(), VVV) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "v");
    } else if (directory_name.compare(0, WWW.size(), WWW) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "w");
    } else if (directory_name.compare(0, XXX.size(), XXX) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "x");
    } else if (directory_name.compare(0, YYY.size(), YYY) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "y");
    } else if (directory_name.compare(0, ZZZ.size(), ZZZ) == 0) {
        directory_name.erase(0, 1);
        directory_name.insert(0, "z");
    } 

   return directory_name;


}


