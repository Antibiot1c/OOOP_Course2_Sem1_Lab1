#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <vector>
using namespace std;

// Рандомізація
// Randomization
int impl = 0;
double RAND(int min, int max) {
	return (min + (rand() % (1000 * (max - min))) / 1000.0f);
}

//-----------------------------------------------------------------------------------------------//
// 
// Список
// List
//
//-----------------------------------------------------------------------------------------------//


// Зв’язний список
// Linked list
template <typename T>
class Linkedlist {
private:
	struct Node {
		T val;
		Node* next;
	};
public:
	Node* head;
	Node* sorted;

	Linkedlist(int size) {
		int m = 0;
		if (size == 0) head = NULL;
		Node* current = NULL;
		for (int i = 0; i < size; i++) {
			if (m == 0) {
				head = new Node;
				head->val = RAND(10, 100);
				head->next = NULL;
				current = head;
			}
			else {
				current->next = new Node;
				current = current->next;
				current->val = RAND(10, 100);
				current->next = NULL;
			}
			m++;
		}
	}
	~Linkedlist() {
		Node* help = NULL;
		while (head != NULL) {
			help = head->next;
			delete head;
			head = help;
		}
	}

	Node* getTail(Node* head, Node*& tail) {
		Node* current = head;
		while (current->next != NULL) {
			tail = current;
			current = current->next;
		}
		return tail;
	}


	//-----------------------------------------------------------------------------------------------//
	// 
	// Алгоритми сортування
	// Sorting algorithms
	//
	//-----------------------------------------------------------------------------------------------//


	//Insertion-Sort (Сортування вставкою) ----------------------------------------------------------
	void insertionSort(Node* headref)
	{
		sorted = NULL;
		Node* current = headref;
		while (current != NULL) {
			Node* next = current->next;
			sortedInsert(current);
			current = next;
		}
		head = sorted;
	}
	void sortedInsert(Node* newnode)
	{
		if (sorted == NULL || sorted->val >= newnode->val) {
			newnode->next = sorted;
			sorted = newnode;
		}
		else {
			Node* current = sorted;
			while (current->next != NULL
				&& current->next->val < newnode->val) {
				current = current->next;
			}
			newnode->next = current->next;
			current->next = newnode;
		}
	}


	//Quick-Sort (Швидке сортування) ---------------------------------------------------------
	Node* paritionLast(Node* start, Node* end)
	{
		if (start == end || start == NULL || end == NULL)
			return start;

		Node* pivot_prev = start;
		Node* curr = start;
		T pivot = end->val;

		while (start != end) {
			if (start->val < pivot) {
				pivot_prev = curr;
				T temp = curr->val;
				curr->val = start->val;
				start->val = temp;
				curr = curr->next;
			}
			start = start->next;
		}

		T temp = curr->val;
		curr->val = pivot;
		end->val = temp;

		return pivot_prev;
	}
	void QuickSort(Node* start, Node* end) {
		if (start == NULL || start == end || start == end->next)
			return;

		Node* pivot_prev = paritionLast(start, end);
		QuickSort(start, pivot_prev);

		if (pivot_prev != NULL && pivot_prev == start)
			QuickSort(pivot_prev->next, end);

		else if (pivot_prev != NULL
			&& pivot_prev->next != NULL)
			QuickSort(pivot_prev->next->next, end);
	}


	//Merge-Sort (Сортування злиттям) ----------------------------------------------------
	Node* sortedMerge(Node* a, Node* b) {
		Node* result = NULL;
		if (a == NULL)
			return b;
		if (b == NULL)
			return a;

		if (a->val <= b->val) {
			result = a;
			result->next = sortedMerge(a->next, b);
		}
		else {
			result = b;
			result->next = sortedMerge(a, b->next);
		}
		return result;
	}
	Node* mergeSort(Node* h) {
		if (h == NULL || h->next == NULL) {
			return h;
		}
		Node* middle = getMiddle(h);
		Node* nextofmiddle = middle->next;

		middle->next = NULL;

		Node* left = mergeSort(h);

		Node* right = mergeSort(nextofmiddle);

		Node* sortedlist = sortedMerge(left, right);
		return sortedlist;
	}

