#pragma once

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
		//Erase 함수를 용이하게 하기위해 CList클래스를 Frined 로 해두었다.
		template <typename T> friend class CList;
	private:
		Node* _node;
	public:
		iterator(Node* node = nullptr)
			:_node(node)

		{
			//인자로 들어온 Node 포인터를 저장
		}

		//후위 쁠쁠
		iterator operator ++(int) 
		{
			//현재 노드를 다음 노드로 이동
			iterator returnIter(_node);
			_node = _node->_Next;
			return returnIter;
		}

		//전위 쁠쁠
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
			//현재 노드의 데이터를 뽑음
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
		//첫번째 노드를 가리키는 이터레이터 리턴
		iterator returnIter(_head->_Next);
		return returnIter;
	}
	iterator end()
	{
		//Tail 노드를 가리키는(데이터가 없는 진짜 더미 끝 노드) 이터레이터를 리턴
		//또는 끝으로 인지할 수 있는 이터레이터를 리턴

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
		//return 할 iter 미리 저장.
		iterator returnIter(delNode->_Next);

		//연결끊기
		delNode->_Prev->_Next = delNode->_Next;
		delNode->_Next->_Prev = delNode->_Prev;

		//동적해제
		delete delNode;

		//사이즈 줄이기
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
	//- 이터레이터의 그 노드를 지움.
	//- 그리고 지운 노드의 다음 노드를 카리키는 이터레이터 리턴
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
	void BubbleSort(bool (*ptr)(T object1, T object2));
	//void DebugPrint(int* arr, int size, int start, int end, int checkindex1, int checkindex2);
};

template<typename T>
inline CList<T>::CList()
	:_head(nullptr),
	 _tail(nullptr)
{
	//더미 노드 생성
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
	//새노드 생성
	Node* pNewNode = new Node;
	pNewNode->_data = data;

	// Push front 에서 새노드의 Next는 Head의 Next노드임
	Node* newNodeNext = _head->_Next;

	// Push front 에서 새노드의 Prev는 더미 Head노드임
	Node* newNodePrev = _head;

	pNewNode->_Next = newNodeNext;
	pNewNode->_Prev = newNodePrev;

	// 노드 연결
	newNodeNext->_Prev = pNewNode;
	newNodePrev->_Next = pNewNode;

	++_size;
}

template<typename T>
inline void CList<T>::push_back(T data)
{
	//새노드 생성
	Node* pNewNode = new Node;
	pNewNode->_data = data;

	// Push back 에서 새노드의 Next는 더미 Tail임
	Node* newNodeNext = _tail;

	// Push back 에서 새노드의 Prev는 Tail->Prev임
	Node* newNodePrev = _tail->_Prev;

	pNewNode->_Next = newNodeNext;
	pNewNode->_Prev = newNodePrev;

	// 노드연결 
	newNodePrev->_Next = pNewNode;
	newNodeNext->_Prev = pNewNode;

	++_size;
}

template<typename T>
inline T CList<T>::pop_front()
{
	Node* delNode = _head->_Next;	
	//노드가 아무것도 없을 경우.
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
	//노드가 아무것도 없을 경우.
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
	// 퀵소트 기반의  Sort
	// 노드데이터를 배열에 복사 후, 소팅을 진행 한뒤,
	// 기존 노드를 Clear하고 다시 소팅한 노드를 Push Back 해준다.
	//------------------------------

	BubbleSort(ptr);
	/*T* arr = new T[_size];

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
	
	delete[] arr;*/
}

template<typename T>
inline void CList<T>::sort()
{
	//------------------------------
	// 퀵소트 기반의  Sort
	// 노드데이터를 배열에 복사 후, 소팅을 진행 한뒤,
	// 기존 노드를 Clear하고 다시 소팅한 노드를 Push Back 해준다.
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

	// 비교대상
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
			//함수포인터 들어가야되는 지점
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
			//함수포인터 들어가야되는 지점
			//--------------------
			if (ptr(arr[pivot], arr[rightIndex]))
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

	// 비교대상
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
			//함수포인터 들어가야되는 지점
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
			//함수포인터 들어가야되는 지점
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

template<typename T>
inline void CList<T>::BubbleSort(bool(*ptr)(T object1, T object2))
{
	auto iter1 = begin();
	auto iter2 = begin();

	++iter2;

	for (int i= _size - 1; i>=0; --i)
	{
	
		for (int j = 0; j < i; j++)
		{
			if (ptr(*iter1, *iter2))
			{
				auto temp = *iter1;
				*iter1 = *iter2;
				*iter2 = temp;
			}
		
			++iter1;
			++iter2;
		}
		iter1 = begin();
		iter2 = begin();
		++iter2;
	}

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
