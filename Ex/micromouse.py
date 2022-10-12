#! /usr/bin/env python

######################
#####Techfest2021#####
######################

#MC-210665
#Robotics Society University of Peradeniya

import rospy
from sensor_msgs.msg import LaserScan
from sensor_msgs.msg import JointState
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from tf.transformations import euler_from_quaternion
import time
import math
import numpy as np
import matplotlib.pyplot as plt
from Bezier import Bezier


##declare the size of the maze
ROWS = 16 
COLUMNS = 16

##declare directions
NORTH = 0
EAST =1
SOUTH =2
WEST =3

#initialize matices
values=np.zeros((ROWS,COLUMNS))
verticalWalls=np.zeros((ROWS,COLUMNS+1))
horizontalWalls=np.zeros((ROWS+1,COLUMNS))

#initialize parameters
base_speed=0.4
trn_speed=1.5
fwdStepSize=0.15275#0.148#9.55 9.525
trnStepSize=3.5

#some global variables
perL_err=0
lftSensorCount=0
perR_err=0
rgtSensorCount=0

runCount=0

initial_lft_val =0.084
initial_fnt_val = 0.075
initial_rgt_val =0.084

lft_tresh=0.12
fnt_tresh=0.12
rgt_tresh=0.12

def calibrate():
  print 'calibrating........'
  global initial_lft_val
  global initial_rgt_val
  global lft_tresh
  global rgt_tresh
  correctOri()
  sumL=0
  sumR=0
  for i in range(100):
    lft,fnt,rgt=getLaserData()
    sumL=sumL+lft
    sumR=sumR+rgt

  initial_lft_val=((sumL/100) + (sumR/100))/2
  initial_rgt_val=initial_lft_val
  lft_tresh=initial_lft_val*1.4
  rgt_tresh=initial_lft_val*1.4
  print 'calibration done'

def getLaserData():
    laserData = rospy.wait_for_message("/my_mm_robot/laser/scan", LaserScan, timeout=None)
    readings=[0,0,0]
    readings[0]=laserData.ranges[359]
    readings[1]=laserData.ranges[180]
    readings[2]=laserData.ranges[0]
    return readings[0] , readings[1],readings[2]

def getEncoderReading():
    encoderReading = rospy.wait_for_message("/joint_states", JointState, timeout=None)
    readings=[0,0]
    readings[0]=encoderReading.position[1]
    readings[1]=encoderReading.position[0]
    return readings[0] , readings[1]

def getPos():
  positions=[0,0]
  odomMsg = rospy.wait_for_message("/odom", Odometry, timeout=None)
  pos=odomMsg.pose.pose.position
  positions[0]= pos.x
  positions[1]= pos.y
  return positions

def takeMouseHeading():
  odomMsg = rospy.wait_for_message("/odom", Odometry, timeout=None)
  orientation_q = odomMsg.pose.pose.orientation
  orientation_list = [orientation_q.x, orientation_q.y, orientation_q.z, orientation_q.w]
  (roll, pitch, yaw) = euler_from_quaternion (orientation_list)
  yawDegree = yaw*180/math.pi
  mouseHeading=10000
  if yawDegree<45 and yawDegree>-45:
    mouseHeading=0
  elif yawDegree<-45 and yawDegree>-135 :
    mouseHeading=1
  elif yawDegree<-135 or yawDegree>135:
    mouseHeading=2
  elif yawDegree<135 and yawDegree>45:
    mouseHeading=3

  return mouseHeading , yawDegree ,yaw

def mtr_cmd(base_speed,angularZ):
    pub=rospy.Publisher('/cmd_vel',Twist,queue_size=10) 
    msg=Twist()
    msg.linear.x=base_speed
    msg.angular.z=angularZ
    pub.publish(msg)

