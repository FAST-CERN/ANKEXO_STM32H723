/*
 * sensor_data.hpp
 *
 *  Created on: Aug 5, 2025
 *      Author: ruich
 */

#ifndef APP_IMU_SENSOR_DATA_HPP_
#define APP_IMU_SENSOR_DATA_HPP_

#include <math.h>

namespace pose{
struct quat_f{
public:
	float data[4];
	float *w;
	float *x;
	float *y;
	float *z;

	quat_f()
	:w(&data[0]),x(&data[1]),y(&data[2]),z(&data[3]){
		*w = 1.0f;
		*x = 0.0f;
		*y = 0.0f;
		*z = 0.0f;
	}

	quat_f(float w, float x, float y, float z)
	:w(&data[0]),x(&data[1]),y(&data[2]),z(&data[3]){
		this->data[0] = w;
		this->data[1] = x;
		this->data[2] = y;
		this->data[3] = z;
	}

	void reset() {
        *w = 1.0f;
        *x = 0.0f;
        *y = 0.0f;
        *z = 0.0f;
    }

	void to_rpy(float rpy[3]) const {
		float q0 = data[0];
		float q1 = data[1];
		float q2 = data[2];
		float q3 = data[3];

        rpy[0] = atan2(q0*q1+q2*q3, 0.5f-q1*q1-q2*q2); 	// Roll
        rpy[1] = asinf(q0*q2 - q3*q1); 					// Pitch
        rpy[2] = atan2(q0*q3+q1*q2, 0.5f-q2*q2-q3*q3); 	// Yaw

        // Convert rad to degree
        rpy[0] *= 57.29578f;
        rpy[1] *= 57.29578f;
        rpy[2] *= 57.29578f;
	}
};

}

namespace sensor_data{
struct imu{
public:
    pose::quat_f quat;
    float acc[3];
    float gyro[3];
    float mag[3];
    float temp;

    // calib parameters
    float gyro_bias[3];			// gyro calibration parameters

    float acc_bias[3];
    float acc_mat[3][3];

    float mag_calib_hard[3];
    float mag_calib_soft[3][3];

    bool data_ready;
    bool use_mag;		// use magnetometer data

    imu(){
		quat = pose::quat_f();
        for (int i = 0; i < 3; ++i) {
            acc[i] = 0.0f;
            gyro[i] = 0.0f;
            mag[i] = 0.0f;
        }
        temp = 0.0f;
        data_ready = false;
        use_mag = true;			// default use magnetometer data
    }

    void reset() {
        quat.reset();
        for (int i = 0; i < 3; ++i) {
            acc[i] = 0.0f;
            gyro[i] = 0.0f;
            mag[i] = 0.0f;
        }
        temp = 0.0f;
        data_ready = false;
    }

    void calib(){
    	// accel
    	float ax,ay,az;
		ax = acc[0] - acc_bias[0];
		ay = acc[1] - acc_bias[1];
		az = acc[2] - acc_bias[2];

		acc[0] = ax * acc_mat[0][0] +
				 ay * acc_mat[0][1] +
				 az * acc_mat[0][2];
		acc[1] = ax * acc_mat[1][0] +
				 ay * acc_mat[1][1] +
				 az * acc_mat[1][2];
		acc[2] = ax * acc_mat[2][0] +
				 ay * acc_mat[2][1] +
				 az * acc_mat[2][2];

        // gyro
		gyro[0] -= gyro_bias[0];
		gyro[1] -= gyro_bias[1];
		gyro[2] -= gyro_bias[2];

		// mag
		if (!use_mag) return;	// if not use magnetometer data, return
		float mx,my,mz;
		mx = mag[0] - mag_calib_hard[0];
		my = mag[1] - mag_calib_hard[1];
		mz = mag[2] - mag_calib_hard[2];

		mag[0] = mx * mag_calib_soft[0][0] +
				 my * mag_calib_soft[0][1] +
				 mz * mag_calib_soft[0][2];
		mag[1] = mx * mag_calib_soft[1][0] +
				 my * mag_calib_soft[1][1] +
				 mz * mag_calib_soft[1][2];
		mag[2] = mx * mag_calib_soft[2][0] +
				 my * mag_calib_soft[2][1] +
				 mz * mag_calib_soft[2][2];
    }
};

}

