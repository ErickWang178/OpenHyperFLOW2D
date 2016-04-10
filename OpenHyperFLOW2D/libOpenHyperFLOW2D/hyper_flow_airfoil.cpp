/*******************************************************************************
*   OpenHyperFLOW2D                                                            *
*                                                                              *
*   Version  1.0.3                                                             *
*   Copyright (C)  1995-2016 by Serge A. Suchkov                               *
*   Copyright policy: LGPL V3                                                  *
*   http://github.com/sergeas67/openhyperflow2d                                      *
*                                                                              *
*   last update: 04/07/2016                                                    *
*******************************************************************************/
#include "libExcept/except.hpp"
#include "libOpenHyperFLOW2D/hyper_flow_airfoil.hpp"
FP SolidBoundAirfoil2D::mean_y(FP t) { //mm=0.6;

    FP m0=0.0,
    m1=0.1,
    m2=0.1,
    m3=0.1,
    m4=0.0;

    return m0*mm*bez_d4_i0(t)+m1*mm*bez_d4_i1(t)+m2*mm*bez_d4_i2(t)+m3*mm*bez_d4_i3(t)+m4*mm*bez_d4_i4(t);
}

FP SolidBoundAirfoil2D::mean_x(FP t) { // pp=0.6,

    FP p0=0.0,
    p1=pp/2.,
    p2=pp,
    p3=(pp+1.)/2.,
       p4=1.0;

    return p0*bez_d4_i0(t)+p1*bez_d4_i1(t)+p2*bez_d4_i2(t)+p3*bez_d4_i3(t)+p4*bez_d4_i4(t);
}

FP SolidBoundAirfoil2D::z_x(FP x) {
    FP x0 = 0.0,
    x1 = 0.0,
    x2 = 0.03571,
    x3 = 0.10714,
    x4 = 0.21429, 
    x5 = 0.35714,
    x6 = 0.53571,
    x7 = 0.75000,
    x8 = 1.00000;

    return x0*bez_d8_i0(x)+x1*bez_d8_i1(x)+x2*bez_d8_i2(x)+x3*bez_d8_i3(x)+x4*bez_d8_i4(x)+x5*bez_d8_i5(x)+
    x6*bez_d8_i6(x)+x7*bez_d8_i7(x)+x8*bez_d8_i8(x);
}

FP SolidBoundAirfoil2D::z_y(FP x, FP tk) {
    FP y0 = 0.0,
    y1 = 0.18556,
    y2 = 0.34863,
    y3 = 0.48919,
    y4 = 0.58214,
    y5 = 0.55724,
    y6 = 0.44992,
    y7 = 0.30281,
    y8 = 0.01050;

    return y0*tk*bez_d8_i0(x)+y1*tk*bez_d8_i1(x)+y2*tk*bez_d8_i2(x)+
    y3*tk*bez_d8_i3(x)+y4*tk*bez_d8_i4(x)+y5*tk*bez_d8_i5(x)+
    y6*tk*bez_d8_i6(x)+y7*tk*bez_d8_i7(x)+y8*tk*bez_d8_i8(x);
}

FP SolidBoundAirfoil2D::airfoil_y1(FP t,FP thick) {
    return mean_y(z_x(t)) + z_y(t, thick);
}
FP SolidBoundAirfoil2D::airfoil_y2(FP t,FP thick) {
    return mean_y(z_x(t)) - z_y(t, thick);
}
FP SolidBoundAirfoil2D::airfoil_y(FP t) {
    return mean_y(z_x(t));
}
FP SolidBoundAirfoil2D::airfoil_x(FP t) {
    return mean_x(z_x(t));
}

