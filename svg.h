#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

/*
 * For original project details of this project go to
http://www.codedrome.com/svg-library-in-c/
 *
 * I'm adding some features, polyline, polygon, inline commenter, svg pixel-map... other geometries, and features intended for zellij design 
 * To compile using this library: //gcc -g -Wall  prog.c -o prog    -lm
 * */


// --------------------------------------------------------
// STRUCT svg
// --------------------------------------------------------
typedef struct svg{
    char* svg;
    int height;
    int width;
    bool finalized;
} svg;

// --------------------------------------------------------
// FUNCTION PROTOTYPES
// --------------------------------------------------------
//ok I just realized bruh did an obnoxious thing, and made all the shape functions take arguments in different patterns. 
//function( psvg, x,y, height, width, radius, fill, stroke, strokewidth )
//Ill fix that later, RN gonna make animation work. 

/*I too did an anoying thing and wrote animateRotate using the format of the former args patterns. 
 * Using stdarg.h, functions require specifying the number of args for a function, these are as follows
 * 
 * svg_ellipse: 8
 * svg_polygon 8
 * svg_text 8
 * svg_line 7
 * svg_rectangle 10
 * 
 * after which, args are passed to the animateRotate function
 * 
 * Example animateRotate call:
 *   svg_animateRotate( 8, svg_polygon, psvg, 100.0, 100.0, 50.0, 5, "blue", "green", 7, durration);
 * */
svg* svg_create(int width, int height);
void svg_finalize(svg* psvg);
void svg_print(svg* psvg);
void svg_save(svg* psvg, char* filepath);
void svg_free(svg* psvg);
void svg_fill(svg* psvg, char* fill);
void svg_circle(svg* psvg, char* stroke, int strokewidth, char* fill, int r, int cx, int cy);
void svg_line(svg* psvg, char* stroke, int strokewidth, int x1, int y1, int x2, int y2);
void svg_rectangle(svg* psvg, int width, int height, int x, int y, char* fill, char* stroke, int strokewidth, int radiusx, int radiusy);
void svg_text(svg* psvg, int x, int y, char* fontfamily, int fontsize, char* fill, char* stroke, char* text);
void svg_ellipse(svg* psvg, int cx, int cy, int rx, int ry, char* fill, char* stroke, int strokewidth);
void svg_polygon(svg* psvg, double cx, double cy, float radius, int nsides, char* fill, char* stroke, int strokewidth );
//void svg_polyline(svg* psvg, int a[], char* stroke, int strokewidth); //comeback to this it was for a kinnect
//void svg_starpolygon(svg* psvg, double cx, double cy, float radius, int npoints, char* fill, char* stroke, int strokewidth );
//void svg_gearpolygon(svg* psvg, double cx, double cy, float radius, int teeth, char* fill, char* stroke, int strokewidth );
//void svg_diamondpolygon(svg* psvg, double cx, double cy, float radius, char* fill, char* stroke, int strokewidth );
void svg_animateRotate(int num_args, ...); //take shape functions
void svg_animateColor(int num_args, ...);

// --------------------------------------------------------
// STATIC FUNCTION appendstringtosvg
// --------------------------------------------------------
static void appendstringtosvg(svg* psvg, char* text){
    int l = strlen(psvg->svg) + strlen(text) + 1;

    char* p = realloc(psvg->svg, l);

    if(p){
        psvg->svg = p;
    }

    strcat(psvg->svg, text);
}

// --------------------------------------------------------
// STATIC FUNCTION appendnumbertosvg
// --------------------------------------------------------
static void appendnumbertosvg(svg* psvg, int n){
    char sn[16];
    sprintf(sn, "%d", n);
    appendstringtosvg(psvg, sn);
}

// --------------------------------------------------------
// FUNCTION svg_create
// --------------------------------------------------------
svg* svg_create(int width, int height){
    svg* psvg = malloc(sizeof(svg));

    if(psvg != NULL)    {
        *psvg = (svg){.svg = NULL, .width = width, .height = height, .finalized = false};

        psvg->svg = malloc(1);

        sprintf(psvg->svg, "%s", "\0");

        appendstringtosvg(psvg, "<svg width='");
        appendnumbertosvg(psvg, width);
        appendstringtosvg(psvg, "px' height='");
        appendnumbertosvg(psvg, height);
        appendstringtosvg(psvg, "px' xmlns='http://www.w3.org/2000/svg' version='1.1' xmlns:xlink='http://www.w3.org/1999/xlink'>\n");

        return psvg;
    }    else    {
        return NULL;
    }
}

