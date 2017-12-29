/**
 * Copyright (c) 2016 Corerain Technologies. All rights reserved. 
 * No part of this document, either material or conceptual may be 
 * copied or distributed, transmitted, transcribed, stored in a retrieval 
 * system or translated into any human or computer language in
 * any form by any means, electronic, mechanical, manual
 * or otherwise, or disclosed to third parties without
 * the express written permission of Corerain Technologies, 
 * 502 Section b, 2305 Zuchongzhi Road, Zhangjiang Hi-Tech Park, 
 * Shanghai 201203, China.
 */

/**
 * This is the command line tool to run the integration test of LeNet 
 */

/* C utilities */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <cmath>
#include <cstdlib>
#include <thread>

/* RainMan headers */
#include "rainman/common.h"
#include "rainman/io.h"
#include "rainman/net.h"
#include "rainman/tcp_sockets.hpp"
#include "rainman/pre_processing.h"

#include <string>

#define MAX_FILE_NAME_LENGTH 100

/* Base Address definitions */
#define DDR_BASEADDR   0      //from DDR_MEM_BASE  
//#define MAP_BASEADDR   DDR_BASEADDR
#define MAP_BASEADDR   0     //from DDR_MEM_BASE
//#define COEF_BASEADDR  DDR_BASEADDR + 0x20000000
#define SEG_BASEADDR   RES_L13

// this is defined in common.h
//#define DDR_MEM_TMP_BASE   0x3F000000



#define MAP_BASEADDR    0  // %% starting from DDR_MEM_BASE = 0x1000_0000
#define RES_L1          (MAP_BASEADDR + 786432)
#define RES_L2          (RES_L1 + 2097152)
#define RES_L3          (RES_L2 + 524288)
#define RES_L4          (RES_L3 + 1048576)
#define RES_L5          (RES_L4 + 262144)
#define RES_L6          (RES_L5 + 524288) 
#define RES_L7          (RES_L6 + 131072)
#define RES_L8          (RES_L7 + 262144)
#define RES_L9          (RES_L8 + 65536)
#define RES_L10         (RES_L9 + 131072)
#define RES_L8_copy     (RES_L10 + 131072)
#define RES_L11         (RES_L8_copy + 262144)
#define RES_L12         (RES_L11 + 262144) 
#define RES_L13         (RES_L12 + 262144)
#define RES_L6_copy     (RES_L13 + 262144)
#define RES_L14         (RES_L6_copy + 524288)
#define RES_L15         (RES_L14 + 524288)
#define RES_L16         (RES_L15 + 524288)
#define RES_L4_copy     (RES_L16 + 524288)
#define RES_L17         (RES_L4_copy + 1048576)
#define RES_L18         (RES_L17 + 1048576)
#define RES_L19         (RES_L18 + 1048576)
#define RES_L2_copy     (RES_L19 + 1048576)
#define RES_L20         (RES_L2_copy + 2097152)
#define RES_L21         (RES_L20 + 2097152)
#define RES_L22         (RES_L21 + 2097152)
#define RES_L23         (RES_L22 + 2097152)
#define RES_END         (RES_L23 + 524288)

#define COEF_BASEADDR   (0x12000000 >> 2)

#define COEF_L1         (COEF_BASEADDR)
#define COEF_L2         (COEF_L1 + 216)
#define COEF_L3         (COEF_L2 + 576)
#define COEF_L4         (COEF_L3 + 1152)
#define COEF_L5         (COEF_L4 + 2304)
#define COEF_L6         (COEF_L5 + 4608)
#define COEF_L7         (COEF_L6 + 9216)
#define COEF_L8         (COEF_L7 + 18432)
#define COEF_L9         (COEF_L8 + 36864)
#define COEF_L10        (COEF_L9 + 73728)
#define COEF_L11        (COEF_L10 + 147456)
#define COEF_L12        (COEF_L11 + 32768)
#define COEF_L13        (COEF_L12 + 73728)
#define COEF_L14        (COEF_L13 + 36864)
#define COEF_L15        (COEF_L14 + 8192)
#define COEF_L16        (COEF_L15 + 18432)
#define COEF_L17        (COEF_L16 + 4096)
#define COEF_L18        (COEF_L17 + 2048)
#define COEF_L19        (COEF_L18 + 4608)
#define COEF_L20        (COEF_L19 + 2304)
#define COEF_L21        (COEF_L20 + 512)
#define COEF_L22        (COEF_L21 + 1152)
#define COEF_L23        (COEF_L22 + 576)
#define COEF_END        (COEF_L23 + 16)

