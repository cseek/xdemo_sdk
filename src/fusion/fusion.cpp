#include "fusion/fusion.h"
#include <functional>
#include <eigen3/Eigen/Dense>
#include <cmath>

// WGS84椭球参数
constexpr double WGS84_A = 6378137.0;
constexpr double WGS84_E2 = 6.69437999014e-3;
constexpr double DEG_TO_RAD = M_PI / 180.0;
constexpr double RAD_TO_DEG = 180.0 / M_PI;
constexpr double HOUR_TO_SEC = 3600.0;

Fusion::Fusion()
{
    // 根据ADIS16465规格初始化噪声参数
    // 陀螺仪零偏不稳定性: 2.0 deg/hr -> rad/s
    m_gyro_bias_instability = 2.0 * DEG_TO_RAD / HOUR_TO_SEC;

    // 角度随机游走: 0.1 deg/sqrt(hr) -> rad/s/sqrt(Hz)
    m_gyro_noise = 0.1 * DEG_TO_RAD / std::sqrt(HOUR_TO_SEC);

    // 速度随机游走: 0.1 m/s/sqrt(hr) -> m/s²/sqrt(Hz)
    m_accel_noise = 0.1 / std::sqrt(HOUR_TO_SEC);

    // 陀螺零偏标准差: 25 deg/hr -> rad/s
    double gyro_bias_std = 25.0 * DEG_TO_RAD / HOUR_TO_SEC;

    // 加速度零偏标准差: 200 mGal -> m/s² (1 mGal = 1e-5 m/s²)
    double accel_bias_std = 200.0 * 1e-5;

    // 零偏相关时间: 1小时 -> 秒
    m_bias_correlation_time = 1.0 * HOUR_TO_SEC;

    // 初始化状态协方差 - 修复矩阵赋值问题
    m_P.setIdentity();
    
    // 使用diagonal()方法设置对角元素
    m_P.block<3, 3>(0, 0).diagonal() = Eigen::Vector3d(10.0, 10.0, 20.0);
    m_P.block<3, 3>(3, 3).diagonal() = Eigen::Vector3d(0.5, 0.5, 1.0);
    m_P.block<3, 3>(6, 6).diagonal() = Eigen::Vector3d(0.1, 0.1, 1.0) * DEG_TO_RAD;
    
    // 创建对角矩阵并显式转换为密集矩阵
    m_P.block<3, 3>(9, 9) = Eigen::Vector3d::Constant(gyro_bias_std * gyro_bias_std).asDiagonal().toDenseMatrix();
    m_P.block<3, 3>(12, 12) = Eigen::Vector3d::Constant(accel_bias_std * accel_bias_std).asDiagonal().toDenseMatrix();

    // 设置杠杆臂 (FRD坐标系: 前-右-下)
    m_lever_arm << -0.073, 0.302, 0.087; // 单位:米

    m_initialized = false;
    m_last_output_time = 0;
}

// IMU数据处理函数 (200Hz)
void Fusion::process_imu_data(const ImuData &imu)
{
    if (!m_initialized)
    {
        // 等待GNSS初始化
        m_last_imu = imu;
        return;
    }

    // 时间增量 (秒)
    double dt = calculate_dt(imu);
    if (dt <= 0 || dt > 0.1)
    {
        m_last_imu = imu;
        return;
    }

    // IMU数据预处理
    Eigen::Vector3d gyro(imu.gyro_x, imu.gyro_y, imu.gyro_z);
    Eigen::Vector3d accel(imu.acc_x, imu.acc_y, imu.acc_z);
    // 机械编排: 更新导航状态
    update_navigation_state(gyro, accel, dt);
    // ESKF预测: 更新误差状态协方差
    error_state_prediction(gyro, accel, dt);
    // 保存当前IMU用于下一次计算
    m_last_imu = imu;
}

// GNSS数据处理函数 (1Hz)
void Fusion::process_gnss_data(const GnssData &gnss)
{
    if (!m_initialized)
    {
        initialize(gnss);
    }
    // 临时保存IMU位置
    double imu_lat = m_lat;
    double imu_lon = m_lon;
    double imu_alt = m_alt;

    // 将IMU位置转换为天线位置
    compensate_lever_arm(false);

    // 处理GNSS更新
    error_state_update(gnss);

    // 恢复IMU位置
    m_lat = imu_lat;
    m_lon = imu_lon;
    m_alt = imu_alt;
}

double Fusion::calculate_dt(const ImuData &current_imu)
{
    if (m_last_imu.sec_week == 0)
        return 0.005; // 初始假设200Hz

    double dt = static_cast<double>(current_imu.sec_week - m_last_imu.sec_week);
    // 处理周秒翻转
    if (dt < 0)
        dt += 604800.0; // GPS周秒最大值

    // 添加帧ID增量
    double frame_diff = static_cast<double>(current_imu.frame_id - m_last_imu.frame_id);
    dt += frame_diff / 200.0; // 200Hz采样率

    return std::max(dt, 1e-6); // 确保最小时间增量
}

