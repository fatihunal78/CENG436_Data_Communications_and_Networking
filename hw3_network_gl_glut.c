#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;


/******************************/
/*verileri tuttugumuz yer*/
struct point {
    double x;
	double y;
};

class node{
   public:
	point p;
	vector<int> e;
	node(double x,double y){  p.x = x;    p.y = y;}
};

/***********************/
vector<node *> n;
int selected_node = -1;
int selected_edge = -1;
int second_node = -1;
int left_pressed =-1;

/************************/

int width = 400;
int height = 400;
/*************************/
int nearest(double x, double y){
   double min = 100; //maximum value
   int k = -1;
   for(int i=0;i<n.size();i++) {
       double d = sqrt((x - n[i]->p.x) * (x - n[i]->p.x) + (y - n[i]->p.y) * (y - n[i]->p.y)); 
	   if(min > d){
	     min = d;
	     k = i;
	   }
	}
   return k;
}
void draw2(void){
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	/********************************************/
    //nodelari çiz
	glPointSize(4.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<n.size();i++){
	   if(i == selected_node)
         glColor3f(1.0,0.0,0.0);  //red node
	   else
         glColor3f(0.0,0.0,0.0);  //black node        	     
	     glVertex2f(n[i]->p.x,n[i]->p.y);	
	}	
	glEnd();
    /*********************************************/
    //linelari ciz
	glLineWidth(2.0f);
	glBegin(GL_LINES); 
	for(int k=0;k<n.size();k++){
		 for(int t= 0 ; t< n[k]->e.size();t++){	
			if(k == selected_node && n[k]->e[t] == selected_edge)
               glColor3f(0.0,0.0,1.0);  //dark blue edge
			else
			   glColor3f(0.0,0.0,0.0);  //black edge   
            
			glVertex2f(n[k]->p.x,n[k]->p.y);
			glVertex2f(n[(n[k]->e[t])]->p.x,n[(n[k]->e[t])]->p.y);            
		 }
	}
	glEnd();	 
	/**********************************************/
	glutSwapBuffers();
}

void mouse(int button,int state,int x ,int y){
 
	double _x = (x-width/2.0)/(width/2.0);
	double _y = (height/2.0 - y)/(height/2.0);
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){

		n.push_back(new node(_x,_y));

	}else if (button == GLUT_LEFT_BUTTON){	

		if(state == GLUT_DOWN){
		selected_node = nearest(_x,_y);
		left_pressed = 1;
		}else
		left_pressed = -1;
	}else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
		int flag = 1;
		if(selected_node != -1) {
			 second_node = nearest(_x,_y);
		     for(int t=0;t<n[selected_node]->e.size();t++)
				 if(n[selected_node]->e[t] == second_node){
                    selected_edge = second_node;
				    flag = 0;
					cout<<selected_node<<" "<<second_node<<endl;
				 }
				 if(flag){
		            n[selected_node]->e.push_back(second_node);
					//n[second_node]->e.push_back(selected_node);
				    selected_edge=-1;
				 }
             cout<<"middle"<<endl;
		}
	   
	}
    draw2();	
	return;
}

void drag(int x, int y){
	double _x = (x-width/2.0)/(width/2.0);
	double _y = (height/2.0 - y)/(height/2.0);
    if(left_pressed == 1) {
		if(selected_node != -1) {
		n[selected_node]->p.x = _x; 
        n[selected_node]->p.y = _y;
		draw2();
		}
	}
}


void save(){
	FILE * out = fopen("graph.hw1","w");

	fprintf(out,"%d\n",selected_node);
	fprintf(out,"%d\n",selected_edge);
	fprintf(out,"%d\n",second_node);
	fprintf(out,"%d\n",n.size());
	for(int i=0;i<n.size();i++){
		fprintf(out,"%lf %lf %d\n",n[i]->p.x,n[i]->p.y,n[i]->e.size());
        
		for(int t=0;t<n[i]->e.size();t++){
			fprintf(out,"%d\n",n[i]->e[t]);
		}
	}
	fclose(out);
    return;
}

void load(){
    int n_size,e_size,e;
	double x,y;
    FILE * in = fopen("graph.hw1","r");
	fscanf(in,"%d",&selected_node);
	fscanf(in,"%d",&selected_edge);
	fscanf(in,"%d",&second_node);
    fscanf(in,"%d",&n_size);
	n.clear();
	for(int i=0;i<n_size;i++){
	 	fscanf(in,"%lf %lf %d",&x,&y,&e_size);
		cout<<x<<" "<<y<<" "<<e_size<<endl;
		n.push_back(new node(x,y));
        for(int t=0;t<e_size;t++){
			fscanf(in,"%d\n",&e);
			n[i]->e.push_back(e);
			cout<<e<<endl;
		}
	}
	fclose(in);
    return;
}

void _delete(){
//node siler
		if(selected_node != -1){		 
		 if(selected_edge != -1){
			for(int i=0;i<n[selected_node]->e.size();i++)
				if(selected_edge == n[selected_node]->e[i])
		         n[selected_node]->e.erase(n[selected_node]->e.begin() + i);
		   cout<<selected_node<<" "<<selected_edge<<endl;
          
		   selected_edge= -1;
		   selected_node = -1;
	       return;
		 }
		 for(int t=0 ; t<n.size();t++)   //kendisine komsu olanlarýda siler
			  for(int r=0; r < (n[t]->e.size());r++)
				  if(n[t]->e[r] == selected_node){
					  n[t]->e.erase(n[t]->e.begin() + r);
				   // cout<<"-->"<<t<<" "<<r<<endl;
				  }
		  n.erase(n.begin() + selected_node);
		  
          selected_edge= -1;
		  selected_node = -1;
		}
		return;
}
void keybord(unsigned char key,int x ,int y){

	if(key == 'd' || key == 'D'){
      _delete();
	}
	else if(key== 'S' || key == 's'){
	  save();
	}
	else if(key== 'l' || key == 'L'){
	  load();
	}
	draw2();
	return;
}



int main(int argc , char** argv){
    
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //bunu kullandýgým için swap buffer demem lazým geldi
	glutInitWindowSize(width,height);
	glutCreateWindow("Hw3");
    glutDisplayFunc(draw2);
    glutKeyboardFunc(keybord);
    glutMouseFunc(mouse);
	glutMotionFunc(drag);
    glutMainLoop();
    
	return(0);
}