def ir_or_enc_pid() :
    global perL_err
    global perR_err

    global lftSensorCount
    global rgtSensorCount

    kp = 0.01 *0.6 #0.01>>oscillation  *0.6
    kd = 0.05 #0.15
    kp_com = 0.2

    lft,fnt,rgt=getLaserData()

    if lft < lft_tresh:
      lftSensorCount=lftSensorCount+1
    elif rgt< rgt_tresh:
      rgtSensorCount=rgtSensorCount+1
    else:
      rgtSensorCount=0
      lftSensorCount=0
      

    if (lft < lft_tresh and lftSensorCount>3):
      rgtSensorCount=0
      perR_err=0
      #following the left wall
      L_err = lft - initial_lft_val
      L_diff = 500*L_err * kp + (500*L_err - 500*perL_err)*kd
      
      mtr_cmd(base_speed,L_diff)
      perL_err = L_err
    
    elif (rgt< rgt_tresh and rgtSensorCount>3):
      lftSensorCount=0
      perL_err=0  
      #following the right wall
      R_err = rgt - initial_rgt_val
      R_diff = 500*R_err * kp + (500*R_err -500*perR_err)*kd
      
      mtr_cmd(base_speed,-R_diff)
      perR_err = R_err
    
    else:
      
      perL_err=0 
      perR_err=0
      mouseHeading , yawDegree ,yaw = takeMouseHeading()
      if(mouseHeading==0):
        err=-yaw

      elif(mouseHeading==1):
        err=-90*math.pi/180-yaw
      
      elif(mouseHeading==2 and yawDegree > 160):
        err=math.pi- yaw

      elif mouseHeading==2 and yawDegree < -160:
        err=-math.pi-yaw
      
      elif(mouseHeading==3):
        err=90*math.pi/180-yaw
      
      diff=err*kp_com
      mtr_cmd(base_speed,diff)
         
def check():
    lft,fnt,rgt=getLaserData()
    if (fnt < fnt_tresh):#when a wall apears infront 
      fnt = 1
    
    else: 
      fnt = 0
    

    if (lft < lft_tresh):#when a wall apears on left 
      lft = 2
    
    else:
      lft = 0
    
    if (rgt < rgt_tresh):#when a wall apears on right
      rgt = 4
    
    else: 
      rgt = 0
    
    
    condition = fnt + lft + rgt
    return condition
  

def step_fwd():

    correctOri()
    
    init_pos=np.array(getPos())
    delta_dis=0

    while(fwdStepSize>delta_dis):

        delta_pos=np.subtract(np.array(getPos()),init_pos)
        delta_dis=np.linalg.norm(delta_pos)

        lft,fnt,rgt=getLaserData()
        if fnt<=initial_fnt_val:
          break
        ir_or_enc_pid()
    mtr_cmd(0,0)

def trn_lft():

  correctOri()
  
  global perL_err
  global perR_err
  lftEnc,rgtEnc=getEncoderReading()
  targetVal= rgtEnc+trnStepSize
  while(targetVal>rgtEnc):
    lftEnc,rgtEnc=getEncoderReading()
    mtr_cmd(0,trn_speed)
  mtr_cmd(0,0)
  perL_err=0 
  perR_err=0

def trn_rgt():

  correctOri()

  global perL_err
  global perR_err
  lftEnc,rgtEnc=getEncoderReading()
  targetVal= lftEnc+trnStepSize
  while(targetVal>lftEnc):
    lftEnc,rgtEnc=getEncoderReading()
    mtr_cmd(0,-trn_speed)
  mtr_cmd(0,0)
  perL_err=0 
  perR_err=0

def u_trn():

  correctOri()

  global perL_err
  global perR_err
  lftEnc,rgtEnc=getEncoderReading()
  targetVal= rgtEnc+trnStepSize*2
  while(targetVal>rgtEnc):
    lftEnc,rgtEnc=getEncoderReading()
    mtr_cmd(0,trn_speed)
  mtr_cmd(0,0)
  perL_err=0 
  perR_err=0

def correctCumiErr():
  kp=10
  while True:
    lft,fnt,rgt=getLaserData()
    err= fnt -initial_fnt_val
    
    if err < 0.005 and err > -0.005:
      break
    mtr_cmd(kp*err,0)
  mtr_cmd(0,0)