void Fusion::initialize(const GnssData &gnss)
{
    // 初始位置设为GNSS天线位置
    m_lat = gnss.lat;
    m_lon = gnss.lon;
    m_alt = gnss.alt;

    // 初始化速度和姿态
    m_vel.setZero();
    m_att.setZero();

    // 初始化零偏
    m_gyro_bias.setZero();
    m_accel_bias.setZero();

    // 补偿杠杆臂得到IMU位置
    compensate_lever_arm(true);

    m_initialized = true;
    m_last_output_time = gnss.sec_week;
}

void Fusion::compensate_lever_arm(bool gnss_to_imu)
{
    // 计算旋转矩阵 (FRD到NED)
    Eigen::Matrix3d R_bn = euler_to_rotation(m_att);

    // 将杠杆臂从载体坐标系转换到导航坐标系
    Eigen::Vector3d lever_arm_ned = R_bn * m_lever_arm;

    double lat_rad = m_lat * DEG_TO_RAD;
    double Rn = WGS84_A / std::sqrt(1.0 - WGS84_E2 * std::pow(std::sin(lat_rad), 2));
    double Rm = Rn * (1.0 - WGS84_E2) / (1.0 - WGS84_E2 * std::pow(std::sin(lat_rad), 2));

    // 计算位置偏移量
    double delta_lat = lever_arm_ned.x() / (Rm + m_alt) * RAD_TO_DEG;
    double delta_lon = lever_arm_ned.y() / ((Rn + m_alt) * std::cos(lat_rad)) * RAD_TO_DEG;
    double delta_alt = -lever_arm_ned.z(); // 高度向下为正

    if (gnss_to_imu)
    {
        // GNSS天线位置 -> IMU位置 (减去偏移)
        m_lat -= delta_lat;
        m_lon -= delta_lon;
        m_alt -= delta_alt;
    }
    else
    {
        // IMU位置 -> GNSS天线位置 (加上偏移)
        m_lat += delta_lat;
        m_lon += delta_lon;
        m_alt += delta_alt;
    }
}

auto Fusion::get_fusion_data(uint64_t sec_week, uint64_t frame_id) -> FusionData
{
    FusionData fusion;
    fusion.frame_id = m_frame_id++;
    fusion.sec_week = sec_week;
    fusion.lat = m_lat;
    fusion.lon = m_lon;
    fusion.alt = m_alt;
    fusion.vel_n = m_vel(0);
    fusion.vel_e = m_vel(1);
    fusion.vel_d = m_vel(2);

    // 转换姿态为角度制
    fusion.att_r = m_att(0) * RAD_TO_DEG;
    fusion.att_p = m_att(1) * RAD_TO_DEG;
    fusion.att_y = m_att(2) * RAD_TO_DEG;
    m_last_output_time = sec_week;

    return std::move(fusion);
}

void Fusion::update_navigation_state(const Eigen::Vector3d &gyro, const Eigen::Vector3d &accel, double dt)
{
    // 1. 补偿零偏
    Eigen::Vector3d unbiased_gyro = gyro - m_gyro_bias;
    Eigen::Vector3d unbiased_accel = accel - m_accel_bias;

    // 2. 更新姿态 (使用四元数)
    update_attitude(unbiased_gyro, dt);

    // 3. 转换加速度到导航系
    Eigen::Matrix3d R_bn = euler_to_rotation(m_att);
    Eigen::Vector3d acc_n = R_bn * unbiased_accel;

    // 4. 考虑重力和科氏力
    Eigen::Vector3d gravity(0, 0, calculate_gravity(m_lat, m_alt));
    acc_n -= gravity;

    // 5. 更新速度
    Eigen::Vector3d prev_vel = m_vel;
    m_vel += acc_n * dt;

    // 6. 更新位置
    update_position((prev_vel + m_vel) * 0.5, dt);
}

double Fusion::calculate_gravity(double lat, double alt)
{
    double sin2 = std::pow(std::sin(lat * DEG_TO_RAD), 2);
    return 9.7803253359 * (1.0 + 0.001931853 * sin2) / std::sqrt(1.0 - WGS84_E2 * sin2);
}