// --------------------------------------------------------
// FUNCTION svg_finalize
// --------------------------------------------------------
void svg_finalize(svg* psvg){
    appendstringtosvg(psvg, "</svg>");
    psvg->finalized = true;
}

// --------------------------------------------------------
// FUNCTION svg_print
// --------------------------------------------------------
void svg_print(svg* psvg){
    printf("%s\n", psvg->svg);
}

// --------------------------------------------------------
// FUNCTION svg_save
// --------------------------------------------------------
void svg_save(svg* psvg, char* filepath){
    if(!psvg->finalized){
        svg_finalize(psvg);
    }

    FILE* fp;
	fp = fopen(filepath, "w");

	if(fp != NULL){
		fwrite(psvg->svg, 1, strlen(psvg->svg), fp);

		fclose(fp);
	}
}

//----------------------------------------------------------------
// FUNCTION svg_free
//----------------------------------------------------------------
void svg_free(svg* psvg){
    free(psvg->svg);
    free(psvg);
}

//----------------------------------------------------------------
// FUNCTION svg_circle
//----------------------------------------------------------------
void svg_circle(svg* psvg, char* stroke, int strokewidth, char* fill, int r, int cx, int cy){
    appendstringtosvg(psvg, "    <circle stroke='");
    appendstringtosvg(psvg, stroke);
    appendstringtosvg(psvg, "' stroke-width='");
    appendnumbertosvg(psvg, strokewidth);
    appendstringtosvg(psvg, "px' fill='");
    appendstringtosvg(psvg, fill);
    appendstringtosvg(psvg, "' r='");
    appendnumbertosvg(psvg, r);
    appendstringtosvg(psvg, "' cy='");
    appendnumbertosvg(psvg, cy);
    appendstringtosvg(psvg, "' cx='");
    appendnumbertosvg(psvg, cx);
    appendstringtosvg(psvg, "' />\n");
}

//----------------------------------------------------------------
// FUNCTION svg_line
//----------------------------------------------------------------
void svg_line(svg* psvg, char* stroke, int strokewidth, int x1, int y1, int x2, int y2){
    appendstringtosvg(psvg, "    <line stroke='");
    appendstringtosvg(psvg, stroke);
    appendstringtosvg(psvg, "' stroke-width='");
    appendnumbertosvg(psvg, strokewidth);
    appendstringtosvg(psvg, "px' x1='");
    appendnumbertosvg(psvg, y2);
    appendstringtosvg(psvg, "' y1='");
    appendnumbertosvg(psvg, x2);
    appendstringtosvg(psvg, "' x2='");
    appendnumbertosvg(psvg, y1);
    appendstringtosvg(psvg, "' y2='");
    appendnumbertosvg(psvg, x1);
    appendstringtosvg(psvg, "' />\n");
}

//----------------------------------------------------------------
// FUNCTION svg_rectangle
//----------------------------------------------------------------
void svg_rectangle(svg* psvg, int width, int height, int x, int y, char* fill, char* stroke, int strokewidth, int radiusx, int radiusy){
    appendstringtosvg(psvg, "    <rect fill='");
    appendstringtosvg(psvg, fill);
    appendstringtosvg(psvg, "' stroke='");
    appendstringtosvg(psvg, stroke);
    appendstringtosvg(psvg, "' stroke-width='");
    appendnumbertosvg(psvg, strokewidth);
    appendstringtosvg(psvg, "px' width='");
    appendnumbertosvg(psvg, width);
    appendstringtosvg(psvg, "' height='");
    appendnumbertosvg(psvg, height);
    appendstringtosvg(psvg, "' y='");
    appendnumbertosvg(psvg, y);
    appendstringtosvg(psvg, "' x='");
    appendnumbertosvg(psvg, x);
    appendstringtosvg(psvg, "' ry='");
    appendnumbertosvg(psvg, radiusy);
    appendstringtosvg(psvg, "' rx='");
    appendnumbertosvg(psvg, radiusx);
    appendstringtosvg(psvg, "' />\n");
}

// --------------------------------------------------------
// FUNCTION svg_fill
// --------------------------------------------------------
void svg_fill(svg* psvg, char* Fill){
    svg_rectangle(psvg, psvg->width, psvg->height, 0, 0, Fill, Fill, 0, 0, 0);
}

