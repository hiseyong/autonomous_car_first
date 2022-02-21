import cv2
import numpy as np
import utils
import serial
import time
ser = serial.Serial('COM3', 9600)
ser.write(str(0).encode())
time.sleep(15)
ser.write(str(13).encode())
time.sleep(5)
lastval = 0
curveList = []
avgVal = 10
def getLaneCurve(img,display=2):
    imgcopy = img.copy()
    imgThres = utils.threshholding(img)
    imgResult = imgcopy
    hT,wT,c = img.shape
    points = utils.valTrackbars()
    imgWarp = utils.warpImg(imgThres,points,wT,hT)
    imgWarpPoints = utils.drawPoints(imgcopy,points)
    middlePoint, imgHist = utils.getHistogram(imgWarp, display=True, minPer=0.5, region=4)
    curveAvergaePoint,imgHist = utils.getHistogram(imgWarp, display=True,minPer=0.9)
    curveRaw = curveAvergaePoint - middlePoint
    curveList.append(curveRaw)
    if len(curveList) > avgVal:
        curveList.pop(0)
    curve = int(sum(curveList)/len(curveList))

    if display != 0:
        imgInvWarp = utils.warpImg(imgWarp, points, wT, hT, inv=True)
        imgInvWarp = cv2.cvtColor(imgInvWarp, cv2.COLOR_GRAY2BGR)
        imgInvWarp[0:hT//3,0:wT] = 0,0,0
        imgLaneColor = np.zeros_like(img)
        imgLaneColor[:] = 0, 255, 0
        imgLaneColor = cv2.bitwise_and(imgInvWarp, imgLaneColor)
        imgResult = cv2.addWeighted(imgResult, 1, imgLaneColor, 1, 0)
        midY = 450
        cv2.putText(imgResult, str(curve), (wT // 2 - 80, 85), cv2.FONT_HERSHEY_COMPLEX, 2, (255, 0, 255), 3)
        cv2.line(imgResult, (wT // 2, midY), (wT // 2 + (curve * 3), midY), (255, 0, 255), 5)
        cv2.line(imgResult, ((wT // 2 + (curve * 3)), midY - 25), (wT // 2 + (curve * 3), midY + 25), (0, 255, 0), 5)
        for x in range(-30, 30):
            w = wT // 20
            cv2.line(imgResult, (w * x + int(curve // 50), midY - 10),
                     (w * x + int(curve // 50), midY + 10), (0, 0, 255), 2)
        #fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer);
        #cv2.putText(imgResult, 'FPS ' + str(int(fps)), (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (230, 50, 50), 3);
    if display == 2:
        imgStacked = utils.stackImages(0.7, ([img,imgWarpPoints,imgWarp],
                                             [imgHist,imgLaneColor,imgResult]))
        cv2.imshow('ImageStack', imgStacked)
    elif display == 1:
            cv2.imshow('Resutlt', imgResult)

    curve = curve/100
    if curve>1: curve == 1
    if curve<-1: curve ==-1
    return curve




if __name__ == '__main__':
    cap = cv2.VideoCapture("vid/FullSizeRender.MOV")
    initialTrackBarVals = [31,50,41,156]
    utils.initializeTrackbars(initialTrackBarVals)
    frameCounter = 0
    curveList = []
    while True:
        frameCounter += 1
        if cap.get(cv2.CAP_PROP_FRAME_COUNT) == frameCounter:
            cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
            frameCounter = 0
        succes, img = cap.read()
        img = cv2.resize(img,(480,240))
        curve = getLaneCurve(img,display=2)

        val = str(curve * 100).encode()
        if lastval == val:
            print("same")
        else:
            print(val)
            ser.write(val)
            lastval = val
        cv2.waitKey(1)