void Fusion::update_attitude(const Eigen::Vector3d &gyro, double dt)
{
    // 转换为四元数
    Eigen::Quaterniond q = euler_to_quaternion(m_att);

    // 构造旋转矢量
    Eigen::Vector3d rotation_vector = gyro * dt;
    double rotation_angle = rotation_vector.norm();

    if (rotation_angle > 1e-12)
    {
        Eigen::Vector3d rotation_axis = rotation_vector / rotation_angle;
        Eigen::Quaterniond dq(Eigen::AngleAxisd(rotation_angle, rotation_axis));
        q = (q * dq).normalized();
    }
    else
    {
        // 小角度近似
        Eigen::Quaterniond dq(1.0,
                              rotation_vector.x() / 2,
                              rotation_vector.y() / 2,
                              rotation_vector.z() / 2);
        q = (q * dq).normalized();
    }

    // 转换回欧拉角
    m_att = quaternion_to_euler(q);

    // 保持航向角在[0, 360)度范围
    if (m_att(2) < 0)
        m_att(2) += 2 * M_PI;
    else if (m_att(2) >= 2 * M_PI)
        m_att(2) -= 2 * M_PI;
}

void Fusion::update_position(const Eigen::Vector3d &vel_avg, double dt)
{
    double lat_rad = m_lat * DEG_TO_RAD;
    double Rn = WGS84_A / std::sqrt(1.0 - WGS84_E2 * std::pow(std::sin(lat_rad), 2));
    double Rm = Rn * (1.0 - WGS84_E2) / (1.0 - WGS84_E2 * std::pow(std::sin(lat_rad), 2));

    m_lat += (vel_avg(0) / (Rm + m_alt)) * RAD_TO_DEG * dt;
    m_lon += (vel_avg(1) / ((Rn + m_alt) * std::cos(lat_rad))) * RAD_TO_DEG * dt;
    m_alt += vel_avg(2) * dt;
}

void Fusion::error_state_prediction(const Eigen::Vector3d &gyro, const Eigen::Vector3d &accel, double dt)
{
    // 状态转移矩阵 (连续时间)
    Eigen::Matrix<double, 15, 15> F = Eigen::Matrix<double, 15, 15>::Zero();

    // 位置误差部分
    F.block<3, 3>(0, 3) = Eigen::Matrix3d::Identity();

    // 速度误差部分
    Eigen::Matrix3d R_bn = euler_to_rotation(m_att);
    F.block<3, 3>(3, 6) = -R_bn * skew_matrix(accel - m_accel_bias);
    F.block<3, 3>(3, 12) = -R_bn;

    // 姿态误差部分
    F.block<3, 3>(6, 6) = -skew_matrix(gyro - m_gyro_bias);
    F.block<3, 3>(6, 9) = -Eigen::Matrix3d::Identity();

    // 零偏相关时间处理
    double tau_inv = 1.0 / m_bias_correlation_time;
    F.block<3, 3>(9, 9) = -tau_inv * Eigen::Matrix3d::Identity();
    F.block<3, 3>(12, 12) = -tau_inv * Eigen::Matrix3d::Identity();

    // 离散化
    Eigen::Matrix<double, 15, 15> Fk = Eigen::Matrix<double, 15, 15>::Identity() + F * dt;

    // 过程噪声协方差
    Eigen::Matrix<double, 15, 15> Q = Eigen::Matrix<double, 15, 15>::Zero();
    Q.block<3, 3>(3, 3) = Eigen::Matrix3d::Identity() * m_accel_noise * m_accel_noise * dt;
    Q.block<3, 3>(6, 6) = Eigen::Matrix3d::Identity() * m_gyro_noise * m_gyro_noise * dt;

    // 零偏过程噪声
    double gyro_bias_noise = m_gyro_bias_instability * std::sqrt(2.0 * dt / m_bias_correlation_time);
    Q.block<3, 3>(9, 9) = Eigen::Matrix3d::Identity() * gyro_bias_noise * gyro_bias_noise;
    Q.block<3, 3>(12, 12) = Eigen::Matrix3d::Identity() * m_accel_noise * m_accel_noise * dt;

    // 协方差预测
    m_P = Fk * m_P * Fk.transpose() + Q;
}

