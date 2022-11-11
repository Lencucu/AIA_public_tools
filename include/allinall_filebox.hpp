// [src_rely] allinall_filebox.cpp
#ifndef AIA_FILEBOX
#include<cstdint>
#include<queue>
#include<deque>
#include"allinall_markset.hpp"// originstr
#include"allinall_block.hpp"// block

namespace AIA{





const char* searchPos(const char* str,const char* symbol,bool atBack =false);
template<class... Ts>
extern const char* searchStr(const char* str, const char* symbol, const char* match, char endchar, Ts&&... blockstrSlots);




constexpr size_t filemaxsize=65536;
// constexpr size_t opSlotCount=32;

// enum class OPtype{
// 	insert,
// 	remove
// };

// type|global ind|insert str|delete size|row|col
// typedef attribute<OPtype,size_t,originstr,size_t,size_t,size_t> FileOP;



struct fileLine{
	size_t BeginSide=0;
	size_t EndSide=0;
	bool inuse=false;
};



class fileBox{
protected:
	originstr filename;
	bool record=false;

	blockstr data;
	
	block<fileLine> IndListofLine;
	// std::queue<FileOP> fileop;
	// std::deque<FileOP> tempRz;

	void reset();
public:
	fileBox(const char* =nullptr);

	bool read(const char*);
	void save(const char* =nullptr);

	// edit
	size_t insert(size_t,const char*,size_t=0);
	size_t remove(size_t,size_t);
	template<class... Ts> size_t insert(size_t,Ts...);
	template<class... Ts> size_t remove(size_t,Ts...);

	// after read/edit
	// make edit to data
	void sort();

	const char*from();
	char*content();
	const char* content() const;
	size_t lenth();
	// number of line
};




}// namespace AIA




#include"inl/allinall_filebox.inl"

#define AIA_FILEBOX
#endif