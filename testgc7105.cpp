
#include "gc7105b.h"

#define USE_GNU_PLOT



#ifdef USE_GNU_PLOT
#include <stdio.h>
#include <stdlib.h>
#include <csignal>  
#include <termio.h> 
#include "gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
 #include <conio.h>   //for getch(), needed in wait_for_key()
 #include <windows.h> //for Sleep()
 void sleep(int i) { Sleep(i*1000);}
#endif

void wait_for_key(); // Programm halts until keypress

using std::cout;
using std::endl; 


using namespace std;


 

Gnuplot g1("lines");

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}
int 
sh_getch(void) {
    int cr;
    struct termios nts, ots;

    if (tcgetattr(0, &ots) < 0) 	
        return EOF;

    nts = ots;
    cfmakeraw(&nts); 	
    if (tcsetattr(0, TCSANOW, &nts) < 0) 	
        return EOF;

    cr = getchar();
    if (tcsetattr(0, TCSANOW, &ots) < 0) 	
        return EOF;

    return cr;
}
#endif
 
int main(int argc, char *argv[])
{
   GC7105B jd;
#ifdef USE_GNU_PLOT    
   std::vector<double> freq;
   std::vector<double> power;
   g1.showonscreen(); // window output
   g1.set_grid();
   g1.set_title("JD Viewer(GC7105B)");
   g1.set_xlabel("Freq(Mhz)");
   g1.set_ylabel("Power(dbm)");
#endif   
  if (jd.init("192.168.1.105"))
   {
	 printf("try to capture data\n");
	 
	
	 jd.capture(freq,power);
	 printf("Peak Power: %.2f at %.2fMhz\n",jd.PeakPower(),jd.PeakFreq());
#ifdef USE_GNU_PLOT	 
 	 g1.reset_plot();	 
	 g1.plot_xy(freq,power,"Spectrum"); 
         wait_for_key();
#endif
 
  }
  return 0; 
}
