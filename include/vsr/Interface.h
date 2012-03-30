//
//  Interface.h
//  A Control Object: Manipulate vsr objects using screen interaction (to be fed mouse input data)
//
//  Created by Pablo Colapinto on 3/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef vsr_Interface_h
#define vsr_Interface_h

#include "GL.h"
#include "vsr.h"
#include "Set.h"
#include "Drawable.h"
#include "Camera.h"

namespace vsr  {
    
    class Model;
    
    /* Mapped Key values from GLV */
    namespace Key{
        enum {
            
            // Standard ASCII non-printable characters
            Enter		=3,		/**< */
            Backspace	=8,		/**< */
            Tab			=9,		/**< */
            Return		=13,	/**< */
            Escape		=27,	/**< */
            Delete		=127,	/**< */
			
            // Non-standard, but common keys.
            F1=256, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, 
            Insert, Left, Up, Right, Down, PageDown, PageUp, End, Home,
        };
    }
        
    namespace Mode {
		enum {
			Add =		1,
			Select =	1 << 1, 
			Edit =		1 << 2,
			Animate =	1 << 3,
			HitTest =	1 << 4,
			
			BuildGui =	1 << 5,
			
			CamArb	=	1 << 6,
			CamTilt =	1 << 7,
			CamTrack =	1 << 8,
			
			Scale	=	1 << 9,
			Rotate	=	1 << 10,
			Grab	=	1 << 11,
			
			Navigate =  1 << 12,
			Transform =	1 << 13
		};
	}

	struct MouseData {
        
		float x, y, dx, dy, ddx, ddy, xrel, yrel; //< 2D Mouse Position, first and second derivatives of motion, position relative to TL corner
		
        Vec click, pos, move, accel, cat, drag, dragAccum, dragCat, projectFar, projectNear;
		Pnt origin;
		Biv biv, bivCat, dragBiv, dragBivCat;
		bool isDown, isMoving, newClick;
		
		void toggle() { newClick = !newClick; }
	};
	
	struct ViewData  {
		float w, h; 
        Vec z; 
        Dll ray, clickray; 
        //Camera camera;
	};
    
	struct KeyboardData {
		bool shift, alt, caps, ctrl, meta, f, down;
		int code;
		KeyboardData() : shift(0), alt(0), caps(0), ctrl(0), code(0){}
	};    
    
    
    
    class Interface {
        
        int mMode;  ///< Edit Mode State
        
    public:
        
        struct ViewImpl {
                        
            ViewData data;
            
            Interface * interface;
            
            Camera * camera;
            
            Camera& active() { return *camera; }
            
            ViewImpl(Interface * i ) : interface(i){
                camera = new Camera();
                camera -> pos(0,0,5);
            }
            
            virtual void fullScreenToggle() = 0;            
            virtual void getData( void * udata) = 0;
            
			/* Set camera size to view Size */
			void fit() { 
				camera -> width( data.w ); camera -> height( data.h ); 
			}
			
            //virtual void win() = 0;
        };
        
        struct InputImpl {
            Interface * interface;
            InputImpl(Interface * i) : interface(i){} 
            virtual void getData( void * udata) = 0;
        };
        
        Interface();
        
        virtual void init() = 0;
        void viewCalc();
        
//        ModelImpl * mimpl;
        Model * model;
        
        ViewImpl * vimpl;
        InputImpl * iimpl;
        
        MouseData       mouse;
       // ViewData        view;
        KeyboardData    keyboard;
        
        ViewData& vd() { return vimpl -> data; }
        ViewData vd() const { return vimpl -> data; }

        State& state();

        Camera& camera() { return vimpl -> active(); }
        
        Vec screenCoord(const Pnt& p);
        bool pntClicked(const State&, double rad = .05);
        
        void ui(State&, double t = 1.0);
        void ui(State& s, State& x, double t);
        void ui(Frame& f, double t = 1.0);
        
        void windowTransform();
        void stateTransform();
        void camSpin();
        void camTranslate();
        void mvTransform();
        
        void onMouseMove();        
        void onMouseDown();
        void onMouseDrag();
        void onMouseUp(){ mouse.isDown = 0; }        
        void onKeyDown();
        void onKeyUp(){}

   //     Dll ray(){ return view.ray;      }
   //    Dll clickray(){ return view.clickray; }
        
        Vec click(){ return mouse.click;   }
        Vec pos(){ return mouse.pos;     }
        void xf ( State * s, State& pos, double t );
        void xfFrame( Frame * f, double t);
        
        /* Mode */
        int& mode() { return mMode; }
        int mode() const { return mMode; }
        bool mode(int q) { return mMode & q; }
        void enable(int bitflag) { mMode |= bitflag; }
        void disable(int bitflag) { mMode &= ~bitflag; }
        void toggle(int bitflag)  { mMode & bitflag ? disable(bitflag) : enable(bitflag); }
        void print(){
            
            //cout << ( mode(Mode::Transform) ? "Transform" 
            
        }
        
    };
    
    class Model : public Set<State>, public Drawable, public Touchable {
        
    public:
        
        Interface * interface;
        
        Model(Interface * i) : interface(i) {}
        
        virtual void draw(){
            for (int i = 0; i < mStates.size(); ++i){
                mStates[i] -> draw();
            }
        }
        
        virtual void ui(){
             for (int i = 0; i < mStates.size(); ++i){
                 interface -> ui( *mStates[i] ); 
             }
        }
    };

    
    //    inline Camera Interface::camera()   { 
    
}



#endif