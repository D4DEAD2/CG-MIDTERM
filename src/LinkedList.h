#pragma once

//referenced code from OOC snake

template<class T>
class Link {
private:
	T value;
	Link<T>* next = nullptr;
public:
	Link(T val) { value = val; }
	T GetValue() { return value; }
	Link<T>* GetNext() { return next; }
	void SetNext(Link<T>* n) { next = n; }
};

template<class T>
class LinkedList {
	Link<T>* start = nullptr;
	Link<T>* cur = nullptr;
	int size = 0;
public:
	LinkedList() {};

	int Size() { return size; }

	void Add(T val) {
		if (size <= 0) {
			start = new Link<T>(val);
		}
		else {
			cur = start;
			while (cur->GetNext() != nullptr) {
				cur = cur->GetNext();
			}
			cur->SetNext(new Link<T>(val));
		}
		size++;
	};

	Link<T>* At(int id) {
		if (id < size) {
			cur = start;
			for (int c = 0; c < id; c++) {
				cur = cur->GetNext();
			}
			return cur;
		}
	}
};

//class Object;
//LinkedList<Object*> myLinkedList;
//
//class Player : public LinkedList<Object*> {
//
//};