SolidBoundAirfoil2D::SolidBoundAirfoil2D(char* name,
                                         UMatrix2D< FlowNode2D< FP, NUM_COMPONENTS> >* JM, // Computation area reference
                                         FP  x,                      // Start profile 
                                         FP  y,                      // coordinates (x,y)
                                         InputData* airfoil_input_data, //
                                         FP  dx,                     // dx - step
                                         FP  dy,                     // dy - step
                                         ulong   ct,                 // condition type
                                         Flow2D* pInFlow2D,          // init flow2d object on circle bound
                                         FP* Y,                      // component matrix
                                         ulong   bctt,               // Bound contour turbulence type
                                         FP  scale,                  // airfoil scale
                                         FP  attack_angle,           // Angle of attack
                                         ostream* dbg_output):BoundContour2D(name,JM,
                                                                             (int)(x/dx+0.4999),
                                                                             (int)(y/dy+0.4999)
                                                                             ),Area2D(name,JM) {
    int     k,i,ret;
    FP      xx1,yy1,xx2,yy2,r,fi;
    int     ix,iy;
    Table*  UpperSurface = airfoil_input_data->GetTable((char*)"UpperSurface");
    Table*  LowerSurface = airfoil_input_data->GetTable((char*)"LowerSurface");

    FP dcx,dcy;
    xx1=yy1=xx2=yy2=0.;
 
    airfoil_type = AFT_TsAGI;

    xx1 = x;
    yy1 = y;

    for (i=0;i<UpperSurface->GetNumNodes();i++) {  //    upper surface
        xx2=x+scale*UpperSurface->GetX(i);
        yy2=y+scale*UpperSurface->GetY(i);
        ix = (int)(xx2/dx+0.4999);
        iy = (int)(yy2/dy+0.4999);

        if (dbg_output)
            *dbg_output << "\ndebug: Add airfoil node ("<< xx1 <<","<< yy1 << ")->[" << ix <<","<< iy << "]" 
            << flush;
#ifdef _UNIFORM_MESH_
        AddBound2D(name, ix,iy,ct,NULL,pInFlow2D,Y,bctt);
#else
        AddBound2D(name, xx2,yy2,ct,NULL,pInFlow2D,Y,bctt);
#endif // _UNIFORM_MESH_
        xx1 = xx2;
        yy1 = yy2;
    }
    
    for (i=LowerSurface->GetNumNodes()-1;i>0;i--) {          //     lower surface
        xx2=x+scale*LowerSurface->GetX(i);
        yy2=y+scale*LowerSurface->GetY(i);

        ix = (int)(xx2/dx+0.4999);
        iy = (int)(yy2/dy+0.4999);
        if (dbg_output)
            *dbg_output << "\ndebug: Add airfoil node ("<< xx1 <<","<< yy1 << ")->[" << ix <<","<< iy << "]" 
            << flush;
#ifdef _UNIFORM_MESH_
        AddBound2D(name,ix,iy,ct,NULL,pInFlow2D,Y,bctt);
#else
        AddBound2D(name, xx2,yy2,ct,NULL,pInFlow2D,Y,bctt);
#endif // _UNIFORM_MESH_
        xx1 = xx2;
        yy1 = yy2;
    }
    CloseContour2D(name, ct, NULL, pInFlow2D,Y,bctt);

    xx1=x+scale*UpperSurface->GetX(UpperSurface->GetNumNodes()/2);
    yy1=y+scale*(UpperSurface->GetY(UpperSurface->GetNumNodes()/2) + 
                 LowerSurface->GetY(LowerSurface->GetNumNodes()/2))/2;
    
    if (attack_angle != 0.) {
        dcx=x-xx1;
        dcy=y-yy1;
        r  = sqrt(dcx*dcx+dcy*dcy+1.e-30);        
        fi = atan2(dcx,dcy);
        xx1 = x + (r*sin(fi+attack_angle)); 
        yy1 = y + (r*cos(fi+attack_angle));
        ret = RotateBoundContour2D(x,y,attack_angle);

        if (dbg_output) {
            if (ret)
                *dbg_output << "\ndebug: Rotate sucess !\n" << flush;
            else
                *dbg_output << "\n@C1debug: Rotate failed !\n" << flush;
        }

    } else {
        ret = 1;
    }

    ix = (int)(xx1/dx+0.4999);
    iy = (int)(yy1/dy+0.4999);

    if (dbg_output)
        *dbg_output << "\n \ndebug:Start point for fill airfoil ("<< xx1 <<","<< yy1 << ")->[" << ix <<","<< iy << "]\n" 
        << flush;
    if (ret) {
        SetBounds();
#ifdef _UNIFORM_MESH_
        FillArea2D((unsigned int)ix,(unsigned int)iy,NT_S_2D);
#else    
        FillArea2D(xx1,yy1,NT_S_2D);
#endif // _UNIFORM_MESH_
    }
}