#define A_L1            (COEF_END)
#define A_L2            (A_L1 + 8)
#define A_L3            (A_L2 + 8)
#define A_L4            (A_L3 + 16)
#define A_L5            (A_L4 + 16)
#define A_L6            (A_L5 + 32)
#define A_L7            (A_L6 + 32)
#define A_L8            (A_L7 + 64)
#define A_L9            (A_L8 + 64)
#define A_L10           (A_L9 + 128)
#define A_L11           (A_L10 + 128)
#define A_L12           (A_L11 + 64)
#define A_L13           (A_L12 + 64)
#define A_L14           (A_L13 + 64)
#define A_L15           (A_L14 + 32)
#define A_L16           (A_L15 + 32)
#define A_L17           (A_L16 + 32)
#define A_L18           (A_L17 + 16)
#define A_L19           (A_L18 + 16)
#define A_L20           (A_L19 + 16)
#define A_L21           (A_L20 + 8)
#define A_L22           (A_L21 + 8)
#define A_L23           (A_L22 + 8)
#define A_END           (A_L23 + 1)

#define B_L1            (A_END)
#define B_L2            (B_L1 + 8)
#define B_L3            (B_L2 + 8)
#define B_L4            (B_L3 + 16)
#define B_L5            (B_L4 + 16)
#define B_L6            (B_L5 + 32)
#define B_L7            (B_L6 + 32)
#define B_L8            (B_L7 + 64)
#define B_L9            (B_L8 + 64)
#define B_L10           (B_L9 + 128)
#define B_L11           (B_L10 + 128)
#define B_L12           (B_L11 + 64)
#define B_L13           (B_L12 + 64)
#define B_L14           (B_L13 + 64)
#define B_L15           (B_L14 + 32)
#define B_L16           (B_L15 + 32)
#define B_L17           (B_L16 + 32)
#define B_L18           (B_L17 + 16)
#define B_L19           (B_L18 + 16)
#define B_L20           (B_L19 + 16)
#define B_L21           (B_L20 + 8)
#define B_L22           (B_L21 + 8)
#define B_L23           (B_L22 + 8)
#define B_END           (B_L23 + 1)

#define BIAS_L1            (B_END)
#define BIAS_L2            (BIAS_L1 + 8)
#define BIAS_L3            (BIAS_L2 + 8)
#define BIAS_L4            (BIAS_L3 + 16)
#define BIAS_L5            (BIAS_L4 + 16)
#define BIAS_L6            (BIAS_L5 + 32)
#define BIAS_L7            (BIAS_L6 + 32)
#define BIAS_L8            (BIAS_L7 + 64)
#define BIAS_L9            (BIAS_L8 + 64)
#define BIAS_L10           (BIAS_L9 + 128)
#define BIAS_L11           (BIAS_L10 + 128)
#define BIAS_L12           (BIAS_L11 + 64)
#define BIAS_L13           (BIAS_L12 + 64)
#define BIAS_L14           (BIAS_L13 + 64)
#define BIAS_L15           (BIAS_L14 + 32)
#define BIAS_L16           (BIAS_L15 + 32)
#define BIAS_L17           (BIAS_L16 + 32)
#define BIAS_L18           (BIAS_L17 + 16)
#define BIAS_L19           (BIAS_L18 + 16)
#define BIAS_L20           (BIAS_L19 + 16)
#define BIAS_L21           (BIAS_L20 + 8)
#define BIAS_L22           (BIAS_L21 + 8)
#define BIAS_L23           (BIAS_L22 + 8)
#define BIAS_END           (BIAS_L23 + 1)



