//
// Created by thomas on 03/10/17.
//

#ifndef UTILITIES_ARRAY2_HPP
#define UTILITIES_ARRAY2_HPP

#include <array>
#include <memory>
#include <algorithm>
#include <iterator>
#include "ptr_iterator.hpp"

namespace ut
{
    template<typename T>
    class array2_view;

    template<typename T>
    class array2
    {
        public:
            // Aliases for types
            using value_type = T;
            using pointer = T *;
            using const_pointer = const T *;
            using reference = T &;
            using const_reference = const T &;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

            using iterator = pointer_iterator_base<T, false>;
            using const_iterator = pointer_iterator_base<T, true>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            // friend class
            friend class array2_view<value_type>;

            // constructors
            array2() noexcept;
            array2(size_type a, size_type b);
            array2(size_type a, size_type b, std::initializer_list<value_type> data);
            explicit array2(std::array<size_type, 2> dim);
            array2(std::array<size_type, 2> dim, std::initializer_list<value_type> data);
            template<size_type N, size_type M>
            explicit array2(value_type (&array)[N][M]);
            explicit array2(const array2_view<value_type> &av);
            array2(const array2 &a);
            array2(array2 &&a) noexcept;
            ~array2() = default;

            // move & copy assigment
            array2 &operator=(const array2 &a);
            array2 &operator=(array2 &&a) noexcept;

            // accessors
            reference operator()(size_type i, size_type j) throw();
            const_reference operator()(size_type i, size_type j) const throw();

            array2_view<value_type> operator()(std::array<size_type, 2> i, std::array<size_type, 2> j) throw();
            const array2_view<value_type> operator()(std::array<size_type, 2> i, std::array<size_type, 2> j) const throw();

            // getters
            const std::array<size_type, 2> &dim() const noexcept { return dims_; }
            template<size_type N>
            const size_type dim() const noexcept { return std::get<N>(dims_); }
            const size_type dim(size_type n) const throw() { return (n < 2) ? dims_[n] : throw std::out_of_range(""); }
            pointer data() noexcept { return data_.get(); }
            const_pointer data() const noexcept { return data_.get(); }
            bool empty() const noexcept { return !(dims_[0] * dims_[1]); }

            // modifiers
            void resize(size_type i, size_type j);
            void resize(std::array<size_type, 2> dim);

            // iterators
            iterator begin() noexcept { return iterator(data_.get()); }
            const_iterator begin() const noexcept { return const_iterator(data_.get()); }
            iterator end() noexcept { return iterator(data_.get() + n_elems_); }
            const_iterator end() const noexcept { return const_iterator(data_.get() + n_elems_); }
            const_iterator cbegin() const noexcept { return const_iterator(data_.get()); }
            const_iterator cend() const noexcept { return const_iterator(data_.get() + n_elems_); }
            reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
            const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
            reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
            const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
            const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
            const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
            iterator iter(size_type i, size_type j) throw();
            const_iterator iter(size_type i, size_type j) const throw();
            const_iterator citer(size_type i, size_type j) const throw();
            reverse_iterator riter(size_type i, size_type j) throw();
            const_reverse_iterator riter(size_type i, size_type j) const throw();
            const_reverse_iterator criter(size_type i, size_type j) const throw();


        private:
            size_type n_elems_;
            std::array<size_type, 2> dims_;
            std::unique_ptr<value_type[]> data_;
    };

    template<typename T>
    array2<T>::array2() noexcept :
        n_elems_{0},
        dims_{std::array<size_type, 2>{0, 0}},
        data_{nullptr} {}

    template<typename T>
    array2<T>::array2(array2::size_type a, array2::size_type b) :
        n_elems_{a * b},
        dims_{std::array<size_type, 2>{a, b}},
        data_{new value_type[n_elems_]} {}

    template<typename T>
    array2<T>::array2(array2::size_type a, array2::size_type b, std::initializer_list<value_type> data) :
        array2(a, b)
    {
        std::copy_n(data.begin(), std::min(data.size(), n_elems_), data_.get());
    }

    template<typename T>
    array2<T>::array2(std::array<size_type, 2> dim) :
        n_elems_{dim[0] * dim[1]},
        dims_{std::move(dim)},
        data_{new value_type[n_elems_]} {}

    template<typename T>
    array2<T>::array2(std::array<size_type, 2> dim, std::initializer_list<value_type> data) :
        array2(dim)
    {
        std::copy_n(data.begin(), std::min(data.size(), n_elems_), data_.get());
    }

