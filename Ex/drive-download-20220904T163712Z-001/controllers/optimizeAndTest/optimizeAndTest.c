/*
 * File:          mazeRunnerProject.c
 * Date:          2021/05/26
 * status:        completed
 * Author:        oshadaRathnayakae
 * Modifications: completed
 */
#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/keyboard.h>
#include <webots/compass.h>
#include <webots/position_sensor.h>
#include <webots/led.h>

//customLibrary
#include "maze.h"

#define TIME_STEP 64

//flags
int flag=0;
int flag_turn=0;
int memory_flag=0;
int task_flag=0;

//variablesWhichAreRelatedToPositionSensor
int pos_val;
int req_pos_val;
int pos_val_trn;
int req_pos_val_trn;

//variablesInSideThe"check()"Function
int fnt;
int lft;
int rgt;
int condition;

//variablesInSideThe"ir_or_enc_pid()"Function
double L_err;
double L_diff;
double R_err;
double R_diff;
double err;
double diff;

//variablesWhichAreRelatedToCompass
int compass_out;
int degree;

//initializeMotorSpeeds
const double L_base_speed = 7.0;
const double R_base_speed = 7.0;

//initializePositionSensorVariables
const int step_dis=146;
const int trn_dis=70;

//initializeDistanceSensorReadings
double initial_fnt_val;
double fnt_tresh=740.0;
double initial_lft_val=480.0;
double lft_tresh=740.0;//(initial_lft_val + value without wall)/2
double initial_rgt_val=480.0;
double rgt_tresh=740.0;

//initialPositionOfTheRobot
int init_mouseRow = 7;
int init_mouseColumn = 0;
int init_mouseHeading = 0;

