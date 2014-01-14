/**
 * @file   Variant.h
 * @author Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef VARIANT_H_
#define VARIANT_H_

#include <cstddef>
#include <type_traits>
#include <typeinfo>
#include <utility>

/**
 * Gets the size of the largest type in the specified list of types.
 */
template<typename ...Ts> struct max_size;

template<typename T, typename ...Ts>
struct max_size<T, Ts...>
{
private:
    static const std::size_t headSize = sizeof(T);
    static const std::size_t tailSize = max_size<Ts...>::value;

public:
    static const std::size_t value = (headSize > tailSize ? headSize : tailSize);
};

template<typename T>
struct max_size<T>
{
    static const std::size_t value = sizeof(T);
};

/**
 * Gets the size of the smallest type in the specified list of types.
 */
template<typename ...Ts> struct min_size;

template<typename T, typename ...Ts>
struct min_size<T, Ts...>
{
private:
    static const std::size_t headSize = sizeof(T);
    static const std::size_t tailSize = min_size<Ts...>::value;

public:
    static const std::size_t value = (headSize > tailSize ? tailSize : headSize);
};

template<typename T>
struct min_size<T>
{
    static const std::size_t value = sizeof(T);
};

/**
 * Typedefs T1 if third template parameter is true otherwise T2 is set.
 */
template<typename T1, typename T2, bool> struct type_if;

template<typename T1, typename T2>
struct type_if<T1, T2, true>
{
    typedef T1 type;
};

template<typename T1, typename T2>
struct type_if<T1, T2, false>
{
    typedef T2 type;
};

/**
 * Gets the smallest type from the specified type list.
 */
template<typename ...Ts> struct min_type;

template<typename T, typename ...Ts>
struct min_type<T, Ts...>
{
private:
    typedef T T1;
    typedef typename min_type<Ts...>::type T2;

public:
    typedef typename type_if<T1, T2, sizeof(T1) <= sizeof(T2)>::type type;
};

template<typename T>
struct min_type<T>
{
    typedef T type;
};

template<int, typename T, typename ...Ts> struct index_of_;

template<int n, typename T, typename U, typename ...Ts>
struct index_of_<n, T, U, Ts...>
{
    static const int value = index_of_<n + 1, T, Ts...>::value;
};

template<int n, typename T, typename ...Ts>
struct index_of_<n, T, T, Ts...>
{
    static const int value = n;
};

template<int n, typename T>
struct index_of_<n, T, T>
{
  static const int value = n;
};

template<int n, typename T, typename U>
struct index_of_<n, T, U>
{
    static const int value = -1;
};

/**
 * Gets index of specified type in the type list.
 */
template<typename T, typename ...Ts>
struct index_of
{
    static const int value = index_of_<0, T, Ts...>::value;
};

template<std::size_t n, std::size_t i, typename ...Ts>
struct type_at_
{
    static_assert(sizeof...(Ts) >= (n + i),
                  "Index out of range of the specified type list.");
};

template<std::size_t n, std::size_t i, typename T, typename ...Ts>
struct type_at_<n, i, T, Ts...>
{
    typedef typename type_at_<n, i + 1, Ts...>::type type;
};

template<std::size_t n, typename T, typename ...Ts>
struct type_at_<n, n, T, Ts...>
{
    typedef T type;
};

/**
 * Gets type at specified index in the type list.
 */
template<std::size_t n, typename ...Ts>
struct type_at
{
    static_assert(sizeof...(Ts) >= n,
                  "Index out of range of the specified type list.");

    typedef typename type_at_<n, 0, Ts...>::type type;
};

template<typename ...Ts> class Visitor;

template<typename T, typename ...Ts>
class Visitor<T, Ts...> : public Visitor<Ts...>
{
public:
    virtual ~Visitor() { }

    using Visitor<Ts...>::visit;

    virtual void visit(T&) =0;
};

template<typename T>
class Visitor<T>
{
public:
    virtual ~Visitor() { }

    virtual void visit(T&) =0;
};

/// @todo Investigate if it would be worth to implement move semantics for
///       this class.
template<typename ...Ts>
class Variant
{
private:
    static_assert(sizeof...(Ts) > 0, "Variant with no types is not allowed.");

    typedef int TypeIndex;

public:
    /**
     * Creates the variant prepared for storing a value of the first type from
     * the type list.
     */
    Variant()
    {
        typedef typename type_at<0, Ts...>::type T;
        new (&m_storage.__data[0]) T();

        m_typeIndex = 0;
        initVTable<T>();
    }

    template<typename T>
    explicit Variant(const T& value)
    {
        const TypeIndex index = index_of<T, Ts...>::value;
        static_assert(index != -1, "Type not storable by this variant.");

        m_typeIndex = index;

        new (&m_storage.__data[0]) T(value);
        initVTable<T>();
    }

    ~Variant()
    {
        (m_vtbl.destroy)(*this);
    }

    /**
     * Sets the value of the variant.
     * @param value Value to set.
     * @remarks Convenience method.
     */
    template<typename T>
    void operator=(const T& value)
    {
        set(value);
    }

    /**
     * Gets value of specified type.
     * @return Stored value.
     * @throw std::bad_cast If stored values is not of requested type.
     */
    template<typename T>
    const T& get() const
    {
        const TypeIndex index = index_of<T, Ts...>::value;
        static_assert(index != -1, "Type not storable by this variant.");

        /// @remarks This could be moved to vtable; however, it saves (?)
        ///          one level of indirection.
        if (m_typeIndex != index)
        {
            throw std::bad_cast();
        }

        return *reinterpret_cast<const T*>(&m_storage.__data[0]);
    }

    /**
     * Sets value.
     * @param value Value to set.
     */
    template<typename T>
    void set(const T& value)
    {
        const TypeIndex index = index_of<T, Ts...>::value;
        static_assert(index != -1, "Type not storable by this variant.");

        (m_vtbl.destroy)(*this);
        new (&m_storage.__data[0]) T(value);

        if (m_typeIndex != index)
        {
            m_typeIndex = index;
            initVTable<T>();
        }
    }

    /**
     * Applies a visitor on the variant.
     * @param visitor Visitor to apply.
     * @remarks Visitor class must inherit after @see class Visitor.
     */
    void apply(Visitor<Ts...>& visitor)
    {
        (m_vtbl.apply)(*this, visitor);
    }

private:
    /// @todo The alignment could most probably be more efficient.
    /// @see aligned_storage
    typedef typename std::aligned_storage<max_size<Ts...>::value>::type AlignedStorage;

    template<typename T>
    struct VTableImpl
    {
        static void destroy(const Variant& operand)
        {
            const T& data = getStoredData(operand);
            data.~T();
        }

        static void apply(Variant& operand, Visitor<Ts...>& visitor)
        {
            T& data = getStoredData(operand);
            visitor.visit(data);
        }

    private:
        static T& getStoredData(const Variant& operand)
        {
            return *const_cast<T*>(reinterpret_cast<const T*>(&operand.m_storage.__data[0]));
        }
    };

    struct VTable
    {
        void (*destroy)(const Variant&);
        void (*apply)(Variant&, Visitor<Ts...>&);
    };

    template<typename T>
    void initVTable()
    {
        m_vtbl.destroy = &VTableImpl<T>::destroy;
        m_vtbl.apply = &VTableImpl<T>::apply;
    }

    AlignedStorage m_storage;
    TypeIndex m_typeIndex;
    VTable m_vtbl;
};

#endif
