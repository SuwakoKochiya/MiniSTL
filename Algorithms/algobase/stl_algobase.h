﻿#pragma once

#include <cstddef> // ptrdiff_t

//不能保证长度相等
template <class InputIterator1,class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)
			return false;
	return true;
}

//接受自定义比较器
template <class InputIterator1, class InputIterator2,class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++first2)
		if (!binary_pred(*first1,*first2))
			return false;
	return true;
}

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first)
		*first = value;
}

//显然，本算法执行覆写操作，因此通常配合inserter完成
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, ForwardIterator last, Size n, const T& value) {
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}

template <class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	return __iter_swap(a, b, value_type(*a));
}

//必须要知道迭代器指向的对象类型，才能够构造对象，因此本处使用了value_type
template <class ForwardIterator1, class ForwardIterator2,class T>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b,T*) {
	T temp = *a;
	*a = *b;
	*b = temp;
}

template <class InputIterator1,class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, 
	InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2)
			return true;
		else if(*first1 > *first2)
			return false;
	}
	return first1 == last1 && first2 != last2;//若第二序列有余，返回true，否则false
}

template <class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, Compare comp) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp(*first1,*first2))
			return true;
		else if (comp(*first2, *first1))
			return false;
	}
	return first1 == last1 && first2 != last2;//若第二序列有余，返回true，否则false
}

//针对原始指针const unsigned char*的全特化版本
inline bool
lexicographical_compare(const unsigned char* first1, const unsigned char* last1,
	const unsigned char* first2, const unsigned char* last2) {
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;
	//先比较长度相同的段落
	const int result = memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;

}

template <class T>
inline const T& max(const T& a.const T& b) {
	return a < b ? b : a;
}

template <class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
	comp(a, b) ? b, a;
}

template <class T>
inline const T& min(const T& a.const T& b) {
	return b<a ? b : a;
}

template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
	return comp(b, a) ? b, a;
}

//显然要求序列1长于序列2
template <class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2) {
	while (first1 != last1 && *first1 == *first2) {
		++first1, ++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, BinaryPredicate binary_pred) {
	while (first1 != last1 && binary_pred(*first1,*first2)) {
		++first1, ++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}

template <class T>
inline void swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

template <class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	return __copy_dispatch<InputIterator, OutputIterator>(first, last, result);//__copy_dispatch是一个仿函数对象
}

//针对指针的偏特化
inline char* copy(const char* first, const char* last, const char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t)*(last - first);
	return result + (last - first);
}

template <class InputIterator, class OutputIterator>
inline struct __copy_dispatch(InputIterator first, InputIterator last, OutputIterator result) {//仿函数对象
	OutputIterator operator()(first, last, result) {
		return __copy(first, last, result, iterator_category(first));
	}
}

//偏特化处理
template <class T>
struct __copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		using t = typename __type_traits<T>::has_trivial_assignment_operator;
		return __copy_t(first, last, result, t());
	}
};

//偏特化处理
template <class T>
struct __copy_dispatch<const T*, T*> {
	T* operator()(const T* first, const T* last, T* result) {
		using t = typename __type_traits<T>::has_trivial_assignment_operator;
		return __copy_t(first, last, result, t());
	}
};

//InputIterator
template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result,input_iterator_tag) {
	for (; first != last; ++first, ++result)//迭代器判同，速度较慢
		*result = *first;
	return result;
}

//RandomIterator
template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));//再细分函数以便复用
}

template <class InputIterator, class OutputIterator, class Distance>
inline OutputIterator __copy_d(InputIterator first, InputIterator last, OutputIterator result, Distance*) {
	for (Distance n = last - first; n > 0; --n, ++first, ++result)//以n决定循环次数，速度较快
		*result = *first;
	return result;
}

//具备trivial copy assignment operator，可执行memmove
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
	memmove(result, first, sizeof(T)*(last - first));
	return result+last-first;
}


//原始指针是一种random_access_iterator
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
	return __copy_d(first, last, result, static_cast<ptrdiff_t*>(nullptr));
}

//copy_backward的实现与copy类似，本篇出于时间原因，并不列出