// [src_rely] allinall_block.cpp
#ifndef AIA_BLOCK
#include<typeinfo>
#include<initializer_list>
#include<tuple>
#include<set>
using size_t=std::size_t;


namespace AIA{


template<class T> class block;
typedef block<char> blockstr;
typedef block<blockstr> wordGroup;


typedef size_t OffsetByte;


// attribute, just environment inuse
template<class... Ts>
class attribute{
public:
	typedef typename std::tuple<Ts...> Items;
protected:
	Items item;

public:
	attribute()=default;
	template<class... Rs>
	attribute(Rs&&...);
	template<size_t I>
	typename std::tuple_element<I,std::tuple<Ts...>>::type& attrib();
	template<size_t I>
	const typename std::tuple_element<I,std::tuple<Ts...>>::type& attrib() const;

};


#define Maxlife 1024




extern void GrowMethoud1(size_t&obj,size_t to,size_t max,size_t arg1,size_t arg2);




// block| can see as a week vector
template<class T>
class block{
	friend class fileBox;
protected:
	T* p=nullptr;
	size_t useSize=0;
	size_t capacity=0;

	unsigned char age=0;
	const unsigned char maxlife;
	
	static const size_t maxsize=1024*1024*1024;
	block* const TFtemp=nullptr;// data race

public:
	block(size_t=0, size_t=Maxlife);
	block(const T*, size_t, size_t=Maxlife);
	block(const block&, size_t=size_t(-1), size_t=Maxlife);
	block(block&&, size_t=Maxlife);
	~block();

	block& operator=(const block&);
	block& operator=(block&&);

	T& operator[](size_t);

	int preview(size_t);
	int alloc(size_t);
	void clear();
	size_t grow(size_t);
	size_t shrink(size_t);

	void swap(block&);
	void takein(T*,size_t,size_t=0);// src should be inited by new, after, ptr will be an empty ptr
	template<class S,class... Ts> block& assign(size_t,S&&,Ts&&...);
	// pre-kind will grow size or stay, and should alian as T
	// the aft-kind will just stay even content is overflow
	// and will drop the frag which piece in outside
	// all of them will keep the no overwriten contents
	size_t chgsizeTake(size_t insertpos,const T*,size_t endind,size_t=0);
	size_t chgsizeTake(size_t insertpos,const block&,size_t=size_t(-1),size_t=0);
	size_t kepsizeTake(size_t insertpos,const T*,size_t endind,size_t=0);
	size_t kepsizeTake(size_t insertpos,const block&,size_t=size_t(-1),size_t=0);
	size_t insert(size_t,const T*,size_t,size_t=0);
	size_t insert(size_t,const block&,size_t=size_t(-1),size_t=0);
	size_t remove(size_t=0,size_t=size_t(-1));

	// flexible ptr & size
	T* ptr(size_t=0);
	template<class S> S* ptras(size_t=0);

	// const part
	const T& operator[](size_t) const;
	bool operator==(const block&) const;

	const T* ptr(size_t=0) const;
	template<class S> const S* ptras(size_t=0) const;
	size_t size() const;
	template<class S> size_t sizeas() const;
	operator bool() const;

};




template<class T=size_t,class R,class S>
void copyContent(R*obj,const S*src,size_t databytelenth){
	size_t lenth=databytelenth/sizeof(T);
	size_t leftblocklenth=databytelenth%sizeof(T);
	T*to=(T*)obj;
	T*from=(T*)src;
	while(--leftblocklenth!=size_t(-1))
		*(((char*)(to+lenth))+leftblocklenth)=*(((char*)(from+lenth))+leftblocklenth);
	while(--lenth!=size_t(-1))
		to[lenth]=from[lenth];
}


#define Maxbytesize 1024*1024*1024





#ifdef WAITING


/******** mem outside *********/

// Xblock
template<class T>
class Xblock{
	template<class S> friend class Xblock;
protected:
	char* p=nullptr;
	size_t bytesize=0;
	int age=0;
	const int maxlife;
	const size_t maxbytesize;

public:
	Xblock(size_t=0, size_t=Maxlife, size_t=Maxbytesize);
	template<class S> Xblock(S*&&, size_t, size_t=Maxlife, size_t=Maxbytesize);
	template<class S> Xblock(const S*, size_t, size_t=Maxlife, size_t=Maxbytesize);
	Xblock(const Xblock&, size_t=size_t(-1), size_t=Maxlife, size_t=Maxbytesize);
	Xblock(Xblock&&, size_t=Maxlife, size_t=Maxbytesize);
	template<class S> Xblock(const Xblock<S>&, size_t=size_t(-1), size_t=Maxlife, size_t=Maxbytesize);
	template<class S> Xblock(Xblock<S>&&, size_t=Maxlife, size_t=Maxbytesize);
	~Xblock();

	void clear();
	void alloc(size_t);// if already have the space: equal-nothing; else judge by age
	void regetnew(size_t);// totally realloc execpt space=size
	template<class S> void regetnewas(size_t);// same as above, should alian as T

	template<class S,class... Ts> Xblock& assign(size_t,S&&,Ts&&...);
	template<class S> void takein(S*&&,size_t);// src should be inited by new, but after, this is an empty ptr
	template<class S> void swap(Xblock<S>&);
	Xblock& operator=(const Xblock&) =delete;
	Xblock& operator=(Xblock&&) =delete;
	template<class S> Xblock& operator=(const Xblock<S>&);// same size, same content
	template<class S> Xblock& operator=(Xblock<S>&&);

	T& operator[](size_t);
	const T& operator[](size_t) const;

	bool operator==(const Xblock&) const;

	// pre-kind will grow size or stay, and should alian as T
	// the aft-kind will just stay even content is overflow
	// and will drop the frag which piece in outside
	// all of them will keep the no overwriten contents
	template<class S> Xblock& chgsizeTake(size_t insertpos,const S*,size_t endind,size_t=0);
	template<class S> Xblock& chgsizeTake(size_t insertpos,const Xblock<S>&,size_t=size_t(-1),size_t=0);
	template<class S> Xblock& kepsizeTake(size_t insertpos,const S*,size_t endind,size_t=0);
	template<class S> Xblock& kepsizeTake(size_t insertpos,const Xblock<S>&,size_t=size_t(-1),size_t=0);

	// flexible ptr & size
	template<class S> const S* ptras(size_t=0) const;
	template<class S> S* ptras(size_t=0);
	const T* ptr(size_t=0) const;
	T* ptr(size_t=0);
	size_t size() const;
	template<class S> size_t sizeas() const;

	operator bool() const;

};


#endif


}// namespace AIA

#include"inl/allinall_block.inl"

#define AIA_BLOCK
#endif