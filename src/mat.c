/*
typedef float mat3[3][3];
typedef float vec3[3];
typedef float vec2[2];

void mat3_mul(mat3 m1, mat3 m0) {
    mat3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < 3; k++) {
                result[i][j] += m1[i][k] * m0[k][j];
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m0[i][j] = result[i][j];
        }
    }
}

void mat3_vec3_mul(mat3 m, vec3 v) {
    vec3 result;
    
    for (int i = 0; i < 3; i++) {
        result[i] = 0.0;
        for (int j = 0; j < 3; j++) {
            result[i] += v[j] * m[i][j];
        }
    }

    for (int i = 0; i < 3; i++) {
        v[i] = result[i];
    }
}

void mat3_vec2_mul(mat3 m, vec2 v) {
    vec3 temp = { v[0], v[1], 1.0 };
    mat3_vec3_mul(m, temp);
    v[0] = temp[0];
    v[1] = temp[1]
    // would need to divide by homogenous component if it's a perspective projection,
    // but i doubt i'll be doing that with 2d vectors
}

void mat2_gen_rotation() {

}
*/
