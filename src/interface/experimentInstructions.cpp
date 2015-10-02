#include "experimentInstructions.h"
#include "../config.h"

static instGroup sel;

void dualLangImg::draw(int x, int y, int w, int h){
	if(h>eng.height*w/eng.width) h=eng.height*w/eng.width;
	if(cfg().english) eng.draw(x,y,w,h);
	else esp.draw(x,y,w,h);
}

instGroup & select(){
	return sel;
}

instGroup::instGroup(){
	active=-1;
	insts.clear();
}

void instGroup::addExperiment(string folder){
	active=insts.size();
	insts.push_back(instruct(folder));
	for(unsigned int i=0; i<insts.size(); i++){
		if(i!=active) insts[i].select.setPressed(false);
	}
}

void instGroup::drawButtons(int x, int y, int w, int h,int pad){
	int wid = w-pad*(insts.size()-1);
	for(unsigned int i=0; i<insts.size(); i++){
		insts[i].select.draw(x+i*(wid/insts.size()+pad),y,wid/insts.size(),h);
	}
}

void instGroup::drawInst(int x, int y, int w, int h){
	insts[active].img.draw(x,y,w,h);
}

bool instGroup::clickDown(int x, int y){
	int ret=0;
	for(unsigned int i=0; i<insts.size(); i++){
		if(insts[i].select.over(x,y)){
			for(unsigned int j=0; j<insts.size(); j++){
				if(i!=j) insts[j].select.setPressed(false);
			}
			active=i;
			ret=i+1;
			insts[i].select.setPressed(true);
		}
	}
	return ret;
}