SolidBoundAirfoil2D::SolidBoundAirfoil2D(char* name,                 // Object name  
                                         UMatrix2D< FlowNode2D< FP, NUM_COMPONENTS> >* JM, // Computation area reference
                                         FP  x,                      // Start profile 
                                         FP  y,                      // coordinates (x,y)
                                         FP  m_m,                    //
                                         FP  p_p,                    //
                                         FP  thick,                  // airfoil thick
                                         FP  dx,                     // dx - step
                                         FP  dy,                     // dy - step
                                         ulong   ct,                 // condition type
                                         Flow2D* pInFlow2D,          // init flow2d object on circle bound
                                         FP* Y,                      // component matrix
                                         ulong   bctt,               // Bound contour turbulence type
                                         FP  scale,                  // airfoil scale
                                         FP  attack_angle,           // Angle of attack
                                         ostream* dbg_output):BoundContour2D(name,JM,
                                                                             (int)(x/dx+0.4999),
                                                                             (int)(y/dy+0.4999)
                                                                             ),Area2D(name,JM) {
    int    k,i,ret;
    FP xx1,yy1,xx2,yy2,r,fi;
    int    ix,iy;
    k = (int)(scale/dx);

    FP dcx,dcy,dt=2./k;
    xx1=yy1=xx2=yy2=0.;
    pp =  p_p;
    mm =  m_m;
    
    airfoil_type = AFT_NACA;
    
    xx1 = x;
    yy1 = y;
    
    for (i=0;i<k/2;i++) {                             //    upper surface
        xx2=x+scale*airfoil_x((i+1)*dt);
        yy2=y+scale*airfoil_y1((i+1)*dt,thick);
        ix = (int)(xx2/dx+0.4999);
        iy = (int)(yy2/dy+0.4999);
        
        if (dbg_output)
            *dbg_output << "\ndebug: Add airfoil node ("<< xx1 <<","<< yy1 << ")->[" << ix <<","<< iy << "]" 
            << flush;
#ifdef _UNIFORM_MESH_
        AddBound2D(name, ix,iy,ct,NULL,pInFlow2D,Y,bctt);
#else
        AddBound2D(name, xx2,yy2,ct,NULL,pInFlow2D,Y,bctt);
#endif // _UNIFORM_MESH_
        xx1 = xx2;
        yy1 = yy2;
    }
    for (;i>0;i--) {          //     lower surface
        xx2=x+scale*airfoil_x((i-1)*dt);
        yy2=y+scale*airfoil_y2((i-1)*dt,thick);
        
        ix = (int)(xx2/dx+0.4999);
        iy = (int)(yy2/dy+0.4999);
        if (dbg_output)
            *dbg_output << "\ndebug: Add airfoil node ("<< xx1 <<","<< yy1 << ")->[" << ix <<","<< iy << "]" 
            << flush;
#ifdef _UNIFORM_MESH_
        AddBound2D(name,ix,iy,ct,NULL,pInFlow2D,Y,bctt);
#else
        AddBound2D(name, xx2,yy2,ct,NULL,pInFlow2D,Y,bctt);
#endif // _UNIFORM_MESH_
        xx1 = xx2;
        yy1 = yy2;
    }
    CloseContour2D(name, ct, NULL, pInFlow2D,Y,bctt);

    xx1=x+scale*airfoil_x(0.5);
    yy1=y+scale*airfoil_y(0.5);
    if (attack_angle != 0.) {
        dcx=x-xx1;
        dcy=y-yy1;
        r  = sqrt(dcx*dcx+dcy*dcy+1.e-30);        
        fi = atan2(dcx,dcy);
        xx1 = x + (r*sin(fi+attack_angle)); 
        yy1 = y + (r*cos(fi+attack_angle));
        ret = RotateBoundContour2D(x,y,attack_angle);
        
        if (dbg_output) {
            if (ret)
                *dbg_output << "\ndebug: Rotate sucess !\n" << flush;
            else
                *dbg_output << "\n@C1debug: Rotate failed !\n" << flush;
        }
    
    } else {
        ret = 1;
    }
    
    ix = (int)(xx1/dx+0.4999);
    iy = (int)(yy1/dy+0.4999);
    
    if (dbg_output)
        *dbg_output << "\n \ndebug:Start point for fill airfoil ("<< xx1 <<","<< yy1 << ")->[" << ix <<","<< iy << "]\n" 
        << flush;
    if (ret) {
        SetBounds();
#ifdef _UNIFORM_MESH_
        FillArea2D((unsigned int)ix,(unsigned int)iy,NT_S_2D);
#else    
        FillArea2D(xx1,yy1,NT_S_2D);
#endif // _UNIFORM_MESH_
    }
}

