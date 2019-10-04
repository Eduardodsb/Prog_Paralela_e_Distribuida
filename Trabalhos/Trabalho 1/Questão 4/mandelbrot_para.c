/* Sequential Mandlebrot program */


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#define		X_RESN	1000       /* x resolution */
#define		Y_RESN	1000      /* y resolution */

typedef struct complextype
	{
        float real, imag;
	} Compl;

int main(int argc, char **argv) {
    int meu_ranque, num_procs, raiz = 0, etiq = 1, quantidade_msg = 0, soma_quantidade_msg; /*Variáveis do MPI*/
    double t_inicial, t_final; /*Variáveis utilizadas para cálculo de tempo de execução*/
    int point[2];
    MPI_Request pedido_envia;
    MPI_Status info;

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
	XColor    color_blue;
	Display		*display;
	XSizeHints	size_hints;
	Pixmap		bitmap;
	XPoint		points[800];
	FILE		*fp, *fopen ();
	char		str[100];
	Colormap screen_colormap;

    /* Mandlebrot variables */
    int i, j, k;
    Compl	z, c;
    float	lengthsq, temp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	

    if(meu_ranque == raiz){
        XSetWindowAttributes attr[1];
        
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
        XAllocNamedColor(display, screen_colormap, "blue", &color_blue, &color_blue);

        XSetBackground (display, gc, WhitePixel (display, screen));
        XSetForeground (display, gc, color_blue.pixel);
        XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);

        attr[0].backing_store = Always;
        attr[0].backing_planes = 1;
        attr[0].backing_pixel = BlackPixel(display, screen);

        XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);

        XMapWindow (display, win);
        XSync(display, 0);
    }
      	 
       /* Calculate and draw points */
        
    //Inicia a contagem do tempo de execução da ordenação.
    t_inicial = MPI_Wtime();

        for(i=meu_ranque; i < X_RESN; i += num_procs){ 
            for(j=0; j < Y_RESN; j++) {

                z.real = z.imag = 0.0;
                c.real = ((float) j - 500.0)/250.0;               /* scale factors for 1000 x 1000 window */
                c.imag = ((float) i - 500.0)/250.0;
                k = 0;

                do {                                             /* iterate for pixel color */

                    temp = z.real*z.real - z.imag*z.imag + c.real;
                    z.imag = 2.0*z.real*z.imag + c.imag;
                    z.real = temp;
                    lengthsq = z.real*z.real+z.imag*z.imag;
                    k++;

                }while(lengthsq < 4.0 && k < 100);

            if (k == 100){
                quantidade_msg++;
                point[0] = j;
                point[1] = i;
                //printf("Eu %d fiz o ponto (%d,%d)\n", meu_ranque,j, i);
                MPI_Isend(point, 2, MPI_INT, raiz, etiq, MPI_COMM_WORLD, &pedido_envia);
            } 

            }
        }
    //Finaliza a contagem do tempo de execução da ordenação. OBS: O tempo considerado é o do processo 0, pois o mesmo é o processo raiz da árvore. 
    t_final = MPI_Wtime();


    MPI_Reduce(&quantidade_msg,&soma_quantidade_msg, 1, MPI_INT, MPI_SUM, raiz, MPI_COMM_WORLD); 

    if(meu_ranque == raiz){
        printf ("Tempo de execução %.5f\n", t_final - t_inicial);
        //printf("total msg %d\n", soma_quantidade_msg);
        for(i = 0; i < soma_quantidade_msg; i++){
           MPI_Recv(point, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &info);
            //printf("imprime o ponto (%d,%d)\n",point[0] ,point[1] );
            XDrawPoint (display, win, gc, point[0], point[1]);
            XFlush(display);               
        }
        XFlush(display);
        sleep (5);
    }
    
    MPI_Finalize();
    return 0;
	/* Program Finished */

}