	Node* getMiddle(Node* head) {
		if (head == NULL)
			return head;

		Node* slow = head, * fast = head;

		while (fast->next != NULL && fast->next->next != NULL) {
			slow = slow->next;
			fast = fast->next->next;
		}
		return slow;
	}
	void printlist(Node* head) {
		while (head != NULL) {
			cout << head->val << " ";
			head = head->next;
		}
		cout << endl;
	}
};

// Масив
// Array
template <typename T1>
class Array {
private:
	T1* arr;
	int n;
public:
	Array(int size) {
		this->n = size;
		this->arr = new T1[size];
		for (int i = 0; i < size; i++) {
			arr[i] = RAND(10, 100);;
		}
	};
	~Array() {
		delete[]arr;
	};
	Array(const Array& other) {
		this->arr = new T1[other.n];
		for (int i = 0; i < other.n; i++) {
			this->arr[i] = other.arr[i];
		}
	}

	//-----------------------------------------------------------------------------------------------//
	// 
	// Алгоритми сортування
	// Sorting algorithms
	//
	//-----------------------------------------------------------------------------------------------//


	//Insertion-Sort (Сортування вставкою) ---------------------------------------------------------
	void insertionSort() {
		int i, key, j;
		for (i = 1; i < n; i++)
		{
			key = arr[i];
			j = i - 1;
			while (j >= 0 && arr[j] > key)
			{
				arr[j + 1] = arr[j];
				j = j - 1;
			}
			arr[j + 1] = key;
		}
	}


	//Quick-Sort (Швидке сортування) -------------------------------------------------------------
	int partition(int begin, int end) {
		int pivot = arr[end];
		int i = (begin - 1);

		for (int j = begin; j < end; j++) {
			if (arr[j] <= pivot) {
				i++;

				int swapTemp = arr[i];
				arr[i] = arr[j];
				arr[j] = swapTemp;
			}
		}

		int swapTemp = arr[i + 1];
		arr[i + 1] = arr[end];
		arr[end] = swapTemp;

		return i + 1;
	}
	void quickSort(int begin, int end) {
		if (begin < end) {
			int partitionIndex = partition(begin, end);
			quickSort(begin, partitionIndex - 1);
			quickSort(partitionIndex + 1, end);
		}
	}


	//Merge-Sort (Сортування злиттям) -------------------------------------------------------------
	void merge(int l, int m, int r) {
		int n1 = m - l + 1;
		int n2 = r - m;

		int* L = new T1[n1];
		int* R = new T1[n2];

		for (int i = 0; i < n1; ++i)
			L[i] = arr[l + i];
		for (int j = 0; j < n2; ++j)
			R[j] = arr[m + 1 + j];

		int i = 0, j = 0;

		int k = l;
		while (i < n1 && j < n2) {
			if (L[i] <= R[j]) {
				arr[k] = L[i];
				i++;
			}
			else {
				arr[k] = R[j];
				j++;
			}
			k++;
		}

		while (i < n1) {
			arr[k] = L[i];
			i++;
			k++;
		}

		while (j < n2) {
			arr[k] = R[j];
			j++;
			k++;
		}
		delete[] L;
		delete[] R;
	}
	void MergeSort(int l, int r)
	{
		if (l < r) {
			// Find the middle point
			int m = l + (r - l) / 2;

			// Sort first and second halves
			MergeSort(l, m);
			MergeSort(m + 1, r);

			// Merge the sorted halves
			merge(l, m, r);
		}
	}
	int getMax() {
		int mx = arr[0];
		for (int i = 1; i < n; i++)
			if (arr[i] > mx)
				mx = arr[i];
		return mx;
	}


