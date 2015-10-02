#pragma once
#include "ofMain.h"
#include "ofExtended.h"

class dualLangImg {
	ofImage eng;
	ofImage esp;
public:
	dualLangImg(){};
	void load(string englsh, string spnsh){
		eng.loadImage(englsh);
		esp.loadImage(spnsh);
	}
	void draw(int x, int y, int w, int h);
};

class instruct {
public:
	dualLangImg img;
	ofButton select;
	instruct(string folder){
		img.load("experiments/"+folder+"/instEnglish.png","experiments/"+folder+"/instSpanish.png");
		select.setup(256,256,"experiments/"+folder+"/btnInactive.png","experiments/"+folder+"/btnActive.png");
	}
};

class instGroup {
public:
	vector<instruct> insts;
	int active;
	instGroup();
	void addExperiment(string folder);
	void drawButtons(int x, int y, int w, int h,int pad=20);
	void drawInst(int x, int y, int w, int h);
	bool clickDown(int x, int y);
};

instGroup & select();