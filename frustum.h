#ifndef __frustum_h__
#define __frustum_h__

#include <math.h>
#include <vector>
using namespace std;

void printVector(vector<float> x);

class Frustum {
public:
    Frustum()
    {
        float tmp1[4] = { -.36, .36, -.999999, 1 };
        copyValues(tmp1, frontUpperLeft, 4);

        float tmp2[4] = { -.36, -.36, -.999999, 1 };
        copyValues(tmp2, frontLowerLeft, 4);

        float tmp3[4] = { .36, .36, -.999999, 1 };
        copyValues(tmp3, frontUpperRight, 4);

        float tmp4[4] = { -.36, .36, -40, 1 };
        copyValues(tmp4, backUpperLeft, 4);

        float tmp5[4] = { -.36, -.36, -40, 1 };
        copyValues(tmp5, backLowerLeft, 4);

        float tmp6[4] = { .36, .36, -40, 1 };
        copyValues(tmp6, backUpperRight, 4);

        float tmp7[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        copyValues(tmp7, identity, 16);
  
        copyValues(identity, transform, 16);
        copyValues(identity, inverse, 16);

        totalRotationY = 0.f;
    }

    void multiplyBack(const vector<float> &transform, const vector<float> &inverse);
    void multiplyFront(const vector<float> &transform, const vector<float> &inverse);
    void rotateY(float angle);
    void moveForward(float distance);
    void moveRight(float distance);

    vector<float> getIdentity() { return identity; }
    vector<float> getInverse() { return inverse; }
    vector<float> getTransform() { return transform; }
    vector<float> getFrontUpperLeft() { return frontUpperLeft; }
    vector<float> getFrontLowerLeft() { return frontLowerLeft; }
    vector<float> getFrontUpperRight() { return frontUpperRight; }
    vector<float> getBackUpperLeft() { return backUpperLeft; }
    vector<float> getBackLowerLeft() { return backLowerLeft; }
    vector<float> getBackUpperRight() { return backUpperRight; }

    vector<float> matrixMult(const vector<float> &L, const vector<float> &R) {
        vector<float> result;
        result.resize(16);
        copyValues(identity, result, 16);
        result[0] = L[0] * R[0] + L[4] * R[1] + L[8] * R[2] + L[12] * R[3];
        result[1] = L[1] * R[0] + L[5] * R[1] + L[9] * R[2] + L[13] * R[3];
        result[2] = L[2] * R[0] + L[6] * R[1] + L[10] * R[2] + L[14] * R[3];
        result[3] = L[3] * R[0] + L[7] * R[1] + L[11] * R[2] + L[15] * R[3];

        result[4] = L[0] * R[4] + L[4] * R[5] + L[8] * R[6] + L[12] * R[7];
        result[5] = L[1] * R[4] + L[5] * R[5] + L[9] * R[6] + L[13] * R[7];
        result[6] = L[2] * R[4] + L[6] * R[5] + L[10] * R[6] + L[14] * R[7];
        result[7] = L[3] * R[4] + L[7] * R[5] + L[11] * R[6] + L[15] * R[7];

        result[8] = L[0] * R[8] + L[4] * R[9] + L[8] * R[10] + L[12] * R[11];
        result[9] = L[1] * R[8] + L[5] * R[9] + L[9] * R[10] + L[13] * R[11];
        result[10] = L[2] * R[8] + L[6] * R[9] + L[10] * R[10] + L[14] * R[11];
        result[11] = L[3] * R[8] + L[7] * R[9] + L[11] * R[10] + L[15] * R[11];

        result[12] = L[0] * R[12] + L[4] * R[13] + L[8] * R[14] + L[12] * R[15];
        result[13] = L[1] * R[12] + L[5] * R[13] + L[9] * R[14] + L[13] * R[15];
        result[14] = L[2] * R[12] + L[6] * R[13] + L[10] * R[14] + L[14] * R[15];
        result[15] = L[3] * R[12] + L[7] * R[13] + L[11] * R[14] + L[15] * R[15];
        return result;
    }

    void copyValues(float *from, vector<float> &to, int count) {
        to.resize(count);
        for (int i = 0; i < count; ++i) {
            to[i] = from[i];
        } 
    }

    void copyValues(vector<float> &from, vector<float> &to, int count) {
        to.resize(count);
        for (int i = 0; i < count; ++i) {
            to[i] = from[i];
        } 
    }

    vector<float> getTranslation(float tx, float ty, float tz) {
        vector<float> result = getIdentity();
        result[12] = tx;
        result[13] = ty;
        result[14] = tz;
        return result;
    }

    vector<float> getRotationY(const float angle) {
        vector<float> result = getIdentity();
        result[0] = cos(angle);
        result[2] = sin(angle);
        result[8] = -result[2];
        result[10] = result[0];
        return result;
    }

    vector<float> transformVector(const vector<float> &M, const vector<float> &v) {
        vector<float> result;
        result.resize(4);
        result[0] = M[0] * v[0] + M[4] * v[1] + M[8] * v[2] + M[12] * v[3];
        result[1] = M[1] * v[0] + M[5] * v[1] + M[9] * v[2] + M[13] * v[3];
        result[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14] * v[3];
        result[3] = M[3] * v[0] + M[7] * v[1] + M[11] * v[2] + M[15] * v[3];
        return result;
    }

    vector<float> scalarByVec4(const float &s, const vector<float> &v) {
        vector<float> r;
        r.resize(4);
        r[0] = s * v[0];
        r[1] = s * v[1];
        r[2] = s * v[2];
        r[3] = v[3];
        return r;
    }

    vector<float> addVec4(const vector<float> &a, const vector<float> &b) {
        vector<float> c;
        c.resize(4);
        c[0] = a[0] + b[0];
        c[1] = a[1] + b[1];
        c[2] = a[2] + b[2];
        c[3] = a[3];
        return c;
    }

    // returns 4-tuple
    vector<float> convertScreenToWorld(int x, int y, int width, int height) {
        vector<float> upperLeft = getFrontUpperLeft();
        vector<float> upperRight = getFrontUpperRight();
        vector<float> lowerLeft = getFrontLowerLeft();

        vector<float> horizontalPoint; 
        horizontalPoint.resize(4);
        vector<float> verticalPoint;
        verticalPoint.resize(4);

        float uValue = float(x) / float(width);
        float vValue = float(y) / float(height);

        vector<float> horizontalVector = scalarByVec4(uValue, addVec4(upperRight, scalarByVec4(-1.0, upperLeft)));

        vector<float> verticalVector = scalarByVec4(vValue, addVec4(lowerLeft, scalarByVec4(-1.0, upperLeft)));

        vector<float> finalPoint;
        finalPoint.resize(4);
        finalPoint = addVec4(upperLeft, addVec4(horizontalVector, verticalVector));

        vector<float> transform = getTransform();
        finalPoint = transformVector(transform, finalPoint);

        return finalPoint;
    }
private:
    vector<float> frontUpperLeft;
    vector<float> frontLowerLeft;
    vector<float> frontUpperRight;
    vector<float> backUpperLeft;
    vector<float> backLowerLeft;
    vector<float> backUpperRight;
    vector<float> transform;
    vector<float> inverse;
    float totalRotationY;

    vector<float> identity;
};

#endif

