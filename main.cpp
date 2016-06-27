#include "lms.h"

using namespace std;
using namespace lms;
int
main() {
	lms::BookDatabase bkdb;
	lms::init("./test.json", bkdb);
	//std::cout << lms::database["books"].Size() << std::endl;
	bkdb.userInterface("./test.json", bkdb);
}
