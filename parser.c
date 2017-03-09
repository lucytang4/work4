#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
  Inputs:   char * filename 
  struct matrix * transform, 
  struct matrix * pm,
  screen s
  Returns: 

  Goes through the file named filename and performs all of the actions listed in that file.
  The file follows the following format:
  Every command is a single character that takes up a line
  Any command that requires arguments must have those arguments in the second line.
  The commands are as follows:
  line: add a line to the edge matrix - 
  takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
  ident: set the transform matrix to the identity matrix - 
  scale: create a scale matrix, 
  then multiply the transform matrix by the scale matrix - 
  takes 3 arguments (sx, sy, sz)
  translate: create a translation matrix, 
  then multiply the transform matrix by the translation matrix - 
  takes 3 arguments (tx, ty, tz)
  rotate: create an rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 2 arguments (axis, theta) axis should be x y or z
  apply: apply the current transformation matrix to the 
  edge matrix
  display: draw the lines of the edge matrix to the screen
  display the screen
  save: draw the lines of the edge matrix to the screen
  save the screen to a file -
  takes 1 argument (file name)
  quit: end parsing

  See the file script for an example of the file format


  IMPORTANT MATH NOTE:
  the trig functions int math.h use radian mesure, but us normal
  humans use degrees, so the file will contain degrees for rotations,
  be sure to conver those degrees to radians (M_PI is the constant
  for PI)
  ====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);

    //line: add a line to the edge matrix - takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
    if (strcmp(line,"line") == 0){
      fgets(line,255,f);
      line[strlen(line)-1]='\0';
      int ctr;
      char *tmp = line;
      int values[6];
      printf("values : ");
      for (ctr = 0; ctr < 6; ctr++){
	values[ctr] = strtol(tmp,&tmp,10);
	printf("%d ",values[ctr]);
      }
      printf("\n");
      add_edge(edges,values[0],values[1],values[2],values[3],values[4],values[5]);
    }

    //ident: set the transform matrix to the identity matrix -
    else if (strcmp(line,"ident") == 0)
      ident(transform);
    
    //scale: create a scale matrix,then multiply the transform matrix by the scale matrix - takes 3 arguments (sx, sy, sz)
    else if (strcmp(line,"scale") == 0){
      fgets(line,255,f);
      line[strlen(line)-1]='\0';
      int ctr;
      char *tmp = line;
      int values[3];
      printf("values : ");
      for (ctr = 0; ctr < 3; ctr++){
	values[ctr] = strtol(tmp,&tmp,10);
	printf("%d ",values[ctr]);
      }
      printf("\n");
      struct matrix *s = make_scale(values[0],values[1],values[2]);
      matrix_mult(s,transform);
    }
 
    //translate: create a translation matrix, then multiply the transform matrix by the translation matrix - takes 3 arguments (tx, ty, tz)
    else if (strcmp(line,"move") == 0){
      fgets(line,255,f);
      line[strlen(line)-1]='\0';
      int ctr;
      char *tmp = line;
      int values[3];
      printf("values : ");
      for (ctr = 0; ctr < 3; ctr++){
	values[ctr] = strtol(tmp,&tmp,10);
	printf("%d ",values[ctr]);
      }
      printf("\n");
      struct matrix *t = make_translate(values[0],values[1],values[2]);
      matrix_mult(t,transform);
    }

    //rotate: create an rotation matrix, then multiply the transform matrix by the rotation matrix - takes 2 arguments (axis, theta) axis should be x y or z
    else if (strcmp(line,"rotate") == 0){
      fgets(line,255,f);
      line[strlen(line)-1]='\0';
      char *tmp = line;
      char *axis;
      int value;
      printf("values : ");
      axis = strsep(&tmp," ");
      value = strtol(tmp,&tmp,10);
      printf("%s %d",axis,value);
      printf("\n");
      struct matrix *r;
      if (strcmp(axis,"x") == 0)
        r = make_rotX(value);
      else if (strcmp(axis,"y") == 0)
        r = make_rotY(value);
      else if (strcmp(axis,"z") == 0)
	r = make_rotZ(value);
      matrix_mult(r,transform);
    }
      
    //apply: apply the current transformation matrix to the edge matrix
    else if (strcmp(line,"apply") == 0)
      matrix_mult(transform,edges);
      
    //display: draw the lines of the edge matrix to the screen, display the screen
    else if (strcmp(line,"display") == 0){
      clear_screen(s);
      color c;
      draw_lines(edges,s,c);
      display(s);
    }
          
    //save: draw the lines of the edge matrix to the screen, save the screen to a file - takes 1 argument (file name)
    else if (strcmp(line,"save") == 0){
      fgets(line,255,f);
      line[strlen(line)]='\0';
      printf("file : %s\n",line);
      save_extension(s,line);
    }
  }
}
  
