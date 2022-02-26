#ifndef QUEUE_ARRAY_HPP
#define QUEUE_ARRAY_HPP

// У Татьяны Николаевны файл называется QueueArray (ограниченная очередь)

#include "QueueVirtual.hpp"
#include "QueueExceptions.hpp"

template <typename T>
class QueueArray final : public QueueVirtual<T> // Модификатор final запрещает наследование от класса.
{
public:
	explicit QueueArray(size_t size);

	QueueArray(const QueueArray<T>& src) = delete;
	QueueArray(QueueArray<T>&& src) noexcept;

	virtual ~QueueArray() override;

	QueueArray<T>& operator=(const QueueArray<T>& src) = delete;
	QueueArray<T>& operator=(QueueArray<T>&& src) noexcept;

	void enQueue(const T& e) override;
	T deQueue() override;
	bool isEmpty() const override;

private:
	T* array_; // Массив элементов очереди.
	size_t head_ = 1; // Очередь пуста, если head[Q] = tail[Q].
	size_t tail_ = 1; // Первоначально: head[Q] = tail[Q] = 1;
	size_t size_; // Размер очереди.

	void swap(QueueArray<T>& right);
};

template <typename T>
QueueArray<T>::QueueArray(size_t size) :
	array_(nullptr),
	head_(1),
	tail_(1),
	size_(size)
{
	try
	{
		array_ = new T[size + 1]; // Пытаемся заказать память под элементы очереди
	}
	catch (...) // Если что-то случилось (например, размер слишком большой)
	{
		throw WrongQueueSize(); // - возникает bad_alloc.
	}
}

template <typename T>
QueueArray<T>::QueueArray(QueueArray<T>&& src) noexcept :
	array_(src.array_),
	head_(src.head_),
	tail_(src.tail_),
	size_(src.size_)
{
	src.array_ = nullptr;
	src.head_ = 1;
	src.tail_ = 1;
	src.size_ = 0;
}

template <typename T>
QueueArray<T>::~QueueArray()
{
	delete[] array_;
}

template <typename T>
QueueArray<T>& QueueArray<T>::operator=(QueueArray<T>&& src) noexcept
{
	array_ = src.array_;
	head_ = src.head_;
	tail_ = src.tail_;
	size_ = src.size_;

	src.array_ = nullptr;
	src.head_ = 1;
	src.tail_ = 1;
	src.size_ = 0;
}

template <typename T>
void QueueArray<T>::enQueue(const T& e)
{
	if ((head_ == tail_ + 1) || ((head_ == 0) && (tail_ == size_)))
	{
		throw QueueOverflow();
	}

	array_[tail_] = e;
	if (tail_ != size_)
	{
		tail_++;
	}
	else
	{
		tail_ = 0;
	}
}

template <typename T>
T QueueArray<T>::deQueue()
{
	if (head_ == tail_)  //  Очередь пуста
	{
		throw QueueUnderflow();
	}

	T result = array_[head_];
	if (head_ != size_)
	{
		head_++;
	}
	else
	{
		head_ = 0;
	}

	return result;
}

template <typename T>
bool QueueArray<T>::isEmpty() const
{
	return head_ == tail_;
}

template <typename T>
void QueueArray<T>::swap(QueueArray<T>& right)
{
	std::swap(array_, right.array_);
	std::swap(head_, right.head_);
	std::swap(tail_, right.tail_);
	std::swap(size_, right.size_);
}

#endif