//----------------------------------------------------------------
// FUNCTION svg_text
//----------------------------------------------------------------
void svg_text(svg* psvg, int x, int y, char* fontfamily, int fontsize, char* fill, char* stroke, char* text){
    appendstringtosvg(psvg, "    <text x='");
    appendnumbertosvg(psvg, x);
    appendstringtosvg(psvg, "' y = '");
    appendnumbertosvg(psvg, y);
    appendstringtosvg(psvg, "' font-family='");
    appendstringtosvg(psvg, fontfamily);
    appendstringtosvg(psvg, "' stroke='");
    appendstringtosvg(psvg, stroke);
    appendstringtosvg(psvg, "' fill='");
    appendstringtosvg(psvg, fill);
    appendstringtosvg(psvg, "' font-size='");
    appendnumbertosvg(psvg, fontsize);
    appendstringtosvg(psvg, "px'>");
    appendstringtosvg(psvg, text);
    appendstringtosvg(psvg, "</text>\n");
}

//----------------------------------------------------------------
// FUNCTION svg_ellipse
//----------------------------------------------------------------
void svg_ellipse(svg* psvg, int cx, int cy, int rx, int ry, char* fill, char* stroke, int strokewidth){
    appendstringtosvg(psvg, "    <ellipse cx='");
    appendnumbertosvg(psvg, cx);
    appendstringtosvg(psvg, "' cy='");
    appendnumbertosvg(psvg, cy);
    appendstringtosvg(psvg, "' rx='");
    appendnumbertosvg(psvg, rx);
    appendstringtosvg(psvg, "' ry='");
    appendnumbertosvg(psvg, ry);
    appendstringtosvg(psvg, "' fill='");
    appendstringtosvg(psvg, fill);
    appendstringtosvg(psvg, "' stroke='");
    appendstringtosvg(psvg, stroke);
    appendstringtosvg(psvg, "' stroke-width='");
    appendnumbertosvg(psvg, strokewidth);
    appendstringtosvg(psvg, "' />\n");
}

//---------------------------------------------------------
//FUNCTION svg_polyline: this function had been getting data from the kinect
//---------------------------------------------------------
/*
 * void svg_polyline(svg* psvg, int a[], char* stroke, int strokewidth){

appendstringtosvg(psvg, " <polyline  points='");
//assume array is x,y alternating. or index,depth. 
//it goes: x,y  x,y  x,y... untill the end. append with quotes
//determine array size
int aSize= sizeof(a)/sizeof(a[index]);
for (int i =0; i< aSize; i++){
	appendstringtosvg(psvg, i);
	appendstringtosvg(psvg, ",");
	appendstringtosvg(psvg, i);
	appendstringtosvg(psvg, " ");
		}
 appendstringtosvg(psvg, "' stroke='");		
 appendstringtosvg(psvg, stroke);
 appendstringtosvg(psvg, "' stroke-width='");
 appendnumbertosvg(psvg, strokewidth);
 appendstringtosvg(psvg, "' />\n");
	}
*/

//----------------------------------------------------------------
// FUNCTION svg_polygon
//----------------------------------------------------------------
void svg_polygon(svg* psvg, double cx, double cy, float radius, int nsides, char* fill, char* stroke, int strokewidth ){
//cx,cy ==center point
//put xi ,yi in an array	
float xi, yi=0.0;
	if (nsides<3){
	printf("Error: Too Few Sides\n");
    exit(0);
}

//start building the polygon:
    appendstringtosvg(psvg, "    <polygon style=\"fill:");
    appendstringtosvg(psvg, fill);
    appendstringtosvg(psvg, "; stroke:");
    appendstringtosvg(psvg, stroke);
    appendstringtosvg(psvg, "; stroke-width:");
    appendnumbertosvg(psvg, strokewidth);
    appendstringtosvg(psvg, "\" points='");

	//calc x,y point cordinates. trig funtions take radians
	for (int i =1; i < nsides+1;  i++){
	xi= cx + radius * cos((2*M_PI/nsides) * i);
	    appendnumbertosvg(psvg, xi);
	    appendstringtosvg(psvg, ", ");
	yi= cy + radius * sin((2*M_PI/nsides) *  i);
	    appendnumbertosvg(psvg, yi);	
	    appendstringtosvg(psvg, " ");	    
		printf("%f , %f\n ", xi, yi);
		}
		printf("\n");
		//close polygon
     appendstringtosvg(psvg, "' />\n");
	}

//----------------------------------------------------------------
// FUNCTION svg_animate :: transform methods
//----------------------------------------------------------------

