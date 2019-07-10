//
//  main.cpp
//  Offline2
//
//  Created by Shanjinur Islam on 7/10/19.
//  Copyright © 2019 Shanjinur Islam. All rights reserved.
//

#include <iostream>
#include <stack>

using namespace std ;

class Point{
public:
    double p[4] ;
    
    Point(double x,double y,double z){
        p[0] = x ;
        p[1] = y ;
        p[2] = z ;
        p[3] = 1 ;
    }
    
    
    Point(double x,double y,double z,double w){
        p[0] = x ;
        p[1] = y ;
        p[2] = z ;
        p[3] = w ;
    }
};

class ModelView{
private:
    stack<double**> modelview;
public:
    void createMatrix(Point p1,Point p2,Point p3){
        double** matrix = new double*[4] ;
        
        for(int i=0;i<4;i++){
            matrix[i] = new double[4] ;
        }
        
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                matrix[i][j] = 0 ;
            }
        }
        
        for(int i=0;i<3;i++){
            matrix[0][i] = p1.p[i] ;
        }
        
        for(int i=0;i<3;i++){
            matrix[1][i] = p2.p[i] ;
        }
        
        for(int i=0;i<3;i++){
            matrix[2][i] = p3.p[i] ;
        }
        
        matrix[3][3] = 1 ;
        
        modelview.push(matrix) ;
    }
    
    double** generateTranslateMatrix(double a,double b,double c){
        double** matrix = new double*[4] ;
        for(int i=0;i<4;i++){
            matrix[i] = new double[4] ;
        }
        
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                matrix[i][j] = 0 ;
            }
        }
        
        matrix[0][3] = a ;
        matrix[1][3] = b ;
        matrix[2][3] = c ;
        matrix[3][3] = 1 ;
        
        return matrix ;
    }
    
    double** generateScaleMatrix(double sx,double sy,double sz){
        double** matrix = new double*[4] ;
        for(int i=0;i<4;i++){
            matrix[i] = new double[4] ;
        }
        
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                matrix[i][j] = 0 ;
            }
        }
        
        matrix[0][0] = sx ;
        matrix[1][1] = sy ;
        matrix[2][2] = sz ;
        matrix[3][3] = 1 ;
        
        return matrix ;
    }
    
    double** generateRotateMatrix(double angle, double ax,double ay,double az){
        double** matrix = new double*[4] ;
        return matrix;
    }
    
    void printTop(){
        double** matrix = modelview.top();
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                cout<<matrix[i][j]<<"\t" ;
            }
            cout<<endl ;
        }
    }
    
    
};


int main(int argc, const char * argv[]) {
    Point a(0,0,1) ;
    Point b(1,2,-1) ;
    Point c(-3,-2,1) ;
    
    ModelView modelview ;
    modelview.createMatrix(a,b,c) ;
    
    modelview.printTop() ;
    return 0;
}
