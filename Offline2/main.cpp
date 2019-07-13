//
//  main.cpp
//  Offline2
//
//  Created by Shanjinur Islam on 7/10/19.
//  Copyright © 2019 Shanjinur Islam. All rights reserved.
//

#include <iostream>
#include <stack>
#include <cmath>
#include <fstream>

using namespace std;

class Vector
{
public:
    double ax, ay, az;
    Vector()
    {
        ax = ay = az = 0;
    }
    Vector(double ax, double ay, double az)
    {
        this->ax = ax;
        this->ay = ay;
        this->az = az;
    }

    void normalize()
    {
        double r = sqrt((ax * ax) + (ay * ay) + (az * az));
        ax = ax / r;
        ay = ay / r;
        az = az / r;
    }

    Vector operator+(Vector v)
    {
        Vector temp;
        temp.ax = this->ax + v.ax;
        temp.ay = this->ay + v.ay;
        temp.az = this->az + v.az;

        return temp;
    }

    Vector operator-(Vector v)
    {
        Vector temp;
        temp.ax = this->ax - v.ax;
        temp.ay = this->ay - v.ay;
        temp.az = this->az - v.az;

        return temp;
    }

    Vector &operator=(Vector v)
    {
        this->ax = v.ax;
        this->ay = v.ay;
        this->az = v.az;

        return *this;
    }

    Vector operator*(double d)
    {
        Vector temp;
        temp.ax = ax * d;
        temp.ay = ay * d;
        temp.az = az * d;
        return temp;
    }
    void print()
    {
        cout << ax << " " << ay << " " << az << endl;
    }

    double dot(Vector x)
    {
        return (this->ax * x.ax) + (this->ay * x.ay) + (this->az * x.az);
    }

    Vector cross(Vector x)
    {
        Vector temp;
        temp.ax = (ay * x.az - az * x.ay);
        temp.ay = (az * x.ax - ax * x.az);
        temp.az = (ax * x.ay - ay * x.ax);
        return temp;
    }
};

class ModelView
{
private:
    double **transformMatrix;

public:
    stack<double **> transformationStack;
    ModelView()
    {
        transformMatrix = new double *[4];

        for (int i = 0; i < 4; i++)
        {
            transformMatrix[i] = new double[4];
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                transformMatrix[i][j] = 0;
            }
        }