/* kernel size definitions */
#define CONV_KERNEL_SIZE      3
#define DECONV_KERNEL_SIZE    2

#define NUM_LAYER             24

/* unet parameters
 * CONV(=1):   H - W  - NC - NF - K - POOLING - BLOCK - PADDING - INPUT_ADDR - COEFF_ADDR - OUTPT_ADDR - CROP_OUTPT_ADDR - a_addr - b_addr - bias_addr
 * DECONV(=0): H - W  - NC - NF - K - PADDING - BLOCK - STRIDE  - INPUT_ADDR - COEFF_ADDR - OUTPT_ADDR - bias_addr
 */
int PARAM[NUM_LAYER][16] =  {
    {1, 512, 512,  3,  8,  3, 0, 18, 1, MAP_BASEADDR, COEF_L1, RES_L1, 0, A_L1, B_L1, BIAS_L1},           //L1
    {1, 512, 512,  8,  8,  3, 1, 18, 1, RES_L1,  COEF_L2,  RES_L2,  RES_L20, A_L2, B_L2, BIAS_L2},        //L2  pool
    {1, 256, 256,  8, 16,  3, 0, 10, 1, RES_L2,  COEF_L3,  RES_L3,  0, A_L3, B_L3, BIAS_L3},              //L3
    {1, 256, 256, 16, 16,  3, 1, 10, 1, RES_L3,  COEF_L4,  RES_L4,  RES_L17, A_L4, B_L4, BIAS_L4},        //L4  pool
    {1, 128, 128, 16, 32,  3, 0, 10, 1, RES_L4,  COEF_L5,  RES_L5,  0, A_L5, B_L5, BIAS_L5},              //L5
    {1, 128, 128, 32, 32,  3, 1, 10, 1, RES_L5,  COEF_L6,  RES_L6,  RES_L14, A_L6, B_L6, BIAS_L6},        //L6  pool
    {1,  64, 64,  32, 64,  3, 0, 10, 1, RES_L6,  COEF_L7,  RES_L7,  0, A_L7, B_L7, BIAS_L7},              //L7 
    {1,  64, 64,  64, 64,  3, 1, 10, 1, RES_L7,  COEF_L8,  RES_L8,  RES_L11, A_L8, B_L8, BIAS_L8},        //L8  pool
    {1,  32, 32,  64,128,  3, 0, 10, 1, RES_L8,  COEF_L9,  RES_L9,  0, A_L9, B_L9, BIAS_L9},              //L9
    {1,  32, 32, 128,128,  3, 0, 10, 1, RES_L9,  COEF_L1, RES_L10, 0, A_L10, B_L10, BIAS_L10},            //L10 
    {0,  32, 32, 128, 64,  2, 0,  8, 2, RES_L10, COEF_L1, RES_L8_copy, 0, BIAS_L11, 1, 0},                //L11 deconv
    {1,  64, 64, 128, 64,  3, 0, 18, 1, RES_L8_copy, COEF_L1, RES_L12, 0, A_L1, B_L1, BIAS_L1},           //L12
    {1,  64, 64,  64, 64,  3, 0, 18, 1, RES_L12, COEF_L13, RES_L13, 0, A_L13, B_L13, BIAS_L13},           //L13
    {0,  64, 64,  64, 32,  2, 0, 16, 2, RES_L13, COEF_L14, RES_L6_copy, 0, BIAS_L14, 1, 0},               //L14 deconv
    {1, 128, 128, 64, 32,  3, 0, 18, 1, RES_L6_copy, COEF_L15, RES_L15, 0, A_L15, B_L15, BIAS_L15},       //L15
    {1, 128, 128, 32, 32,  3, 0, 18, 1, RES_L15, COEF_L16, RES_L16, 0, A_L16, B_L16, BIAS_L16},           //L16
    {0, 128, 128, 32, 16,  2, 0, 16, 2, RES_L16, COEF_L17, RES_L4_copy, 0, BIAS_L17, 1, 0},               //L17 deconv
    {1, 256, 256, 32, 16,  3, 0, 18, 1, RES_L4_copy, COEF_L18, RES_L18, 0, A_L18, B_L18, BIAS_L18},       //L18
    {1, 256, 256, 16, 16,  3, 0, 18, 1, RES_L18, COEF_L19, RES_L19, 0, A_L19, B_L19, BIAS_L19},           //L19
    {0, 256, 256, 16,  8,  2, 0, 16, 2, RES_L19, COEF_L20, RES_L2_copy, 0, BIAS_L20, 1, 0},               //L20  deconv
    {1, 512, 512, 16,  8,  3, 0, 18, 1, RES_L2_copy, COEF_L21, RES_L21, 0, A_L21, B_L21, BIAS_L21},       //L21
    {1, 512, 512,  8,  8,  3, 0, 18, 1, RES_L21, COEF_L22, RES_L22, 0, A_L22, B_L22, BIAS_L22},           //L22
    {0, 512, 512,  8,  1,  1, 0, 16, 1, RES_L22, COEF_L23, RES_L23, 0, BIAS_L23, 0, 0},                   //L23 deconv
    {2, 512, 512,  1,  3,  0, 0, 0,  0, RES_L23, 0       , RES_END, 0, 0, 0, 0}                           //POS_Processing
    }; 