    template<typename T>
    template<typename array2<T>::size_type N, typename array2<T>::size_type M>
    array2<T>::array2(value_type (&array)[N][M]) :
        array2(N, M)
    {
        std::copy_n(&array[0][0], n_elems_, data_.get());
    }

    template<typename T>
    array2<T>::array2(const array2_view<T> &av) :
        array2(av.dims_)
    {
        std::copy_n(av.begin(), n_elems_, data_.get());
    }

    template<typename T>
    array2<T>::array2(const array2 &a) :
        array2(a.dims_)
    {
        std::copy_n(a.data_.get(), n_elems_, data_.get());
    }

    template<typename T>
    array2<T>::array2(array2 &&a) noexcept :
        n_elems_{std::move(a.n_elems_)},
        dims_{std::move(a.dims_)},
        data_{std::move(a.data_)} {}

    template<typename T>
    array2<T> &array2<T>::operator=(const array2 &a)
    {
        data_.reset(new value_type[a.n_elems_]);
        std::copy(a.begin(), a.end(), begin());
        n_elems_ = a.n_elems_;
        dims_ = a.dims_;

        return *this;
    }

    template<typename T>
    array2<T> &array2<T>::operator=(array2 &&a) noexcept
    {
        data_ = std::move(a.data_);
        n_elems_ = std::move(a.n_elems_);
        dims_ = std::move(a.dims_);

        return *this;
    }

