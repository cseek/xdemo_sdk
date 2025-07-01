/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-17 22:41:57
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-07-02 00:48:25
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __FUSION_H__
#define __FUSION_H__

#include "common.h"
#include <cstddef>
#include <eigen3/Eigen/Dense>

class Fusion
{
public:
    Fusion();
    // IMU数据处理函数
    void process_imu_data(const ImuData &imu);
    // GNSS数据处理函数
    void process_gnss_data(const GnssData &gnss);
    // 获取融合数据
    auto get_fusion_data(uint64_t sec_week, uint64_t frame_id) -> FusionData;

private:
    // 计算时间增量
    double calculate_dt(const ImuData &current_imu);
    // 使用GNSS数据初始化系统
    void initialize(const GnssData &gnss);
    // 杆臂补偿
    void compensate_lever_arm(bool gnss_to_imu);
    // 机械编排: 更新导航状态
    void update_navigation_state(const Eigen::Vector3d &gyro, const Eigen::Vector3d &accel, double dt);
    // 计算重力 (WGS84模型)
    double calculate_gravity(double lat, double alt);
    // 更新姿态 (使用四元数)
    void update_attitude(const Eigen::Vector3d &gyro, double dt);
    // 更新位置 (考虑地球曲率)
    void update_position(const Eigen::Vector3d &vel_avg, double dt);
    // ESKF预测步骤
    void error_state_prediction(const Eigen::Vector3d &gyro, const Eigen::Vector3d &accel, double dt);
    // ESKF更新步骤
    void error_state_update(const GnssData &gnss);
    // 欧拉角转旋转矩阵
    Eigen::Matrix3d euler_to_rotation(const Eigen::Vector3d &euler);
    // 欧拉角转四元数
    Eigen::Quaterniond euler_to_quaternion(const Eigen::Vector3d &euler);
    // 四元数转欧拉角
    Eigen::Vector3d quaternion_to_euler(const Eigen::Quaterniond &q);
    // 向量叉乘矩阵
    Eigen::Matrix3d skew_matrix(const Eigen::Vector3d &v);

private:
    // 系统状态
    double m_lat = 0;             // 纬度 [°]
    double m_lon = 0;             // 经度 [°]
    double m_alt = 0;             // 高度 [m]
    Eigen::Vector3d m_vel;        // 速度 (NED) [m/s]
    Eigen::Vector3d m_att;        // 姿态 (横滚,俯仰,航向) [rad]
    Eigen::Vector3d m_gyro_bias;  // 陀螺零偏 [rad/s]
    Eigen::Vector3d m_accel_bias; // 加速度零偏 [m/s²]
    // 协方差矩阵
    Eigen::Matrix<double, 15, 15> m_P;
    // 噪声参数
    double m_gyro_noise;            // 陀螺白噪声 [rad/s/sqrt(Hz)]
    double m_accel_noise;           // 加速度白噪声 [m/s²/sqrt(Hz)]
    double m_gyro_bias_instability; // 陀螺零偏不稳定性 [rad/s]
    double m_bias_correlation_time; // 零偏相关时间 [s]
    // 杆臂 (FRD坐标系)
    Eigen::Vector3d m_lever_arm;
    // 数据缓存
    ImuData m_last_imu;
    uint64_t m_frame_id = 0;
    uint64_t m_last_output_time = 0;
    bool m_initialized = false;
};

#endif // __FUSION_H__