void Fusion::error_state_update(const GnssData &gnss)
{
    // 观测矩阵 H [3×15]
    Eigen::Matrix<double, 3, 15> H = Eigen::Matrix<double, 3, 15>::Zero();
    H.block<3, 3>(0, 0) = Eigen::Matrix3d::Identity();

    // 观测噪声协方差
    Eigen::Matrix3d R = Eigen::Matrix3d::Identity();
    R(0, 0) = gnss.lat_std * gnss.lat_std;
    R(1, 1) = gnss.lon_std * gnss.lon_std;
    R(2, 2) = gnss.alt_std * gnss.alt_std;

    // 卡尔曼增益
    Eigen::MatrixXd Ht = H.transpose();
    Eigen::MatrixXd S = H * m_P * Ht + R;
    Eigen::MatrixXd K = m_P * Ht * S.inverse();

    // 位置观测残差
    Eigen::Vector3d residual;
    residual(0) = gnss.lat - m_lat;
    residual(1) = gnss.lon - m_lon;
    residual(2) = gnss.alt - m_alt;

    // 转换为米
    double lat_rad = m_lat * DEG_TO_RAD;
    double Rn = WGS84_A / std::sqrt(1.0 - WGS84_E2 * std::pow(std::sin(lat_rad), 2));
    double Rm = Rn * (1.0 - WGS84_E2) / (1.0 - WGS84_E2 * std::pow(std::sin(lat_rad), 2));

    residual(0) *= DEG_TO_RAD * (Rm + m_alt);
    residual(1) *= DEG_TO_RAD * (Rn + m_alt) * std::cos(lat_rad);

    // 误差状态更新
    Eigen::Matrix<double, 15, 1> dx = K * residual;

    // 状态修正
    m_lat += dx(0) / (Rm + m_alt) * RAD_TO_DEG;
    m_lon += dx(1) / ((Rn + m_alt) * std::cos(lat_rad)) * RAD_TO_DEG;
    m_alt += dx(2);

    m_vel += dx.segment<3>(3);

    // 姿态修正 (使用四元数)
    Eigen::Vector3d dtheta = dx.segment<3>(6);
    Eigen::Quaterniond q = euler_to_quaternion(m_att);
    Eigen::Quaterniond dq(1.0, dtheta.x() / 2, dtheta.y() / 2, dtheta.z() / 2);
    dq.normalize();
    q = (q * dq).normalized();
    m_att = quaternion_to_euler(q);

    // 零偏修正
    m_gyro_bias += dx.segment<3>(9);
    m_accel_bias += dx.segment<3>(12);

    // 协方差更新 (Joseph形式)
    Eigen::MatrixXd I = Eigen::Matrix<double, 15, 15>::Identity();
    Eigen::MatrixXd IKH = I - K * H;
    m_P = IKH * m_P * IKH.transpose() + K * R * K.transpose();

    // 强制协方差对称
    m_P = (m_P + m_P.transpose()) / 2.0;
}

Eigen::Matrix3d Fusion::euler_to_rotation(const Eigen::Vector3d &euler)
{
    double cr = std::cos(euler(0));
    double sr = std::sin(euler(0));
    double cp = std::cos(euler(1));
    double sp = std::sin(euler(1));
    double cy = std::cos(euler(2));
    double sy = std::sin(euler(2));

    Eigen::Matrix3d R;
    R << cp * cy, sr * sp * cy - cr * sy, cr * sp * cy + sr * sy,
        cp * sy, sr * sp * sy + cr * cy, cr * sp * sy - sr * cy,
        -sp, sr * cp, cr * cp;
    return R;
}

Eigen::Quaterniond Fusion::euler_to_quaternion(const Eigen::Vector3d &euler)
{
    double cr = std::cos(euler(0) / 2);
    double sr = std::sin(euler(0) / 2);
    double cp = std::cos(euler(1) / 2);
    double sp = std::sin(euler(1) / 2);
    double cy = std::cos(euler(2) / 2);
    double sy = std::sin(euler(2) / 2);

    Eigen::Quaterniond q;
    q.w() = cr * cp * cy + sr * sp * sy;
    q.x() = sr * cp * cy - cr * sp * sy;
    q.y() = cr * sp * cy + sr * cp * sy;
    q.z() = cr * cp * sy - sr * sp * cy;
    return q;
}

Eigen::Vector3d Fusion::quaternion_to_euler(const Eigen::Quaterniond &q)
{
    Eigen::Vector3d euler;

    // 横滚角 (x-axis)
    double sinr_cosp = 2 * (q.w() * q.x() + q.y() * q.z());
    double cosr_cosp = 1 - 2 * (q.x() * q.x() + q.y() * q.y());
    euler(0) = std::atan2(sinr_cosp, cosr_cosp);

    // 俯仰角 (y-axis)
    double sinp = 2 * (q.w() * q.y() - q.z() * q.x());
    if (std::fabs(sinp) >= 1)
        euler(1) = std::copysign(M_PI / 2, sinp); // 处理万向节锁
    else
        euler(1) = std::asin(sinp);

    // 航向角 (z-axis)
    double siny_cosp = 2 * (q.w() * q.z() + q.x() * q.y());
    double cosy_cosp = 1 - 2 * (q.y() * q.y() + q.z() * q.z());
    euler(2) = std::atan2(siny_cosp, cosy_cosp);

    return euler;
}

Eigen::Matrix3d Fusion::skew_matrix(const Eigen::Vector3d &v)
{
    Eigen::Matrix3d S;
    S << 0, -v(2), v(1),
        v(2), 0, -v(0),
        -v(1), v(0), 0;
    return S;
}
