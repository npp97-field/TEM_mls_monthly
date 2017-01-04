/* *************************************************************
TTEMDAT45A.CPP - object to read and write the structure of the
                    output data from files generated by the
                    Terrestrial Ecosystem Model (TEM)


****************************************************************
************************************************************* */

#include<cstdio>

  using std::fscanf;
  using std::FILE;

#include<iostream>

  using std::ios;
  using std::endl;

#include<fstream>

  using std::ifstream;
  using std::ofstream;

#include<iomanip>

  using std::setprecision;

#include<string>

  using std::string;


#include "ttemdat45a.h"

Temdat45::Temdat45( void )
{

  temend = 1;
  lagpos = -99;
  curpos = 0;

};

/* *************************************************************
************************************************************* */

int Temdat45::get( ifstream& ifile )
{

  int dm;

  lagpos = ifile.tellg();

  ifile >> col;
  ifile >> row;
  ifile >> varname;
  ifile >> icohort;
  ifile >> standage;
  ifile >> potveg;
  ifile >> currentveg;
  ifile >> subtype;
  ifile >> cmnt;
  ifile >> psiplusc;
  ifile >> qlcon;
  ifile >> carea;
  ifile >> subarea;
  ifile >> year;

  ifile >> total;
  ifile >> max;
  ifile >> ave;
  ifile >> min;

  for( dm = 0; dm < CYCLE; ++dm )  { ifile >> mon[dm]; }

  ifile >> region;

  ifile.seekg( 0, ios::cur );

  curpos = ifile.tellg();

  if( curpos < (lagpos + 10) ) { temend = -1; }

  return temend;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Temdat45::getdel( FILE* ifile )
{
  char tmpvarname[40];
  char tmpregion[40];

  temend = fscanf( ifile,
                   "%f,%f, %s ,%d,%d,%d,%d,%d,%d,%lf,%d,%ld,%ld,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf, %s",
                   &col,
                   &row,
                   tmpvarname,
                   &icohort,
                   &standage,
                   &potveg,
                   &currentveg,
                   &subtype,
                   &cmnt,
                   &psiplusc,
                   &qlcon,
                   &carea,
                   &subarea,
                   &year,
                   &total,
                   &max,
                   &ave,
                   &min,
                   mon+0,
                   mon+1,
                   mon+2,
                   mon+3,
                   mon+4,
                   mon+5,
                   mon+6,
                   mon+7,
                   mon+8,
                   mon+9,
                   mon+10,
                   mon+11,
                   tmpregion );

  varname = tmpvarname;
  region = tmpregion;

  return temend;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Temdat45::getyrsum( double mon[CYCLE] )
{

  int dm;

  total = 0.0;
  max   = -900000.0;
  min   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > max ) { max = mon[dm]; }

    if( mon[dm] < min ) { min = mon[dm]; }

    total += mon[dm];
  }

  ave = total / (double) CYCLE;

  if( total <= ((MISSING*CYCLE)+CYCLE) ) { total = MISSING; }

};
/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Temdat45::out( ofstream& ofile,
                     const float& col,
                     const float& row,
                     const string& varname,
                     const int& icohort,
                     const int& standage,
                     const int& potveg,
                     const int& currentveg,
                     const int& subtype,
                     const int& cmnt,
                     const double& psiplusc,
                     const int& qlcon,
                     const long& carea,
                     const long& subarea,
                     const int& year,
                     double mon[CYCLE],
                     const string& region )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax  = -900000.0;
  predmin  =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;

  if( predtotl <= ((MISSING*CYCLE)+CYCLE) )
  {
    predtotl = MISSING;
  }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << col << ' ';
  ofile << setprecision( 1 ) << row << ' ';
  ofile << varname << ' ';
  ofile << setprecision( 0 ) << icohort << ' ';
  ofile << setprecision( 0 ) << standage << ' ';
  ofile << setprecision( 0 ) << potveg << ' ';
  ofile << setprecision( 0 ) << currentveg << ' ';
  ofile << setprecision( 0 ) << subtype << ' ';
  ofile << setprecision( 0 ) << cmnt << ' ';
  ofile << setprecision( 2 ) << psiplusc << ' ';
  ofile << setprecision( 0 ) << qlcon << ' ';
  ofile << setprecision( 0 ) << carea << ' ';
  ofile << setprecision( 0 ) << subarea << ' ';
  ofile << setprecision( 0 ) << year << ' ';

  ofile << setprecision( 1 ) << predtotl << ' ';
  ofile << setprecision( 1 ) << predmax << ' ';
  ofile << setprecision( 2 ) << predave << ' ';
  ofile << setprecision( 1 ) << predmin << ' ';

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 1 ) << mon[dm] << ' ';
  }

  ofile << region;
  ofile << endl;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Temdat45::outdel( ofstream& ofile,
                        const float& col,
                        const float& row,
                        const string& varname,
                        const int& icohort,
                        const int& standage,
                        const int& potveg,
                        const int& currentveg,
                        const int& subtype,
                        const int& cmnt,
                        const double& psiplusc,
                        const int& qlcon,
                        const long& carea,
                        const long& subarea,
                        const int& year,
                        double mon[CYCLE],
                        const string& region )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax   = -900000.0;
  predmin   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;

  if( predtotl <= ((MISSING*CYCLE)+CYCLE) )
  {
    predtotl = MISSING;
  }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 4 );

  ofile << col << ",";
  ofile << setprecision( 4 ) << row << ", ";
  ofile << varname << " ,";
  ofile << setprecision( 0 ) << icohort << ",";
  ofile << setprecision( 0 ) << standage << ",";
  ofile << setprecision( 0 ) << potveg << ",";
  ofile << setprecision( 0 ) << currentveg << ",";
  ofile << setprecision( 0 ) << subtype << ",";
  ofile << setprecision( 0 ) << cmnt << ",";
  ofile << setprecision( 2 ) << psiplusc << ",";
  ofile << setprecision( 0 ) << qlcon << ",";
  ofile << setprecision( 0 ) << carea << ",";
  ofile << setprecision( 0 ) << subarea << ",";
  ofile << setprecision( 0 ) << year << ",";

  ofile << setprecision( 1 ) << predtotl << ",";
  ofile << setprecision( 1 ) << predmax << ",";
  ofile << setprecision( 2 ) << predave << ",";
  ofile << setprecision( 1 ) << predmin << ",";

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 2 ) << mon[dm] << ",";
  }

  ofile << " " << region;
  ofile << endl;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Temdat45::pctout( ofstream& ofile,
                        const float& col,
                        const float& row,
                        const string& varname,
                        const int& icohort,
                        const int& standage,
                        const int& potveg,
                        const int& currentveg,
                        const int& subtype,
                        const int& cmnt,
                        const double& psiplusc,
                        const int& qlcon,
                        const long& carea,
                        const long& subarea,
                        const int& year,
                        double mon[CYCLE],
                        const string& region )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax   = -900000.0;
  predmin   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;

  if( predtotl <= ((MISSING*CYCLE)+CYCLE) )
  {
    predtotl = MISSING;
  }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << col << ' ';
  ofile << setprecision( 1 ) << row << ' ';
  ofile << varname << ' ';
  ofile << setprecision( 0 ) << icohort << ' ';
  ofile << setprecision( 0 ) << standage << ' ';
  ofile << setprecision( 0 ) << potveg << ' ';
  ofile << setprecision( 0 ) << currentveg << ' ';
  ofile << setprecision( 0 ) << subtype << ' ';
  ofile << setprecision( 0 ) << cmnt << ' ';
  ofile << setprecision( 2 ) << ' ';
  ofile << psiplusc << ' ';
  ofile << setprecision( 0 ) << qlcon << ' ';
  ofile << setprecision( 0 ) << carea << ' ';
  ofile << setprecision( 0 ) << subarea << ' ';
  ofile << setprecision( 0 ) << year << ' ';

  ofile << setprecision( 2 ) << predtotl << ' ';
  ofile << setprecision( 2 ) << predmax << ' ';
  ofile << setprecision( 2 ) << predave << ' ';
  ofile << setprecision( 2 ) << predmin << ' ';

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 2 ) << mon[dm] << ' ';
  }

  ofile << region;
  ofile << endl;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Temdat45::poutdel( ofstream& ofile,
                         const float& col,
                         const float& row,
                         const string& varname,
                         const int& icohort,
                         const int& standage,
                         const int& potveg,
                         const int& currentveg,
                         const int& subtype,
                         const int& cmnt,
                         const double& psiplusc,
                         const int& qlcon,
                         const long& carea,
                         const long& subarea,
                         const int& year,
                         double mon[CYCLE],
                         const string& region )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax   = -900000.0;
  predmin   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;

  if( predtotl <= ((MISSING*CYCLE)+CYCLE) )
  {
    predtotl = MISSING;
  }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << col << ",";
  ofile << setprecision( 1 ) << row << ", ";
  ofile << varname << " ,";
  ofile << setprecision( 0 ) << icohort << ",";
  ofile << setprecision( 0 ) << standage << ",";
  ofile << setprecision( 0 ) << potveg << ",";
  ofile << setprecision( 0 ) << currentveg << ",";
  ofile << setprecision( 0 ) << subtype << ",";
  ofile << setprecision( 0 ) << cmnt << ",";
  ofile << setprecision( 2 ) << psiplusc << ",";
  ofile << setprecision( 0 ) << qlcon << ",";
  ofile << setprecision( 0 ) << carea << ",";
  ofile << setprecision( 0 ) << subarea << ",";
  ofile << setprecision( 0 ) << year << ",";

  ofile << setprecision( 2 ) << predtotl << ",";
  ofile << setprecision( 2 ) << predmax << ",";
  ofile << setprecision( 2 ) << predave << ",";
  ofile << setprecision( 2 ) << predmin << ",";

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 2 ) << mon[dm] << ",";
  }

  ofile << " " << region;
  ofile << endl;

};
