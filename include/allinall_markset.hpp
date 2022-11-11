// [src_rely] allinall_markset.cpp
#ifndef AIA_MARKSET
#include<typeinfo>
#include<set>
#include<cstdio>
#ifdef AIA_LINK_MARKSET_WITH_BLOCK
#include"allinall_block.hpp"
#else
using size_t=std::size_t;
#endif

extern void aiatest();
namespace AIA{




/*_________PRE DEFINE_______*/
// old AIAstr
#define AIA_OVERFLOW size_t(-1)
// originstr
#define dftstr "none"
#define Maxlife_str 10
// originid
constexpr bool endianlittle(){
	union{int i=1;char c[sizeof(int)];};
	return c[0];
}
constexpr int blonpos(){
	return endianlittle()?int(sizeof(size_t)-1):0;
}
// signal
#define signal_querry '#'
// mark
#define note_(note,area) mark note{#note}
#define snote_(note,area) smark note{#note}
/*_________TYPE DEFINE_________*/
// originid
typedef char BLon;
/*_________CLASS  LIST_________*/
class originstr;
class originid;

template<class,bool> class Mark;
class formark;
template<class T> using Gmark=Mark<T,true>;
template<class T> using Gsmark=Mark<T,false>;
typedef Mark<formark,true> mark;
typedef Mark<formark,false> smark;

template<class,bool> class localSet;
template<class T> using GmarkSet=localSet<Gmark<T>,false>;
template<class T> using GmarkSetAuto=localSet<Gmark<T>,true>;
typedef localSet<mark,false> markSet;
typedef localSet<mark,true> markSetAuto;
// template<class T> class GmarkSet;
// typedef must asalias a already define type
/*_________FUNC   LIST_________*/
// oringinstr
extern size_t aslen(size_t len);
extern size_t aslen(const char*str);
extern size_t content(size_t i,const char*str);
// oringinid
extern BLon takeBLon(size_t);
extern void setSignal(originstr&);
template<class... Ts>
void print(Ts&&... tgt){
	char temp[]={( tgt.print() ,0)...,0};
}




/*_________CONTENT_____________*/
// originstr
/*
originstr="nihao"

originstr.c_str()
originstr.lenth()

(const char*)originstr
(bool)originstr//test if initialized
*/
class originstr{
protected:
#	ifdef AIA_LINK_STRPOOL_WITH_BASETYPE
	block<char> line;
#	else
	union{size_t*as=nullptr;char*str;};
	size_t length=0;
	size_t capacity=0;

	size_t age=0;
	const size_t maxlife;
#	endif
public:
	void alloc(size_t);
	void reset();

	originstr(size_t maxlife=Maxlife_str);
	originstr(const char*,size_t maxlife=Maxlife_str);
	originstr(const originstr&,size_t maxlife=Maxlife_str);
	originstr(originstr&&,size_t maxlife=Maxlife_str);
	~originstr();

	originstr& operator=(const char*);
	originstr& operator=(const originstr&);
	originstr& operator=(originstr&&);
	template<class... Ts>
	originstr& combineStr(Ts...);

	bool operator==(const originstr&) const;
	bool operator!=(const originstr&) const;
	bool operator<(const originstr&) const;
	bool operator==(const char*) const;
	bool operator!=(const char*) const;

	bool validStr() const;

	explicit operator const char*() const;
	const char* c_str() const;
	operator bool() const;
	size_t lenth() const;
};




// originid
/*
originid=602// this will cover inBLon(follow)
originid.inBLon=7
originid.setIndex(602)

originid.index()
originid.ID()//full

(size_t)originid
(bool)originid//==0
*/
class originid{
protected:
	union{size_t num=0;BLon pos[sizeof(size_t)];};
public:
	BLon&inBLon=pos[blonpos()];

public:
	void reset();

	originid(BLon=0);
	originid(size_t);
	originid(int);
	originid(const originid&);

	void setIndex(size_t);
	originid& operator=(size_t);
	originid& operator=(const originid&);

	bool operator==(const originid&) const;
	bool operator!=(const originid&) const;
	bool operator<(const originid&) const;
	bool operator==(size_t) const;
	bool operator!=(size_t) const;

	bool validID(size_t =0) const;

	explicit operator size_t() const;
	operator bool() const;
	size_t ID() const;
	size_t index() const;
};




// Gmark|mark as Gmark<formark>
#define MARK_TEMPLATE template<class T,bool is_link>
#define MARK Mark<T,is_link>
#define OTHER_MARK_TEMPLATE template<class W,bool _is_link>
#define OTHER_MARK Mark<W,_is_link>

/*
Gmark="nihao"
Gmark=602// this will cover inBLon
Gmark.inBLon=7
Gmark.setIndex(602)

Gmark.c_str()
Gmark.index()
Gmark.ID()

(const char*)Gmark
(size_t)Gmark
(bool)Gmark//str&id all initialized

Gmark.Pag
*/
MARK_TEMPLATE
class Mark:public originstr,public originid{
	friend void ::aiatest();
	template<class,bool> friend class Mark;
	template<class,bool> friend class localSet;

public:
	const void* from=nullptr;// use to record the pre bind
	bool link=is_link;
	bool linked() const;
	void unlink();