def correctOri():
  kp_com = 2
  while True:
    mouseHeading , yawDegree ,yaw = takeMouseHeading()
    if(mouseHeading==0):
      err=-yaw

    elif(mouseHeading==1):
      err=-90*math.pi/180-yaw
        
    elif(mouseHeading==2 and yawDegree > 160):
      err=math.pi- yaw

    elif mouseHeading==2 and yawDegree < -160:
      err=-math.pi-yaw
        
    elif(mouseHeading==3):
      err=90*math.pi/180-yaw
    
    if err < 0.05 and err > -0.05:
      break
    diff=err*kp_com
    mtr_cmd(0,diff)
  mtr_cmd(0,0)

def NanoMouseMaze():
    global verticalWalls
    global horizontalWalls
    #initialize verticalWalls (add exterior walls)
    for i in range(ROWS):
        for j in range (COLUMNS + 1):
            if (j == 0 or j == COLUMNS):
                verticalWalls[i][j] = 1
          
        
      

    #initialize horizontalWalls (add exterior walls)
    for i in range(ROWS + 1):
        for j in range(COLUMNS):
          if (i == 0 or i == ROWS):
            horizontalWalls[i][j] = 1

def addwalls(dir) :
    global verticalWalls
    global horizontalWalls
    global mouseRow
    global mouseColumn
    if (dir == 0 or dir == 4):
        horizontalWalls[mouseRow][mouseColumn] = 1
      
    elif (dir == 1):
        verticalWalls[mouseRow][mouseColumn + 1] = 1
      
    elif (dir == 2):
        horizontalWalls[mouseRow + 1][mouseColumn] = 1
      
    elif (dir == 3 or dir == -1):
        verticalWalls[mouseRow][mouseColumn] = 1
              
#FloodFill Algorithm
def solve():
    global verticalWalls
    global horizontalWalls
    global values
    
    for i in range(ROWS):
        for j in range(COLUMNS):
            if (i == targetRow and j == targetColumn):
                values[i][j] = 0
          
            else :
                values[i][j] = 255
          
        
      
    count = 1
    foold_val = 0
    while (count != 0) :
        count = 0
        for i in range(ROWS):
            for j in range(COLUMNS):
                if (values[i][j] == foold_val):
                    if (horizontalWalls[i][j] == 0 and values[i - 1][j] > foold_val and (i - 1) >= 0):
                        values[i - 1][j] = foold_val + 1
              
                    if (horizontalWalls[i + 1][j] == 0 and values[i + 1][j] > foold_val and (i + 1) < ROWS) :
                        values[i + 1][j] = foold_val + 1
              

                    if (verticalWalls[i][j] == 0 and values[i][j - 1] > foold_val and (j - 1) >= 0) :
                        values[i][j - 1] = foold_val + 1
              
                    if (verticalWalls[i][j + 1] == 0 and values[i][j + 1] > foold_val and (j + 1) < ROWS) :
                        values[i][j + 1] = foold_val + 1
              
            
                elif (values[i][j] > foold_val) :
                    count=count+1
            
          
        
        foold_val=foold_val+1
      
def printRes():
    global verticalWalls
    global horizontalWalls
    global values
    global mouseHeading
    global mouseRow
    global mouseColumn
    print "################### MAP ######################"
    
    for i in range(2*ROWS + 1):
        #strLine=[]
        for j in range (2*COLUMNS + 1):
        
              
            #Add Horizontal Walls
            if (i % 2 == 0 and j % 2 == 1):
          
                if (horizontalWalls[int(i / 2)][int(j / 2)] == 1):
            
                    print " ---",
            
                else:
            
                    print "    ",
            
          

            #Add Vertical Walls
            if (i % 2 == 1 and j % 2 == 0):
          
                if (verticalWalls[int(i / 2)][int(j / 2)] == 1):
            
                    print"|", 
            
                else:
            
                    print"", 
            
          
          
            #Add Flood Fill Values
            if (i % 2 == 1 and j % 2 == 1):
          
                if (int((i - 1) / 2) == mouseRow and int((j - 1) / 2) == mouseColumn):
            
                    if (mouseHeading == NORTH):
              
                        print" ^ ",
              
                    elif (mouseHeading == EAST):
              
                        print" > ",
              
                    elif (mouseHeading == SOUTH):
              
                        print" v ", 
              
                    elif (mouseHeading == WEST):
              
                        print" < ", 
              
            
                else:
            
                    value = values[int((i - 1) / 2)][int((j - 1) / 2)]
                    if (value >= 100):
              
                        print str(int(value)),
              
                    else:
              
                        print'', 
                        print str(int(value)), 
              
                    if (value < 10):
                        
                        print'', 
        
        print""

