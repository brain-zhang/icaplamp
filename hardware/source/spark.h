//收到上位机信息后，控制P62引脚交替输出高电平/低电平，控制头灯闪烁

#ifndef _SPARK_H
#define _SPARH_H

//头灯闪烁间隔时间
#define SPARK_INTERVAL 100

//头灯闪烁次数，默认比此参数少1次
#define SPARK_COUNT 6

void Spark_init();
void Spark_action();
void Spark_stop();
#endif