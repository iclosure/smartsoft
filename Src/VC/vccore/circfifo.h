
#ifndef _HR_CIRCULAR_FIFO_H_
#define _HR_CIRCULAR_FIFO_H_

#include "vccore_global.h"

namespace hr
{

#include <assert.h>

	template <typename atom, size_t count>
	class VCCORE_EXPORT circfifo
	{
		enum : size_t { _COUNT = count };	// helper for expression evaluator
	public:
		circfifo() : _count(_COUNT), _size(0), _indexRead(0),
			_indexWrite(0), _buffer(0)
		{
			assert(_count > 0);

			_buffer = new atom[_count];

			memset(_buffer, 0, _count * sizeof(atom));
		}
		~circfifo()
		{
			dispatch();
		}

		inline void clear()
		{
			_size = 0;
			_indexRead = 0;
			_indexWrite = 0;
		}

		inline void dispatch()
		{
			if (_buffer != NULL)
			{
				delete[] _buffer;
				_buffer = NULL;
			}
		}

		inline size_t count() const { return _count; }
		inline size_t size() const { return _size; }

		inline bool empty() const
		{
			return (_size == 0) ? true : false;
		}

		inline bool full() const
		{
			return (_size == _count) ? true : false;
		}

		inline atom take()
		{
			if (_size == 0)
			{
				assert(false);  // please call empty() to check _size before taking one atom
				return atom();
			}

			atom value = _buffer[_indexRead];
			_indexRead = (_indexRead + 1) % _count;
			_size--;

			return value;
		}

		inline atom & front() const
		{
			if (_size == 0)
			{
				assert(false);  // please call empty() to check _size before taking one atom
				throw _size;
			}

			return _buffer[_indexRead];
		}

		inline void pop_front()
		{
			if (_size == 0)
			{
				return;
			}

			_indexRead = (_indexRead + 1) % _count;
			_size--;
		}

		inline void push_back(const atom & value)
		{
			if (_size == _count)
			{
				// remove first item
				_indexRead = (_indexRead + 1) % _count;
				_size--;
			}

			_buffer[_indexWrite] = value;
			_indexWrite = (_indexWrite + 1) % _count;
			_size++;
		}

		inline long read(atom* buffer, long count)
		{
			if (_size = 0)
			{
				count = 0;
			}
			else
			{
				if (count > _count)
				{
					count = _count;
				}

				long overflow = _indexRead + count - _count;

				if (overflow < 0)
				{
					memcpy(buffer, &_buffer[_indexRead], count * sizeof(atom));
					_indexRead = (_indexRead + count) % _count;
					_size -= count;
				}
				else
				{
					memcpy(buffer, &_buffer[_indexRead], overflow * sizeof(atom));

					count -= overflow;
					memcpy(&buffer[overflow], _buffer, count * sizeof(atom));
					_indexRead = count;

				}
			}

			return count;
		}

		inline long write(atom* buffer, long count)
		{
			long overflow;

			overflow = _size + count - _count;

			if (overflow > 0)
			{
				_indexRead = (_indexRead + overflow) % _count;
				_size -= overflow;
			}

			overflow = _indexWrite - _count;

			if (overflow < 0)
			{
				memcpy(&_buffer[_indexWrite], buffer, count * sizeof(atom));
				_indexWrite = (_indexWrite + count) % _count;
				_size += count;
			}
			else
			{
				memcpy(&_buffer[_indexWrite], buffer, overflow);

				count -= overflow;
				memcpy(_buffer, &buffer[overflow], count * sizeof(atom));
				_indexWrite = count;
			}
		}

	private:
		HR_DISABLE_COPY(circfifo)
		long     _count;        // max count
		long     _size;         // current valid size
		long     _indexRead;    // index of read
		long     _indexWrite;   // index of write
		atom*    _buffer;       // data buffer
	};
}

#endif	// _HR_CIRCULAR_FIFO_H_