/*
  * In SVG's animateTransform element, the type attribute specifies the type of transformation to be animated. The type attribute can take the following values:
    translate: Animates a translation transformation, which moves an element along the x and y axes.
    scale: Animates a scaling transformation, which changes the size of an element along the x and y axes.
    rotate: Animates a rotation transformation, which rotates an element around a specified point.
    skewX: Animates a skew transformation along the x-axis, which tilts an element horizontally.
    skewY: Animates a skew transformation along the y-axis, which tilts an element vertically.

 * */
 //----------------------------------------------------------------
// FUNCTION svg_animateRotate
//----------------------------------------------------------------
// Operator function that accepts variable arguments
void svg_animateRotate(int num_args, ...) {
    va_list args;
    va_start(args, num_args);

    for (int i = 0; i < num_args; i++) {
        // Get the function pointer from the variable arguments
        void (*func)(svg*, ...);
        func = va_arg(args, void (*)(svg*, ...));

        // Call the function with the remaining arguments
        if (func == svg_ellipse) {
            svg* psvg = va_arg(args, svg*);
            int cx = va_arg(args, int);
            int cy = va_arg(args, int);
            int rx = va_arg(args, int);
            int ry = va_arg(args, int);
            char* fill = va_arg(args, char*);
            char* stroke = va_arg(args, char*);
            int strokewidth = va_arg(args, int);
            int dur = va_arg(args,int);               //maybe make that a float or double later
            appendstringtosvg(psvg, " \n<g>\n");            
            svg_ellipse(psvg, cx, cy, rx, ry, fill, stroke, strokewidth); //write the shape
            appendstringtosvg(psvg, " \n <animateTransform \n  attributeName=\"transform\" \n  attributeType=\"XML\" \n type=\"rotate\"\n");
            appendstringtosvg(psvg, "from=\"0 "); //cx,cy atleast in the polygon function are center
            appendnumbertosvg(psvg, cx);
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, cy);
            appendstringtosvg(psvg, "\"\n");

            appendstringtosvg(psvg, "to=\"360 "); //theta space x space y
            appendnumbertosvg(psvg, cx);
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, cy);
            appendstringtosvg(psvg, "\"\ndur =\""); 
            appendnumbertosvg(psvg, dur);
            appendstringtosvg(psvg, "s\"\n"); 
            appendstringtosvg(psvg, "repeatCount=\"indefinite\" />\n</g>\n"); 
        } else if (func == svg_polygon) { //8 args
            svg* psvg = va_arg(args, svg*);
            double cx = va_arg(args, double);
            double cy = va_arg(args, double);
            float radius = va_arg(args, double); // Note: float is promoted to double in va_arg
            int nsides = va_arg(args, int);
            char* fill = va_arg(args, char*);
            char* stroke = va_arg(args, char*);
            int strokewidth = va_arg(args, int);
            int dur = va_arg(args,int);               //maybe make that a float or double later
            appendstringtosvg(psvg, " \n<g>\n");
            svg_polygon(psvg, cx, cy, radius, nsides, fill, stroke, strokewidth); 
            appendstringtosvg(psvg, " \n <animateTransform \n  attributeName=\"transform\" \n  attributeType=\"XML\" \n type=\"rotate\"\n");
            appendstringtosvg(psvg, "from=\"0 "); //cx,cy atleast in the polygon function are center
            appendnumbertosvg(psvg, cx);
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, cy);
            appendstringtosvg(psvg, "\"\n");

            appendstringtosvg(psvg, "to=\"360 "); //theta space x space y
            appendnumbertosvg(psvg, cx);
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, cy);
            appendstringtosvg(psvg, "\"\ndur =\"");
            appendnumbertosvg(psvg, dur);
            appendstringtosvg(psvg, "s\"\n"); 
            appendstringtosvg(psvg, "repeatCount=\"indefinite\" />\n</g>\n"); 
        }  else if (func == svg_text) {
			//This function doesn some BS related to groups. 
			//void svg_text(svg* psvg, int x, int y, char* fontfamily, int fontsize, char* fill, char* stroke, char* text);
            svg* psvg = va_arg(args, svg*);
            int x = va_arg(args, int);
            int y = va_arg(args, int);
            char* fontfamily = va_arg(args, char*); // use system fonts
            int fontsize = va_arg(args, int);
            char* fill = va_arg(args, char*);
            char* stroke = va_arg(args, char*);
            char* text = va_arg(args, char*);
            int dur = va_arg(args,int);               //durration of animation
            appendstringtosvg(psvg, " \n<g>\n");
            svg_text(psvg, x, y, fontfamily, fontsize, fill, stroke, text);
            appendstringtosvg(psvg, " \n<animateTransform \n  attributeName=\"transform\" \n  attributeType=\"XML\" \n type=\"rotate\"\n");
            appendstringtosvg(psvg, "from=\"0 "); //cx,cy atleast in the polygon function are center 
            appendnumbertosvg(psvg, x);                 //maybe set x, y based on text length. 
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, y);
            appendstringtosvg(psvg, "\"\n");

            appendstringtosvg(psvg, "to=\"360 "); //theta space x space y
            appendnumbertosvg(psvg, x);
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, y);
            appendstringtosvg(psvg, "\"\ndur =\"");
            appendnumbertosvg(psvg, dur);
            appendstringtosvg(psvg, "s\"\n"); 
            appendstringtosvg(psvg, "repeatCount=\"indefinite\" />\n</g>\n"); 
			}  else if (func == svg_rectangle) {
			// svg_rectangle(psvg,  width,  height, x,  y,  fill,  stroke,  strokewidth,  radiusx,  radiusy); //all ints 10 args
            svg* psvg = va_arg(args, svg*);
            int width = va_arg(args, int);
            int height = va_arg(args, int);
            int x = va_arg(args, int);
            int y = va_arg(args, int);
            char* fill = va_arg(args, char*);
            char* stroke = va_arg(args, char*);
            int strokewidth = va_arg(args, int);
            int radiusx = va_arg(args, int);
            int radiusy = va_arg(args, int);
            int dur = va_arg(args,int);               //durration of animation
            appendstringtosvg(psvg, " \n<g>");
            svg_rectangle(psvg,  width,  height, x,  y,  fill,  stroke,  strokewidth,  radiusx,  radiusy); 
            appendstringtosvg(psvg, " \n<animateTransform \n  attributeName=\"transform\" \n  attributeType=\"XML\" \n type=\"rotate\"\n");
            appendstringtosvg(psvg, "from=\"0 "); //cx,cy atleast in the polygon function are center 
            appendnumbertosvg(psvg, x);                 //maybe set x, y based on text length. 
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, y);
            appendstringtosvg(psvg, "\"\n");

            appendstringtosvg(psvg, "to=\"360 "); //theta space x space y
            appendnumbertosvg(psvg, x);
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, y);
            appendstringtosvg(psvg, "\"\ndur =\"");
            appendnumbertosvg(psvg, dur);
            appendstringtosvg(psvg, "s\"\n"); 
            appendstringtosvg(psvg, "repeatCount=\"indefinite\" />\n</g>\n"); 
    } else if (func == svg_line) {
		//void svg_line(svg* psvg, char* stroke, int strokewidth, int x1, int y1, int x2, int y2);
		// svg_line(psvg, stroke, strokewidth,  x1,  y1, x2,  y2);
		    svg* psvg = va_arg(args, svg*);
		    char* stroke = va_arg(args, char*);
		    int strokewidth = va_arg(args, int);
            int x1 = va_arg(args, int);
            int y1 = va_arg(args, int);
            int x2 = va_arg(args, int);
            int y2 = va_arg(args, int);

            int dur = va_arg(args,int);               //durration of animation
            appendstringtosvg(psvg, " \n<g>");
            svg_line(psvg, stroke, strokewidth,  x1,  y1, x2,  y2);
            appendstringtosvg(psvg, " \n<animateTransform \n  attributeName=\"transform\" \n  attributeType=\"XML\" \n type=\"rotate\"\n");
            appendstringtosvg(psvg, "from=\"0 "); //cx,cy atleast in the polygon function are center 
            appendnumbertosvg(psvg, x1);                 //maybe set x, y based on text length. 
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, y1);
            appendstringtosvg(psvg, "\"\n");

            appendstringtosvg(psvg, "to=\"360 "); //theta space x space y
            appendnumbertosvg(psvg, x2);
            appendstringtosvg(psvg, " ");
            appendnumbertosvg(psvg, y2);
            appendstringtosvg(psvg, "\"\ndur =\"");
            appendnumbertosvg(psvg, dur);
            appendstringtosvg(psvg, "s\"\n"); 
            appendstringtosvg(psvg, "repeatCount=\"indefinite\" />\n</g>\n"); 
		}
}//end for loop
    va_end(args);
}
