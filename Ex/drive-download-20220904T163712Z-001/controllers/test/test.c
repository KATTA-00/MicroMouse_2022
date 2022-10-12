/*
 * File:          test.c
 * Date:
 * Description:
 * Author:
 * Modifications:
 */

/*
 * You may need to add include files like <webots/distance_sensor.h> or
 * <webots/motor.h>, etc.
 */
#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/keyboard.h>
#include <webots/compass.h>
#include <webots/position_sensor.h>
/*
 * You may want to add macros here.
 */
#define TIME_STEP 64

/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */
int main(int argc, char **argv) {
  /* necessary to initialize webots stuff */
  wb_robot_init();
  
  wb_keyboard_enable(TIME_STEP);
  
  WbDeviceTag mtrs[2];
  char mtr_names[2][10]={"lft_mtr","rgt_mtr"};
  for(int i=0;i<2;i++){
  mtrs[i]=wb_robot_get_device(mtr_names[i]);
  wb_motor_set_position(mtrs[i], INFINITY);
  }
  
  WbDeviceTag ps[2];
  char ps_names[2][10]={"ps_lft","ps_rgt"};
  for(int i=0;i<2;i++){
  ps[i]=wb_robot_get_device(ps_names[i]);
  wb_position_sensor_enable(ps[i], TIME_STEP);
  }
  
  WbDeviceTag ds[3];
  char ds_names[3][10]={"ds_lft","ds_fwd","ds_rgt"};
  for(int i=0;i<3;i++){
  ds[i]=wb_robot_get_device(ds_names[i]);
  wb_distance_sensor_enable(ds[i],TIME_STEP);
  }
  
  WbDeviceTag compass[1];
  compass[0]=wb_robot_get_device("compass");
  wb_compass_enable(compass[0],TIME_STEP);
  
  
  

  /*
   * You should declare here WbDeviceTag variables for storing
   * robot devices like this:
   *  WbDeviceTag my_sensor = wb_robot_get_device("my_sensor");
   *  WbDeviceTag my_actuator = wb_robot_get_device("my_actuator");
   */

  /* main loop
   * Perform simulation steps of TIME_STEP milliseconds
   * and leave the loop when the simulation is over
   */
  while (wb_robot_step(TIME_STEP) != -1) {
    double lft_mtr_val=0;
    int rgt_mtr_val=0;
    /*
     * Read the sensors :
     * Enter here functions to read sensor data, like:
     *  double val = wb_distance_sensor_get_value(my_sensor);
     */
    int key =wb_keyboard_get_key(); 
    //printf("%d\n",key);
    
    if (key==315){
    lft_mtr_val=1.0;
    rgt_mtr_val=1;
    }
    else if(key==314){
    lft_mtr_val=-1;
    rgt_mtr_val=1;
    }
    else if(key==316){
    lft_mtr_val=1;
    rgt_mtr_val=-1;
    }
    else if(key==317){
    lft_mtr_val=-1;
    rgt_mtr_val=-1;
    }
    
    int initial_pos_val=2*wb_position_sensor_get_value(ps[0]);
    /* Process sensor data here */

    /*
     * Enter here functions to send actuator commands, like:
     * wb_motor_set_position(my_actuator, 10.0);
     */
    wb_motor_set_velocity(mtrs[0], lft_mtr_val);
    wb_motor_set_velocity(mtrs[1], rgt_mtr_val);
    const double *north = wb_compass_get_values(compass[0]);
    double rad = atan2(north[0], north[2]);
    int degree = rad*180/M_PI;
    if (degree < 0){
      degree = degree + 360;
     }
    //printf("#################\n%d\n",initial_pos_val);
    printf("#################\n%f\n%f\n",wb_distance_sensor_get_value(ds[0]),wb_distance_sensor_get_value(ds[2]));
     
  };

  /* Enter your cleanup code here */

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();

  return 0;
}
