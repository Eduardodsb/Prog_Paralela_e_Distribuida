/* Sequential Mandlebrot program */


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#define		X_RESN	800       /* x resolution */
#define		Y_RESN	800       /* y resolution */

typedef struct complextype
	{
        float real, imag;
	} Compl;


int main (){
double t_inicial, t_final; /*Variáveis utilizadas para cálculo de tempo de execução*/
	Window		win;                            /* initialization for a window */
	unsigned
	int             width, height,                  /* window size */
                        x, y,                           /* window position */
                        border_width,                   /*border width in pixels */
                        display_width, display_height,  /* size of screen */
                        screen;                         /* which screen */

	char            *window_name = "Mandelbrot Set", *display_name = NULL;
	GC              gc;
	unsigned
	long		valuemask = 0;
	XGCValues	values;
	XColor    red;
	Display		*display;
	XSizeHints	size_hints;
	Pixmap		bitmap;
	XPoint		points[800];
	FILE		*fp, *fopen ();
	char		str[100];
	Colormap screen_colormap;
	
	XSetWindowAttributes attr[1];

       /* Mandlebrot variables */
        int i, j, k;
        Compl	z, c;
        float	lengthsq, temp;
       
	/* connect to Xserver */

	if (  (display = XOpenDisplay (display_name)) == NULL ) { //Faz a conexão com o servidor X e caso não consiga imprime um erro e finaliza. 
		fprintf (stderr, "drawon: cannot connect to X server %s\n", XDisplayName (display_name) );
		exit (-1);
	}
	
	/* get screen size */

	screen = DefaultScreen (display);
	display_width = DisplayWidth (display, screen);
	display_height = DisplayHeight (display, screen);

	/* set window size */

	width = X_RESN;
	height = Y_RESN;

	/* set window position */

	x = 0;
	y = 0;

        /* create opaque window */

	border_width = 4;
	win = XCreateSimpleWindow (display, RootWindow (display, screen), x, y, width, height, border_width, BlackPixel (display, screen), WhitePixel (display, screen));

	size_hints.flags = USPosition|USSize;
	size_hints.x = x;
	size_hints.y = y;
	size_hints.width = width;
	size_hints.height = height;
	size_hints.min_width = 300;
	size_hints.min_height = 300;
	
	XSetNormalHints (display, win, &size_hints);
	XStoreName(display, win, window_name);

        /* create graphics context */

	gc = XCreateGC (display, win, valuemask, &values);
	
	screen_colormap = DefaultColormap(display, DefaultScreen(display));
	XAllocNamedColor(display, screen_colormap, "blue", &red, &red);

	XSetBackground (display, gc, WhitePixel (display, screen));
	XSetForeground (display, gc, red.pixel);
	XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);

	attr[0].backing_store = Always;
	attr[0].backing_planes = 1;
	attr[0].backing_pixel = BlackPixel(display, screen);

	XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);

	XMapWindow (display, win);
	XSync(display, 0);

    //Inicia a contagem do tempo de execução da ordenação.
    t_inicial = MPI_Wtime();

       /* Calculate and draw points */

        for(i=0; i < X_RESN; i++){ 
			for(j=0; j < Y_RESN; j++) {

				z.real = z.imag = 0.0;
				c.real = ((float) j - 400.0)/200.0;               /* scale factors for 800 x 800 window */
				c.imag = ((float) i - 400.0)/200.0;
				k = 0;

				do {                                             /* iterate for pixel color */

					temp = z.real*z.real - z.imag*z.imag + c.real;
					z.imag = 2.0*z.real*z.imag + c.imag;
					z.real = temp;
					lengthsq = z.real*z.real+z.imag*z.imag;
					k++;

				}while(lengthsq < 4.0 && k < 100);

			if (k == 100){
				sleep(0.000001);
				XDrawPoint (display, win, gc, j, i);
			}

			}
	}

	//Finaliza a contagem do tempo.
	t_final = MPI_Wtime();
	printf ("Tempo de execução %.5f\n", t_final - t_inicial);
	 
	XFlush (display);
	sleep (5);
	return 0;

	/* Program Finished */

}
