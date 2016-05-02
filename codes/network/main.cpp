#include "network.h"
#include <string>
using namespace std;


int main(int argc, char *argv[]){
	if(argc != 2)
		exit(0);
	string mode(argv[1]);
/*	CPU unit;
	unit.readOverhead();
	unit.loopOverhead();
*/
	network unit;
	//unit.RAMaccessTime();
	if(mode == "server")
		unit.setServer();
	else{
		// for(int i=0; i<20; i++)
		// 	unit.roundTrip();
		// for(int i=0; i<20; i++)
		// 	unit.connectionOverhead();
		unit.peakBandwidth();

	}
	// unit.roundTrip();
	// unit.peakBandwidth();
	// unit.connectionOverhead();
	return 0;

}