int main(int argc, char **argv) {
 
  wb_robot_init();
  
  //motors
  WbDeviceTag mtrs[2];
  char mtr_names[2][10]={"lft_mtr","rgt_mtr"};
  for(int i=0;i<2;i++){
  mtrs[i]=wb_robot_get_device(mtr_names[i]);
  wb_motor_set_position(mtrs[i], INFINITY);
  }
  
  //psisionSensors
  WbDeviceTag ps[2];
  char ps_names[2][10]={"ps_lft","ps_rgt"};
  for(int i=0;i<2;i++){
  ps[i]=wb_robot_get_device(ps_names[i]);
  wb_position_sensor_enable(ps[i], TIME_STEP);
  }
  
  //distanseSensors
  WbDeviceTag ds[3];
  char ds_names[3][10]={"ds_lft","ds_fwd","ds_rgt"};
  for(int i=0;i<3;i++){
  ds[i]=wb_robot_get_device(ds_names[i]);
  wb_distance_sensor_enable(ds[i],TIME_STEP);
  }
  
  //compass
  WbDeviceTag compass[1];
  compass[0]=wb_robot_get_device("compass");
  wb_compass_enable(compass[0],TIME_STEP);
  
  //LED
  WbDeviceTag led[1];
  led[0]=wb_robot_get_device("led");
  
  //basicFunctions
  void mtr_cmd(double lm, double rm) {
    /////////lm///////////////////////
    if (0 < lm) {
      if (lm >= 10) {
        lm = 10;
      }
      else {
        lm = lm;
      }
  
    }
  
  
    else {
      if (lm <= -10) {
        lm = -10;
      }
      else {
        lm = lm;
      }
    }
  
    ///////////rm/////////////////////
    if (0 < rm) {
      if (rm >= 10) {
        rm = 10;
      }
      else {
        rm = rm;
      }
    }
  
  
    else {
      if (rm <= -10) {
        rm = -10;
      }
      else {
        rm = rm;
      }
    }
  
    ///////////////////Lmtr//////////////
  
    wb_motor_set_velocity(mtrs[0], lm);
  
    /////////////////////Rmtr///////////
    wb_motor_set_velocity(mtrs[1], rm);
  }
  
  void takeMouseHeading(){
    const double *north = wb_compass_get_values(compass[0]);
    double rad = atan2(north[0], north[2]);
    degree = rad*180/M_PI;
    if (degree < 0){
      degree = degree + 360;
     }
     if(degree>350 || degree<10){
       mouseHeading=0;
     }
     else if(degree>80 && degree<100){
       mouseHeading=1;
     }
     else if(degree>170 && degree<190){
       mouseHeading=2;
     }
     else if(degree>260 && degree<280){
       mouseHeading=3;
     }
    
  }
  
  void ir_or_enc_pid() {
    double kp = 0.01;
    double kp_com = 0.3;
    //printf("#################\n%f\n%f\n",wb_distance_sensor_get_value(ds[0]),wb_distance_sensor_get_value(ds[2]));
    if (wb_distance_sensor_get_value(ds[0]) < lft_tresh) {
      //following the left wall
      L_err = wb_distance_sensor_get_value(ds[0]) - initial_lft_val;
      L_diff = L_err * kp;
      mtr_cmd(L_base_speed - L_diff, R_base_speed + L_diff);
    }
    else if (wb_distance_sensor_get_value(ds[2]) < rgt_tresh) {
      //following the right wall
      R_err = wb_distance_sensor_get_value(ds[2]) - initial_rgt_val;
      R_diff = R_err * kp;
      mtr_cmd(L_base_speed + R_diff, R_base_speed - R_diff);
    }
    else {
      takeMouseHeading();
      if(mouseHeading==0 && degree<10){
        err=degree;
      }
      else if(mouseHeading==0 && degree>350){
        err=degree-360;
      }
      else if(mouseHeading==1){
        err=degree-90;
      }
      else if(mouseHeading==2){
        err=degree-180;
      }
      else if(mouseHeading==3){
        err=degree-270;
      }
      diff=err*kp_com;
      mtr_cmd(L_base_speed-diff,R_base_speed+diff);
      
    }
  }
  
  void check(){
    if (wb_distance_sensor_get_value(ds[1]) < fnt_tresh/*when a wall apears infront*/) {
      fnt = 1;
    }
    else {
      fnt = 0;
    }

    if (wb_distance_sensor_get_value(ds[0]) < lft_tresh/*when a wall apears on left*/) {
      lft = 2;
    }
    else {
      lft = 0;
    }
    if (wb_distance_sensor_get_value(ds[2]) < rgt_tresh/*when a wall apears on right*/) {
      rgt = 4;
    }
    else {
      rgt = 0;
    }
    
    condition = fnt + lft + rgt;
    
  }
   
  void step_fwd(){
    wb_led_set(led[0],0);
    if (flag==0){
        pos_val=20*wb_position_sensor_get_value(ps[0]);
        req_pos_val =pos_val+step_dis;
        flag=1;
      }
      if(pos_val<=req_pos_val && flag==1){      
        pos_val=20*wb_position_sensor_get_value(ps[0]);
        ir_or_enc_pid();
      }
      else{
        flag=0;
        flag_turn=0;
        
        mtr_cmd(L_base_speed/5,R_base_speed/5);
        wb_led_set(led[0],1);
       
      }
  }
   
  void trn_lft(){
    if (flag==0){
        pos_val_trn=20*wb_position_sensor_get_value(ps[0]);
        req_pos_val_trn =pos_val_trn-trn_dis;
        flag=1;
      }
      if(pos_val_trn>=req_pos_val_trn && flag==1){      
        pos_val_trn=20*wb_position_sensor_get_value(ps[0]);
        mtr_cmd(-L_base_speed/2,R_base_speed/2);
      }
      else{
        if (flag_turn==0){
          pos_val=20*wb_position_sensor_get_value(ps[0]);
          req_pos_val =pos_val+step_dis;
          flag_turn=1;
        }
        else if (flag_turn==1){        
          step_fwd();
        }
                
      }
  }
  
  void trn_rgt(){
    if (flag==0){
        pos_val_trn=20*wb_position_sensor_get_value(ps[0]);
        req_pos_val_trn =pos_val_trn+trn_dis;
        flag=1;
      }
      if(pos_val_trn<=req_pos_val_trn && flag==1){      
        pos_val_trn=20*wb_position_sensor_get_value(ps[0]);
        mtr_cmd(L_base_speed/2,-R_base_speed/2);
      }
      else{
        if (flag_turn==0){
          pos_val=20*wb_position_sensor_get_value(ps[0]);
          req_pos_val =pos_val+step_dis;
          flag_turn=1;
        }
        else if (flag_turn==1){        
          step_fwd();
        }
                
      }
  }
  
  void u_trn(){
    if (flag==0){
        pos_val_trn=20*wb_position_sensor_get_value(ps[0]);
        req_pos_val_trn =pos_val_trn-(trn_dis*2);
        flag=1;
      }
      if(pos_val_trn>=req_pos_val_trn && flag==1){      
        pos_val_trn=20*wb_position_sensor_get_value(ps[0]);
        mtr_cmd(-L_base_speed/2,R_base_speed/2);
      }
      else{
        if (flag_turn==0){
          pos_val=20*wb_position_sensor_get_value(ps[0]);
          req_pos_val =pos_val+step_dis;
          flag_turn=1;
        }
        else if (flag_turn==1){        
          step_fwd();
        }
                
      }
  }
  
  //initializeTheMaze
  mouseRow = init_mouseRow;
  mouseColumn = init_mouseColumn;
  mouseHeading = init_mouseHeading;
  
  NanoMouseMaze();
  
  //mainLoop
  while (wb_robot_step(TIME_STEP) != -1) {
 ///////////////////////////////////////////start navigating through the maze + start mapping ////////////////////////////// 
      if(task_flag==0){
        if (flag==0){
          takeMouseHeading();
          check();
          printf("               MAP\n");
          solve();
          print();
          memory_flag=0;          
        }
        
        if (condition==0){
          step_fwd();
          if (mouseHeading==0 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
        }
  ///////////////////////////////////////      
        else if (condition==1){
          trn_lft();
          if (memory_flag==0){
            addwalls(mouseHeading);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
        }
  ////////////////////////////////////////      
        else if (condition==2){
          step_fwd();
          if (memory_flag==0){
            addwalls(mouseHeading - 1);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
        }
  ////////////////////////////////////////
        else if (condition==3){
          trn_rgt();
          if (memory_flag==0){
            addwalls(mouseHeading);
            addwalls(mouseHeading-1);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
        }
  ///////////////////////////////////////
        else if (condition==4){
          trn_lft();
          if (memory_flag==0){
            addwalls(mouseHeading + 1);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
        }
  //////////////////////////////////////
        else if (condition==5){
          trn_lft();
          if (memory_flag==0){
           addwalls(mouseHeading + 1);
           addwalls(mouseHeading);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
        }
  //////////////////////////////////////
        else if (condition==6){
          step_fwd();
          if (memory_flag==0){
           addwalls(mouseHeading + 1);
           addwalls(mouseHeading - 1);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
        }
  ///////////////////////////////////////
        else if (condition==7){
          u_trn();
          if (memory_flag==0){
           addwalls(mouseHeading + 1);
           addwalls(mouseHeading - 1);
           addwalls(mouseHeading);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
        }
         
        if(mouseRow == init_mouseRow && mouseColumn == init_mouseColumn && flag==0){
          mtr_cmd(0,0);
          solve();
          print();
          task_flag=1;                 
        }
      }
///////////////////////////////////////////end navigating through the maze + end mapping ///////////////////////////////////


//////////////////////////////////////////start placing the robot at the initial position///////////////////////////////////     
      else if(task_flag==1){
        if (flag==0){ 
          takeMouseHeading();    
          check();
          memory_flag=0;          
        }
        if (condition==0){
          step_fwd();
          if (mouseHeading==0 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
        }
  ///////////////////////////////////////      
        else if (condition==1){
          trn_lft();
          if (memory_flag==0){
            addwalls(mouseHeading);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
        }
  ////////////////////////////////////////      
        else if (condition==2){
          step_fwd();
          if (memory_flag==0){
            addwalls(mouseHeading - 1);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
        }
  ////////////////////////////////////////
        else if (condition==3){
          trn_rgt();
          if (memory_flag==0){
            addwalls(mouseHeading);
            addwalls(mouseHeading-1);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
        }
  ///////////////////////////////////////
        else if (condition==4){
          trn_lft();
          if (memory_flag==0){
            addwalls(mouseHeading + 1);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
        }
  //////////////////////////////////////
        else if (condition==5){
          trn_lft();
          if (memory_flag==0){
           addwalls(mouseHeading + 1);
           addwalls(mouseHeading);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
        }
   //////////////////////////////////////
        else if (condition==6){
          step_fwd();
          if (memory_flag==0){
           addwalls(mouseHeading + 1);
           addwalls(mouseHeading - 1);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
        }
  ///////////////////////////////////////
        else if (condition==7){
          u_trn();
          if (memory_flag==0){
           addwalls(mouseHeading + 1);
           addwalls(mouseHeading - 1);
           addwalls(mouseHeading);
          }
          
          if (mouseHeading==0 && memory_flag==0){
            mouseRow++;
            memory_flag=1;
          }
          else if (mouseHeading==1 && memory_flag==0){
            mouseColumn--;
            memory_flag=1;
          }
          else if (mouseHeading==2 && memory_flag==0){
            mouseRow--;
            memory_flag=1;
          }
          else if (mouseHeading==3 && memory_flag==0){
            mouseColumn++;
            memory_flag=1;
          }
        }
         
         if(mouseRow==(init_mouseRow-1) && flag==0){/////////////////////////
                                                    /////////////////////////        
            mouseColumn=init_mouseColumn;
            mouseHeading=init_mouseHeading;
            task_flag++;                        
         }
             
      }
      
 //////////////////////////////////////////end placing the robot at the initial position/////////////////////////////////////
 
 ///////////////////////////////////////////start following the shortest path////////////////////////////////////////////////      
      else if(task_flag==2){
        if(mouseRow != targetRow || mouseColumn != targetColumn) {
          if (mouseHeading == 0) {
            if (horizontalWalls[mouseRow][mouseColumn] == false && values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1) {
              step_fwd();
              if(flag==0){
                mouseRow--;
              }
            }
            else if (verticalWalls[mouseRow][mouseColumn] == false && values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1) {
              trn_lft();
              if(flag==0){
                mouseColumn--;
                mouseHeading = 3;
              }
            }
            else if (verticalWalls[mouseRow][mouseColumn + 1] == false && values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1) {
              trn_rgt();
              if(flag==0){
                mouseColumn++;
                mouseHeading = 1;
              }
            }
            else {
              if(flag==0){
                mouseRow++;
                mouseHeading = 2;
              }
            }
      
          }
          else if (mouseHeading == 1) {
            if (verticalWalls[mouseRow][mouseColumn + 1] == false && values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1) {
              step_fwd();
              if(flag==0){
                mouseColumn++;
              }
      
            }
            else if (horizontalWalls[mouseRow][mouseColumn] == false && values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1) {
              trn_lft();
              if(flag==0){
                mouseRow--;
                mouseHeading = 0;
              }
            }
            else if (horizontalWalls[mouseRow + 1][mouseColumn] == false && values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1) {
              trn_rgt();
              if(flag==0){
                mouseRow++;
                mouseHeading = 2;
              }
            }
            else {
              if(flag==0){
                mouseColumn--;
                mouseHeading = 3;
              }
            }
          }
          else if (mouseHeading == 2) {
            if (horizontalWalls[mouseRow + 1][mouseColumn] == false && values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1) {
              step_fwd();
              if(flag==0){mouseRow++;}
            }
            else if (verticalWalls[mouseRow][mouseColumn + 1] == false && values[mouseRow][mouseColumn + 1] == values[mouseRow][mouseColumn] - 1) {
              trn_lft();
              if(flag==0){
                mouseColumn++;
                mouseHeading = 1;
              }
            }
            else if (verticalWalls[mouseRow][mouseColumn] == false && values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1) {
              trn_rgt();
              if(flag==0){
              mouseColumn--;
              mouseHeading = 3;
              }
            }
            else {
              if(flag==0){
                  mouseRow--;
                  mouseHeading = 0;
                }
            }
          }
          else if (mouseHeading == 3) {
            if (verticalWalls[mouseRow][mouseColumn] == false && values[mouseRow][mouseColumn - 1] == values[mouseRow][mouseColumn] - 1) {
              step_fwd();
              if(flag==0){mouseColumn--;}
      
            }
            else if (horizontalWalls[mouseRow + 1][mouseColumn] == false && values[mouseRow + 1][mouseColumn] == values[mouseRow][mouseColumn] - 1) {
              trn_lft();
              if(flag==0){
                mouseRow++;
                mouseHeading = 2;
              }
            }
            else if (horizontalWalls[mouseRow][mouseColumn] == false && values[mouseRow - 1][mouseColumn] == values[mouseRow][mouseColumn] - 1) {
              trn_rgt();
              if(flag==0){
                mouseRow--;
                mouseHeading = 0;
              }
            }
            else {
              if(flag==0){
              mouseColumn++;
              mouseHeading = 1;
              }
            }
          }
          print();
          
        }
  
        else{
          mtr_cmd(0,0);
          break;
        }           
      }
 ///////////////////////////////////////////////////////////////////////////////////////////////end following the shortest path////////////////////////////////////////////////                                                   
  };//endOfTheMainLoop
  wb_robot_cleanup();
  return 0;
}