namespace filter{
namespace lpf{
class butworth_2nd{
public:
	butworth_2nd(float cutoff_freq, float sample_freq){
		for (int i = 0; i < 2; ++i) {
			m_in_his[i] = 0.0f;
			m_out_his[i] = 0.0f;
		}
		float warped_freq = 2.0f * sample_freq * tan(M_PI * cutoff_freq / sample_freq);
		float k1 = sqrt(2.0f) / warped_freq;
		float k2 = 1.0f / (warped_freq * warped_freq);

		m_a0 = k2 + k1 + 1.0f;
		m_a1 = 2.0f * (k2 - 1.0f);
		m_a2 = k2 - k1 + 1.0f;

		m_b0 = 1.0f;
		m_b1 = 2.0f;
		m_b2 = 1.0f;

		m_a1 /= m_a0;
		m_a2 /= m_a0;
		m_b0 /= m_a0;
		m_b1 /= m_a0;
		m_b2 /= m_a0;
	}

	/**
	 * @brief 对单个输入样本应用滤波器
	 * @param input 新的原始数据
	 * @return 滤波后的数据
	 */
	float apply(float input) {
		// y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]

		float output = m_b0 * input +
					   m_b1 * m_in_his[0] +
					   m_b2 * m_in_his[1] -
					   m_a1 * m_out_his[0] -
					   m_a2 * m_out_his[1];

		m_in_his[1] = m_in_his[0];
		m_in_his[0] = input;

		m_out_his[1] = m_out_his[0];
		m_out_his[0] = output;

		return output;
	}

	/**
	 * @brief 重置滤波器状态。
	 * * 将所有历史输入和输出清零
	 */
	void reset() {
		for (int i = 0; i < 2; ++i) {
			m_in_his[i] = 0.0f;
			m_out_his[i] = 0.0f;
		}
	}


private:
	// 滤波器系数
	float m_a0, m_a1, m_a2;
	float m_b0, m_b1, m_b2;

	// 历史数据缓冲区
	// m_in_his[0]  -> x[n-1]
	// m_in_his[1]  -> x[n-2]
	float m_in_his[2];
	// m_out_his[0] -> y[n-1]
	// m_out_his[1] -> y[n-2]
	float m_out_his[2];
};

class lpf_1st{
public:
    /**
     * @brief 构造函数。
     * @param cutoff_freq 滤波器的截止频率 (Hz)。
     * @param sample_freq 信号的采样频率 (Hz)。
     */
	lpf_1st(float cutoff_freq, float sample_freq) {
        float rc = 0.159154943f / cutoff_freq;
        float dt = 1.0f / sample_freq;

        m_alpha = dt / (rc + dt);

        m_last_out = 0.0f;
        m_is_initial = false;
    }

    /**
     * @brief 对单个输入样本应用滤波器。
     * @param input 新的原始数据点。
     * @return 滤波后的数据点。
     */
    float apply(float input) {
        // 如果是第一次应用滤波器，直接将当前输入作为初始输出，以避免启动时的跳变
        if (!m_is_initial) {
        	m_last_out = input;
        	m_is_initial = true;
        }

        // 应用一阶低通滤波的核心公式
        float output = m_alpha * input + (1.0f - m_alpha) * m_last_out;

        // 更新上一次的输出值，为下一次计算做准备
        m_last_out = output;

        return output;
    }

    /**
     * @brief 重置滤波器状态。
     * 将历史输出清零，以便于处理一段新的、不连续的信号。
     */
    void reset() {
    	m_last_out = 0.0f;
        m_is_initial = false;
    }

private:
    float m_alpha;
    float m_last_out;
    bool  m_is_initial;
};

class mov_ave{
public:
    explicit mov_ave(size_t window_size)
        : m_window_size(window_size > 0 ? window_size : 1), // 确保窗口大小至少为1
          m_window(nullptr)
    {
        m_window = new float[m_window_size];
        reset();
    }

    ~mov_ave() {
        delete[] m_window;
        m_window = nullptr;
    }

    mov_ave(const mov_ave&) = delete;
    mov_ave& operator=(const mov_ave&) = delete;

    float apply(float input) {
        if (!m_is_initialized) {
            for (size_t i = 0; i < m_window_size; ++i) {
                m_window[i] = input;
            }
            m_current_sum = input * m_window_size;
            m_is_initialized = true;
        }

        m_current_sum -= m_window[m_current_index];
        m_window[m_current_index] = input;
        m_current_sum += input;

        m_current_index++;
        if (m_current_index >= m_window_size) {
            m_current_index = 0;
        }

        return m_current_sum / m_window_size;
    }

    void reset() {
        for (size_t i = 0; i < m_window_size; ++i) {
            m_window[i] = 0.0f;
        }
        m_current_sum = 0.0f;
        m_current_index = 0;
        m_is_initialized = false;
    }

private:
    size_t m_window_size;       // 窗口大小
    float* m_window;            // 指向数据窗口的指针
    float m_current_sum;        // 当前窗口内所有数据的总和
    size_t m_current_index;     // 指向窗口中最旧数据的索引
    bool m_is_initialized;      // 标志位，用于处理第一次输入
};


}
}



#endif /* APP_IMU_SENSOR_DATA_HPP_ */
