#include<utility>
#include<type_traits>
#include<cassert>

namespace AIA{




// originstr

#	ifdef AIA_LINK_STRPOOL_WITH_BASETYPE

template<class... Ts>
originstr& originstr::combineStr(Ts... strs){
	block<char> midline;
	char temp[]={( midline.insert(midline.size(),strs,strlen(strs)) ,0)...,0};
	midline.insert(midline.size(),"\0",1);
	line=std::move(midline);
	return *this;
}
#	else
template<class... Ts>
originstr& originstr::combineStr(Ts... strs){
	const char* temp[]={( length+=strlen(strs) ,strs)...,0};
	originstr tempstr;
	tempstr.alloc(aslen(length));

	size_t tempLength=0;
	for(size_t i=0;i<sizeof(temp)/sizeof(const char*)-1;++i)
	{	memmove(tempstr.str+tempLength,temp[i],strlen(temp[i]));
		tempLength+=strlen(temp[i]);
	}
	tempstr.str[length]=0;
	
	(*this)=std::move(tempstr);
	return *this;
}
#	endif








// Mark

MARK_TEMPLATE
void MARK::reset(){
	originstr::reset();
	originid::reset();
	resetLink();
	Pag=T{};
}


MARK_TEMPLATE MARK::
Mark(const char*str,size_t id):
originstr(str),originid(id){}

MARK_TEMPLATE MARK::
Mark(const char*str,int id):
originstr(str),originid(id){}

MARK_TEMPLATE MARK::
Mark(size_t id,const char*str):
originstr(str),originid(id){}

MARK_TEMPLATE MARK::
Mark(int id,const char*str):
originstr(str),originid(id){}

MARK_TEMPLATE MARK::
Mark(const char*str,BLon inBLon):
originstr(str){ this->inBLon=inBLon; }

MARK_TEMPLATE MARK::
Mark(BLon inBLon,const char*str):
originstr(str){ this->inBLon=inBLon; }

MARK_TEMPLATE MARK::
Mark(const MARK&src){
	(*this)=src;
}

MARK_TEMPLATE MARK::
Mark(MARK&&src){
	(*this)=std::move(src);
}

MARK_TEMPLATE
OTHER_MARK_TEMPLATE MARK::
Mark(const OTHER_MARK &src){
	assignMark(src);
}

MARK_TEMPLATE
OTHER_MARK_TEMPLATE MARK::
Mark(OTHER_MARK &&src){
	moveMark(std::move(src));
}

MARK_TEMPLATE MARK::
~Mark(){}


MARK_TEMPLATE
MARK& MARK::operator=(size_t id){
	originid::operator=(id);
	return *this;
}

MARK_TEMPLATE
MARK& MARK::operator=(const char* str){
	if(str==c_str()) return *this;
	originstr::operator=(str);
	return *this;
}

MARK_TEMPLATE
MARK& MARK::operator=(const originid& src){
	originid::operator=(src);
	return *this;
}

MARK_TEMPLATE
MARK& MARK::operator=(const originstr& src){
	if(src.c_str()==c_str()) return *this;
	originstr::operator=(src);
	return *this;
}

MARK_TEMPLATE
MARK& MARK::operator=(originstr&& src){
	originstr::operator=(std::move(src));
	return *this;
}

MARK_TEMPLATE
MARK& MARK::operator=(const MARK& src){
	if((void*)(&src)==(void*)this) return *this;
	originid::operator=(src);
	if( !link && inBLon ) inBLon=0;
	originstr::operator=(src);
	from=src.from;
	if(!std::is_same<T,formark>::value) Pag=src.Pag;
	return *this;
}

MARK_TEMPLATE
MARK& MARK::operator=(MARK&& src){
	originid::operator=(src);
	if( !link && inBLon ) inBLon=0;
	originstr::operator=(std::move(src));
	from=src.from;
	if(!std::is_same<T,formark>::value) Pag=std::move(src.Pag);
	return *this;
}

MARK_TEMPLATE
OTHER_MARK_TEMPLATE
MARK& MARK::operator=(const OTHER_MARK& src){
	assignMark(src);
	return *this;
}

MARK_TEMPLATE
OTHER_MARK_TEMPLATE
MARK& MARK::operator=(OTHER_MARK&& src){
	moveMark(std::move(src));
	return *this;
}

MARK_TEMPLATE
MARK& MARK::assignMark(const mark& src){
	if((void*)(&src)==(void*)this) return *this;
	originid::operator=(src);
	originstr::operator=(src);
	return *this;
}

MARK_TEMPLATE
MARK& MARK::moveMark(mark&& src){
	originid::operator=(src);
	originstr::operator=(std::move(src));
	return *this;
}

MARK_TEMPLATE
OTHER_MARK_TEMPLATE
bool MARK::operator==(const OTHER_MARK& obj) const{
	if(&obj==this) return true;
	if(typeid(W)!=typeid(T)) return false;
	return equalMark(obj)/*&&Pag==obj.Pag*/;
}

MARK_TEMPLATE
OTHER_MARK_TEMPLATE
bool MARK::operator!=(const OTHER_MARK& obj) const{
	return !((*this)==obj);
}

MARK_TEMPLATE
bool MARK::operator<(const MARK& obj) const{
	if(originid::operator==(obj)||originstr::operator==(obj))
		return false;
	return originid::operator<(obj);
}

MARK_TEMPLATE
bool MARK::equalMark(const mark& obj) const{
	return originid::operator==(obj)&&originstr::operator==(obj);
}


MARK_TEMPLATE
template<bool is_auto>
bool MARK::valid() const{
	return is_auto?true:validID() && validStr();
}

MARK_TEMPLATE
bool MARK::linked() const{
	return inBLon;
}

MARK_TEMPLATE
void MARK::unlink(){
	link=false;
	inBLon=0;
}


MARK_TEMPLATE
MARK::operator mark&&(){
	return std::move(getMark());
}

MARK_TEMPLATE
MARK::operator mark&(){
	return getMark();
}

MARK_TEMPLATE
MARK::operator mark&() const{
	return getMark();
}

MARK_TEMPLATE
MARK::operator const char*() const{
	return str;
}

MARK_TEMPLATE
MARK::operator size_t() const{
	return num;
}

MARK_TEMPLATE
MARK::operator bool() const{
	return originid::operator bool()&&originstr::operator bool();
}

MARK_TEMPLATE
void MARK::print(FILE* stream) const{
	fprintf(stream,"%03d:%05zu[%p]\"%s\"\n",inBLon,index(),from,str);
}

MARK_TEMPLATE
mark& MARK::getMark() const{
	return reinterpret_cast<mark&>(const_cast<MARK&>(*this));
}








// localSet

LOCALSET_TEMPLATE
void LOCALSET::
setBLon(BLon id){ const_cast<BLon&>(this->id)=id; }


LOCALSET_TEMPLATE
LOCALSET::
localSet(BLon id):id(id){}

LOCALSET_TEMPLATE
template<class... Ts> LOCALSET::
localSet(BLon id,Ts&&... notes):
id(id){
	char temp[]={ (insert(std::forward<Ts>(notes)),0)..., 0 };
}

LOCALSET_TEMPLATE
LOCALSET::
localSet(BLon id,const LOCALSET&src):
id(id),set(src),ignum(src.ignum),added(src.added){}

LOCALSET_TEMPLATE
LOCALSET::
localSet(BLon id,LOCALSET&&src):
id(id),set(std::move(src)),ignum(src.ignum),added(src.added){}

LOCALSET_TEMPLATE
LOCALSET::
localSet(const LOCALSET&src):
id(src.id),set(src),ignum(src.ignum),added(src.added){}

LOCALSET_TEMPLATE
LOCALSET::
localSet(LOCALSET&&src):
id(src.id),set(std::move(src)),ignum(src.ignum),added(src.added){}

LOCALSET_TEMPLATE
template<class S,bool _is_auto> LOCALSET::
localSet( BLon id, const localSet<S,_is_auto>& src):
id(id){ copyFrom(src); }

LOCALSET_TEMPLATE
template<class S,bool _is_auto> LOCALSET::
localSet( BLon id, localSet<S,_is_auto>&& src):
id(id){ moveFrom(std::move(src)); }

LOCALSET_TEMPLATE
template<class S,bool _is_auto> LOCALSET::
localSet(const localSet<S,_is_auto>& src):
id(src.id){ copyFrom(src); }

LOCALSET_TEMPLATE
template<class S,bool _is_auto> LOCALSET::
localSet(localSet<S,_is_auto>&& src):
id(src.id){ moveFrom(std::move(src)); }

// if not exist will cause error
LOCALSET_TEMPLATE
template<class S> typename decltype(T::Pag)& LOCALSET::
operator[](S&&note){
	assert(find(note)!=end());
	if(std::is_reference<S>::value)
		return find(note)->Pag;
	else
		return find(std::move(note))->Pag;
}


LOCALSET_TEMPLATE
LOCALSET& LOCALSET::
operator=(const LOCALSET&src){
	if(is_auto) added=src.added;
	const_cast<size_t&>(ignum)=src.ignum;

	set::operator=(src);
	for(auto it=begin();it!=end();++it)
		it->inBLon=id;

	return *this;
}

LOCALSET_TEMPLATE
LOCALSET& LOCALSET::
operator=(LOCALSET&&src){
	if(is_auto) added=src.added;
	const_cast<size_t&>(ignum)=src.ignum;

	set::operator=(std::move(src));
	for(auto it=begin();it!=end();++it)
		it->inBLon=id;

	src.clear();
	return *this;
}

LOCALSET_TEMPLATE
template<class S,bool _is_auto>
LOCALSET& LOCALSET::
copyFrom(const localSet<S,_is_auto>& src){
	// ready
	clear();
	const_cast<size_t&>(ignum)=src.ignum;

	// copy
	for(auto it=src.begin();it!=src.end();++it)
		insert(*it);

	return *this;
}

LOCALSET_TEMPLATE
template<class S,bool _is_auto>
LOCALSET& LOCALSET::
moveFrom(localSet<S,_is_auto>&& src){
	// ready
	clear();
	const_cast<size_t&>(ignum)=src.ignum;

	// move
	for(auto it=src.begin();it!=src.end();++it)
		insert(std::move(*it));

	src.clear();
	return *this;
}


LOCALSET_TEMPLATE
const typename LOCALSET::iterator LOCALSET::
findIndex( const mark& note, bool assign) const{
	if(note.validID(ignum))
	{	auto rst=set::find(note);
		if( rst!=end() && rst->index()==note.index() )// found
		{	if(assign) const_cast<mark&>(note)=rst->c_str();
			return rst;
		}
	}
	return end();
}


LOCALSET_TEMPLATE
const typename LOCALSET::iterator LOCALSET::
findStr(const mark& note,bool assign) const{
	for(auto rst=begin();rst!=end();++rst)
		if(note==rst->c_str())
		{	if(assign) const_cast<mark&>(note).setIndex(rst->index());
			return rst;
		}
	return end();
}

LOCALSET_TEMPLATE
const typename LOCALSET::iterator LOCALSET::
find(mark&note,bool checkStr,bool assign,bool enchantid) const{
	bool strValid=note.validStr();
	iterator rst =end();

	if( id==0 || !note.link || !note.linked() ) goto PASS;
	if(id!=note.inBLon) return rst;
	
	PASS:
	if( note.from==this || ( note.link && !note.from ) )// or uninitial
	{	if(note.validID(ignum))
		{	rst=findIndex( note, assign || note.c_str()[0]==signal_querry );
			if( is_auto && checkStr && strValid && rst!=end() )
				if(note!=rst->c_str())
					rst=end();
		}
		if(  checkStr && rst==end() && strValid )
			rst=findStr(note);
	}
	else
		if( !note.link && strValid ) rst=findStr(note);

	if( note.from!=this && rst!=end() )
		note.from=this;

	if( note.link && !note.linked() && enchantid )
		enchantID(note);

	return rst;
}

LOCALSET_TEMPLATE
const typename LOCALSET::iterator LOCALSET::
find( const mark& note, bool checkStr) const{
	bool strValid=note.validStr();
	iterator rst=end();

	if( id==0 || !note.link || !note.linked() ) goto PASS;
	if(id!=note.inBLon) return rst;
	
	PASS:
	rst=findIndex(note);
	if( rst!=end() && strValid )
		if( !is_auto || checkStr && note==rst->c_str() )
			return rst;
	if( checkStr && strValid ) rst=findStr(note,false);

	return rst;
}

LOCALSET_TEMPLATE
template<class S> const typename LOCALSET::iterator LOCALSET::
find(S&&note) const{
	if(!std::is_const<S>::value&&std::is_reference<S>::value)
		return find(note.getMark());
	return find(std::forward<const mark&>(note.getMark()));
}


LOCALSET_TEMPLATE
std::pair<typename LOCALSET::iterator,bool> LOCALSET::
insert(mark&note){
	return insert(const_cast<const mark&>(note));
}


// INSERT
// should keep the property in pre status

LOCALSET_TEMPLATE
std::pair<typename LOCALSET::iterator,bool> LOCALSET::
insert(const mark&note){
	if(!note.valid<is_auto>()) return std::pair<iterator,bool>{end(),false};
	// try insert
	auto thePair=set::insert( enchant(T{ note.c_str(), is_auto?++added:note.index() }) );
	// fail back
	if(!thePair.second)
		--added;
	else if(!note.linked())// success enchant
	{	enchant(note);// enchant: set inBLon, from if need set
		if(is_auto) const_cast<mark&>(note).setIndex(added);
	}
	return thePair;
}

LOCALSET_TEMPLATE
std::pair<typename LOCALSET::iterator,bool> LOCALSET::
insert(mark&&note){
	if(!note.valid<is_auto>()) return std::pair<iterator,bool>{end(),false};
	if(is_auto) note.setIndex(++added);
	auto thePair=set::insert( T{enchant(std::move(note))} );
	if(!thePair.second)
		--added;
	return thePair;
}

// END INSERT


LOCALSET_TEMPLATE
template<class S> std::pair<typename LOCALSET::iterator,bool> LOCALSET::
insert(S&&note){
	// && will be value, & keep in reference
	if(std::is_reference<S>::value)
		return insert(note.getMark());
	else
		return insert(std::move(note.getMark()));
}

LOCALSET_TEMPLATE
template<class... Ts> size_t LOCALSET::
insert(Ts&&... notes){
	size_t count=0;
	char temp[]={(insert(std::forward<Ts>(notes)).second?++count:0,0)...,0};
	return count;
}


// ERASE

LOCALSET_TEMPLATE
size_t LOCALSET::
erase(const mark& note){
	auto temp=find(note);
	if(temp==end()) return 0;
	if( is_auto && temp->index()==added ) --added;
	return set::erase(*temp);
}

// END ERASE


LOCALSET_TEMPLATE
template<class S> size_t LOCALSET::
erase(const S&note){
	return erase(note.getMark());
}

LOCALSET_TEMPLATE
template<class... Ts> size_t LOCALSET::
erase(Ts&&... notes){
	size_t i=0;
	//                                  4                                  3                       2                              1                                 1      2      3      4
	char temp[]={( i+=erase(std::forward<typename std::add_lvalue_reference<typename std::add_const<typename std::remove_reference<typename std::remove_cv<Ts>::type>::type>::type>::type>(notes)) ,0)...,0};
	return i;
}

LOCALSET_TEMPLATE
void LOCALSET::
clear(){
	if(is_auto) added=0;
	set::clear();
}


LOCALSET_TEMPLATE
void LOCALSET::
setIgnum(size_t num){
	ignum=num;
}

LOCALSET_TEMPLATE
BLon LOCALSET::
ID() const{
	return id;
}

LOCALSET_TEMPLATE
void LOCALSET::
print(FILE* stream){
	fprintf(stream,"@ ID %03d  size %zu\n^ [address][%p]\n",id,size(),this);
	for(auto it=begin();it!=end();++it)
	{	fputs("> ",stream);
		it->print(stream);
	}
}

LOCALSET_TEMPLATE
template<class S> S&& LOCALSET::
enchantID(S&& note) const{
	if( note.link && note.inBLon!=id )
		const_cast<typename std::add_lvalue_reference<typename std::remove_cv<typename std::remove_reference<S>::type>::type>::type>(note).inBLon=id;
	return std::forward<S>(note);
}

LOCALSET_TEMPLATE
template<class S> S&& LOCALSET::
enchant(S&& note) const{
	const_cast<typename std::add_lvalue_reference<typename std::remove_cv<typename std::remove_reference<S>::type>::type>::type>(note).from=this;
	return enchantID(std::forward<S>(note));
}

LOCALSET_TEMPLATE
template<class... Ts> void LOCALSET::
enchantID(Ts&&... notes) const{
	char temp={( enchantID(notes) ,0)...,0};
}

LOCALSET_TEMPLATE
template<class... Ts> void LOCALSET::
enchant(Ts&&... notes) const{
	char temp={( enchant(notes) ,0)...,0};
}




}// namespace AIA
