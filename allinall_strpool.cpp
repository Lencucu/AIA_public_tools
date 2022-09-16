#include"include/allinall_strpool.hpp"
#include<cstring>
#include<cassert>
#include<utility>

namespace AIA{

/*___________FUNC___________*/
// static
static size_t aslen(size_t len){
	return len/sizeof(size_t)+1;
}

static size_t aslen(const char*str){
	return strlen(str)/sizeof(size_t)+1;
}

static size_t content(size_t i,const char*str){
	size_t maxindex = aslen(str)-1;
	assert(i<=maxindex+1);
	if(i<maxindex) return ((const size_t*)str)[i];
	union{size_t asp=0;char strp[sizeof(size_t)];};
	int j=-1;
	do{
		strp[++j]=str[maxindex*sizeof(size_t)+j];
	}while(strp[j]);
	return asp;
}
// originid
BLon takeBLon(size_t Num){
	union{size_t num;BLon pos[sizeof(size_t)];};
	num=Num;
	return pos[blonpos()];
}
void setSignal(originstr&str,char signal){
	const_cast<char*>(str.c_str())[0]=signal;
}


/*___________CLASS FUNC___________*/
// originstr
void originstr::alloc(size_t len){
	// len is base on size_type
	if(++count==maxlife||capacity<len){
		count=0;
		if(capacity) delete as;
		capacity=len;
		as=new size_t[capacity];
	}
}

void originstr::reset(){
	count=life-1;
	(*this)=dftstr;
}

originstr::originstr(size_t max):life(max){
	reset();
}

originstr::originstr(const char*str,size_t max):life(max){
	if(str==NULL) reset();
	else (*this)=str;
}

originstr::originstr(const originstr&src,size_t max):life(max){
	(*this)=src;
}

originstr::originstr(originstr&&src,size_t max):life(max){
	(*this)=(std::move(src));
}

originstr::~originstr(){
	if(capacity) delete as;
}

originstr& originstr::operator=(const char*str){
	if(str==this->str||(this->capacity&&!strcmp(str,this->str))) return *this;
	size_t cmpcapacity=aslen(str);
	alloc(cmpcapacity);
	for(size_t i=cmpcapacity-1;i!=AIA_OVERFLOW;--i)
		as[i]=content(i,str);
	length=strlen(str);

	return *this;
}

originstr& originstr::operator=(const originstr&src){
	if(&src==this) return *this;
	alloc(src.capacity);
	for(size_t i=src.capacity-1;i!=AIA_OVERFLOW;--i)
		as[i]=src.as[i];
	length=src.length;

	return *this;
}

originstr& originstr::operator=(originstr&&src){
	if(capacity) delete as;
	as=src.as;
	capacity=src.capacity;
	length=src.length;
	count=0;

	src.as=nullptr;
	src.capacity=0;

	return *this;
}

bool originstr::operator==(const originstr&obj) const{
	return !((*this)!=obj);
}

bool originstr::operator!=(const originstr&obj) const{
	if(length!=obj.length) return true;
	for(size_t i=aslen(length)-1;i!=AIA_OVERFLOW;--i)
		if(as[i]!=obj.as[i])
			return true;
	return false;
}

bool originstr::operator<(const originstr&obj) const{
	return strcmp(str,obj.str)<0?true:false;
}

bool originstr::operator==(const char*str) const{
	return !((*this)!=str);
}

bool originstr::operator!=(const char*str) const{
	return strcmp(str,this->str);
}


originstr::operator const char*() const{
	return str;
}

originstr::operator bool() const{
	return strcmp(dftstr,str);
}

const char* originstr::c_str() const{
	return str;
}

size_t originstr::lenth() const{
	return length;
}








// originid
void originid::reset(){
	if(!inBLon){
		num=0;
		return;
	}
	BLon mid=inBLon;
	num=0;
	inBLon=mid;
}

originid::originid(BLon inBLon):num(0){this->inBLon=inBLon;}
originid::originid(size_t id):num(id){}
originid::originid(int id):num(id){}
originid::originid(const originid&src):num(src.num){}

void originid::setIndex(size_t id){
	BLon temp=inBLon;
	num=id;
	inBLon=temp;
}

originid& originid::operator=(size_t id){
	num=id;
	return *this;
}
originid& originid::operator=(const originid&src){
	num=src.num;
	return *this;
}

bool originid::operator==(const originid&obj) const{
	return num==obj.num;
}
bool originid::operator!=(const originid&obj) const{
	return num!=obj.num;
}

bool originid::operator<(const originid&obj) const{
	return index()<obj.index();
}

bool originid::operator==(size_t num) const{
	return num==this->num;
}
bool originid::operator!=(size_t num) const{
	return num!=this->num;
}

originid::operator size_t() const{
	return num;
}

originid::operator bool() const{
	return num;
}

size_t originid::ID() const{
	return num;
}

size_t originid::index() const{
	BLon mid=inBLon;
	inBLon=0;
	size_t mid2=num;
	inBLon=mid;
	return mid2;
}


// mark
void mark::reset(){
	originstr::reset();
	originid::reset();
}


mark::mark(){}

mark::mark(const char*str,size_t id):originstr(str),originid(id){}
mark::mark(const char*str,int id):originstr(str),originid(id){}
mark::mark(size_t id,const char*str):originstr(str),originid(id){}
mark::mark(int id,const char*str):originstr(str),originid(id){}
mark::mark(const char*str,BLon inBLon):originstr(str){
	this->inBLon=inBLon;
}
mark::mark(BLon inBLon,const char*str):originstr(str){
	this->inBLon=inBLon;
}

mark::mark(const mark&src){
	(*this)=src;
}

mark::mark(mark&&src){
	(*this)=std::move(src);
}

mark::~mark(){}


mark& mark::operator=(size_t id){
	originid::operator=(id);
	return *this;
}

mark& mark::operator=(const char*str){
	if(str==this->str) return *this;
	originstr::operator=(str);
	return *this;
}

mark& mark::operator=(const originstr&src){
	if(src.str==this->str) return *this;
	originstr::operator=(src);
	return *this;
}
mark& mark::operator=(const originid&src){
	originid::operator=(src);
	return *this;
}

mark& mark::operator=(const mark&src){
	if(&src==this) return *this;
	originid::operator=(src);
	originstr::operator=(src);
	return *this;
}

mark& mark::operator=(originstr&&src){
	originstr::operator=(std::move(src));
	return *this;
}

mark& mark::operator=(mark&&src){
	originid::operator=(src);
	originstr::operator=(std::move(src));
	return *this;
}


bool mark::operator==(const mark&obj) const{
	if(&obj==this) return true;
	return originid::operator==(obj)&&originstr::operator==(obj);
}

bool mark::operator!=(const mark&obj) const{
	return !((*this)==obj);
}

bool mark::operator<(const mark&obj) const{
	if(originid::operator==(obj)||originstr::operator==(obj))
		return false;
	return originid::operator<(obj);
}

mark::operator const char*() const{
	return str;
}

mark::operator size_t() const{
	return num;
}

mark::operator bool() const{
	return originid::operator bool()&&originstr::operator bool();
}



}// namespace AIA