int layer_name[NUM_LAYER][2] = {
   {1, 1},
   {1, 2},
   {2, 1},
   {2, 2},
   {3, 1},
   {3, 2},
   {4, 1},
   {4, 2},
   {5, 1},
   {5, 2},
   {6, 6},
   {6, 1},
   {6, 2},
   {7, 7},
   {7, 1},
   {7, 2},
   {8, 8},
   {8, 1},
   {8, 2},
   {9, 9},
   {9, 1},
   {9, 2},
   {0, 0},
   {0, 0}
};

/* change crop parameters */
crop_param_t get_crop_param(int start, int end, int size) {
  //check that the parameters are valid
  if (start > end) {
    printf("invalid cropping parameters");
    exit(1);
  }

  crop_param_t crop_param;
  
  crop_param.crop_lines = size;
  crop_param.crop_rows  = size;

  crop_param.crop_start_x = start;
  crop_param.crop_start_y = start;

  crop_param.crop_end_x = end;
  crop_param.crop_end_y = end;
  
  return crop_param;
} 


int main(int argc, char *argv[]) {
  
  //variable for switch case
  int c,iteration;
  
  //output file variables
  unsigned int outpt_size;
  addr_t outpt_addr;
  char data_dir_name[MAX_FILE_NAME_LENGTH];
  char outpt_file_name[MAX_FILE_NAME_LENGTH];
  char trans_file_name[MAX_FILE_NAME_LENGTH];
  char server_address[MAX_FILE_NAME_LENGTH];
  unsigned int *a_p = NULL;
  unsigned int *b_p = NULL;
  unsigned int *bias_p = NULL;

  //creating space for output and input in memory
  memset(data_dir_name, 0, MAX_FILE_NAME_LENGTH);
  memset(outpt_file_name, 0, MAX_FILE_NAME_LENGTH);
  memset(trans_file_name, 0, MAX_FILE_NAME_LENGTH);
  sprintf(trans_file_name,"result.jpg");

  //searching through input flags
  while ((c = getopt(argc, argv, "d:o:s:t:")) != -1)
    switch (c) {
      //data directory name
      case 'd':
        strncpy(data_dir_name, optarg, MAX_FILE_NAME_LENGTH);
        break;
      //output file name
      case 'o':
        strncpy(outpt_file_name, optarg, MAX_FILE_NAME_LENGTH);
        break;
      case 's':
        strncpy(server_address, optarg, MAX_FILE_NAME_LENGTH);
        break;
      case 't':
        iteration = atoi(optarg);
        break;
      //incorrect arguments
      default:
        fprintf(stderr, "Cannot recognize %c command\n", c);
        fprintf(stderr, "usage: %s -d [data dir to lenet]\n", argv[0]);
        exit(1);
    }
  //need to define data directory
  if (strlen(data_dir_name) <= 0) {
    fprintf(stderr, "[RAINMAN] Error: Data directory name should be larger than 0\n");
    exit(1);
  }
 
  #ifdef USE_REG_MMAP
    open_reg_device();
  #endif
    open_mem_device();

  for (int j = 0; j < iteration; j++) {

  tcp_sockets_recv_file();
  pre_processing("./test.bin", sizeof(char), 18, "data.bin", 1);

  //unet_small_param_t unet_small_param = load_unet(data_dir_name); 

  conv_param_t    temp_conv_param; 
  deconv_param_t  temp_deconv_param; 
  pos_argmax_param_t temp_pos_argmax;
  
  for(int i = 0; i < NUM_LAYER; i++) { 
    int layer = layer_name[i][0], part = layer_name[i][1];
    if(PARAM[i][0]==1) {
      //initialise the parameters
      conv_param_init(& temp_conv_param);
      //get addresses
      if(i>0)
        temp_conv_param.input = get_mem_addr( PARAM[i][9] );
      if(i==1)
        temp_conv_param.crop_param = get_crop_param(0, 511, 512);
      if(i==3)
        temp_conv_param.crop_param = get_crop_param(0, 255, 256);
      if(i==5)
        temp_conv_param.crop_param = get_crop_param(0, 127, 128);
      if(i==7)
        temp_conv_param.crop_param = get_crop_param(0, 63, 64);

      temp_conv_param.start_addr      = PARAM[i][9];
      temp_conv_param.coeff_addr      = PARAM[i][10];
      temp_conv_param.outpt_addr      = PARAM[i][11];
      temp_conv_param.outpt_crop_addr = PARAM[i][12];
      temp_conv_param.a_addr          = PARAM[i][13];
      temp_conv_param.b_addr          = PARAM[i][14];
      temp_conv_param.bias_addr       = PARAM[i][15];
      //get parameters
      temp_conv_param.height       = PARAM[i][1];
      temp_conv_param.width        = PARAM[i][2];
      temp_conv_param.num_channels = PARAM[i][3];
      temp_conv_param.num_filters  = PARAM[i][4];
      temp_conv_param.kernel_size  = PARAM[i][5];
      temp_conv_param.pooling      = PARAM[i][6];
      temp_conv_param.crop_en      = PARAM[i][6];
      temp_conv_param.block_size   = PARAM[i][7];
      temp_conv_param.pad_line     = PARAM[i][8];
      temp_conv_param.pad          = PARAM[i][8];
      temp_conv_param.bn_en        = PARAM[i][0];
      temp_conv_param.bias_en      = PARAM[i][0];
      temp_conv_param.relu_en      = PARAM[i][0];

      //get data
      
      if(i==0){
        temp_conv_param.input  = load_file(
          "./data.bin",
          temp_conv_param.start_addr,
          conv_get_input_origin_size(temp_conv_param)
          );
      }

      temp_conv_param.coeff  = load_file(
        (char *) (std::string(data_dir_name) + "/fixed_little/layer"+std::to_string(layer)+"_conv_"+std::to_string(part)+"_weights.bin").c_str(),
        temp_conv_param.coeff_addr,
        conv_get_coeff_size(temp_conv_param)
        ); 
      
      a_p  = load_file(
        (char *) (std::string(data_dir_name) + "/fixed_little/layer"+std::to_string(layer) +"_bn_"+std::to_string(part)+"_A.bin").c_str(),
        temp_conv_param.a_addr,
        temp_conv_param.num_filters
        ); 

      b_p  = load_file(
        (char *) (std::string(data_dir_name) + "/fixed_little/layer"+std::to_string(layer) +"_bn_"+std::to_string(part)+"_B.bin").c_str(),
        temp_conv_param.b_addr,
        temp_conv_param.num_filters
        ); 

      bias_p  = load_file(
        (char *) (std::string(data_dir_name) + "/fixed_little/layer"+std::to_string(layer)+"_conv_"+std::to_string(part)+"_bias.bin").c_str(),
        temp_conv_param.bias_addr,
        temp_conv_param.num_filters
        ); 

      conv_forward(temp_conv_param);
    
    }
    else {
        if(PARAM[i][0]==0){
        //initialise the parameters
        deconv_param_init(& temp_deconv_param);
        //get addresses
        temp_deconv_param.input = get_mem_addr(PARAM[i][9]);
        temp_deconv_param.start_addr      = PARAM[i][9];
        temp_deconv_param.coeff_addr      = PARAM[i][10];
        temp_deconv_param.outpt_addr      = PARAM[i][11];
        temp_deconv_param.bias_addr       = PARAM[i][13];
        //temp_deconv_param.outpt_crop_addr = PARAM[i][12];
        //get parameters
        temp_deconv_param.height       = PARAM[i][1];
        temp_deconv_param.width        = PARAM[i][2];
        temp_deconv_param.num_channels = PARAM[i][3];
        temp_deconv_param.num_filters  = PARAM[i][4];
        temp_deconv_param.kernel_size  = PARAM[i][5];
        temp_deconv_param.block_size   = PARAM[i][7];
        temp_deconv_param.bias_en      = !PARAM[i][0];
        temp_deconv_param.relu_en      = PARAM[i][14];
        temp_deconv_param.stride       = PARAM[i][5];
        //get data

        if(i != NUM_LAYER -2){     
          temp_deconv_param.coeff  = load_file(
        (char *) (std::string(data_dir_name) + "/fixed_little/layer"+std::to_string(layer) +"_deconv_layer"+std::to_string(part)+"_deconv_weights.bin").c_str(),
          temp_deconv_param.coeff_addr,
          deconv_get_coeff_size(temp_deconv_param)
        ); 
      
          bias_p  = load_file(
          (char *) (std::string(data_dir_name) + "/fixed_little/layer"+std::to_string(layer) +"_deconv_layer"+std::to_string(part)+"_deconv_bias.bin").c_str(),
          temp_deconv_param.bias_addr,
          temp_deconv_param.num_filters
          ); 
        } else {
          temp_deconv_param.coeff  = load_file(
        (char *) (std::string(data_dir_name) + "/fixed_little/final_final_weights.bin").c_str(),
          temp_deconv_param.coeff_addr,
          deconv_get_coeff_size(temp_deconv_param)
        );

          bias_p  = load_file(
          (char *) (std::string(data_dir_name) + "/fixed_little/final_final_bias.bin").c_str(),
          temp_deconv_param.bias_addr,
          temp_deconv_param.num_filters
          );
        }
      
        deconv_forward(temp_deconv_param);
        
    }
    else {
        pos_argmax_param_init(& temp_pos_argmax);     
        temp_pos_argmax.start_addr   = PARAM[i][9];
        temp_pos_argmax.outpt_addr   = PARAM[i][11];
        temp_pos_argmax.height       = PARAM[i][1];
        temp_pos_argmax.width        = PARAM[i][2];
        temp_pos_argmax.input_channels = PARAM[i][3];
        temp_pos_argmax.output_channels  = PARAM[i][4];
        temp_pos_argmax.input = get_mem_addr(PARAM[i][9]);
        temp_pos_argmax.output = (char*) get_mem_addr(PARAM[i][11]);
     }
   }
  }
  if(!PARAM[NUM_LAYER-1][0]){
    outpt_size = deconv_get_outpt_size(temp_deconv_param);
    outpt_addr = temp_deconv_param.outpt_addr;
    dump_file(outpt_file_name, outpt_addr, outpt_size);
  }
  else {
    outpt_size = conv_get_padding_outpt_size(temp_conv_param);
    outpt_addr = temp_conv_param.outpt_addr;
    dump_file(outpt_file_name, outpt_addr, outpt_size);
  }

  pos_processing(outpt_file_name, temp_pos_argmax , trans_file_name);
  thread socket_send(tcp_sockets_send_file, trans_file_name, server_address);
  socket_send.join();
  }
   #ifdef USE_REG_MMAP
    close_reg_device();
  #endif
    close_mem_device(); 
}