	//-----------------------------------------------------------------------------------------------//
	// 
	// Швидкі алгоритми, не засновані на порівнянні 
	// Fast algorithms, not based on comparison
	//
	//-----------------------------------------------------------------------------------------------//

	
	//Count-Sort (Сортування за кількістю) -------------------------------------------------------------
	void CountSort() {
		int* output = new T1[n];
		int max = getMax(), i;
		int* count = new int[max + 1];
		for (i = 0; i < max + 1; i++)
			count[i] = 0;
		for (i = 0; i < n; i++)
			count[arr[i]] += 1;

		for (i = 1; i < max + 1; i++) {
			count[i] += count[i - 1];
		}
		for (i = n - 1; i >= 0; i--) {
			output[count[arr[i]] - 1] = arr[i];
			count[arr[i]]--;
		}
		for (i = 0; i < n; i++)
			arr[i] = output[i];
		delete[] output;
		delete[] count;
	}


	//Radix-Sort (Порозрядне сортування) -------------------------------------------------------------
	void countSort_R(int exp) {
		int* output = new T1[n];
		int i, count[10] = { 0 };
		for (i = 0; i < n; i++)
			count[(arr[i] / exp) % 10]++;

		for (i = 1; i < 10; i++)
			count[i] += count[i - 1];

		for (i = n - 1; i >= 0; i--) {
			output[count[(arr[i] / exp) % 10] - 1] = arr[i];
			count[(arr[i] / exp) % 10]--;
		}
		for (i = 0; i < n; i++)
			arr[i] = output[i];
		delete[] output;
	}
	void radixSort() {
		int m = getMax();
		for (int exp = 1; m / exp > 0; exp *= 10)
			countSort_R(exp);
	}
	void printArray() {
		int i;
		for (i = 0; i < n; i++)
			cout << arr[i] << " ";
		cout << endl;
	}
};

// Вектор (Бібліотечний засіб)
// Vector (Library tool)
template <typename T1>
class Vector_ {
private:
	vector<T1> vec;
	int n;
public:
	Vector_(int k) {
		this->n = k;
		for (int i = 0; i < k; i++) {
			vec.push_back(RAND(10, 100));
		}
	}


	//-----------------------------------------------------------------------------------------------//
	// 
	// Алгоритми сортування
	// Sorting algorithms
	//
	//-----------------------------------------------------------------------------------------------//


	//Insertion-Sort (Сортування вставкою) -----------------------------------------------------------
	void insertion_sort() {
		for (size_t j = 1; j < vec.size(); j++) {
			int key = vec[j];
			int i = j - 1;

			while (i >= 0 && vec[i] > key) {
				vec[i + 1] = vec[i];
				i--;
			}
			vec[i + 1] = key;
		}
	}


	//Quick-Sort (Швидке сортування) ---------------------------------------------------------------
	int partition(int left, int right) {
		int pivotIndex = left + (right - left) / 2;
		int pivotValue = vec[pivotIndex];
		int i = left, j = right;
		T1 temp;
		while (i <= j) {
			while (vec[i] < pivotValue) {
				i++;
			}
			while (vec[j] > pivotValue) {
				j--;
			}
			if (i <= j) {
				temp = vec[i];
				vec[i] = vec[j];
				vec[j] = temp;
				i++;
				j--;
			}
		}
		return i;
	}
	void quicksort(int left, int right) {
		if (left < right) {
			int pivotIndex = partition(left, right);
			quicksort(left, pivotIndex - 1);
			quicksort(pivotIndex, right);
		}
	}


