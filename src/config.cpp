#include "config.h"
#include "interface/experimentInstructions.h"

static configuration config;

configuration & cfg(){
	return config;
};

void configuration::readGeneralConfig(string cfgFile){
	ifstream config(ofToDataPath(cfgFile).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=\r\n");
		if(token.size()){
			/*if(token[0]=="DESTINATION_FOLDER"){
				dest=token[1];
			}
			else if(token[0]=="STARTING_NUMBER"){
				startingNumber=ofToInt(token[1]);
			}
			else if(token[0]=="NUM_SETS"){
				numSets=ofToInt(token[1]);
			}
			else if(token[0]=="FOLDER_ROOT_NAME"){
				folderRoot=token[1];
			}
			else if(token[0]=="FILE_ROOT_NAME"){
				fileRoot=token[1];
			}*/
			if(token[0]=="FULLSCREEN"){
				ofSetFullscreen(ofToInt(token[1]));			//set fullscreen mode 
			}
			else if(token[0]=="LOOP"){
				loopVideo=ofToInt(token[1]);			//set fullscreen mode 
			}
			else if(token[0]=="AUTO_PLAY"){
				autoPlay=ofToInt(token[1]);			//set fullscreen mode 
			}
			else if(token[0]=="AUTO_PLAY_DELAY"){
				autoPlayDelay=ofToFloat(token[1]);			//set fullscreen mode 
			}
			else if(token[0]=="AUTO_MODE"){
				autoMode=ofToInt(token[1]);
			}
			/*else if(token[0]=="FRAME_SIZE"){
				camWid=ofToInt(token[1].substr(0,token[1].find('x')));
				camHgt=ofToInt(token[1].substr(token[1].find('x')+1));
			}
			else if(token[0]=="FRAMES_PER_SECOND"){
				framesPerSec=ofToInt(token[1]);
			}*/
			else if(token[0]=="TIME_TO_RECORD"){
				recordTime=ofToFloat(token[1]);
			}
			else if(token[0]=="FALL_PERCENT"){
				fallPercent=ofToFloat(token[1]);
			}
			else if(token[0]=="TIME_TO_RAISE"){
				raiseTime=ofToFloat(token[1]);
			}
			else if(token[0]=="EXPERIMENT"){
				select().addExperiment(token[1]);
			}
			else if(token[0]=="CAMERA_GAIN"){
				cameraGain=ofToFloat(token[1]);
			}
		}
	}
	config.close();
}