def updateMap():
  global mouseRow
  global mouseColumn
  global mouseHeading

  mouseHeading,_,_ =takeMouseHeading()
            
  condition=check()
  

  if (condition==1):
    addwalls(mouseHeading)
    
  elif (condition==2):
    addwalls(mouseHeading - 1)
    
  elif (condition==3):
    addwalls(mouseHeading)
    addwalls(mouseHeading-1)

  elif (condition==4):
    addwalls(mouseHeading + 1)
    
  elif (condition==5):
    addwalls(mouseHeading + 1)
    addwalls(mouseHeading)
    
  elif (condition==6):
    addwalls(mouseHeading + 1)
    addwalls(mouseHeading - 1)
    
  elif (condition==7):
    addwalls(mouseHeading + 1)
    addwalls(mouseHeading - 1)
    addwalls(mouseHeading)
    correctCumiErr()
    u_trn()
   
  #time.sleep(0.00005)
  
  solve()
  

if __name__ == '__main__':
    try:
        rospy.init_node('Micromouse')

        global mouseRow
        global mouseColumn
        global mouseHeading
        global targetRow
        global targetColumn

        journeyFlag=0#to middle

        #declare the destination
        desRow=8
        desColumn=8
        #declare the target
        targetRow = desRow
        targetColumn = desColumn

        #declare the starting
        init_mouseRow=15
        init_mouseColumn=15

        mouseHeading,_,_ =takeMouseHeading()
        mouseRow=init_mouseRow
        mouseColumn=init_mouseColumn

        #calibrate()
        #print initial_lft_val , initial_rgt_val ,lft_tresh,rgt_tresh
        NanoMouseMaze()

        time.sleep(2)


        waypoints=[]
        ############################                            #########################
        ############################ Begining of the Search runs#########################
        ############################                            #########################
        while runCount <4:
            if runCount < 2:
              updateMap()
            elif runCount ==2:
              # start to record waypoints on the shortest path 
              currentWaypoint=getPos()
              waypoints.append(currentWaypoint)

            mouseHeading,_,_ =takeMouseHeading()

            #checking current condition
            condition=check()

            if (condition==0):
              if mouseHeading==0:

                if values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseRow=mouseRow-1

                elif values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseColumn=mouseColumn-1
                elif values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow+1

          
              elif mouseHeading==1:

                if values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseColumn=mouseColumn+1

                elif values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseRow=mouseRow-1
                elif values[mouseRow+1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseRow=mouseRow+1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn-1

              elif mouseHeading==2:

                if values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseRow=mouseRow+1

                elif values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                elif values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseColumn=mouseColumn-1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow-1
    
              elif mouseHeading==3:

                if values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseColumn=mouseColumn-1

                elif values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseRow=mouseRow+1
                elif values[mouseRow-1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseRow=mouseRow-1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn+1
              else:
                pass
            

            elif (condition==1):
              correctCumiErr()
              
              if mouseHeading==0:
                if values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseColumn=mouseColumn-1
                elif values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow+1

          
              elif mouseHeading==1:
                if values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseRow=mouseRow-1
                elif values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseRow=mouseRow+1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn-1


              elif mouseHeading==2:
                if values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                elif values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseColumn=mouseColumn-1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow-1
    
              elif mouseHeading==3:
                if values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseRow=mouseRow+1
                elif values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseRow=mouseRow-1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn+1

              else:
                pass


            elif (condition==2):
              if mouseHeading==0:

                if values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseRow=mouseRow-1

                elif values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow+1
                  

          
              elif mouseHeading==1:
                
                if values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseColumn=mouseColumn+1
                
                elif values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseRow=mouseRow+1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn-1

                  


              elif mouseHeading==2:

                if values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseRow=mouseRow+1

                elif values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseColumn=mouseColumn-1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow-1
                  
    
              elif mouseHeading==3:

                if values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseColumn=mouseColumn-1

                elif values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseRow=mouseRow-1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                  
              else:
                pass

            elif (condition==3):
              correctCumiErr()
              if mouseHeading==0:
                if values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow+1

          
              elif mouseHeading==1:
                if values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseRow=mouseRow+1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn-1


              elif mouseHeading==2:
                if values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseColumn=mouseColumn-1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow-1
    
              elif mouseHeading==3:
                if values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_rgt()
                  step_fwd()
                  mouseRow=mouseRow-1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn+1
              else:
                pass

            elif (condition==4):
              if mouseHeading==0:
                if values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseRow=mouseRow-1
                
                elif values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseColumn=mouseColumn-1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow+1
                  
          
              elif mouseHeading==1:

                if values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseColumn=mouseColumn+1

                elif values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseRow=mouseRow-1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn-1



              elif mouseHeading==2:
                if values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseRow=mouseRow+1

                elif values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow-1
                  
    
              elif mouseHeading==3:
                if values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  step_fwd()
                  mouseColumn=mouseColumn-1

                elif values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseRow=mouseRow+1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn+1
              else:
                pass


            elif (condition==5):
              correctCumiErr()
              if mouseHeading==0:
                if values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseColumn=mouseColumn-1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow+1

          
              elif mouseHeading==1:
                if values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseRow=mouseRow-1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn-1


              elif mouseHeading==2:
                if values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseColumn=mouseColumn+1
                else:
                  u_trn()
                  step_fwd()
                  mouseRow=mouseRow-1
    
              elif mouseHeading==3:
                if values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1:
                  trn_lft()
                  step_fwd()
                  mouseRow=mouseRow+1
                else:
                  u_trn()
                  step_fwd()
                  mouseColumn=mouseColumn+1
              else:
                pass

            elif (condition==6):
              step_fwd()
              if mouseHeading==0:
                mouseRow=mouseRow-1
          
              elif mouseHeading==1:
                mouseColumn=mouseColumn+1

              elif mouseHeading==2:
                mouseRow=mouseRow+1
    
              elif mouseHeading==3:
                mouseColumn=mouseColumn-1
              else:
                pass

            elif (condition==7):
              
              correctCumiErr()
              u_trn()

            #mouseHeading,_,_ =takeMouseHeading()
            #printRes()

            #To Switch the target
            if mouseRow== targetRow  and mouseColumn==targetColumn:
              if runCount ==2:
                currentWaypoint=getPos()
                waypoints.append(currentWaypoint)

              runCount= runCount+1
              if journeyFlag==0:
                targetRow=init_mouseRow
                targetColumn=init_mouseColumn
                journeyFlag=1
              elif journeyFlag==1:
                targetRow=desRow
                targetColumn=desColumn
                journeyFlag=0
              solve()

        ############################                            #########################
        ############################ End of the Search runs     #########################
        ############################                            #########################

        ############################                            #########################
        ############################ Begining of the Fast run   #########################
        ############################                            #########################
        if runCount ==4:

          correctCumiErr()
          u_trn()
          correctOri()
          
          #Start Interpolate waypoints 
          reqPoints=20
          waypoints_np=np.array(waypoints)
          points = np.zeros((2,2))
          t_points = np.linspace(0,1,reqPoints)
          out_points_x=[] 
          out_points_y=[] 

          for j in range (0,len(waypoints)-1):
                
                  for i in range (0,2):
                          points[i] = [waypoints[j+i][0],waypoints[j+i][1]]
                          B=Bezier()
                          curve1 = B.Curve(t_points,points)
                          
                          out_points_x.append(curve1[:,0])
                          out_points_y.append(curve1[:,1])

                  if(i+j == len(waypoints)-1):
                          break
  
          for i in range (len(out_points_x)):
                  for j in range (reqPoints):
                          waypoints=list(waypoints)
                          waypoints.append([out_points_x[i][j],out_points_y[i][j]])
          waypoints=np.array(waypoints)  

          ##visualize results
          #print(waypoints)
          # plt.plot(
          #   waypoints[:, 0],  # x-coordinates.
          #   waypoints[:, 1],  # y-coordinates.
          #   'ro:'             # Styling (red, circles, dotted).
          # )
          # plt.grid()
          # plt.show() 
          ##End Interpolate waypoints

          #Lateral Controller parameters
          kp_lat=0.5
          kd_lat = 0.075
          ki_lat =0
          previous_lat_err = 0

          #Collision avoidance controller parameters
          kpNav=6.5
          kdNav=kpNav*0.75
          preNavErr=0
          

          #Lateral Controller 
          min_idx= 0
          while True:
            currentPos=getPos()
            x=currentPos[0]
            y=currentPos[1]
            
            min_dist      = float("inf")
            for i in range(len(waypoints)):
                dist = np.linalg.norm(np.array([waypoints[i][0] - x,waypoints[i][1] - y]))
                if dist < min_dist and min_idx<=i and i<= min_idx+5:
                  min_dist = dist
                  min_idx = i

            if min_idx+1 < len(waypoints)-1:
                nearest_waypoint_x=waypoints[min_idx+1][0]
                nearest_waypoint_y=waypoints[min_idx+1][1]

                mouseHeading,yawDegree,yaw =takeMouseHeading()
                if yawDegree<0:
                  yawDegree=360+yawDegree

                currentPos=getPos()
                x=currentPos[0]
                y=currentPos[1]

                mousefixedframe_x=np.cos(yawDegree*np.pi/180)*(nearest_waypoint_x-x)+np.sin(yawDegree*np.pi/180)*(nearest_waypoint_y-y)
                mousefixedframe_y=-1*np.sin(yawDegree*np.pi/180)*(nearest_waypoint_x-x)+np.cos(yawDegree*np.pi/180)*(nearest_waypoint_y-y)

                if (mousefixedframe_y<0):
                  lat_err=np.arctan(-mousefixedframe_x/mousefixedframe_y)

                else:
                  lat_err=np.arctan(mousefixedframe_x/mousefixedframe_y)
                
                lat_diff=kp_lat*lat_err+kd_lat*(lat_err-previous_lat_err)+ki_lat*(lat_err+previous_lat_err)

                #Collision avoidance controller
                laserData = rospy.wait_for_message("/my_mm_robot/laser/scan", LaserScan, timeout=None)
                
                lftReadings=laserData.ranges[185:359]
                rgtReadings=laserData.ranges[0:175]

                minLftReading=min(list(lftReadings))
                minRgtReading=min(list(rgtReadings))

                if minLftReading < lft_tresh*0.5:
                  navErrL=-(initial_lft_val-minLftReading)
                else :
                  navErrL=0
                if minRgtReading< rgt_tresh*0.5:
                  navErrR=(initial_rgt_val-minRgtReading)
                else :
                  navErrR=0
                
                navErr=navErrL+navErrR

                diffNavErr=kpNav*navErr+kdNav*(navErr-preNavErr)
                
                #End of the task
                if (-0.11-0.04)<=x and (-0.11+0.04)>=x and (0.07-0.04)<=y and (0.07+0.04)>=y:
                  mtr_cmd(0,0)
                  break

                if lat_err>0.5 or lat_err<-0.5:
                  speed=0.075
                else:
                  speed=0.4

                mtr_cmd(speed,lat_diff+diffNavErr)
                preNavErr=navErr
                previous_lat_err = lat_err
                
            else:
              mtr_cmd(0,0)

          # ##Visualize
          # groundTruth=np.array(groundTruth)

          # plt.plot(waypoints[:, 0],waypoints[:, 1],'ro:')
          # plt.plot(groundTruth[:, 0],groundTruth[:, 1],'bo:')
          # plt.grid()
          # plt.show()
       
    except rospy.ROSInterruptException:
        pass