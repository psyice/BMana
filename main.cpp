#include "lms.h"

using namespace std;
using namespace lms;
int
main() {
	Database test("books.json");
	test.init();
	test.booksInterface();
}
