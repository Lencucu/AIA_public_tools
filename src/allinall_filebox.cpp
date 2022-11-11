#include"../include/allinall_filebox.hpp"
#include<cassert>
#include<cstring>
#include<cstdio>



namespace AIA{


const char* searchPos(const char*str,const char*symbol,bool atBack){
	str=strstr(str,symbol);
	if(!str) return nullptr;
	if(atBack) str+=strlen(symbol);	
	return str;
}

fileBox::fileBox(const char*filename){
	read(filename);
}


void fileBox::reset(){
	filename.reset();
	data.remove();
}


bool fileBox::read(const char*filename){
	if(filename==nullptr) return false;
	this->filename=filename;

	FILE*file=fopen(filename,"r");
	if(!file) return false;// not found the file

	fseek(file,0,SEEK_END);
	size_t oslenth=ftell(file);
	data.alloc(size_t(oslenth));

	fseek(file,0,SEEK_SET);
	// length at most equal as filemaxsize-1
	data.useSize=fread(data.p,1,oslenth<filemaxsize?oslenth:filemaxsize-1,file);
	data[data.useSize]=0;

	IndListofLine.alloc(oslenth-data.useSize+1);
	++(data.useSize);
	// window can work, linux unknow(file end format)

	fclose(file);
	return true;

}

void fileBox::save(const char*filename){
	if(!record||!data.useSize) return;
	if(!filename) filename=this->filename.c_str();

	FILE*file=fopen(filename,"w");
	fwrite(data.p,1,data.useSize,file);

	fclose(file);

}


size_t fileBox::insert(size_t pos,const char*str,size_t range){
	return data.insert(pos,str,strlen(str),0);
}

size_t fileBox::remove(size_t pos,size_t range){
	return data.remove(pos,range);
}


void fileBox::sort(){
	size_t t=IndListofLine.size();
	size_t linecount=1;
	size_t lastpos=0;

	// get linecount and record info into already memory
	for(size_t i=0,l=0;i<=data.useSize;++i)
	{	if(data[i]=='\n')
		{	++linecount;
			if(l<t)
			{	/*begin*/IndListofLine[l].BeginSide=l?(IndListofLine[l-1].EndSide+1):0;
				/*end*/IndListofLine[l].EndSide=i;
				/*inuse*/IndListofLine[l].inuse=true;
				++l;
				lastpos=i+1;
			}
		}
	}

	// record the left info
	if(linecount<IndListofLine.size())
		IndListofLine[linecount].inuse=false;
	else if(linecount==IndListofLine.size())
		IndListofLine[linecount-1].inuse=true;
	else// t>1
	{	IndListofLine.grow(linecount-t);
		for(size_t i=lastpos;i<=data.useSize;++i)
		{	if(data[i]=='\n')
			{	IndListofLine[t].BeginSide=IndListofLine[t-1].EndSide+1;
				IndListofLine[t].EndSide=i;
				IndListofLine[t].inuse=true;
				++t;
				lastpos=i+1;
			}
		}
	}
	
	// after last '\n'
	IndListofLine[linecount-1].BeginSide=lastpos;
	IndListofLine[linecount-1].EndSide=data.useSize;
	// if last line is empty, then lastpos
	//  will equal as length

}


const char* fileBox::from(){
	return filename.c_str();
}

char* fileBox::content(){
	return data.p;
}

const char* fileBox::content() const{
	return data.p;
}

size_t fileBox::lenth(){
	return data.useSize;
}




}// namespace AIA