        transformMatrix[0][0] = 1;
        transformMatrix[1][1] = 1;
        transformMatrix[2][2] = 1;
        transformMatrix[3][3] = 1;
        transformationStack.push(transformMatrix);
    }

    double **generateTranslateMatrix(double a, double b, double c)
    {
        double **matrix = new double *[4];
        for (int i = 0; i < 4; i++)
        {
            matrix[i] = new double[4];
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix[i][j] = 0;
            }
        }

        matrix[0][0] = 1;
        matrix[1][1] = 1;
        matrix[2][2] = 1;
        matrix[3][3] = 1;

        matrix[0][3] = a;
        matrix[1][3] = b;
        matrix[2][3] = c;

        return matrix;
    }

    double **generateScaleMatrix(double sx, double sy, double sz)
    {
        double **matrix = new double *[4];
        for (int i = 0; i < 4; i++)
        {
            matrix[i] = new double[4];
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix[i][j] = 0;
            }
        }

        matrix[3][3] = 1;
        matrix[0][0] = sx;
        matrix[1][1] = sy;
        matrix[2][2] = sz;
        return matrix;
    }

    double **generateRotateMatrix(double angle, double ax, double ay, double az)
    {
        Vector a(ax, ay, az);

        a.normalize();
        angle = (acos(-1.0) / 180.0) * angle;

        Vector i(1, 0, 0);
        Vector j(0, 1, 0);
        Vector k(0, 0, 1);

        Vector c1 = i * cos(angle) + a * ((a.dot(i)) * (1 - cos(angle))) + a.cross(i) * sin(angle);

        Vector c2 = j * cos(angle) + a * ((a.dot(j)) * (1 - cos(angle))) + a.cross(j) * sin(angle);

        Vector c3 = k * cos(angle) + a * ((a.dot(k)) * (1 - cos(angle))) + a.cross(k) * sin(angle);

        double **matrix = new double *[4];
        for (int i = 0; i < 4; i++)
        {
            matrix[i] = new double[4];
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix[i][j] = 0;
            }
        }

        matrix[3][3] = 1;
        matrix[0][0] = c1.ax;
        matrix[1][0] = c1.ay;
        matrix[2][0] = c1.az;

        matrix[0][1] = c2.ax;
        matrix[1][1] = c2.ay;
        matrix[2][1] = c2.az;

        matrix[0][2] = c3.ax;
        matrix[1][2] = c3.ay;
        matrix[2][2] = c3.az;

        return matrix;
    }

    double **crossMatrix(double **a, double **b, int r1, int c2)
    {
        double **matrix = new double *[r1];

        for (int i = 0; i < 4; i++)
        {
            matrix[i] = new double[c2];
        }

        for (int i = 0; i < r1; i++)
        {
            for (int j = 0; j < c2; j++)
            {
                matrix[i][j] = 0;
            }
        }

        for (int i = 0; i < r1; ++i)
            for (int j = 0; j < c2; ++j)
                for (int k = 0; k < 4; ++k)
                {
                    matrix[i][j] += a[i][k] * b[k][j];
                }

        return matrix;
    }

    double **movetooroginMatrix(Vector eye)
    {
        double **matrix = new double *[4];
        for (int i = 0; i < 4; i++)
        {
            matrix[i] = new double[4];
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix[i][j] = 0;
            }
            matrix[i][i] = 1;
        }

        matrix[0][3] = -eye.ax;
        matrix[1][3] = -eye.ay;
        matrix[2][3] = -eye.az;

        return matrix;
    }

    double **rotationMatrix(Vector l, Vector r, Vector u)
    {
        double **matrix = new double *[4];
        for (int i = 0; i < 4; i++)
        {
            matrix[i] = new double[4];
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix[i][j] = 0;
            }
        }

        matrix[0][0] = r.ax;
        matrix[0][1] = r.ay;
        matrix[0][2] = r.az;

        matrix[1][0] = u.ax;
        matrix[1][1] = u.ay;
        matrix[1][2] = u.az;

        matrix[2][0] = -l.ax;
        matrix[2][1] = -l.ay;
        matrix[2][2] = -l.az;

        matrix[3][3] = 1;

        return matrix;
    }

    double **projectionMatrix(double fovY, double aspectRatio, double near, double far)
    {
        double **matrix = new double *[4];
        for (int i = 0; i < 4; i++)
        {
            matrix[i] = new double[4];
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix[i][j] = 0;
            }
        }
        double fovX = fovY * aspectRatio;
        double t, r;
        t = near * tan(fovY / 2);
        r = near * tan(fovX / 2);

        matrix[0][0] = near / r;
        matrix[1][1] = near / t;
        matrix[2][2] = -(far + near) / (far - near);
        matrix[2][3] = -(2 * far * near) / (far - near);
        matrix[3][2] = -1;

        return matrix;
    }

    void printMatrix(double **matrix, int r, int c)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main(int argc, const char *argv[])
{
    ModelView view;
    Vector eye, look, up;
    double fovY, aspectRatio, near, far;

    ifstream in;
    ofstream out1, out2, out3;
    in.open("scene.txt", ifstream::in);

    out1.open("stage1.txt", ofstream::out);
    out1.setf(ios::fixed);
    out1.setf(ios::showpoint);
    out1.precision(7);

    out2.open("stage2.txt", ofstream::out);
    out2.setf(ios::fixed);
    out2.setf(ios::showpoint);
    out2.precision(7);

    out3.open("stage3.txt", ofstream::out);
    out3.setf(ios::fixed);
    out3.setf(ios::showpoint);
    out3.precision(7);
    string line;
    if (in.is_open())
    {
        in >> eye.ax >> eye.ay >> eye.az;
        in >> look.ax >> look.ay >> look.az;
        in >> up.ax >> up.ay >> up.az;

        Vector l = (look - eye);
        l.normalize();
        Vector r = l.cross(up);
        r.normalize();
        up = r.cross(l);

        double **originMatrix = view.movetooroginMatrix(eye);
        double **rotationMatrix = view.rotationMatrix(l, r, up);

        in >> fovY >> aspectRatio >> near >> far;
        double **projectionMatrix = view.projectionMatrix(fovY * (acos(-1.0) / 180.0), aspectRatio, near, far);

        getline(in, line);
        while (in >> line)
        {
            if (line == "triangle")
            {
                double **point = new double *[4];
                for (int i = 0; i < 4; i++)
                {
                    point[i] = new double[1];
                }
                for (int i = 0; i < 3; i++)
                {
                    in >> point[0][0] >> point[1][0] >> point[2][0];
                    point[3][0] = 1;
                    view.printMatrix(view.transformationStack.top(), 4, 4);
                    out1 << endl;
                    out2 << endl;
                    out3 << endl;
                    point = view.crossMatrix(view.transformationStack.top(), point, 4, 1);
                    if (point[3][0] != 1)
                    {
                        point[0][0] = point[0][0] / point[3][0];
                        point[1][0] = point[1][0] / point[3][0];
                        point[2][0] = point[2][0] / point[3][0];
                        point[3][0] = 1;
                    }
                    out1 << point[0][0] << " " << point[1][0] << " " << point[2][0];

                    point = view.crossMatrix(originMatrix, point, 4, 1);
                    point = view.crossMatrix(rotationMatrix, point, 4, 1);

                    if (point[3][0] != 1)
                    {
                        point[0][0] = point[0][0] / point[3][0];
                        point[1][0] = point[1][0] / point[3][0];
                        point[2][0] = point[2][0] / point[3][0];
                        point[3][0] = 1;
                    }
                    out2 << point[0][0] << " " << point[1][0] << " " << point[2][0];

                    point = view.crossMatrix(projectionMatrix, point, 4, 1);

                    if (point[3][0] != 1)
                    {
                        point[0][0] = point[0][0] / point[3][0];
                        point[1][0] = point[1][0] / point[3][0];
                        point[2][0] = point[2][0] / point[3][0];
                        point[3][0] = 1;
                    }
                    out3 << point[0][0] << " " << point[1][0] << " " << point[2][0];
                }
                out1 << endl;
                out2 << endl;
                out3 << endl;

                //view.printMatrix(view.crossMatrix(view.transformationStack.top(),triangleMatrix),3);
            }

            if (line == "translate")
            {
                double tx, ty, tz;
                in >> tx >> ty >> tz;
                double **translateMatrix = view.generateTranslateMatrix(tx, ty, tz);
                double **newTransformationMatrix = view.crossMatrix(view.transformationStack.top(), translateMatrix, 4, 4);
                //view.printMatrix(newTransformationMatrix,4,4) ;
                view.transformationStack.pop();
                view.transformationStack.push(newTransformationMatrix);
            }

            if (line == "scale")
            {
                double sx, sy, sz;
                in >> sx >> sy >> sz;
                double **scaleMatrix = view.generateScaleMatrix(sx, sy, sz);
                double **newTransformationMatrix = view.crossMatrix(view.transformationStack.top(), scaleMatrix, 4, 4);
                view.transformationStack.pop();
                view.transformationStack.push(newTransformationMatrix);
            }

            if (line == "rotate")
            {
                double angle, ax, ay, az;
                in >> angle >> ax >> ay >> az;

                double **rotationMatrix = view.generateRotateMatrix(angle, ax, ay, az);
                double **newTransformationMatrix = view.crossMatrix(view.transformationStack.top(), rotationMatrix, 4, 4);
                view.transformationStack.pop();
                view.transformationStack.push(newTransformationMatrix);
            }

            if (line == "push")
            {
                double **currentState = view.transformationStack.top();
                view.transformationStack.push(currentState);
            }

            if (line == "pop")
            {
                view.transformationStack.pop();
            }

            if (line == "end")
            {
                break;
            }
        }
    }

    return 0;
}
