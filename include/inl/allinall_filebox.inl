namespace AIA{


template<class... Ts>
const char* searchStr(const char*str,const char*symbol,const char*match,char endchr,Ts&&... slots){
	if(!sizeof...(Ts)) return nullptr;

	if(!(str=searchPos(str,symbol,true)))
		return nullptr;
	
	const char*endSide=strchr(str,endchr);
	if(!endSide) return nullptr;
	
	// allright
	size_t length=size_t(endSide)-size_t(str);
	char temp[]={ (slots.alloc(length),0)... ,0};

	// read into slots
	sscanf(str,match,slots.ptr()...);

	return str;
}




// fileBox
template<class... Ts>
size_t fileBox::insert(size_t pos,Ts... strs){
	size_t count=0;
	size_t wordlength=0;
	char temp[]={( count+=data.insert(pos,strs,wordlength=strlen(strs),0) , pos+=wordlength, 0)...,0};
	return count;
}

template<class... Ts>
size_t fileBox::remove(size_t pos,Ts... ranges){
	size_t count=0;
	char temp[]={( count+=data.remove(pos,ranges) ,0)...,0};
	return count;
}




}// namespace AIA