	mutable T Pag;

	void reset();
	// pre with G, releate with Pag
	// but Pag should define no arg ctor
	// because i'm low, just devide Pag's ctor with here
	Mark() =default;
	Mark(const char*, size_t=0);
	Mark(const char*, int);
	Mark(size_t, const char* =nullptr);
	Mark(int, const char* =nullptr);
	Mark(const char*, BLon);
	Mark(BLon, const char* =nullptr);
	Mark(const Mark&);
	Mark(Mark&&);
	OTHER_MARK_TEMPLATE Mark(const OTHER_MARK&);
	OTHER_MARK_TEMPLATE Mark(OTHER_MARK&&);
	~Mark();

	Mark& operator=(size_t);
	Mark& operator=(const char*);
	Mark& operator=(const originid&);
	Mark& operator=(const originstr&);
	Mark& operator=(originstr&&);
	Mark& operator=(const Mark&);
	Mark& operator=(Mark&&);
	OTHER_MARK_TEMPLATE Mark& operator=(const OTHER_MARK&);
	OTHER_MARK_TEMPLATE Mark& operator=(OTHER_MARK&&);
	// just assign str & id
	Mark& assignMark(const mark&);
	Mark& moveMark(mark&&);

	using originstr::operator==;
	using originstr::operator!=;
	using originid::operator==;
	using originid::operator!=;
	OTHER_MARK_TEMPLATE bool operator==(const OTHER_MARK&) const;
	OTHER_MARK_TEMPLATE bool operator!=(const OTHER_MARK&) const;
	bool operator<(const Mark&) const;
	// for conveniant
	bool equalMark(const mark&) const;

	template<bool =false> bool valid() const;

	operator mark&&();
	operator mark&();
	operator mark&() const;
	explicit operator const char*() const;
	explicit operator size_t() const;
	operator bool() const;
	void print(FILE* =stdout) const;

	mark& getMark() const;

};
class formark{};


// Gmark's Pag interface
// at least operator=
#define PagNeed1(A) A& operator=(const A&)
#define PagNeed2(A) A& operator=(A&&)
#define PagNeed3(A) bool operator==(const A&) const








// localSet
/*
localSet.insert(Gmark...)
localSet.erase(Gmark<same str & id or just one exist>)
localSet.clear()

localSet.find(Gmark)
localSet[Gmark]
localSet.begin()
localSet.end()

localSet.setIgnum(ind)// which index won't add/use/...

localSet.enchant(Gmark)

localSet.ID()// value as Gmark.inBLon
localSet.size()
*/
#define LOCALSET_TEMPLATE template<class T,bool is_auto>
#define LOCALSET localSet<T,is_auto>

LOCALSET_TEMPLATE// mark or Gmark<X>
class localSet:public std::set<T>{
	template<class S,bool _is_auto> friend class localSet;
	friend class GLCommunicator;
	friend class DataStation;


	const BLon id;
	const size_t ignum=0;

protected:
	size_t added=0;

	void setBLon(BLon);
	void setIgnum(size_t);

public:
	localSet(BLon=0);
	template<class... Ts>
	localSet(BLon,Ts&&...);
	localSet(BLon,const localSet&);
	localSet(BLon,localSet&&);
	localSet(const localSet&);
	localSet(localSet&&);

	template<class S,bool _is_auto>
	localSet(BLon,const localSet<S,_is_auto>&);
	template<class S,bool _is_auto>
	localSet(BLon,localSet<S,_is_auto>&&);
	template<class S,bool _is_auto>
	localSet(const localSet<S,_is_auto>&);
	template<class S,bool _is_auto>
	localSet(localSet<S,_is_auto>&&);

	localSet& operator=(const localSet&);
	localSet& operator=(localSet&&);

	template<class S,bool _is_auto>
	localSet& copyFrom(const localSet<S,_is_auto>&);
	template<class S,bool _is_auto>
	localSet& moveFrom(localSet<S,_is_auto>&&);

	std::pair<iterator,bool> insert(mark&);
	virtual std::pair<iterator,bool> insert(const mark&);
	virtual std::pair<iterator,bool> insert(mark&&);
	template<class S> std::pair<iterator,bool> insert(S&&);
	template<class... Ts> size_t insert(Ts&&...);

	virtual size_t erase(const mark&);
	template<class S> size_t erase(const S&);
	template<class... Ts> size_t erase(Ts&&...);
	virtual void clear();

	const iterator findIndex(const mark&,bool=false) const;
	const iterator findStr(const mark&,bool=true) const;
	const iterator find(mark&,bool=true,bool=true,bool=true) const;
	const iterator find(const mark&,bool=true) const;
	template<class S> const iterator find(S&&) const;

	template<class S> typename decltype(T::Pag)& operator[](S&&);
	BLon ID() const;
	void print(FILE* =stdout);

	template<class S> S&& enchantID(S&&) const;
	template<class S> S&& enchant(S&&) const;
	template<class... Ts> void enchantID(Ts&&...) const;
	template<class... Ts> void enchant(Ts&&...) const;

};




}// namespace AIA


#include"inl/allinall_markset.inl"

#define AIA_MARKSET
#endif