#include <stdio.h>
#include <stdlib.h>
//Curvature guassian
int main()
{
system("rm Data/*.xml");
system("python generate_0xml.py");
system("python generate_more_sporos_xml.py");
system("./main 5001 0.xml -r");
system("python analysis_see_movement.py");
return (0);
} 
