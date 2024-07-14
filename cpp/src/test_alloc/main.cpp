#include "../common/memory.hpp"

static auto A = xStaticPoolAllocator<int>{ 3 };

int main(int, char **) {

	try {
		auto P1 = CreateValue<int>(&A, 1);
		auto P2 = CreateValue<int>(&A, 2);
		auto P3 = CreateValue<int>(&A, 3);
		auto P0 = CreateValue<int>(&A, 0);

		cout << P1 << ":" << *P1 << endl;
		cout << P2 << ":" << *P2 << endl;
		cout << P3 << ":" << *P3 << endl;
		cout << P0 << ":" << endl;

		Destroy<int>(&A, P1);
		Destroy<int>(&A, P2);
		Destroy<int>(&A, P3);

		P0 = CreateValue<int>(&A, 1024);
		cout << P0 << ":" << *P0 << endl;
		Destroy<int>(&A, P0);

	} catch (...) {
		cout << "Exception" << endl;
	}

	return 0;
}