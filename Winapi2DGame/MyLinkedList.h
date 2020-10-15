#pragma once
#include "MemoryTracer.h"

template <typename T>
class CList
{
public:
	struct Node
	{
		T _data;
		Node* _Prev;
		Node* _Next;
	};
	class iterator
	{
		//Erase �Լ��� �����ϰ� �ϱ����� CListŬ������ Frined �� �صξ���.
		template <typename T> friend class CList;
	private:
		Node* _node;
	public:
		iterator(Node* node = nullptr)
			:_node(node)

		{
			//���ڷ� ���� Node �����͸� ����
		}

		//���� �ܻ�
		iterator operator ++(int) 
		{
			//���� ��带 ���� ���� �̵�
			iterator returnIter(_node);
			_node = _node->_Next;
			return returnIter;
		}

		//���� �ܻ�
		iterator& operator++()
		{
			_node = _node->_Next;
			return *this;
		}

		iterator operator --(int)
		{
			iterator returnIter(_node);
			_node = _node->_Prev;
			return returnIter;

		}

		iterator& operator--()
		{
			_node = _node->_Prev;
			return *this;
		}

		T& operator *()
		{
			//���� ����� �����͸� ����
			return _node->_data;
		}
		bool operator ==(const iterator& other)
		{
			return other._node == _node;
		}
		bool operator !=(const iterator& other)
		{
			return other._node != _node;
		}
		 
	};

public:
	CList();
	~CList();

	iterator begin()
	{
		//ù��° ��带 ����Ű�� ���ͷ����� ����
		iterator returnIter(_head->_Next);
		return returnIter;
	}
	iterator end()
	{
		//Tail ��带 ����Ű��(�����Ͱ� ���� ��¥ ���� �� ���) ���ͷ����͸� ����
		//�Ǵ� ������ ������ �� �ִ� ���ͷ����͸� ����

		iterator returnIter(_tail);
		return returnIter;
	}
	iterator erase(iterator iter)
	{
		Node* delNode = iter._node;
		
		if (delNode == _tail)
		{
			iterator returnIter(_tail);
			return returnIter;
		}
		//return �� iter �̸� ����.
		iterator returnIter(delNode->_Next);

		//�������
		delNode->_Prev->_Next = delNode->_Next;
		delNode->_Next->_Prev = delNode->_Prev;

		//��������
		delete delNode;

		//������ ���̱�
		_size--;

		return returnIter;		
	}
	void push_front(T data);
	void push_back(T data);
	T pop_front();
	T pop_back();
	void clear();
	int size() { return _size; };
	bool empty() { return _size == 0; };
	void sort(bool (*ptr)(T object1, T object2));
	void sort();
	//- ���ͷ������� �� ��带 ����.
	//- �׸��� ���� ����� ���� ��带 ī��Ű�� ���ͷ����� ����
	void remove(T Data)
	{
		iterator iter = begin();

		for (; iter != end(); )
		{
			if (*iter == Data)
				iter =erase(iter);
			else
				++iter;
		}
	}
private:
	int _size = 0;
	Node* _head;
	Node* _tail;
private:
	void QuickSort(T* arr, int left, int right, bool (*ptr)(T object1, T object2));
	void QuickSort(T* arr, int left, int right);
	//void DebugPrint(int* arr, int size, int start, int end, int checkindex1, int checkindex2);
};

template<typename T>
inline CList<T>::CList()
	:_head(nullptr),
	 _tail(nullptr)
{
	//���� ��� ����
	_head = new Node;
	_tail = new Node;

	_head->_Next = _tail;
	_head->_Prev = nullptr;
	_tail->_Prev = _head;
	_tail->_Next = nullptr;

}

template<typename T>
inline CList<T>::~CList()
{
	CList<T>::clear();
	delete _head;
	delete _tail;
}

template<typename T>
inline void CList<T>::push_front(T data)
{
	//����� ����
	Node* pNewNode = new Node;
	pNewNode->_data = data;

	// Push front ���� ������� Next�� Head�� Next�����
	Node* newNodeNext = _head->_Next;

	// Push front ���� ������� Prev�� ���� Head�����
	Node* newNodePrev = _head;

	pNewNode->_Next = newNodeNext;
	pNewNode->_Prev = newNodePrev;

	// ��� ����
	newNodeNext->_Prev = pNewNode;
	newNodePrev->_Next = pNewNode;

	++_size;
}

template<typename T>
inline void CList<T>::push_back(T data)
{
	//����� ����
	Node* pNewNode = new Node;
	pNewNode->_data = data;

	// Push back ���� ������� Next�� ���� Tail��
	Node* newNodeNext = _tail;

	// Push back ���� ������� Prev�� Tail->Prev��
	Node* newNodePrev = _tail->_Prev;

	pNewNode->_Next = newNodeNext;
	pNewNode->_Prev = newNodePrev;

	// ��忬�� 
	newNodePrev->_Next = pNewNode;
	newNodeNext->_Prev = pNewNode;

	++_size;
}

template<typename T>
inline T CList<T>::pop_front()
{
	Node* delNode = _head->_Next;	
	//��尡 �ƹ��͵� ���� ���.
	if (delNode == _tail)
	{
		return 0;
	}

	T returnData = delNode->_data;
	delNode->_Prev->_Next = delNode->_Next;
	delNode->_Next->_Prev = delNode->_Prev;
	delete delNode;

	--_size;
	return  returnData;
}

