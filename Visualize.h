#ifndef VISUALIZE_H_

#include <iostream>
#include <string>

#ifndef VISUALIZE
    // #define VISUALIZE 
#endif

using namespace std;


class Visualizer
{
    public:
        // int lineW;
        // int lineR,lineG,lineB;
        // float lineA,fillA;
        // int fillR,fillG,fillB;

    
        // Visualizer()
        // {
        //     lineW=1;
        //     lineR=lineG=lineB=200;
        //     fillR=fillG=fillB=200;
        //     lineA=fillA=0.5;
        // }
        void setLineWidth(int w)
        {
#ifdef VISUALIZE
            // if(lineW != w)
            // {
                cout<<"v setLineWidth "<<w<<endl;
            // }
                // lineW = w;
#endif
        }
        void setLineColor(int r,int g,int b,float a)
        {
#ifdef VISUALIZE

            // if( (r != lineR) || (g != lineG) || (b != lineB) ||(a != lineA))
            // {
                // lineR = r;
                // lineG = g;
                // lineB = b;
                // lineA = a;
                cout<<"v setLineColor "<<r<<" "<<g<<" "<<b<<" "<<a<<endl;
            // }
#endif
        }
        void setFillColor(int r,int g,int b,float a)
        {
#ifdef VISUALIZE
            // if( (r != fillR) || (g != fillG) || (b != fillB) || (a != fillA))
            // {
                // fillR = r;
                // fillG = g;
                // fillB = b;
                // fillA = a;
                cout<<"v setFillColor "<<r<<" "<<g<<" "<<b<<" "<<a<<endl;
            // }
#endif
        }
        void arrow(int r1,int c1,int r2,int c2)
        {
#ifdef VISUALIZE
            cout<<"v arrow "<<r1<<" "<<c1<<" "<<r2<<" "<<c2<<endl;
#endif
        }
        void line(int r1,int c1,int r2,int c2)
        {
#ifdef VISUALIZE
            cout<<"v line "<<r1<<" "<<c1<<" "<<r2<<" "<<c2<<endl;
#endif
        }
        void tile(int r1,int c1)
        {
#ifdef VISUALIZE
            cout<<"v tile "<<r1<<" "<<c1<<endl;
#endif
        }
        void circle(int r1,int c1,int rad,bool fill)
        {
#ifdef VISUALIZE
        if(fill)
            cout<<"v circle "<<r1<<" "<<c1<<" "<<rad<<" true"<<endl;
        else
            cout<<"v circle "<<r1<<" "<<c1<<" "<<rad<<" false"<<endl;
#endif
        }
        void rect(int r1,int c1,int h,int w,bool fill)
        {
#ifdef VISUALIZE
        if(fill)
            cout<<"v rect "<<r1<<" "<<c1<<" "<<h<<" "<<w<<" true"<<endl;
        else
            cout<<"v rect "<<r1<<" "<<c1<<" "<<h<<" "<<w<<" false"<<endl;
#endif
        }
        void star(int r1,int c1,int irad,int orad,int pt,bool fill)
        {
#ifdef VISUALIZE
        if(fill)
            cout<<"v star "<<r1<<" "<<c1<<" "<<irad<<" "<<orad<<" "<<pt<<" true"<<endl;
        else
            cout<<"v star "<<r1<<" "<<c1<<" "<<irad<<" "<<orad<<" "<<pt<<" false"<<endl;
#endif
        }
        void tileBorder(int r1,int c1,string subtile)
        {
#ifdef VISUALIZE
            cout<<"v tileBorder "<<r1<<" "<<c1<<" "<<subtile.c_str()<<endl;
#endif
        }
        void tileSubTile(int r1,int c1,string subtile)
        {
#ifdef VISUALIZE
            cout<<"v tileSubTile "<<r1<<" "<<c1<<" "<<subtile.c_str()<<endl;
#endif
        }
};
#endif