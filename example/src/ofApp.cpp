#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setup();
    kinect.addDepthGenerator();
    kinect.addImageGenerator();
    kinect.setRegister(false);
    kinect.setMirror(true);
    kinect.addUserGenerator();
    kinect.setMaxNumUsers(3);
    kinect.setSkeletonProfile(XN_SKEL_PROFILE_ALL);
    kinect.start();
    hadUsers = false;
    
    ofAddListener(kinect.userEvent, this, &ofApp::userEvent);
    
    ofSetWindowShape(640, 480);
    
    font.loadFont("verdana.ttf", 18);
    
    j = JOINT_RIGHT_HAND;
    f = VELOCITY_MEAN;
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();

    for (int i = 0; i < kinect.getNumTrackedUsers(); i++) {
        ofxOpenNIUser user = kinect.getTrackedUser(i);
        //The following "if" statement is a hard-coded alternative for if(kinect.getUserGenerator().IsNewDataAvailable()), which doesn't work properly in ofxOpenNI
        if (user.getJoint((Joint)0).getWorldPosition() != ofPoint(0,0,0) &&
            (!featExtractor.skeletonExists(0) ||
             user.getJoint((Joint)0).getWorldPosition() != featExtractor.getSkeleton(0)->getPosition(0) )) {
            map<int, ofPoint> joints;
            for (int j = 0; j < user.getNumJoints(); j++) {
                joints[j] = user.getJoint((Joint)j).getWorldPosition();
            }
            featExtractor.updateSkeleton(i, joints);
        }
    }
        
    //This is a trick to reset the user generator if all users are lost
    if (kinect.getNumTrackedUsers()) {
        hadUsers = true;
    } else if (!kinect.getNumTrackedUsers() && hadUsers){
        hadUsers = false;
        kinect.setPaused(true);
        kinect.removeUserGenerator();
        kinect.addUserGenerator();
        kinect.setPaused(false);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(ofColor::white);
    float w = ofGetWidth(); //w
    float h = ofGetHeight(); //h
    
    //image
    kinect.drawImage();
    kinect.drawSkeletons();
    
    ostringstream os;
    os << "ofxKinectFeatures example " << endl;
    os << "FPS: " << ofGetFrameRate() << endl;
    if (featExtractor.skeletonExists(0)) {
        ofPoint jointProjectivePosition = kinect.worldToProjective(featExtractor.getSkeleton(0)->getPosition((Joint)j));
        os << "Quantity of Motion: " << featExtractor.getSkeleton(0)->getQom() << endl;
        os << "Symmetry: " << featExtractor.getSkeleton(0)->getSymmetry() << endl;
        os << "Contraction Index: " << featExtractor.getSkeleton(0)->getCI() << endl << endl;
        os << "Current joint (left-right to change): " << getJointAsString((Joint)j) << endl;
        os << "Current feature (up-down to change): ";
        switch (f) {
            case VELOCITY_MEAN:
                os << "Velocity magnitude mean" << endl;
                font.drawString(ofToString(featExtractor.getSkeleton(0)->getVelocityMean((Joint)j)), jointProjectivePosition.x, jointProjectivePosition.y);
                break;
            case ACCELERATION_Y:
                os << "Acceleration along y axis (up-down movement)" << endl;
                font.drawString(ofToString(featExtractor.getSkeleton(0)->getAcceleration((Joint)j).y), jointProjectivePosition.x, jointProjectivePosition.y);
                break;
            case RELPOSTOTORSO_X:
                os << "Relative position to torso in x axis" << endl;
                font.drawString(ofToString(featExtractor.getSkeleton(0)->getRelativePositionToTorso((Joint)j).x), jointProjectivePosition.x, jointProjectivePosition.y);
                break;
                
            default:
                break;
        }
    }
    
    ofSetColor(0,0,0,100);
    ofRect(10, 10, 500, 150);
    ofSetColor(255,255,255);
    ofDrawBitmapString(os.str(), 20, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_RIGHT:
            if (j < JOINT_COUNT-1) {
                j++;
            }
            break;
        case OF_KEY_LEFT:
            if (j > 0){
                j--;
            }
            break;
        case OF_KEY_UP:
            if (f < RELPOSTOTORSO_X) {
                f++;
            }
            break;
        case OF_KEY_DOWN:
            if (f > 0){
                f--;
            }
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::userEvent(ofxOpenNIUserEvent &event){
    if (event.userStatus == USER_TRACKING_STOPPED) {
        featExtractor.removeSkeleton(0);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}