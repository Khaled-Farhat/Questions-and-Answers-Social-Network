#include "AskFm.h"
#include "Database.h"

int main() {
	Database systemDatabase;
	AskFm askFm(systemDatabase);
	
	askFm.run();

	return 0;
}