	//Merge-Sort (Сортування злиттям) ---------------------------------------------------------------
	void MergeSortedIntervals(int start, int mid, int end) {
		vector<T1> temp;

		int i, j;
		i = start;
		j = mid + 1;

		while (i <= mid && j <= end) {
			if (vec[i] <= vec[j]) {
				temp.push_back(vec[i]);
				++i;
			}
			else {
				temp.push_back(vec[j]);
				++j;
			}
		}
		while (i <= mid) {
			temp.push_back(vec[i]);
			++i;
		}
		while (j <= end) {
			temp.push_back(vec[j]);
			++j;
		}
		for (int i = start; i <= end; ++i)
			vec[i] = temp[i - start];
	}
	void MergeSort(int start, int end) {
		if (start < end) {
			int mid = (start + end) / 2;
			MergeSort(start, mid);
			MergeSort(mid + 1, end);
			MergeSortedIntervals(start, mid, end);
		}
	}
	void print() {
		for (int x : vec)
			cout << x << " ";
		cout << '\n';
	}
};


//-----------------------------------------------------------------------------------------------//
// 
// Менюшка
// Menu
//
//-----------------------------------------------------------------------------------------------//


void Menu() {
	cout << "--------------- Оберіть реалізацію: ---------------" << endl
		<< "(1) Однозв'язний список" << endl
		<< "(2) Масив" << endl
		<< "(3) Вектор" << endl;
	cout << "---------------------------------------------------" << endl
		<< "(0) Завершити роботу" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "Ваш вибір: " << endl;
	cin >> impl;
}
void Menu_Sort_(int& tsort) {
	cout << "--------------- Оберіть метод сортування даних: ---------------" << endl
		<< "(1) Insert sort" << endl
		<< "(2) Quick sort" << endl
		<< "(3) Merge sort" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "Ваш вибір: " << endl;
	cout << "---------------------------------------------------" << endl;
	cin >> tsort;
}
void Menu_Sort_Arr(int& tsort) {
	cout << "--------------- Оберіть метод сортування даних: ---------------" << endl
		<< "(1) Insert sort" << endl
		<< "(2) Quick sort" << endl
		<< "(3) Merge sort" << endl
		<< "(4) Count sort" << endl
		<< "(5) Radix sort" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "Ваш вибір: " << endl;
	cout << "---------------------------------------------------" << endl;
	cin >> tsort;
}
void after_case() {
	system("pause");
	system("cls");
	Menu();
}
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int type_s = 0, size = 10;
	Linkedlist<double> list(size);
	Linkedlist<double> list_z(0);
	Array<int> arr(size);
	Vector_<int> vec(size);
	Menu();
	while (impl) {
		switch (impl) {
		case 1: system("cls");
			Menu_Sort_(type_s); system("cls");
			cout << "--------------- Дані до сортування ---------------" << endl;
			list.printlist(list.head);
			cout << endl;
			if (type_s == 1)
				list.insertionSort(list.head);
			if (type_s == 2) {
				list_z.head = list.getTail(list.head, list_z.head);
				list.QuickSort(list.head, list_z.head);
			}
			if (type_s == 3)
				list.head = list.mergeSort(list.head);
			cout << "--------------- Дані після сортування ---------------" << endl;
			list.printlist(list.head);
			after_case(); break;

		case 2: system("cls");
			Menu_Sort_Arr(type_s); system("cls");
			cout << "--------------- Дані до сортування ---------------" << endl;
			arr.printArray();
			if (type_s == 1) arr.insertionSort();
			if (type_s == 2) arr.quickSort(0, size - 1);
			if (type_s == 3) arr.MergeSort(0, size - 1);
			if (type_s == 4) arr.CountSort();
			if (type_s == 5) arr.radixSort();
			cout << "--------------- Дані після сортування ---------------" << endl;
			arr.printArray();
			after_case(); break;

		case 3: system("cls");
			Menu_Sort_(type_s); system("cls");
			cout << "--------------- Дані до сортування ---------------" << endl;
			vec.print();
			if (type_s == 1) vec.insertion_sort();
			if (type_s == 2) vec.quicksort(0, size - 1);
			if (type_s == 3) vec.MergeSort(0, size - 1);
			cout << "--------------- Дані після сортування ---------------" << endl;
			vec.print();
			after_case(); break;
		default:
			cout << "--------------- Неправильно введено код операції! ---------------" << endl;
			after_case(); break;
		}
	}
	return 0;
}