    template<typename T>
    typename array2<T>::reference array2<T>::operator()(array2::size_type i, array2::size_type j) throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        return data_.get()[i * dims_[1] + j];
    }

    template<typename T>
    typename array2<T>::const_reference array2<T>::operator()(array2::size_type i, array2::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        return data_.get()[i * dims_[1] + j];
    }

    template<typename T>
    array2_view<T> array2<T>::operator()(std::array<size_type, 2> i, std::array<size_type, 2> j) throw()
    {
        if(i[0] > i[1] || j[0] > j[1] || i[0] >= dims_[0] || j[0] >= dims_[1])
            throw std::exception();
        return array2_view<value_type>(*this, i[0], j[0], std::min(i[1] + 1, dims_[0]) - i[0],
                                       std::min(j[1] + 1, dims_[1]) - j[0]);
    }

    template<typename T>
    const array2_view<T> array2<T>::operator()(std::array<size_type, 2> i, std::array<size_type, 2> j) const throw()
    {
        if(i[0] > i[1] || j[0] > j[1] || i[0] >= dims_[0] || j[0] >= dims_[1])
            throw std::exception();
        return array2_view<value_type>(const_cast<array2<T> &>(*this), i[0], j[0], std::min(i[1] + 1, dims_[0]) - i[0],
                                       std::min(j[1] + 1, dims_[1]) - j[0]);
    }

    template<typename T>
    void array2<T>::resize(size_type i, size_type j)
    {
        std::unique_ptr<value_type[]> temp(new value_type[i * j]);
        for(size_type k = 0; k < std::min(i, dims_[0]); ++k)
            for(size_type l = 0; l < std::min(j, dims_[1]); ++l)
                temp.get()[k * j + l] = data_.get()[k * dims_[1] + l];

        data_ = std::move(temp);
        dims_[0] = i;
        dims_[1] = j;
        n_elems_ = i * j;
    }

    template<typename T>
    void array2<T>::resize(std::array<size_type, 2> dim)
    {
        resize(dim[0], dim[1]);
    }

    template<typename T>
    typename array2<T>::iterator array2<T>::iter(array2::size_type i, array2::size_type j) throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        return array2<T>::iterator(&data_.get()[i * dims_[1] + j]);
    }

    template<typename T>
    typename array2<T>::const_iterator array2<T>::iter(array2::size_type i, array2::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        return array2<T>::const_iterator(&data_.get()[i * dims_[1] + j]);
    }

    template<typename T>
    typename array2<T>::const_iterator array2<T>::citer(array2::size_type i, array2::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        return array2<T>::const_iterator(&data_.get()[i * dims_[1] + j]);
    }

    template<typename T>
    typename array2<T>::reverse_iterator array2<T>::riter(array2::size_type i, array2::size_type j) throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        auto it = array2<T>::iterator(&data_.get()[i * dims_[1] + j]);
        return array2<T>::reverse_iterator(++it);
    }

    template<typename T>
    typename array2<T>::const_reverse_iterator array2<T>::riter(array2::size_type i, array2::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        auto it = array2<T>::const_iterator(&data_.get()[i * dims_[1] + j]);
        return array2<T>::const_reverse_iterator(++it);
    }

    template<typename T>
    typename array2<T>::const_reverse_iterator array2<T>::criter(array2::size_type i, array2::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        auto it = array2<T>::const_iterator(&data_.get()[i * dims_[1] + j]);
        return array2<T>::const_reverse_iterator(++it);
    }


    template<typename T, bool is_const>
    struct array2_view_iterator_base;

    template<typename T>
    class array2_view
    {
        public:
            // Aliases
            using value_type = T;
            using pointer = T *;
            using const_pointer = const T *;
            using reference = T &;
            using const_reference = const T &;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

            using iterator = array2_view_iterator_base<T, false>;
            using const_iterator = array2_view_iterator_base<T, true>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            // friend class
            friend class array2<value_type>;
            friend class array2_view_iterator_base<value_type, false>;
            friend class array2_view_iterator_base<value_type, true>;

            array2_view(array2<value_type> &array) noexcept :
                array_{array}, offset_{{0, 0}}, dims_{array.dims_} {}
            array2_view(array2<value_type> &array, size_type offset_y, size_type offset_x) noexcept :
                array_{array}, offset_{{offset_y, offset_x}}, dims_{{array.dim(0) - offset_y, array.dim(1) - offset_x}} {}
            array2_view(array2<value_type> &array, size_type offset_y, size_type offset_x, size_type size_y,
                        size_type size_x) noexcept :
                array_{array}, offset_{{offset_y, offset_x}},
                dims_{{std::min(array.dim(0) - offset_[0], size_y), std::min(array.dim(1) - offset_[1], size_x)}} {}

            array2_view &operator=(const array2_view &lhs) throw();
            array2_view &operator=(const array2<value_type> &lhs) throw();

            // accessors
            reference operator()(size_type i, size_type j) throw();
            const_reference operator()(size_type i, size_type j) const throw();

            array2_view operator()(std::array<size_type, 2> i, std::array<size_type, 2> j) { return array_(i, j); }
            const array2_view operator()(std::array<size_type, 2> i, std::array<size_type, 2> j) const { return array_(i, j); }

            //getters
            const std::array<size_type, 2> &dim() const noexcept { return dims_; }

            template<size_type N>
            const size_type dim() const noexcept { return std::get<N>(dims_); }

            const size_type dim(size_type n) const throw() { return (n < 2) ? dims_[n] : throw std::out_of_range(""); }

            pointer data() noexcept { return array_.data(); }

            const_pointer data() const noexcept { return array_.data(); }

            bool empty() const noexcept { return !(dims_[0] && dims_[1]); }

            // iterators
            iterator begin() noexcept { return iterator(this); }

            const_iterator begin() const noexcept { return const_iterator(this); }

            const_iterator cbegin() const noexcept { return const_iterator(this); }

            iterator end() noexcept
            {
                if(empty()) return begin();
                iterator it(this, dims_[0] - 1, dims_[1] - 1);
                return ++it;
            }

            const_iterator end() const noexcept
            {
                if(empty()) return begin();
                const_iterator it(this, dims_[0] - 1, dims_[1] - 1);
                return ++it;
            }

            const_iterator cend() const noexcept
            {
                if(empty()) return begin();
                const_iterator it(this, dims_[0] - 1, dims_[1] - 1);
                return ++it;
            }

            reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

            const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }

            reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

            const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }

            const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

            const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

            iterator iter(size_type i, size_type j) throw();

            const_iterator iter(size_type i, size_type j) const throw();

            const_iterator citer(size_type i, size_type j) const throw();

            reverse_iterator riter(size_type i, size_type j) throw();

            const_reverse_iterator riter(size_type i, size_type j) const throw();

            const_reverse_iterator criter(size_type i, size_type j) const throw();

        private:
            array2<value_type>& array_;
            std::array<size_type, 2> offset_;
            std::array<size_type, 2> dims_;
    };

    template<typename T>
    array2_view<T> &array2_view<T>::operator=(const array2_view<T> &lhs) throw()
    {
        if(dim() != lhs.dim())
            throw std::exception();
        std::copy(lhs.begin(), lhs.end(), begin());

        return *this;
    }

    template<typename T>
    array2_view<T> &array2_view<T>::operator=(const array2<T> &lhs) throw()
    {
        if(dim() != lhs.dim())
            throw std::exception();
        std::copy(lhs.begin(), lhs.end(), begin());

        return *this;
    }

    template<typename T>
    typename array2_view<T>::reference
    array2_view<T>::operator()(array2_view::size_type i, array2_view::size_type j) throw()
    {
        if(dims_[0] < i || dims_[1] < j)
            throw std::out_of_range("");
        return array_(i + offset_[0], j + offset_[1]);
    }

    template<typename T>
    typename array2_view<T>::const_reference
    array2_view<T>::operator()(array2_view::size_type i, array2_view::size_type j) const throw()
    {
        if(dims_[0] < i || dims_[1] < j)
            throw std::out_of_range("");
        return array_(i + offset_[0], j + offset_[1]);
    }

    template<typename T>
    typename array2_view<T>::iterator array2_view<T>::iter(array2_view::size_type i, array2_view::size_type j) throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        return array2_view<T>::iterator(this, i, j);
    }

    template<typename T>
    typename array2_view<T>::const_iterator
    array2_view<T>::iter(array2_view::size_type i, array2_view::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        return array2_view<T>::const_iterator(this, i, j);
    }

    template<typename T>
    typename array2_view<T>::const_iterator
    array2_view<T>::citer(array2_view::size_type i, array2_view::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        return array2_view<T>::const_iterator(this, i, j);
    }

    template<typename T>
    typename array2_view<T>::reverse_iterator
    array2_view<T>::riter(array2_view::size_type i, array2_view::size_type j) throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");;
        auto it = array2_view<T>::iterator(this, i, j);
        return array2_view<T>::reverse_iterator(++it);
    }

    template<typename T>
    typename array2_view<T>::const_reverse_iterator
    array2_view<T>::riter(array2_view::size_type i, array2_view::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");;
        auto it = array2_view<T>::const_iterator(this, i, j);
        return array2_view<T>::const_reverse_iterator(++it);
    }

    template<typename T>
    typename array2_view<T>::const_reverse_iterator
    array2_view<T>::criter(array2_view::size_type i, array2_view::size_type j) const throw()
    {
        if(i >= dims_[0] || j >= dims_[1])
            throw std::out_of_range("");
        auto it = array2_view<T>::const_iterator(this, i, j);
        return array2_view<T>::const_reverse_iterator(++it);
    }


    template<typename T, bool is_const = false>
    struct array2_view_iterator_base
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = typename choose<is_const, const T *, T *>::type;
        using reference = typename choose<is_const, const T &, T &>::type;
        using difference_type = std::ptrdiff_t;

        using array2_pointer = typename choose<is_const, const array2_view<value_type> *, array2_view<value_type> *>::type;

        explicit array2_view_iterator_base(array2_pointer av = nullptr) noexcept : av_{av}, i_{0}, j_{0} {}

        array2_view_iterator_base(array2_pointer av, size_t i, size_t j) noexcept : av_{av}, i_{i}, j_{j} {}

        array2_view_iterator_base(const array2_view_iterator_base<value_type, false> &it) noexcept : av_{it.av_},
                                                                                                     i_{it.i_},
                                                                                                     j_{it.j_} {}


        // operators
        bool operator==(const array2_view_iterator_base<T, true> other) const noexcept
        {
            if(av_ == other.av_ && av_ == nullptr)
                return true;
            else if(av_->data() == other.av_->data() && av_->dims_ == other.av_->dims_ &&
                    av_->offset_ == other.av_->offset_)
                return i_ == other.i_ && j_ == other.j_;
            return false;
        }

        bool operator!=(const array2_view_iterator_base<T, true> other) const noexcept { return !(*this == other); }

        reference operator*() const { return (*av_)(i_, j_); }

        pointer operator->() const { return &(*av_)(i_, j_); }

        array2_view_iterator_base &operator++() noexcept
        {
            ++j_;
            if(j_ >= av_->dim(1))
            {
                j_ = 0;
                ++i_;
            }
            return *this;
        }

        array2_view_iterator_base operator++(int) noexcept
        {
            auto temp(*this);
            ++(*this);
            return temp;
        }

        array2_view_iterator_base &operator--() noexcept
        {
            if(j_ == 0)
            {
                j_ = av_->dim(1) - 1;
                --i_;
            }
            else
                --j_;
            return *this;
        }

        array2_view_iterator_base operator--(int) noexcept
        {
            auto temp(*this);
            --(*this);
            return temp;
        }


        array2_pointer av_ = nullptr;
        size_t i_, j_;
    };
};
#endif //UTILITIES_ARRAY2_HPP