template<typename T>
inline T CList<T>::pop_back()
{
	Node* delNode = _tail->_Prev;
	//��尡 �ƹ��͵� ���� ���.
	if (delNode == _head)
	{
		return 0;
	}
	T returnData = delNode->_data;
	delNode->_Prev->_Next = delNode->_Next;
	delNode->_Next->_Prev = delNode->_Prev;
	delete delNode;

	--_size;
	return  returnData;
}

template<typename T>
inline void CList<T>::clear()
{
	Node* curNode = _head->_Next;

	while (curNode != _tail)
	{
		Node* delNode = curNode;
		curNode = curNode->_Next;
		delete delNode;
	}

	_head->_Next = _tail;
	_tail->_Prev = _head;
	_size = 0;
}

template<typename T>
inline void CList<T>::sort(bool(*ptr)(T object1, T object2))
{
	//------------------------------
	// ����Ʈ �����  Sort
	// ��嵥���͸� �迭�� ���� ��, ������ ���� �ѵ�,
	// ���� ��带 Clear�ϰ� �ٽ� ������ ��带 Push Back ���ش�.
	//------------------------------

	T* arr = new T[_size];

	auto iter = this->begin();

	for (int index = 0; iter != this->end(); ++iter, ++index)
	{
		arr[index] = *iter;
	}

	QuickSort(arr, 0, _size - 1, ptr);
	int size = _size;
	this->clear();
	for (int i = 0; i < size; ++i)
	{
		push_back(arr[i]);
	}
	
	delete[] arr;
}

template<typename T>
inline void CList<T>::sort()
{
	//------------------------------
	// ����Ʈ �����  Sort
	// ��嵥���͸� �迭�� ���� ��, ������ ���� �ѵ�,
	// ���� ��带 Clear�ϰ� �ٽ� ������ ��带 Push Back ���ش�.
	//------------------------------
	T* arr = new T[_size];

	auto iter = this->begin();

	for (int index = 0; iter != this->end(); ++iter, ++index)
	{
		arr[index] = *iter;
	}

	QuickSort(arr, 0, _size - 1);
	int size = _size;
	this->clear();
	for (int i = 0; i < size; ++i)
	{
		push_back(arr[i]);
	}

	delete[] arr;
}

template <typename T>
void CList<T>::QuickSort(T* arr, int left, int right, bool (*ptr)(T object1, T object2))
{
	int pivot = left;
	int leftIndex = left;
	int rightIndex = right;
	int size = right - left + 1;

	// �񱳴��
	if (left > right)
	{
		return;
	}
	while (leftIndex < rightIndex)
	{
		while (true)
		{
			if (leftIndex > right)
			{
				leftIndex--;
				break;
			}
			//--------------------
			//�Լ������� ���ߵǴ� ����
			//--------------------
			if (ptr(arr[leftIndex], arr[pivot]))//(//if (arr[leftIndex] > arr[pivot])
			{
				break;
			}
			++leftIndex;
		}
		while (true)
		{
			if (rightIndex < left)
			{
				rightIndex = left;
				break;
			}
			//--------------------
			//�Լ������� ���ߵǴ� ����
			//--------------------
			if (ptr(arr[pivot], arr[leftIndex]))
			{
				break;
			}
			--rightIndex;

		}
		if (leftIndex > rightIndex)
		{
			break;
		}
		T temp = arr[leftIndex];
		arr[leftIndex] = arr[rightIndex];
		arr[rightIndex] = temp;

	}
	T temp = arr[pivot];
	arr[pivot] = arr[rightIndex];
	arr[rightIndex] = temp;

	pivot = rightIndex;

	QuickSort(arr, left, pivot - 1, ptr);
	QuickSort(arr, pivot + 1, right, ptr);

}
template<typename T>
inline void CList<T>::QuickSort(T* arr, int left, int right)
{
	int pivot = left;
	int leftIndex = left;
	int rightIndex = right;
	int size = right - left + 1;

	// �񱳴��
	if (left > right)
	{
		return;
	}
	while (leftIndex < rightIndex)
	{
		while (true)
		{
			if (leftIndex > right)
			{
				leftIndex--;
				break;
			}
			//--------------------
			//�Լ������� ���ߵǴ� ����
			//--------------------
			if (arr[leftIndex] > arr[pivot])
			{
				break;
			}
			++leftIndex;
		}
		while (true)
		{
			if (rightIndex < left)
			{
				rightIndex = left;
				break;
			}
			//--------------------
			//�Լ������� ���ߵǴ� ����
			//--------------------
			if (arr[leftIndex] < arr[pivot])
			{
				break;
			}
			--rightIndex;

		}
		if (leftIndex > rightIndex)
		{
			break;
		}
		T temp = arr[leftIndex];
		arr[leftIndex] = arr[rightIndex];
		arr[rightIndex] = temp;

	}
	T temp = arr[pivot];
	arr[pivot] = arr[rightIndex];
	arr[rightIndex] = temp;

	pivot = rightIndex;

	QuickSort(arr, left, pivot - 1);
	QuickSort(arr, pivot + 1, right);
}
//template <typename T>
//void CList<T>::DebugPrint(int* arr, int size, int start, int end, int checkindex1, int checkindex2)
//{
//	for (int i = 0; i < size; i++)
//	{
//		if (i == checkindex1 || i == checkindex2)
//		{
//			cout << "[" << arr[i] << "]" << " ";
//
//		}
//		else
//		{
//			cout << " " << arr[i] << " " << " ";
//		}
//	}
//	cout << endl;
//
//	for (int i = 0; i < size; i++)
//	{
//		if (start <= i && i <= end)
//		{
//			cout << "^^^" << " ";
//		}
//		else
//		{
//			cout << "   " << " ";
//		}
//
//	}
//	cout << endl;
//}