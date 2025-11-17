// ======================= author: FAST_CARN ============================== //
#include <imu_mahony.h>


// use first accel data to initial AHRS orientation
void ahrs_init(float accel[3], AHRS_Param *param){
	float recipNorm;
	float ax, ay, az;
	float cr2,cp2,cy2,sr2,sp2,sy2;	// sin, cos value of 1/2 roll, pitch, yaw

	float pitch, roll, yaw;
	float q0, q1, q2, q3;

	ax = accel[0];
	ay = accel[1];
	az = accel[2];

	// Normalize accelerometer data
	recipNorm = inv_sqrt_f32(ax*ax + ay*ay + az*az);
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;

	float norm;
	arm_sqrt_f32(ay*ay + az*az, &norm);

	// Calculate roll, pitch angles from accelerometer data
	arm_atan2_f32(ay, az, &roll);		// roll = atan2(ay, az)
	arm_atan2_f32(-ax, az, &norm);	    // pitch = atan2(-ax, az)
	yaw = 0.0f; 						// yaw is set to 0.0f initially

	cr2 = arm_cos_f32(roll * 0.5f);
	cp2 = arm_cos_f32(pitch* 0.5f);
	cy2 = arm_cos_f32(yaw  * 0.5f);
	sr2 = arm_sin_f32(roll * 0.5f);
	sp2 = arm_sin_f32(pitch* 0.5f);
	sy2 = arm_sin_f32(yaw  * 0.5f);

	q0 = cr2 * cp2 * cy2 + sr2 * sp2 * sy2;
	q1 = sr2 * cp2 * cy2 - cr2 * sp2 * sy2;
	q2 = cr2 * sp2 * cy2 + sr2 * cp2 * sy2;
	q3 = cr2 * cp2 * sy2 - sr2 * sp2 * cy2;

	recipNorm = inv_sqrt_f32(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	param -> ori.quaternion[0] = q0 * recipNorm;
	param -> ori.quaternion[1] = q1 * recipNorm;
	param -> ori.quaternion[2] = q2 * recipNorm;
	param -> ori.quaternion[3] = q3 * recipNorm;

	param -> ori.rpy[0] = roll;
	param -> ori.rpy[1] = pitch;
	param -> ori.rpy[2] = yaw;
}

void ahrs_init_mag(float accel[3], float mag[3], AHRS_Param *param){
	float recipNorm;
	float ax, ay, az;
	float mx, my, mz;
	float sr,cr,sp,cp;
	float cr2,cp2,cy2,sr2,sp2,sy2;	// sin, cos value of 1/2 roll, pitch, yaw

	float pitch, roll, yaw;
	float q0, q1, q2, q3;

	ax = accel[0];
	ay = accel[1];
	az = accel[2];

	mx = mag[0];
	my = mag[1];
	mz = mag[2];

	// Normalize accelerometer data
	recipNorm = inv_sqrt_f32(ax*ax + ay*ay + az*az);
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;

	// Normalize magnetometer data
	recipNorm = inv_sqrt_f32(mx*mx + my*my + mz*mz);
	mx *= recipNorm;
	my *= recipNorm;
	mz *= recipNorm;

	float norm;
	arm_sqrt_f32(ay*ay + az*az, &norm);

	// Calculate roll, pitch angles from accelerometer data
	arm_atan2_f32(ay, az, &roll);		// roll = atan2(ay, az)
	arm_atan2_f32(-ax, az, &norm);	    // pitch = atan2(-ax, az)

	// calculate yaw
	sr = arm_sin_f32(roll);
	sp = arm_sin_f32(pitch);
	cr = arm_cos_f32(roll);
	cp = arm_cos_f32(pitch);

	float magX = mx * cp + my * sp * sr + mz * sp * cr;
	float magY = my * cr - mz * sr;
	arm_atan2_f32(-magY, magX, &yaw);	// yaw = atan2(-my, mx)

	cr2 = arm_cos_f32(roll * 0.5f);
	cp2 = arm_cos_f32(pitch* 0.5f);
	cy2 = arm_cos_f32(yaw  * 0.5f);
	sr2 = arm_sin_f32(roll * 0.5f);
	sp2 = arm_sin_f32(pitch* 0.5f);
	sy2 = arm_sin_f32(yaw  * 0.5f);

	q0 = cr2 * cp2 * cy2 + sr2 * sp2 * sy2;
	q1 = sr2 * cp2 * cy2 - cr2 * sp2 * sy2;
	q2 = cr2 * sp2 * cy2 + sr2 * cp2 * sy2;
	q3 = cr2 * cp2 * sy2 - sr2 * sp2 * cy2;

	recipNorm = inv_sqrt_f32(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	param -> ori.quaternion[0] = q0 * recipNorm;
	param -> ori.quaternion[1] = q1 * recipNorm;
	param -> ori.quaternion[2] = q2 * recipNorm;
	param -> ori.quaternion[3] = q3 * recipNorm;

	param -> ori.rpy[0] = roll;
	param -> ori.rpy[1] = pitch;
	param -> ori.rpy[2] = yaw;
}


/**
 * @brief AHRS (Attitude and Heading Reference System) 姿态更新函数
 * @details
 * 该函数实现了 Madgwick 互补滤波器算法，用于融合陀螺仪和加速度计数据，
 * 以更新设备的姿态估计（以四元数和欧拉角表示）。
 *
 * @param[in] gyro[3]  包含陀螺仪三轴角速度数据的浮点数组 (rad/s)。  顺序：[gx, gy, gz]。
 * @param[in] accel[3] 包含加速度计三轴加速度数据的浮点数组 (m/s^2)。顺序：[ax, ay, az]。
 * @param[in,out] param 指向 AHRS_Param 结构体的指针。
 *
 * @note
 * - 在调用此函数之前，必须正确设置 `param` 结构体中的 `k_p`, `k_i` 和 `sample_freq`。
 */
void ahrs_update(float gyro[3], float accel[3], AHRS_Param *param){
	if(!param->is_init) {
		param->is_init = 1; // set initialized flag
		ahrs_init(accel,param);
		return;
	}

	float q0, q1, q2, q3;
	float ax, ay, az;
	float gx, gy, gz;

	//	temporary variables for calculations
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	q0 = param->ori.quaternion[0];
	q1 = param->ori.quaternion[1];
	q2 = param->ori.quaternion[2];
	q3 = param->ori.quaternion[3];

	ax = accel[0];
	ay = accel[1];
	az = accel[2];

	gx = gyro[0];
	gy = gyro[1];
	gz = gyro[2];

	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
		recipNorm = inv_sqrt_f32(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;

		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		if(param->k_i > 0.0f) {
			param->integral[0] += param->k_i * halfex * param->inv_sampfreq;
			param->integral[1] += param->k_i * halfey * param->inv_sampfreq;
			param->integral[2] += param->k_i * halfez * param->inv_sampfreq;

			gx += param->integral[0];
			gy += param->integral[1];
			gz += param->integral[2];
		}
		else {
			param->integral[0] = 0.0f;
			param->integral[1] = 0.0f;
			param->integral[2] = 0.0f;
		}

		gx += param->k_p * halfex;
		gy += param->k_p * halfey;
		gz += param->k_p * halfez;
	}

	gx *= (0.5f * param->inv_sampfreq);
	gy *= (0.5f * param->inv_sampfreq);
	gz *= (0.5f * param->inv_sampfreq);

	qa = q0;
	qb = q1;
	qc = q2;

	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += ( qa * gx + qc * gz - q3 * gy);
	q2 += ( qa * gy - qb * gz + q3 * gx);
	q3 += ( qa * gz + qb * gy - qc * gx);

	recipNorm = inv_sqrt_f32(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;

	param -> ori.quaternion[0] = q0;
	param -> ori.quaternion[1] = q1;
	param -> ori.quaternion[2] = q2;
	param -> ori.quaternion[3] = q3;

	quat_2_rpy(param -> ori.quaternion,param -> ori.rpy);
}

void ahrs_update_mag(float gyro[3], float accel[3], float mag[3], AHRS_Param *param){
	if(!param->is_init) {
			param->is_init = 1; // set initialized flag
			ahrs_init_mag(accel,mag,param);
			return;
	}

	float q0, q1, q2, q3;
	float ax, ay, az;
	float gx, gy, gz;
	float mx, my, mz;

	//	temporary variables for calculations
	float recipNorm;
	float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz;
	float halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	q0 = param->ori.quaternion[0];
	q1 = param->ori.quaternion[1];
	q2 = param->ori.quaternion[2];
	q3 = param->ori.quaternion[3];

	ax = accel[0];
	ay = accel[1];
	az = accel[2];

	gx = gyro[0];
	gy = gyro[1];
	gz = gyro[2];

	mx = mag[0];
	my = mag[1];
	mz = mag[2];

	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
		recipNorm = inv_sqrt_f32(ax*ax + ay*ay + az*az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		recipNorm = inv_sqrt_f32(mx*mx + my*my + mz*mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		q0q0 = q0 * q0;
		q0q1 = q0 * q1;
		q0q2 = q0 * q2;
		q0q3 = q0 * q3;
		q1q1 = q1 * q1;
		q1q2 = q1 * q2;
		q1q3 = q1 * q3;
		q2q2 = q2 * q2;
		q2q3 = q2 * q3;
		q3q3 = q3 * q3;

		// Reference direction of Earth's magnetic field
		hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
		hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
		arm_sqrt_f32(hx * hx + hy * hy, &bx);
		bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;

//		halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
//		halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
//		halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

		halfwx = bx * (q1q2 + q0q3)       + bz * (q1q3 - q0q2);
		halfwy = bx * (0.5f - q1q1 - q3q3) + bz * (q2q3 + q0q1);
		halfwz = bx * (q2q3 - q0q1)       + bz * (0.5f - q1q1 - q2q2);

		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

		if(param->k_i > 0.0f) {
			param->integral[0] += param->k_i * halfex * param->inv_sampfreq;
			param->integral[1] += param->k_i * halfey * param->inv_sampfreq;
			param->integral[2] += param->k_i * halfez * param->inv_sampfreq;

			gx += param->integral[0];
			gy += param->integral[1];
			gz += param->integral[2];
		}
		else {
			param->integral[0] = 0.0f;
			param->integral[1] = 0.0f;
			param->integral[2] = 0.0f;
		}

		gx += param->k_p * halfex;
		gy += param->k_p * halfey;
		gz += param->k_p * halfez;
	}

	gx *= (0.5f * param->inv_sampfreq);
	gy *= (0.5f * param->inv_sampfreq);
	gz *= (0.5f * param->inv_sampfreq);

	qa = q0;
	qb = q1;
	qc = q2;

	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += ( qa * gx + qc * gz - q3 * gy);
	q2 += ( qa * gy - qb * gz + q3 * gx);
	q3 += ( qa * gz + qb * gy - qc * gx);

	recipNorm = inv_sqrt_f32(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;

	param -> ori.quaternion[0] = q0;
	param -> ori.quaternion[1] = q1;
	param -> ori.quaternion[2] = q2;
	param -> ori.quaternion[3] = q3;

	quat_2_rpy(param -> ori.quaternion,param -> ori.rpy);
}

void ahrs_update_mag_adapt(float gyro[3], float accel[3], float mag[3], AHRS_Param *param) {
    if (!param->is_init) {
        param->is_init = 1;
        ahrs_init_mag(accel, mag, param);
        return;
    }

    float q0 = param->ori.quaternion[0];
    float q1 = param->ori.quaternion[1];
    float q2 = param->ori.quaternion[2];
    float q3 = param->ori.quaternion[3];

    float gx = gyro[0];
    float gy = gyro[1];
    float gz = gyro[2];

    float ax = accel[0];
    float ay = accel[1];
    float az = accel[2];

    float mx = mag[0];
    float my = mag[1];
    float mz = mag[2];

    // 临时变量
    float recipNorm;
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
    float hx, hy, bx, bz;
    float halfvx, halfvy, halfvz;
    float halfwx, halfwy, halfwz;
    float halfex, halfey, halfez;
    float qa, qb, qc;

    // 只有在加速度计数据有效时才进行更新
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        float mag_norm;
        arm_sqrt_f32(mx * mx + my * my + mz * mz, &mag_norm);

        float mag_weight = 1.0f;
        float mag_norm_diff = fabs(mag_norm - param->mag_norm_t);
		mag_weight -= (mag_norm_diff / param->mag_norm_t);
		if (mag_weight < 0.0f) {
			mag_weight = 0.0f;
	    }

        // 归一化加速度计和磁力计读数
        recipNorm = inv_sqrt_f32(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        recipNorm = inv_sqrt_f32(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // 辅助变量，避免重复计算
        q0q0 = q0 * q0; q0q1 = q0 * q1; q0q2 = q0 * q2; q0q3 = q0 * q3;
        q1q1 = q1 * q1; q1q2 = q1 * q2; q1q3 = q1 * q3;
        q2q2 = q2 * q2; q2q3 = q2 * q3; q3q3 = q3 * q3;

        // --- 2. 计算参考向量和误差 ---

        // 加速度计部分
        // 从机体坐标系计算出的重力向量
        halfvx = q1 * q3 - q0 * q2;
        halfvy = q0 * q1 + q2 * q3;
        halfvz = q0q0 - 0.5f + q3q3;
        // 加速度计测量值与参考重力向量的误差
        float ex_acc = (ay * halfvz - az * halfvy);
        float ey_acc = (az * halfvx - ax * halfvz);
        float ez_acc = (ax * halfvy - ay * halfvx);

        // 磁力计部分
        // 将机体坐标系的磁场读数转换到地理坐标系
        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
        // 计算水平面上的磁场参考方向
        arm_sqrt_f32(hx * hx + hy * hy, &bx);
        bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));
        // 从地理坐标系计算出的磁场参考向量
        halfwx = bx * (q1q2 + q0q3) + bz * (q1q3 - q0q2);
		halfwy = bx * (0.5f - q1q1 - q3q3) + bz * (q2q3 + q0q1);
		halfwz = bx * (q2q3 - q0q1) + bz * (0.5f - q1q1 - q2q2);

//        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
//        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
//        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);
        // 磁力计测量值与参考磁场向量的误差
        float ex_mag = (my * halfwz - mz * halfwy);
        float ey_mag = (mz * halfwx - mx * halfwz);
        float ez_mag = (mx * halfwy - my * halfwx);

        // --- 3. 融合加权后的误差 ---
        halfex = ex_acc + mag_weight * ex_mag;
        halfey = ey_acc + mag_weight * ey_mag;
        halfez = ez_acc + mag_weight * ez_mag;

        // --- 4. PI控制器和四元数更新 ---
        if (param->k_i > 0.0f) {
            param->integral[0] += param->k_i * halfex * param->inv_sampfreq;
            param->integral[1] += param->k_i * halfey * param->inv_sampfreq;
            param->integral[2] += param->k_i * halfez * param->inv_sampfreq;
            gx += param->integral[0];
            gy += param->integral[1];
            gz += param->integral[2];
        } else {
            param->integral[0] = 0.0f;
            param->integral[1] = 0.0f;
            param->integral[2] = 0.0f;
        }

        gx += param->k_p * halfex;
        gy += param->k_p * halfey;
        gz += param->k_p * halfez;
    }

    // 陀螺仪积分更新四元数
    gx *= (0.5f * param->inv_sampfreq);
    gy *= (0.5f * param->inv_sampfreq);
    gz *= (0.5f * param->inv_sampfreq);

    qa = q0;
    qb = q1;
    qc = q2;

    q0 += (-qb * gx - qc * gy - q3 * gz);
    q1 += (qa * gx + qc * gz - q3 * gy);
    q2 += (qa * gy - qb * gz + q3 * gx);
    q3 += (qa * gz + qb * gy - qc * gx);

    // 归一化四元数
    recipNorm = inv_sqrt_f32(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    param->ori.quaternion[0] = q0;
    param->ori.quaternion[1] = q1;
    param->ori.quaternion[2] = q2;
    param->ori.quaternion[3] = q3;

    // 更新欧拉角
    quat_2_rpy(param->ori.quaternion, param->ori.rpy);
}

// clear AHRS parameters to default values
void ahrs_param_deinit(AHRS_Param *param){
	param -> is_init = 0;			// set initialized flag
	param -> sample_freq = AHRS_SAMPLE_FREQ_DEFAULT;
	param -> inv_sampfreq = 1.0f/AHRS_SAMPLE_FREQ_DEFAULT;
	param -> k_p = AHRS_KP_DEFAULT; 			// default proportional gain
	param -> k_i = AHRS_KI_DEFAULT; 			// default integral gain

	// clear orientation
	param -> ori.rpy[0] = 0.0;
	param -> ori.rpy[1] = 0.0;
	param -> ori.rpy[2] = 0.0;

	param -> ori.quaternion[0] = 1.0; // w
	param -> ori.quaternion[1] = 0.0; // x
	param -> ori.quaternion[2] = 0.0; // y
	param -> ori.quaternion[3] = 0.0; // z

	// clear integral error
	param -> integral[0] = 0.0;
	param -> integral[1] = 0.0;
	param -> integral[2] = 0.0;

	param -> mag_norm_t = 1.0;
}

// Convert quaternion to roll, pitch, yaw (RPY) angles(in degree)
void quat_2_rpy(float quat[4],float rpy[3]) {
	const float q0 = quat[0];
	const float q1 = quat[1];
	const float q2 = quat[2];
	const float q3 = quat[3];

	arm_atan2_f32(q0*q1 + q2*q3,0.5f - (q1*q1 + q2*q2),&rpy[0]); 	// Roll

	float temp = q0*q2 - q3*q1;
	if(fabsf(temp)> 1.0f){
		rpy[1] = copysignf(M_PI / 2, temp);
	}
	else{
		rpy[1] = asinf(temp); 										// Pitch
	}

	arm_atan2_f32(q0*q3 + q1*q2,0.5f - (q2*q2 + q3*q3),&rpy[2]); 	// Yaw

	// Convert rad to degree
	rpy[0] *= 57.29578f;
	rpy[1] *= 57.29578f;
	rpy[2] *= 57.29578f;
}


// Fast inverse square root: DSP
float inv_sqrt_f32(float x) {
	float temp;
	arm_sqrt_f32(x,&temp);
	return 1/temp;
}

// Fast inv sqrt: float
float inv_sqrt_f(float x) {
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;

    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

// Fast inv sqrt: double
double inv_sqrt_d(double x) {
    double halfx = 0.5 * x;
    double y = x;
    long long i;

    i = *(long long*)&y;
    i = 0x5FE6EB50C7B537AA - (i >> 1);
    y = *(double*)&i;
    y = y * (1.5 - (halfx * y * y));
    return y;
}

