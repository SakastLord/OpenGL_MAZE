/************************************************************
  Copyright (C), 2008-2010, Jbctech. Co., Ltd.
  FileName: test.cpp
  Author:Jacob Chen            
  Description: 旋转地球    
  Version:     V1.0     
  History:     NONE
  Date:		   2009.8.31
***********************************************************/
# include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <gl/glaux.h>
# include <gl/glut.h>


GLuint		    texture[1];	
GLUquadricObj	*q;	
AUX_RGBImageRec *TextureImage[1];	
GLfloat	xrot=0.1;				// Angle For The Triangle 

/*************************************************
  Function:       LoadBMP
  Description:    load a BMP picture from the disk
  Input:          Filename is the path of the file
  Return:         return a point to the Bitmap picture if success,
                  return NUll while faile to open the file.
  Others:         none
*************************************************/


AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle
								
	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

/*************************************************
  Function:       init
  Description:    init material, light and texture.then enable the 
  Input:          none
  Return:         none
  Others:         none
*************************************************/

void init ( void )
 {
    GLfloat mat_specular [ ] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse [ ] = { 0.0, 2.0, 2.0, 0.1};
    GLfloat mat_shininess [ ] = { 100.0 };

   // Light Parameters
    GLfloat LightAmb[] = {0.3f, 0.3f, 0.3f, 1.0f};	// Ambient Light
    GLfloat LightDif[] = {1.0f, 1.0f, 1.0f, 1.0f};	// Diffuse Light	
    GLfloat light0_position [ ] = { 2.0, 0.1, 0.0, 0.0 };
    GLfloat light1_position [ ] = { 2.0, 0.1, 0.0, 0.0 };
    GLfloat light2_position [ ] = { 10.0, 0.0, 0.0, 0.0 };
 
    glClearColor ( 0.0, 0.0, 0.0, 1.0 );
    glShadeModel ( GL_SMOOTH );

    glLightfv ( GL_LIGHT1, GL_AMBIENT, LightAmb);			// Set The Ambient Lighting For Light0
    glLightfv ( GL_LIGHT1, GL_DIFFUSE, LightDif);			// Set The Diffuse Lighting For Light0	 
    glLightfv ( GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv ( GL_LIGHT1, GL_POSITION, light1_position);


/*******************use the quadratic to creat the object**************************************/
    q = gluNewQuadric();								  // Create A New Quadratic

    gluQuadricNormals(q, GL_SMOOTH);					  // Generate Smooth Normals For The Quad
    gluQuadricTexture(q, GL_TRUE);						  // Enable Texture Coords For The Quad

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping

/*******************Init Texture**************************************/

    memset(TextureImage,0,sizeof(void *)*1);			  // Set The Pointer To NULL

    if (TextureImage[0]=LoadBMP("DATA/jbBall~.bmp")) 		
    { 							
      glGenTextures(1, &texture[0]);					  // Create The Texture
					
      glBindTexture(GL_TEXTURE_2D, texture[0]);
	  //10月14日早上2点钟，这里发生了一个很神奇的错误，我自己p的bmp位图，下面的函数无法进行识别，报
	  //0x027A04A0 (ig75icd32.dll)处(位于 Rolling-earth.exe 中)引发的异常: 0xC0000005: 读取位置 0x0897D000 时发生访问冲突。

	  //如有适用于此异常的处理程序，该程序便可安全地继续运行
	  //这样的错误，我的内心是崩溃的，而且明明没有边框，非要把RGB前面的一项选择了之后，才能够正常的进行显示
	  //一个猜想，会不会再变化之后，图片的像素值，深度值，或者是颜色的编码发生了变化，从rgb变成了其他的颜色编码
	  
	  
	  
	  glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	  
	  
	  
	  
	  
	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	  if (TextureImage[0])						// If Texture Exists
      {
        if (TextureImage[0]->data)			// If Texture Image Exists
        {
          free(TextureImage[0]->data);		// Free The Texture Image Memory
        }

        free(TextureImage[0]);				   // Free The Image Structure 
      }
    }
	glEnable (GL_TEXTURE_2D);   
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    glEnable (GL_DEPTH_TEST);

	//glTranslatef(-0.5,0.0,0.0);
glRotatef(-70,1.0,0.0,0.0);
	//glRotatef(70.0, 1.0, 0.0, 0.0);
}

/*************************************************
  Function:       display
  Description:    the object we draw
  Input:          none
  Return:         none
  Others:         none
*************************************************/

 void display ( void )
 {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glRotatef(xrot,0.0f,0.0f,1.0f);	
	gluSphere(q, 1.3f, 100, 100);

	glRasterPos2f(0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');

	glFlush ();
 }
/*************************************************
  Function:       update
  Description:    every 25 millisecond we update the xrot to execute display again and again.
  Input:          value is the time we want rotate the earth.
  Return:         none
  Others:         none
*************************************************/

void update(int value)
{
  xrot = 0.1f;      
  value = 25;
  glutPostRedisplay(); //Tell GLUT that the scene has changed          
  glutTimerFunc(value, update, 0);//Tell GLUT to call update again in 25 milliseconds   
}

/*************************************************
  Function:       reshap
  Description:    creat a window
  Input:          w is the width of the window you created while h is the height.
  Return:         none 
  Others:         none
*************************************************/

 void reshape (int w, int h)
 {
	 printf("JBC");
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
	
    if (w <= h) 
        glOrtho (-1.5, 1.5, -1.5 * ( GLfloat ) h / ( GLfloat ) w, 1.5 * ( GLfloat ) h / ( GLfloat ) w, -10.0, 10.0 );
    else
        glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h, 1.5 * ( GLfloat ) w / ( GLfloat ) h, -1.5, 1.5, -10.0, 10.0);

    glMatrixMode ( GL_MODELVIEW ); 
	 glLoadIdentity ( ) ;
	 glRotatef(-70,1.0,0.0,0.0);
 }

 /*************************************************
  Function:       keboard
  Description:    response to keyboard action
  Input:          key is a character corresponding to the keyboard
  Return:         NONE
  Others:         NONE
*************************************************/

 void keyboard ( unsigned char key, int x, int y)
 {
     
    switch (key)  
    {   case 'r':
		case 'R':
		
			xrot=0.2;
			glutPostRedisplay();
			
			break;
        case 27://press exit if you wanna exit
        exit (0); 
        break;
    }
 }

 /*************************************************
  Function:     main
  Description:  Main functionat,at first we init glut and set the display mode,
				set the position of the window.then creat the window,call OpenGL 
				init function.Next,we register the display,reshape and keyboard 
				function to glut.For we need to rotater the earth,so we add a timer
				here.Finally,we go to the glut's main loop.
  Input:          
  Return:       return 0 if success
  Others:       We get a return until exit glutMainloop().
*************************************************/

 int main(int argc, char** argv)
 {

    glutInit (&argc, argv);

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize (600, 600);

    glutInitWindowPosition (100, 100);

    glutCreateWindow ( argv [ 0 ] );

    init ( );

    glutDisplayFunc ( display );

    glutReshapeFunc ( reshape );

    glutKeyboardFunc ( keyboard );

	glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop( );

	return 0;
 }
 