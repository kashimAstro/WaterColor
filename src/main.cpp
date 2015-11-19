#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:
        ofShader wc;
        ofImage img;
        ofImage gray;
        ofVideoPlayer player;
        ofFbo fbo,fboDepth;
        int w,h;
        ofxPanel gui;
        ofParameter<float> stepGradient;
        ofParameter<float> advectStep;
        ofParameter<float> flipHeightMap;
        ofParameter<ofVec4f> advectMatrix;
        ofParameter<float> time;
        ofParameter<bool> switchVideo;

        void setup(){
            ofSetVerticalSync(false);

            w=ofGetScreenWidth();
            h=ofGetScreenHeight();

            ofDisableArbTex();
            img.load("1.jpg");
            player.load("1.mp4");
            player.play();
            player.setLoopState(OF_LOOP_NORMAL);
            gray = img;
            gray.setImageType(OF_IMAGE_GRAYSCALE);
            wc.load("wcolor.vert","wcolor.frag");

            ofFbo::Settings s;
            s.depthStencilAsTexture=true;
            s.useDepth=true;
            s.width=w;
            s.height=h;
            fboDepth.allocate(s);
            fbo.allocate(w,h);
            fbo.begin();
            ofClear(0,0,100,255);
            fbo.end();

            gui.setup();
            gui.add(stepGradient.set("step gradient",    .0015, -1., 1.));
            gui.add(advectStep.set("step advect",        .0015, -.1, .1));
            gui.add(flipHeightMap.set("flip height map",  0.7,   0.,  2.));
            gui.add(time.set("time",  0.,   0.,  1.));
            gui.add(advectMatrix.set("advect matrix",  ofVec4f(0.1),   ofVec4f(-1.),  ofVec4f(1.)));
            gui.add(switchVideo.set("switch video", false));
        }

        void update(){
            ofSetWindowTitle(ofToString(ofGetFrameRate()));
            player.update();
        }

        void draw(){
            wc.begin();
            if(!switchVideo){
                wc.setUniformTexture("colorMap", img.getTexture(),  1);
                wc.setUniformTexture("heightMap",gray.getTexture(), 2);
            }
            else{
                wc.setUniformTexture("colorMap", player.getTexture(), 1);
                fboDepth.begin();
                player.draw(0,0);
                fboDepth.end();
                wc.setUniformTexture("heightMap",fboDepth.getDepthTexture(),2);
            }
            wc.setUniform1f("time", ofGetElapsedTimef()*time);
            wc.setUniform1f("gradientStep", stepGradient);
            wc.setUniform1f("advectStep",   advectStep);
            wc.setUniform1f("flipHeightMap",flipHeightMap);
            wc.setUniform4f("advectMatrix",advectMatrix->w,advectMatrix->x,advectMatrix->y,advectMatrix->z);

            fbo.draw(0,0);

            wc.end();
            img.draw(0,0,img.getWidth()/4,img.getHeight()/4);
            player.draw(img.getWidth()/4,0,img.getWidth()/4,img.getHeight()/4);
            gui.draw();
        }

        void keyPressed(int key){
            if(key == '1')
                img.load("1.jpg");
            if(key == '2')
                img.load("2.jpg");
            if(key == '3')
                img.load("3.jpg");
        }
};

int main( ){
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new ofApp());
}
