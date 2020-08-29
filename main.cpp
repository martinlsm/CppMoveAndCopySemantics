// https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners

#include <iostream>

using std::string;

class Array {
private:
	string name;
	size_t size, capacity;
	int* data;
public:
	Array(string name) : name(name), size(0), capacity(10) {
		std::cout << "Constructing " << name << std::endl;
		data = new int[capacity];
	}

	~Array() {
		std::cout << "Destructing " << name << std::endl;
		if (data != nullptr) {
			delete[] data;
		}
		data = nullptr;
		size = 0;
		capacity = 0;
		name = "DESTRUCTED";
	}

	Array(const Array& rhs) : size(rhs.size), capacity(rhs.capacity) {
		std::cout << "Copy Construction from " << rhs.name << " to " << name << std::endl;
		data = new int[capacity];
		for (size_t i = 0; i < size; i++) {
			data[i] = rhs.data[i];
		}
	}

	Array& operator=(const Array& rhs) {
		std::cout << "Copy Assignment from " << rhs.name << " to " << name << std::endl;
		if (this == &rhs) {
			return *this;
		}
		delete[] data;
		size = rhs.size;
		capacity = rhs.capacity;
		data = new int[capacity];
		for (size_t i = 0; i < size; i++) {
			data[i] = rhs.data[i];
		}
		return *this;
	}

	Array(Array&& rhs) {
		std::cout << "Move Constructor from " << rhs.name << std::endl;
		name = rhs.name;
		size = rhs.size;
		data = rhs.data;
		capacity = rhs.capacity;
		rhs.name = "DESTRUCTED";
		rhs.capacity = 0;
		rhs.size = 0;
		rhs.data = nullptr;
	}

	Array& operator=(Array&& rhs) noexcept {
		std::cout << "Move Assignment from " << rhs.name << " to " << name << std::endl;
		if (this == &rhs) {
			return *this;
		}
		delete[] data;
		size = rhs.size;
		capacity = rhs.capacity;
		data = rhs.data;
		rhs.size = 0;
		rhs.capacity = 0;
		rhs.data = nullptr;
		return *this;
	}

	void append(int e) {
		if (size == capacity) {
			capacity *= 2;
			int* extended_data = new int[capacity];
			for (size_t i = 0; i < capacity; i++) {
				extended_data[i] = data[i];
			}
			delete[] data;
			data = extended_data;
		}
		data[size] = e;
		size += 1;
	}

	void print() {
		std::cout << name << "[";
		for (size_t i = 0; i < size; i++) {
			std::cout << data[i];
			if (i < size - 1) {
				std::cout << ", ";
			}
		}
		std::cout << "]" << std::endl;
	}
};

Array create_array(string name) {
	Array a{name};
	a.append(1);
	a.append(2);
	a.append(3);
	return a;
}

void case_A() {
	std::cout << "Case A" << std::endl;
	Array a1{"a1"};

	// Case A
	// Constructing a1
	// Destructing a1
}

void case_B() {
	std::cout << "Case B" << std::endl;
	Array a1{"a1"};
	a1.append(123);
	Array a2{a1};
	a2.append(456);
	a1.print();
	a2.print();

	// Case B
	// Constructing a1
	// Copy Construction from a1 to 
	// a1[123]
	// [123, 456]
	// Destructing 
	// Destructing a1
}

void case_C() {
	std::cout << "Case C" << std::endl;
	Array a1{"a1"};
	Array a2{"a2"};
	a1.append(123);
	a2.append(456);
	a1 = a2;
	a1.print();
	a2.print();
    
	// Case C
	// Constructing a1
	// Constructing a2
	// Copy Assignment from a2 to a1
	// a1[456]
	// a2[456]
	// Destructing a2
	// Destructing a1
}

void case_D() {
	std::cout << "Case D" << std::endl;
	Array a1{"a"};
	a1 = Array{"a_rvalue"};
	a1.append(456);
	a1.print();

	// Case D
	// Constructing a
	// Constructing a_rvalue
	// Move Assignment from a_rvalue to a
	// Destructing a_rvalue
	// a[456]
	// Destructing a
}

void case_E() {
	std::cout << "Case E" << std::endl;
	Array a = create_array("a");
	a.print();
    
	// Case E
	// Constructing a
	// a[1, 2, 3]
	// Destructing a
}

void case_F() {
	std::cout << "Case F" << std::endl;
	Array a{"a"};
	a = create_array("ac");
	a.print();

	// Case F
	// Constructing a
	// Constructing ac
	// Move Assignment from ac to a
	// Destructing ac
	// a[1, 2, 3]
	// Destructing a
}

void case_G() {
	std::cout << "Case G" << std::endl;
	Array a1{"a1"};
	a1 = create_array("a1");
	Array a2 = std::move(a1);
	a2.print();

	// Case G
	// Constructing a1
	// Constructing a1
	// Move Assignment from a1 to a1
	// Destructing a1
	// Move Constructor from a1
	// a1[1, 2, 3]
	// Destructing a1
	// Destructing DESTRUCTED
}

int main() {
	case_A();
	case_B();
	case_C();
	case_D();
	case_E();
	case_F();
	case_G();
}
