namespace AIA{




template<class T_range>
bool rangeRecorder<T_range>::alian(T_range pos,itType& start,itType& thend){
	thend=infobox.lower_bound(pos);
	if(thend==infobox.end()) return false;
	else if(thend->first==pos)
	{	if(!thend->second.endSide) ++thend;
		start=std::prev(thend);
	}
	else if(thend->second.endSide)
		start=std::prev(thend);
	else return false;
	// check logic OK

	return true;
}

template<class T_range>
size_t rangeRecorder<T_range>::play(T_range pos){
	itType start,thend;
	if(!alian(pos,start,thend)) return 0;
	
	if(Func!=nullptr) Func((void*)(start->first),start->second.typeID);
	
	return 1;
}

template<class T_range>
size_t rangeRecorder<T_range>::remove(T_range pos){
	itType start,thend;
	if(!alian(pos,start,thend)) return 0;
	
	AIAclear((void*)(start->first),start->second.typeID);
	
	infobox.erase(start);
	infobox.erase(thend);
	return 1;
}


template<class T_range>
bool rangeRecorder<T_range>::alian(T_range begin,T_range end,itType& start,itType& thend){
	start=infobox.lower_bound(begin);
	if(start==infobox.end()) return false;
	// equal & left,keep
	// equal & right,--
	// unequal & right,--
	// unequal & left if outside return else keep
	if(start->first>end&&!start->second.endSide) return false;
	if(start->second.endSide) --start;
	thend=infobox.upper_bound(end);
	if(thend==infobox.end()||!thend->second.endSide) --thend;
	// check logic OK

	return true;
}

template<class T_range>
size_t rangeRecorder<T_range>::play(T_range begin,T_range end){
	if(begin>=end||!infobox.size()) return 0;

	itType start,thend;
	if(!alian(begin,end,start,thend)) return 0;
	
	size_t count=0;
	for(itType temp=start;temp!=thend;++temp)
	{	if(!temp->second.endSide)
		{	++count;
			if(Func!=nullptr) Func((void*)(temp->first),temp->second.typeID);
		}
	}
	
	return count;
}

template<class T_range>
size_t rangeRecorder<T_range>::remove(T_range begin,T_range end){
	if(begin>=end||!infobox.size()) return 0;

	itType start,thend;
	if(!alian(begin,end,start,thend)) return 0;

	size_t count=0;
	for(itType temp=start;temp!=thend;++temp)
	{	if(!temp->second.endSide)
		{	++count;
			AIAclear((void*)(temp->first),temp->second.typeID);
		}
	}
	
	infobox.erase(start,++thend);
	return count;
}


template<class T_range>
void rangeRecorder<T_range>::insert(T_range begin,T_range end,size_t typeID){
	remove(begin,end);
	infobox[begin]=posinfo{false,typeID};
	infobox[end]=posinfo{true,typeID};
}


template<class T_range>
void rangeRecorder<T_range>::setGlobalFunc(AOfunc func){
	Func=func;
}

template<class T_range>
void* rangeRecorder<T_range>::getptr(size_t typeID,size_t ind){
	for(itType it=infobox.begin();it!=infobox.end();++it)
	{	if(!it->second.endSide&&it->second.typeID==typeID)
		{	if(!ind) return (void*)(it->first);
			else --ind;
		}
	}
	return nullptr;
}


template<class T_range>
size_t rangeRecorder<T_range>::size(){
	return infobox.size()/2;
}




}// namespace AIA