#include "lms.h"

using namespace std;
using namespace lms;
int
main() {
	Database test("book.json");
	test.init();
	test.booksInterface();
}
