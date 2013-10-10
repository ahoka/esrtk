#ifndef REF_HH
#define REF_HH

template <class T>
class Ref
{
public:
   Ref(T& r) : reference(r)
   {
   }

   Ref(T&& r) = delete;

   T& get() const
   {
      return reference;
   }

   operator T&() const
   {
      return get();
   }

   Ref& operator=(const Ref& other)
   {
      reference = other.get();
   }

private:
   T& reference;
};

template <class T>
class RefPrimitive
{
public:
   RefPrimitive(T p) : primitive(p)
   {
   }

   RefPrimitive(T& r) = delete;

   RefPrimitive(T&& r) = delete;

   T get() const
   {
      return primitive;
   }

   operator T() const
   {
      return get();
   }

   RefPrimitive& operator=(const RefPrimitive& other)
   {
      primitive = other.get();
   }

private:
   T primitive;
};

#endif
