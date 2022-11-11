#include<cstring>
#include<cassert>
#include<utility>
#include<type_traits>



namespace AIA{




// block
template<class T>
block<T>::block(size_t size,size_t maxlife):maxlife(maxlife){
	if(!size) return;
	alloc(size);
}

template<class T>
block<T>::block(const T*src,size_t size,size_t maxlife):maxlife(maxlife){
	if(src&&size) chgsizeTake(0,src,size);
}

template<class T>
block<T>::block(const block<T>&src, size_t size,size_t maxlife):maxlife(maxlife){
	if(src&&size) chgsizeTake(0,src,size);
}

template<class T>
block<T>::block(block<T>&&src,size_t maxlife):maxlife(maxlife){
	if(src) swap(src);
}

template<class T>
block<T>::~block(){
	clear();
	if(TFtemp!=nullptr) delete TFtemp;
}


template<class T>
block<T>& block<T>::operator=(const block<T>&src){
	if(!src.useSize)
	{	clear();
		return *this;
	}
	alloc(src.useSize);
	if(std::is_scalar<T>::value) memcpy(p,src.p,useSize*sizeof(T));
	else
		for(size_t i=useSize-1;i!=size_t(-1);--i)
			p[i]=src.p[i];
	return *this;
}

template<class T>
block<T>& block<T>::operator=(block<T>&&src){
	clear();
	p=src.p;
	useSize=src.useSize;
	capacity=src.capacity;
	src.p=nullptr;
	src.useSize=0;
	src.capacity=0;
	return *this;
}


template<class T>
T& block<T>::operator[](size_t pos){
	return p[pos];
}


// -1 mean no realloc, 0 mean can't alloc
// 1 mean will realloc
template<class T>
int block<T>::preview(size_t size){
	if(!size||size>maxsize) return 0;
	if(capacity==size) return -1;
	if(age==maxlife||capacity<size)
	{	if(TFtemp==nullptr) const_cast<block*&>(TFtemp)=new block<T>;
		return 1;
	}
	return -1;
}

template<class T>
int block<T>::alloc(size_t size){
	if(!size||size>maxsize) return 0;
	if(capacity==size) return -1;
	if(age==maxlife||capacity<size)
	{	if(TFtemp==nullptr) const_cast<block*&>(TFtemp)=new block<T>;
		if(capacity) delete[] p;

		GrowMethoud1(capacity,size,maxsize,age,maxlife);
		useSize=size<=capacity?size:capacity;
		
		p=new T[capacity];
		age=0;
		
		return 1;
	}
	++age;
	return -1;
}

template<class T>
void block<T>::clear(){
	if(!capacity) return;
	delete[] p;
	p=nullptr;
	useSize=0;
	capacity=0;
}

template<class T>
size_t block<T>::grow(size_t size){
	switch(preview(useSize+size))
	{	case -1:
		useSize+=size;
		break;
		case 0:
		break;
		case 1:
		*TFtemp=std::move(*this);
		alloc(TFtemp->useSize+size);
		size=useSize-TFtemp->useSize;
		if(std::is_scalar<T>::value)
			memcpy(p,TFtemp->p,useSize*sizeof(T));
		else
		{	for(size_t i=TFtemp->useSize-1;i!=size_t(-1);--i)
				p[i]=std::move(TFtemp->p[i]);
		}
		break;
	}
	return size;
}

template<class T>
size_t block<T>::shrink(size_t size){
	if(size>useSize) size=useSize;
	switch(preview(useSize-size))
	{	case -1:
		useSize-=size;
		break;
		case 1:
		TFtemp->swap(*this);
		alloc(TFtemp->useSize-size);
		if(std::is_scalar<T>::value)
			memcpy(p,TFtemp->p,useSize*sizeof(T));
		else
			for(size_t i=useSize-1;i!=size_t(-1);--i)
				p[i]=std::move(TFtemp->p[i]);
		break;
	}
	return size;
}


template<class T>
void block<T>::swap(block<T>&tgt){
	T*temp=p;
	size_t tempSize=useSize;
	size_t tempCapacity=capacity;
	p=tgt.p;
	useSize=tgt.useSize;
	capacity=tgt.capacity;
	tgt.p=temp;
	tgt.useSize=tempSize;
	tgt.capacity=tempCapacity;
}

template<class T>
void block<T>::takein(T*src,size_t size,size_t capty){
	if(!size) return;
	clear();
	p=src;
	src=nullptr;
	useSize=size;
	capacity=capty?capty:size;
	age=0;
}

template<class T>template<class S,class... Ts>
block<T>& block<T>::assign(size_t pos,S&& head,Ts&&...srcs){
	if(pos>=capacity) return *this;
	size_t insertcount=0;
	char temp[]={(++insertcount,p[pos]=std::forward<S>(head),0),(pos+insertcount<capacity?(p[pos+insertcount++]=std::forward<Ts>(srcs),0):0)...};
	if(pos+insertcount>useSize) useSize=pos+insertcount;
	return *this;
}

// thend should not larger than the sizeof src
template<class T>
size_t block<T>::chgsizeTake(size_t offset,const T*src,size_t thend,size_t start){
	if(thend<=start) return 0;

	size_t incomingLenth=thend-start;
	size_t needLenth=incomingLenth+offset;
	bool overlap=(src==p);

	switch(preview(needLenth))
	{	case 0:
		return 0;
		case 1:
		*TFtemp=std::move(*this);
		alloc(needLenth);
		if(std::is_scalar<T>::value)
		{	if(offset) memcpy(p,TFtemp->p,offset*sizeof(T));
			if(needLenth<TFtemp->useSize) memcpy(p+needLenth,TFtemp->p+needLenth,sizeof(T)*(TFtemp->useSize-needLenth));
		}
		else
		{	if(offset)// 0~offset
			{	for(size_t i=0;i<offset;++i)
				{	if(!overlap)
						p[i]=std::move(TFtemp->p[i]);
					else
						p[i]=TFtemp->p[i];
				}
			}
			if(needLenth<TFtemp->useSize)// rightside
			{	for(size_t i=needLenth;i<TFtemp->useSize;++i)
				{	if(!overlap)
						p[i]=std::move(TFtemp->p[i]);
					else
						p[i]=TFtemp->p[i];
				}
			}
		}
		if(overlap)
		{	src=TFtemp->p;
			overlap=false;
		}
		case -1:
		useSize=needLenth;
		if(std::is_scalar<T>::value)
		{	if(!overlap)
				memcpy(p+offset,src,incomingLenth*sizeof(T));
			else
				memmove(p+offset,src,incomingLenth*sizeof(T));
		}
		else
		{	if( !overlap || offset<start )
				for(;start<thend;++start,++offset)// rewrite
					p[offset]=src[start];
			else if(offset!=start)
				for(--thend,offset+=incomingLenth-1;start<=thend;--thend,--offset)
					p[offset]=src[thend];
		}
		break;
	}

	return incomingLenth;
}

template<class T>
size_t block<T>::chgsizeTake(size_t offset,const block<T>&src,size_t thend,size_t start){
	if(thend>src.useSize) thend=src.useSize;
	return chgsizeTake(offset,src.p,thend,start);
}

template<class T>
size_t block<T>::kepsizeTake(size_t offset,const T*src,size_t thend,size_t start){
	if( thend<=start || offset>capacity ) return 0;

	size_t dis=thend-start;
	if(offset+dis>useSize)
	{	useSize=offset+dis;
		if(useSize>capacity)
			useSize=capacity;
		dis=useSize-offset;
		thend=dis+start;
	}

	if(std::is_scalar<T>::value)
	{	if(src!=p)
			memcpy(p+offset,src,dis*sizeof(T));
		else
			memmove(p+offset,src,dis*sizeof(T));
	}
	else
	{	if( src!=p || offset<start )
			for(;start<thend;++start,++offset)
				p[offset]=src[start];
		else if(offset!=start)
			for(--thend,offset+=dis-1;start<=thend;--thend,--offset)
				p[offset]=src[thend];
	}
	
	return dis;
}

template<class T>
size_t block<T>::kepsizeTake(size_t offset,const block<T>&src,size_t thend,size_t start){
	if(thend>src.useSize) thend=src.useSize;
	return kepsizeTake(offset,src.p,thend,start);
}


extern int i6;
template<class T>
size_t block<T>::insert(size_t pos,const T*src,size_t thend,size_t start){
	if(start>=thend) return 0;
	size_t dis=thend-start;
	if(useSize+dis>maxsize)
		dis=maxsize-useSize;

	if(pos>=useSize)
		return chgsizeTake(pos,src,thend,start);

	// data restore
	if(dis+useSize<=capacity)
	{	if(std::is_scalar<T>::value)
		{	memmove(p+pos+dis,p+pos,useSize-pos);
			memcpy(p+pos,src+start,dis);
		}
		else
		{	for(size_t i=useSize-pos-1;i!=size_t(-1);--i)
				p[pos+dis+i]=std::move(p[pos+i]);
			for(size_t i=0;i<dis;++i)
				p[pos+i]=src[start+i];
		}
	}
	else
	{	TFtemp->swap(*this);
		alloc(dis+TFtemp->useSize);
		if(std::is_scalar<T>::value)
		{	memcpy(p,TFtemp->p,pos);
			memcpy(p+pos+dis,TFtemp->p+pos,TFtemp->useSize-pos);
			memcpy(p+pos,src+start,dis);
		}
		else
		{	for(size_t i=0;i<pos;++i)
				p[i]=std::move(TFtemp->p[i]);
			for(size_t i=TFtemp->useSize-pos-1;i!=size_t(-1);--i)
				p[pos+dis+i]=std::move(TFtemp->p[pos+i]);
			for(size_t i=0;i<dis;++i)
				p[pos+i]=src[start+i];
		}
	}

	useSize+=dis;
	return dis;

}

template<class T>
size_t block<T>::insert(size_t pos,const block<T>&src,size_t thend,size_t start){
	if(thend>src.useSize) thend=src.useSize;
	return insert(pos,src.p,thend,start);
}

template<class T>
size_t block<T>::remove(size_t pos,size_t range){
	if(pos>=useSize) return 0;
	if(pos+range>=useSize) return shrink(useSize-pos);// may some realloc if pre is large & left is so small
	else
	{	if(std::is_scalar<T>::value)
			memcpy(p+pos,p+pos+range,useSize-pos-range);
		else
		{	for(size_t i=0;i<useSize-pos-range;++i)
				p[pos+i]=std::move(p[pos+range+i]);
		}
		useSize-=range;
	}
	return range;
}


template<class T>
T* block<T>::ptr(size_t offset){
	if(offset<useSize) return p+offset;
	return nullptr;
}

template<class T>template<class S>
S* block<T>::ptras(size_t offset){
	if(offset<sizeas<S>()) return ((S*)p)+offset;
	return nullptr;
}


template<class T>
const T& block<T>::operator[](size_t pos) const{
	return p[pos];
}

template<class T>
bool block<T>::operator==(const block<T>&obj) const{
	if(useSize!=obj.useSize) return false;
	if(std::is_scalar<T>::value) return !memcmp(p,obj.p,useSize*sizeof(T));
	for(size_t i=useSize-1;i!=size_t(-1);--i)
		if(p[i]!=obj.p[i]) return false;
	return true;
}

template<class T>
const T* block<T>::ptr(size_t offset) const{
	if(offset<useSize) return p+offset;
	return nullptr;
}

template<class T>template<class S>
const S* block<T>::ptras(size_t offset) const{
	if(offset<sizeas<S>()) return ((S*)p)+offset;
	return nullptr;
}

template<class T>
size_t block<T>::size() const{
	return useSize;
}

template<class T>template<class S>
size_t block<T>::sizeas() const{
	return useSize*sizeof(T)/sizeof(S);
}

template<class T>
block<T>::operator bool() const{
	return p&&useSize;
}













#ifdef WAITING







// Xblock
template<class T>
Xblock<T>::Xblock(size_t size,size_t maxlife,size_t maxbytesize):maxlife(maxlife),maxbytesize(maxbytesize){
	if(!size) return;
	alloc(size*sizeof(T));
}

template<class T>template<class S>
Xblock<T>::Xblock(S*&&src,size_t size,size_t maxlife,size_t maxbytesize):maxlife(maxlife),maxbytesize(maxbytesize){
	if(src&&size) takein(std::move(src),size);// src should be alloced by new
}

template<class T>template<class S>
Xblock<T>::Xblock(const S*src,size_t size,size_t maxlife,size_t maxbytesize):maxlife(maxlife),maxbytesize(maxbytesize){
	if(src&&size) chgsizeTake(0,src,size);
}

template<class T>
Xblock<T>::Xblock(const Xblock<T>&src, size_t size,size_t maxlife,size_t maxbytesize):maxlife(maxlife),maxbytesize(maxbytesize){
	if(src&&size) chgsizeTake(0,src,size);
}

template<class T>
Xblock<T>::Xblock(Xblock<T>&&src,size_t maxlife,size_t maxbytesize):maxlife(maxlife),maxbytesize(maxbytesize){
	if(src) swap(src);
}

template<class T>template<class S>
Xblock<T>::Xblock(const Xblock<S>&src, size_t size,size_t maxlife,size_t maxbytesize):maxlife(maxlife),maxbytesize(maxbytesize){
	if(src&&size) chgsizeTake(0,src,size);
}

template<class T>template<class S>
Xblock<T>::Xblock(Xblock<S>&&src,size_t maxlife,size_t maxbytesize):maxlife(maxlife),maxbytesize(maxbytesize){
	if(src) swap(src);
}

template<class T>
Xblock<T>::~Xblock(){
	clear();
}


template<class T>
void Xblock<T>::clear(){
	if(!bytesize) return;
	delete[] p;
	p=nullptr;
	bytesize=0;
}

template<class T>
void Xblock<T>::alloc(size_t bytesize){
	assert(bytesize);
	assert(bytesize<=maxbytesize);
	if(this->bytesize==bytesize) return;
	if(age==maxlife||this->bytesize<bytesize)
	{	if(this->bytesize) delete[] p;
		p=new char[bytesize];
		this->bytesize=bytesize;
		age=0;
	}
	++age;
}

template<class T>
void Xblock<T>::regetnew(size_t size){
	if(bytesize==size*sizeof(T)) return;
	clear();
	alloc(size*sizeof(T));
}

template<class T>template<class S>
void Xblock<T>::regetnewas(size_t size){
	if(bytesize==size*sizeof(S)) return;
	clear();
	alloc(size*sizeof(S));
}


template<class T>template<class S,class... Ts>
Xblock<T>& Xblock<T>::assign(size_t befbytelenth,S&& head,Ts&&...srcs){
	typedef typename std::remove_reference<S>::type justype;
	if(befbytelenth+sizeof(justype)*(sizeof...(Ts)+1)>bytesize) return *this;
	justype*tgt=(justype*)(ptras<char>()+befbytelenth);
	size_t ind=0;
	tgt[ind]=std::forward<S>(head);
	char temp[]={(tgt[++ind]=std::forward<Ts>(srcs),0)...,0};
	return *this;
}

template<class T>template<class S>
void Xblock<T>::takein(S*&&src,size_t size){
	if(!size) return;
	clear();
	p=(char*)src;
	src=nullptr;
	bytesize=size*sizeof(S);
	age=0;
}

template<class T>template<class S>
void Xblock<T>::swap(Xblock<S>&tgt){
	char*temp=p;
	size_t tempsize=bytesize;
	p=tgt.p;
	bytesize=tgt.bytesize;
	tgt.p=temp;
	tgt.bytesize=tempsize;
}

template<class T>template<class S>
Xblock<T>& Xblock<T>::operator=(const Xblock<S>&src){
	regetnewas<S>(src.size());
	if(bytesize) memcpy(p,src.p,src.bytesize);
	return *this;
}

template<class T>template<class S>
Xblock<T>& Xblock<T>::operator=(Xblock<S>&&src){
	clear();
	p=src.p;
	src.p=nullptr;
	bytesize=src.bytesize;
	src.bytesize=0;
	return *this;
}


template<class T>
T& Xblock<T>::operator[](size_t pos){
	return ((T*)p)[pos];
}

template<class T>
const T& Xblock<T>::operator[](size_t pos) const{
	return ((T*)p)[pos];
}


template<class T>
bool Xblock<T>::operator==(const Xblock&obj) const{
	if(bytesize!=obj.bytesize) return false;
	return !memcmp(p,obj.p,bytesize);
}


template<class T>template<class S>
Xblock<T>& Xblock<T>::chgsizeTake(size_t befbytelenth,const S*src,size_t endind,size_t start){
	if( (char*)src==p || endind==start ) return *this;

	size_t rangebytelenth=(endind-start)*sizeof(S);

	Xblock<char> temp;
	// if realloc, which must be caused by the new size is larger than old
	// and this case will just happen when insert happen around the end
	// the origin content will be removed
	// but it should keep the origin content, so it need a temp
	if(befbytelenth+rangebytelenth>bytesize)
	{	temp.regetnew(befbytelenth);
		copyContent(temp.p,p,befbytelenth);
	}
	// avoid realloc by age
	if(age==maxlife) --age;
	alloc( befbytelenth + rangebytelenth );
	if(befbytelenth+rangebytelenth>bytesize) copyContent(p,temp.p,befbytelenth);
	
	copyContent(ptras<char>()+befbytelenth,src+start,rangebytelenth);
	
	return *this;
}

template<class T>template<class S>
Xblock<T>& Xblock<T>::chgsizeTake(size_t befbytelenth,const Xblock<S>&src,size_t endind,size_t start){
	if(endind==size_t(-1)) endind=src.bytesize/sizeof(S);
	else assert(endind<=src.bytesize/sizeof(S));
	return chgsizeTake(befbytelenth,src.p,endind,start);
}

template<class T>template<class S>
Xblock<T>& Xblock<T>::kepsizeTake(size_t befbytelenth,const S*src,size_t endind,size_t start){
	if( (char*)src==p || endind==start || befbytelenth>bytesize ) return *this;
	size_t rangebytelenth=(endind-start)*sizeof(S);
	if(befbytelenth+rangebytelenth>bytesize)
	{	rangebytelenth=bytesize-befbytelenth;
		rangebytelenth-=rangebytelenth%sizeof(S);
	}
	copyContent(p+befbytelenth,src+start,rangebytelenth);
	return *this;
}

template<class T>template<class S>
Xblock<T>& Xblock<T>::kepsizeTake(size_t befbytelenth,const Xblock<S>&src,size_t endind,size_t start){
	if( endind==size_t(-1) || endind>src.bytesize/sizeof(S) ) endind=src.bytesize/sizeof(S);
	return kepsizeTake(befbytelenth,src.p,endind,start);
}


template<class T>template<class S>
const S* Xblock<T>::ptras(size_t byteoffset) const{
	return (S*)(p+byteoffset);
}

template<class T>template<class S>
S* Xblock<T>::ptras(size_t byteoffset){
	return (S*)(p+byteoffset);
}

template<class T>
const T* Xblock<T>::ptr(size_t byteoffset) const{
	return (T*)(p+byteoffset);
}

template<class T>
T* Xblock<T>::ptr(size_t byteoffset){
	return (T*)(p+byteoffset);
}

template<class T>
size_t Xblock<T>::size() const{
	return bytesize/sizeof(T);
}

template<class T>template<class S>
size_t Xblock<T>::sizeas() const{
	return bytesize/sizeof(S);
}

template<class T>
Xblock<T>::operator bool() const{
	return p&&bytesize;
}



#endif




// attribute
template<class... Ts>template<class... Rs>
attribute<Ts...>::attribute(Rs&&... args):item(std::forward<Rs>(args)...){}

template<class... Ts>template<size_t I>
typename std::tuple_element<I,std::tuple<Ts...>>::type& attribute<Ts...>::attrib(){
	return std::get<I>(item);
}

template<class... Ts>template<size_t I>
const typename std::tuple_element<I,std::tuple<Ts...>>::type& attribute<Ts...>::attrib() const{
	return std::get<I>(item);
}





}// namespace AIA