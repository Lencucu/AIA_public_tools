#include"../include/allinall_markset.hpp"
#include<cstring>
#include<cassert>
#include<utility>

namespace AIA{

/*___________FUNC___________*/
// static
size_t aslen(size_t len){
	return len/sizeof(size_t)+1;
}

size_t aslen(const char*str){
	return strlen(str)/sizeof(size_t)+1;
}

size_t content(size_t i,const char*str){
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

#ifndef AIA_LINK_STRPOOL_WITH_BASETYPE

void originstr::alloc(size_t len){
	// len is base on size_type
	if(++age==maxlife||capacity<len){
		age=0;
		if(capacity) delete as;
		capacity=len;
		as=new size_t[capacity];
	}
}

void originstr::reset(){
	age=maxlife-1;
	(*this)=dftstr;
}

originstr::originstr(size_t maxlife):maxlife(maxlife){
	reset();
}

originstr::originstr(const char*str,size_t maxlife):maxlife(maxlife){
	if(str==NULL) reset();
	else (*this)=str;
}

originstr::originstr(const originstr&src,size_t maxlife):maxlife(maxlife){
	(*this)=src;
}

originstr::originstr(originstr&&src,size_t maxlife):maxlife(maxlife){
	(*this)=(std::move(src));
}

originstr::~originstr(){
	if(capacity) delete[] as;
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
	age=0;

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


#else


void originstr::alloc(size_t bytesize){
	line.alloc(bytesize);
}

void originstr::reset(){
	(*this)=dftstr;
}

originstr::originstr(size_t maxlife):line(size_t(0),maxlife){
	reset();
}

originstr::originstr(const char*str,size_t maxlife):line(size_t(0),maxlife){
	if(str==nullptr) reset();
	else (*this)=str;
}

originstr::originstr(const originstr&src,size_t maxlife):line(size_t(0),maxlife){
	(*this)=src;
}

originstr::originstr(originstr&&src,size_t maxlife):line(size_t(0),maxlife){
	(*this)=(std::move(src));
}

originstr::~originstr(){}

originstr& originstr::operator=(const char*str){
	if(str==line.ptr()||(line.size()&&!strcmp(str,line.ptr()))) return *this;
	size_t length=strlen(str);
	line.chgsizeTake(0,str,length+1);
	line[length]=0;

	return *this;
}

originstr& originstr::operator=(const originstr&src){
	if(&src==this) return *this;
	line.chgsizeTake(0,src.line);

	return *this;
}

originstr& originstr::operator=(originstr&&src){
	if(&src==this) return *this;
	line=(std::move(src.line));

	return *this;
}

bool originstr::operator==(const originstr&obj) const{
	return line==obj.line;
}

bool originstr::operator!=(const originstr&obj) const{
	return !(line==obj.line);
}

bool originstr::operator<(const originstr&obj) const{
	return strcmp(line.ptr(),obj.line.ptr())<0?true:false;
}

bool originstr::operator==(const char*str) const{
	if(str==nullptr) return false;
	return !strcmp(str,line.ptr());
}

bool originstr::operator!=(const char*str) const{
	if(str==nullptr) return true;
	return strcmp(str,line.ptr());
}


originstr::operator const char*() const{
	return line.ptr();
}

originstr::operator bool() const{
	return strcmp(dftstr,line.ptr());
}

const char* originstr::c_str() const{
	return line.ptr();
}

size_t originstr::lenth() const{
	return line.size()-1;
}

#endif


bool originstr::validStr() const{
	if(str[0]!=signal_querry)
	if(strcmp(str,""))
	if(strcmp(str,dftstr))
		return true;
	return false;
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

bool originid::validID(size_t ignum) const{
	if(index()!=ignum) return true;
	return false;
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




}// namespace AIA

