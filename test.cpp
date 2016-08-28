#include <iostream>
#include <string.h>

using namespace std;

int main ()
{
  char deviceUUID[40] = "NEW"; //unique id of the device globally if "UNCLAIMED"
  if (strcmp(deviceUUID, "NEW")==0){
    cout <<"matched"<<"\n";
  }
}
