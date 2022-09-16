#ifndef AIA_STRPOOL
#include<typeinfo>
#include<set>

namespace AIA{


/*_________PRE DEFINE_______*/
// old AIAstr
#define AIA_OVERFLOW size_t(-1)
// originstr
#define dftstr "none"
#define maxlife 10
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
/*_________TYPE DEFINE_________*/
// originid
typedef char BLon;
/*_________CLASS  LIST_________*/
class originstr;
class originid;
class mark;
class Gpag;
template<class T> class Gmark;
typedef Gmark<int> Gmarki;
template<class T> class localSet;
typedef localSet<mark> markSet;
// typedef must asalias a already define type
template<class T> class GmarkSet;
typedef GmarkSet<int> GmarkSeti;
/*_________FUNC   LIST_________*/
// oringinid
extern BLon takeBLon(size_t);
extern void setSignal(originstr&);

/*_________CONTENT_____________*/
// originstr
class originstr{
	friend class mark;
protected:
	union{size_t*as;char*str;};
	size_t length,capacity=0;

	size_t count=0;
	const size_t life;
public:
	void alloc(size_t);
	void reset();

	originstr(size_t life=maxlife);
	originstr(const char*,size_t life=maxlife);
	originstr(const originstr&,size_t life=maxlife);
	originstr(originstr&&,size_t life=maxlife);
	~originstr();

	originstr& operator=(const char*);
	originstr& operator=(const originstr&);
	originstr& operator=(originstr&&);

	bool operator==(const originstr&) const;
	bool operator!=(const originstr&) const;
	bool operator<(const originstr&) const;
	bool operator==(const char*) const;
	bool operator!=(const char*) const;

	operator const char*() const;
	const char* c_str() const;
	operator bool() const;
	size_t lenth() const;
};




// originid
class originid{
	friend class mark;
public:
	union{size_t num;BLon pos[sizeof(size_t)];};
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

	operator size_t() const;
	operator bool() const;
	size_t ID() const;
	size_t index() const;
};




// mark
class mark:public originstr,public originid{
public:
	void reset();

public:
	mark();
	mark(const char*, size_t=0);
	mark(const char*, int);
	mark(size_t, const char* =nullptr);
	mark(int, const char* =nullptr);
	mark(const char*, BLon);
	mark(BLon, const char* =nullptr);
	mark(const mark&);
	mark(mark&&);
	~mark();

	mark& operator=(size_t);
	mark& operator=(const char*);
	mark& operator=(const originid&);
	mark& operator=(const originstr&);
	mark& operator=(const mark&);
	mark& operator=(originstr&&);
	mark& operator=(mark&&);

	using originstr::operator==;
	using originstr::operator!=;
	using originid::operator==;
	using originid::operator!=;
	bool operator==(const mark&) const;
	bool operator!=(const mark&) const;
	bool operator<(const mark&) const;

	operator const char*() const;
	operator size_t() const;
	operator bool() const;

};


template<class T>
class Gmark:public mark{
	friend class Gmark;
public:
	T Pag;

public:
	// pre with G, releate with Pag
	// but Pag should define no arg ctor
	// because i'm low, just devide Pag's ctor with here
	Gmark();
	Gmark(const char*, size_t=0);
	Gmark(const char*, int);
	Gmark(size_t, const char* =nullptr);
	Gmark(int, const char* =nullptr);
	Gmark(const char*, BLon);
	Gmark(BLon, const char* =nullptr);
	Gmark(const mark&);
	Gmark(mark&&);
	Gmark(const Gmark&);
	Gmark(Gmark&&);
	~Gmark();

	Gmark& operator=(size_t);
	Gmark& operator=(const char*);
	Gmark& operator=(const originid&);
	Gmark& operator=(const originstr&);
	Gmark& operator=(originstr&&);
	Gmark& operator=(mark&&);
	Gmark& operator=(const mark&);
	Gmark& operator=(const Gmark&);
	Gmark& operator=(Gmark&&);
	// for conveniant
	template<class W> Gmark& assignMark(const Gmark<W>&);
	template<class W> Gmark& moveMark(Gmark<W>&&);

	using mark::operator==;
	using mark::operator!=;
	template<class W> bool operator==(const Gmark<W>&) const;
	template<class W> bool operator!=(const Gmark<W>&) const;
	// for conveniant
	template<class W> bool equalMark(const Gmark<W>&) const;

	operator const char*() const;
	operator size_t() const;
	operator bool() const;
	T& pag() const;

	mark& getMark();

private:
	mark& getMark() const;
};




class Gpag{
	virtual Gpag& operator=(const Gpag&)=0;
	virtual Gpag& operator=(Gpag&&)=0;
};

#define Pagy auto





template<class T>
class localSet:public std::set<T>{
	const BLon id;
	size_t ignum=0;

public:
	localSet(BLon);
	template<class... Ts>
	localSet(BLon,Ts&&...);
	localSet(BLon,const localSet&);
	localSet(BLon,localSet&&);
	localSet(const localSet&);
	localSet(localSet&&);

	const iterator find(T&) const;
	const iterator find(const T&&) const;

	std::pair<iterator,bool> insert(const T&);
	std::pair<iterator,bool> insert(T&&);
	template<class T1,class T2,class... Ts>
	localSet& insert(T1&&,T2&&,Ts&&...);
	size_t erase(const T&);

	void setIgnum(size_t);

	T& enchant(T&);
	BLon ID() const;
};



template<class T>
class GmarkSet:public localSet<Gmark<T>>{
public:
	GmarkSet(BLon);
	template<class... Ts>
	GmarkSet(BLon,Ts&&...);
	GmarkSet(BLon,std::initializer_list<T>);
	GmarkSet(const GmarkSet&);
	GmarkSet(GmarkSet&&);
};

}// namespace AIA

#include"allinall_strpool.inl"

#define AIA_STRPOOL
#endif