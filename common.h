// Commonly shared parameters for different run conditions
// Want to access these in Analyzer.cxx and also macros

int inline GetRunSet(int run){
/*
Set		Runs
0		46~70
1		81~99
2		102~130
3		136~141
4		142~181
5		187~234
*/
  if (run>=46 && run<=70){
    return (0);
  }
  else if (run>=81 && run<=99){
    return (1);
  }
  else if (run>=102 && run<=130){
    return (2);
  }
  else if (run>=136 && run<=141){
    return (3);
  }
  else if (run>=142 && run<=181){
    return (4);
  }
  else if (run>=187 && run<=234){
    return (5);
  }
  else {
    cerr << "\t\tNo RunSet for run: " << run << endl;
    return(-1);
  }
}


float inline GetRfShift(int rf, int run){
  if (rf!=0 && rf!=1){
    cerr << "\t\tRF was not 0 or 1!" << endl;
    return(-1000.);
  }
  int RunSet = GetRunSet(run);
  if (RunSet<0 || RunSet>6){
    cerr << "\t\tNo RF shift data for run: " << endl;
    return(-1000.);
  }
/*
From macros/rf_shift.C
        === RF0 ===
        Set     Peak 0          Peak 1          Delta           Shift
        0       42.9935         102.322         59.3282         -24.9146
        1       18.0788         79.361          61.2821         0
        2       7.90395         68.997          61.093          10.1749
        3       17.7335         79.0016         61.2682         0.345369
        4       24.1699         85.4664         61.2966         -6.09103
        5       14.3485         75.5473         61.1989         3.73036
        
        Average:                                61.2277
        
        === RF1 ===
        Set     Peak 0          Peak 1          Delta           Shift
        0       31.9655         93.0474         61.0819         -24.1983
        1       7.76724         68.2702         60.503          0
        2       -0.53215        57.9569         58.489          8.29939
        3       7.48953         68.0005         60.511          0.277708
        4       13.5623         74.4096         60.8473         -5.79508
        5       4.4102          64.5308         60.1206         3.35704
        
        Average:                                60.6128
*/
  float rf_shift[2][6]={
  {  
  -24.9146,
  0.,
  10.1749,
  0.345369,
  -6.09103,
  3.73036,
  },{  
  -24.1983,
  0.,
  8.29939,
  0.277708,
  -5.79508,
  3.35704
  }};
  return (rf_shift[rf][RunSet]); 
}

float inline GetBrho(int run){
  int RunSet = GetRunSet(run);
  if (RunSet<0 || RunSet>6){
    cerr << "\t\tNo Brho data for run: " << endl;
    return(-1000.);
  }

  float brho[6]={
  0.56941,
  0.62064,
  0.60553,
  0.62199,
  0.62199,
  0.60730
  };
  return (brho[RunSet]);
}
