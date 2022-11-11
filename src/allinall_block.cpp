#include"../include/allinall_block.hpp"


namespace AIA{




// ************ grow methoud ***************
void GrowMethoud1(size_t&obj,size_t to,size_t max,size_t arg1,size_t arg2){
	// float a=float(arg1);
	// float b=float(arg2);
	// obj=size_t(float(to)*(1.1f-a/b*a/b)*7.5f);
	obj=2*to;
	if(obj>max)obj=max;
}
// *****************************************




}// namespace AIA