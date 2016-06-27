#include "lms.h"

using namespace std;
using namespace lms;
int
main() {
	Database test("test.json");
	test.init();
	test.booksInterface();
}
