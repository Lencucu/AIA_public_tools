#if !defined(AIA_CLEANER) || defined(AIA_END)

#ifndef AIA_END
#include<map>
#else
#include<typeinfo>
#endif


namespace AIA{

#ifndef AIA_END








extern void AIAclear(void*,size_t hash);




struct posinfo{
	bool endSide;
	size_t typeID;
};




//					ptr_obj typeID
typedef void(*AOfunc)(void*,size_t);

// T_range must have:
// operator==
// operator void*
// cmp methoud
template<class T_range>
class rangeRecorder{
	typedef typename std::map<T_range,posinfo> InfoBox;
	// ---- release ----
	InfoBox* const data=new InfoBox;
	bool released=false;
	void release(){delete data;released=true;}
	friend void AIAclear(void*,size_t);
	// ---- ------- ----
	InfoBox& infobox=*data;
	AOfunc Func=nullptr;

public:
	~rangeRecorder(){release();};
public:
	typedef typename InfoBox::iterator itType;

	bool alian(T_range,itType&,itType&);
	size_t play(T_range);
	size_t remove(T_range);

	bool alian(T_range,T_range,itType&,itType&);
	size_t play(T_range,T_range);
	size_t remove(T_range,T_range);

	void insert(T_range,T_range,size_t typeID);

	void setGlobalFunc(AOfunc);
	void* getptr(size_t,size_t);

	size_t size();


};

typedef rangeRecorder<size_t> rangeRecorderdft;





#define AIA_CLEANER
#else








// conversion: use release func as dstr interface
#define AIA_CLEAR_BLOCK(type) if(typeID==typeid(type).hash_code())\
((type*)p)->release();\
goto AIAclearEND;

void AIAclear(void*p,size_t typeID){
	#ifdef AIA_STRPOOL
	AIA_CLEAR_BLOCK(mark);
	AIA_CLEAR_BLOCK(originstr);
	//...
	#endif
	//...

	
	AIAclearEND:
	return;
}








#endif

}// namespace AIA




#ifndef AIA_END
#include"inl/allinall_cleaner.inl"
#endif

#endif