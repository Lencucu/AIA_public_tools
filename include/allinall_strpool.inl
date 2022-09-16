#include<utility>

namespace AIA{

template<class T>Gmark<T>::Gmark(){}
template<class T>Gmark<T>::Gmark(const char* str, size_t id=0):mark(str,id){}
template<class T>Gmark<T>::Gmark(const char* str, int id=0):mark(str,id){}
template<class T>Gmark<T>::Gmark(size_t id, const char* str=nullptr):mark(str,id){}
template<class T>Gmark<T>::Gmark(int id, const char* str=nullptr):mark(str,id){}
template<class T>Gmark<T>::Gmark(const char* str, BLon inBLon):mark(str,inBLon){}
template<class T>Gmark<T>::Gmark(BLon inBLon, const char* str=nullptr):mark(str,inBLon){}
template<class T>Gmark<T>::Gmark(const mark&src):mark(src){}
template<class T>Gmark<T>::Gmark(mark&&src):mark(std::move(src)){}
template<class T>Gmark<T>::Gmark(const Gmark<T>&src){
	(*this)=src;
}
template<class T>Gmark<T>::Gmark(Gmark<T>&&src){
	(*this)=(std::move(src));
}
template<class T>Gmark<T>::~Gmark(){}

template<class T>
Gmark<T>& Gmark<T>::operator=(size_t id){
	mark::operator=(id);
	return *this;
}

template<class T>
Gmark<T>& Gmark<T>::operator=(const char*str){
	mark::operator=(str);
	return *this;
}

template<class T>
Gmark<T>& Gmark<T>::operator=(const originid&src){
	mark::operator=(src);
	return *this;
}

template<class T>
Gmark<T>& Gmark<T>::operator=(const originstr&src){
	mark::operator=(src);
	return *this;
}

template<class T>
Gmark<T>& Gmark<T>::operator=(const mark&src){
	mark::operator=(src);
	return *this;
}

template<class T>
Gmark<T>& Gmark<T>::operator=(originstr&&src){
	mark::operator=(std::move(src));
	return *this;
}

template<class T>
Gmark<T>& Gmark<T>::operator=(mark&&src){
	mark::operator=(std::move(src));
	return *this;
}

template<class T>
Gmark<T>& Gmark<T>::operator=(const Gmark<T>&src){
	if(&src==this) return *this;
	mark::operator=(src.getMark());
	Pag=src.Pag;
	return *this;
}

template<class T>
Gmark<T>& Gmark<T>::operator=(Gmark<T>&&src){
	mark::operator=(src.getMark());
	Pag=(std::move(src.Pag));
	return *this;
}

template<class T>template<class W>
Gmark<T>& Gmark<T>::assignMark(const Gmark<W>&src){
	if(&src==this) return *this;
	mark::operator=(src.getMark());
	return *this;
}

template<class T>template<class W>
Gmark<T>& Gmark<T>::moveMark(Gmark<W>&&src){
	mark::operator=(src.getMark());
	return *this;
}

template<class T>template<class W>
bool Gmark<T>::operator==(const Gmark<W>&obj) const{
	if(typeid(W)!=typeid(T)) return false;
	if((const Gmark<T>*)(&obj)==this) return true;
	return equalMark(obj)&&Pag==obj.Pag;
}

template<class T>template<class W>
bool Gmark<T>::operator!=(const Gmark<W>&obj) const{
	return !((*this)==obj);
}

template<class T>template<class W>
bool Gmark<T>::equalMark(const Gmark<W>&obj) const{
	return (*this)==obj.getMark();
}

template<class T>
Gmark<T>::operator const char*() const{
	return str;
}

template<class T>
Gmark<T>::operator size_t() const{
	return num;
}

template<class T>
Gmark<T>::operator bool() const{
	return mark::operator bool();
}


template<class T>
mark& Gmark<T>::getMark(){
	return *this;
}

template<class T>
mark& Gmark<T>::getMark() const{
	return const_cast<Gmark<T>&>(*this);
}

template<class T>
T& Gmark<T>::pag() const{
	return const_cast<T&>(Pag);
}




// localSet
template<class T>
localSet<T>::localSet(BLon id):id(id){}

template<class T>template<class... Ts>
localSet<T>::localSet(BLon id,Ts&&... srcs):id(id){
	char temp[]={ (insert(std::forward<Ts>(srcs)),0)..., 0 };
}

template<class T>
localSet<T>::localSet(BLon id,const localSet&src):id(id),set(src){}

template<class T>
localSet<T>::localSet(BLon id,localSet&&src):id(id),set(std::move(src)){}

template<class T>
localSet<T>::localSet(const localSet&src):id(src.id),set(src){}

template<class T>
localSet<T>::localSet(localSet&&src):id(src.id),set(std::move(src)){}

template<class T>
const typename localSet<T>::iterator localSet<T>::find(T&note) const{
	auto temp=set::find(note);
	if(temp!=end())
	{	if(note.c_str()[0]==signal_querry)
			note=temp->c_str();
		else
			note=temp->index();
	}
	else if(note.index()==ignum&&note!=dftstr)
	{	for(temp=begin();temp!=end();++temp)
		{	if(*temp==note.c_str())
			{	note=temp->index();
				break;
			}
		}
	}
	return temp;
}

template<class T>
const typename localSet<T>::iterator localSet<T>::find(const T&&note) const{
	auto temp=set::find(note);
	if(temp!=end()) return temp;
	else if(note.index()==ignum&&note!=dftstr)
	{	for(temp=begin();temp!=end();++temp)
			if(*temp==note.c_str()) break;
	}
	return temp;
}

template<class T>
std::pair<typename localSet<T>::iterator,bool> localSet<T>::insert(const T&src){
	if(src==ignum||src==dftstr) return std::pair<iterator,bool>{end(),false};
	return set::insert(enchant(const_cast<T&>(src)));
}

template<class T>
std::pair<typename localSet<T>::iterator,bool> localSet<T>::insert(T&&src){
	if(src==ignum||src==dftstr) return std::pair<iterator,bool>{end(),false};
	return set::insert(std::move(enchant(src)));
}

template<class T>
template<class T1,class T2,class... Ts>
localSet<T>& localSet<T>::insert(T1&&src1,T2&&src2,Ts&&... srcs){
	insert(std::forward<T1>(src1));
	insert(std::forward<T2>(src2));
	char temp[]={(insert(std::forward<Ts>(srcs)),0)...,0};
	return *this;
}

template<class T>
size_t localSet<T>::erase(const T&note){
	auto temp=find(std::move(note));
	if(temp==end()) return 0;
	if(*temp==note || (!(const originstr)(note)&&note.index()!=ignum) || ((const originstr)(note)&&note.index()==ignum) )
		return set::erase(*temp);
	return 0;
}

template<class T>
void localSet<T>::setIgnum(size_t num){
	ignum=num;
}

template<class T>
T& localSet<T>::enchant(T&target){
	target.inBLon=id;
	return target;
}

template<class T>
BLon localSet<T>::ID() const{
	return id;
}


template<class T>GmarkSet<T>::GmarkSet(BLon id):localSet(id){}
template<class T>template<class... Ts>
GmarkSet<T>::GmarkSet(BLon id,Ts&&... srcs):localSet(id,std::forward<Ts>(srcs)...){}
template<class T>GmarkSet<T>::GmarkSet(BLon id,std::initializer_list<T>srcs):localSet(id,srcs){}
template<class T>GmarkSet<T>::GmarkSet(const GmarkSet<T>&src):localSet(src){}
template<class T>GmarkSet<T>::GmarkSet(GmarkSet<T>&&src):localSet(std::move(src)){}


}// namespace AIA
