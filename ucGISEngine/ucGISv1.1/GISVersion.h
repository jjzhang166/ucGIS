/*
*********************************************************************************************************
*                                             uC/GIS V1.01
*                        gis lib software for embedded applications
*
*                       (c) Copyright 2009, 向旭东
* 
*
*              uc/GIS is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GISVersion.h
Purpose     : Include file defining current GIS version
---------------------------END-OF-HEADER------------------------------
*/

#ifndef  GIS_VERSION_H
#define  GIS_VERSION_H

#define GIS_VERSION 10100

//ver4.0预计功能
//1、实现DEM图片显示
//2、GML格式接入
//3、支持文webgis

//ver3.0预计功能
//1、好友管理
//2、动态拼车算法
//3、优化算法
//ver2.0预计功能
//1、重建路网索引，实现路径规划，最短路径
//2、实现语音导航
//3、实现线形多样化，美化显示
//ver1.1新增
//1、采用对一级索引进行二次索引，采用格网内细分格网的办法，实现由于索引文件过大耗损内存的问题
//2、因格网分批动态加载图元后不能查询完整的数据，采用重构索引的方式实现点查询，标注查询等
//ver1.0新增
//1、一级矢量格网索引技术，索引文件包括所有格网的逻辑坐标以及其内包含的矢量图元的文件偏移地址以及图元的编号
//2、按格网进行检索当前视窗的矢量图元，动态加载和释放，减少内存开销，并以索引的图元编号判断
//3、增加序列号保护措施
//4、增加统一对外接口函数
//5、增加动态分配内部函数在转换点坐标时的数组，宏定义可选
//6、防止动态内存分配出现内存碎片的问题，统一释放图元所占内存
//ver0.9功能
//1、基本矢量地图显示、缩放、文本标注顺序查询、测距，面积测量等功能
//2、数据自有格式，ANSI c标准文件操作函数模式
//3、标注自动错位技术
//4、自动视野分层显示控制，多图层模式
//5、GPS打点支持，实现GPS数据串解析，每个点拥有自身ID,用于区分是自身GPS数据或者是好友数据
//6、根据自身GPS数据进行动态移动地图
//7、支持用户图层
//8、支持车辆显示，轨迹显示存储
//9、支持语音输出函数指针，用系统语音输出驱动函数进行接口后，支持内部的语音播报

#endif   /* Avoid multiple inclusion */

/*************************** End of file ****************************/
