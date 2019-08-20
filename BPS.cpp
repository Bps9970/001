
//========================================================================================================================================
// **************************** ���������� ���������� ��� ������������ �������� BPS.SYS ***********************************************
//========================================================================================================================================
#define DRIVERNAME _T("BPS.SYS")                                     //  ��� ������� �������� � �������������� ������������ ����� 
#define DRIVERBINARY _T("C:\\DRV\\BPS.sys")                         //  ��� ������� �������� � �������������� ������������ ����� 
#define DRIVERBINARY _T("C:\\DRV\\BPS.sys")                 //  ��� ������� �������� � �������������� ������������ ����� 
#define DRIVERBINARY _T("C:\\DRV\\BPS.sys")                //  ��� ������� �������� � �������������� ������������ ����� 
#define sc

//=====================================================================================================================================

// ������������ �����, ������� ���������� ������ �����: #include <windows.h> #include <stdio.h> #include <winioctl.h> #include <tchar.h>
#include "Ioctl.h" 
#include "sal.h" 
#include <windows.h>
#include <stdio.h> 
#include <winioctl.h> 
#include <tchar.h>
#include <iostream>
#include <tchar.h>
#include "stdafx.h"

using namespace std;

// �������� ����������� � SCM � ����������. ����� � ������� ��������� �������� �� ������ ��� �������. 

BOOL InstallDriver(SC_HANDLE scm, LPCTSTR DriverName, LPCTSTR driverExec)
{
 SC_HANDLE Service = CreateService (scm,DriverName,DriverName,  SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
 SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, driverExec, NULL ,NULL, NULL, NULL, NULL); 
if (Service == NULL) // ��������� ������
 // �������� ���������� � SCManager DriverName,
// ��� ������� � Primer DriverName, 
// ��� ������ �� ����� SERVICE_ALL_ACCESS, // �������� ������ SERVICE_KERNEL_DRIVER,
// ��� ������� SERVICE_DEMAND_START, 
// ��� ������� SERVICE_ERROR_NORMAL,
// ���������, ��� �������������� ������ driverExec, 
// ���� � ��������� ����� 
// ��������� ��������� �� ������������ � ������ NULL NULL, 
// �� ���������� ������ �������� NULL, NULL, NULL, NULL); 
if (Service == NULL) 
// ������� 
{ 
DWORD err = GetLastError(); 
if (err == ERROR_SERVICE_EXISTS) 
 {
  /* ��� ���������� */
 }   // ����� ��������� ������: 
else 
     cout << ( "ERR: Can�t create service") <<  (err); 
     return FALSE;
 } 
CloseServiceHandle (Service);
//Check Also;
return TRUE; 
}


// ������� �������� �������� �� ������ SCM ������� 
BOOL RemoveDriver(SC_HANDLE scm, LPCTSTR DriverName) 
{ 
SC_HANDLE Service = OpenService (scm, DriverName, SERVICE_ALL_ACCESS); 
if (Service == NULL) return FALSE;
 BOOL ret = DeleteService (Service); 
if (!ret) 
{ /* ������� ��� �������� �������� */ 
CloseServiceHandle (Service); 
} 
return ret;
} 


// ������� ������� �������� �� ������ SCM ������� 

BOOL StartDriver(SC_HANDLE scm, LPCTSTR DriverName)
{ 
SC_HANDLE Service = OpenService (scm, DriverName, SERVICE_ALL_ACCESS); 
if (Service == NULL) return FALSE; 
/* open failed */
 BOOL ret = StartService(Service, NULL,NULL);
 // ���������� 0, // ����� ���������� NULL); // ��������� �� ��������� 
if (!ret) 
{ 
// ������� 
DWORD err = GetLastError(); 
if (err == ERROR_SERVICE_ALREADY_RUNNING)
{ 
ret = TRUE; 
// ������� ��� �������� 
}
else  
{
cout << ("ERR: Can�t create service");     //  [  ������ �������� ... ]
CloseServiceHandle (Service);
{
return ret; 
}
} 
}
}

// ������� ��������� �������� �� ������ SCM �������

 // ================================================================================

//BOOL StopDriver(SC_HANDLE scm, LPCTSTR DriverName); 
//{
//SC_HANDLE Service = OpenService (scm, DriverName, SERVICE_ALL_ACCESS);
// if (Service == NULL) // ���������� ��������� ��������� ��������
 //{
 //������� �� ��������. ��� �������� ���������� � ������� GitHub ��� macOS
// � ������� GitHub ��� macOS �� ������ 1.3.4 beta 1 ������� ����������� ������� ��������� ���
//DWORD err = GetLastError();
 //return FALSE; 
//}
//SERVICE_STATUS serviceStatus;
//BOOL ret = ControlService(Service, SERVICE_CONTROL_STOP, &serviceStatus); 
//if (!ret) 
//{ 
// }
//CloseServiceHandle (Service); 
//return ret; 
//}




// ������� ������ �������� �� ���������, �������, �������� 
// � �������� �������� (��� ������� �����). ������������ ���� �������� � ������ ������� ��� �� ��������, 
// �  ������� �������������� ��. 

 void Test_SCM_Installation(void)  //���������
{ SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS); 
if(scm == NULL) 
// ������� 
{ 
// �������� ��� ������ � �� ��������� ���������� 
unsigned long err = GetLastError(); 
cout << " Code error[1] : " <<  (err) << "\n"; 
return;
 }
 BOOL res;
 res = InstallDriver(scm, DRIVERNAME, DRIVERBINARY); 
// ������ ����� ���� �� ���������. ����������:
  res = StartDriver (scm, DRIVERNAME); 
 if(res) 
   { // ����� ������� ���������� ������� ������ � ��������� 
  //res = StopDriver (scm, DRIVERNAME);  // ���������
if(res) res = RemoveDriver (scm, DRIVERNAME); 
  } 
cout << " Code error[2] : " <<  (res) << "\n"; 
  CloseServiceHandle(scm);
  //cout << res; 
  return; 
}
 
//#define SCM_SERVICE ;
//===============================================================================================
// ��. ����  ������ ������� �������� ����������, ��� ������
 // �������� ����� ��������� ������������� SCM ��������� �������� 
// � ������ ������� ����������.
// �������� ������� ������������ ����������. 
// ����� ����������� ��������� �������� ������� ����� ���� ��������. 
// � �������� ����������� ��� ������  ������!
//===============================================================================================

BOOL  bps()
{
unsigned long IoctlCode; //= IOCTL_PRINT_DEBUG_ MESS;                            //IOCTL_PRINT_DEBUG_ ;    //  ??? ( �� ��������� ) [ 157 ]
DWORD BytesReturned; // ���������� ��� �������� ����� �  ���������� ���� ...
//#define SCM_SERVICE ; // ���������� ������ SCM ��� ������� ��������.
SC_HANDLE scm;
#ifdef SCM_SERVICE 
int res; 
{
// �������� ������ � SCM : 
SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
 
if (scm == NULL)
{
   return -1; // �������
}
   // ������ ������� ��������� �������� 
 res = InstallDriver(scm, DRIVERNAME, DRIVERBINARY); 
  if (!res);
   {
       // �������, ��, ���� �����, ������� ��� ���������� ?
      cout << " Cannot install service ";
   }
res = StartDriver (scm, DRIVERNAME); 
  if (!res)
   {
    cout << " Cannot start driver! ";
    res = RemoveDriver (scm, DRIVERNAME);
   }
  if (!res) 
   {
     cout << (" Cannot remove driver ! "); 
     CloseServiceHandle(scm); // ����������� �� SCM 
    return -1; 
  }


#endif
HANDLE hHandle = CreateFile(DRIVERNAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | 
FILE_SHARE_WRITE, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);

 // �������� ������ � �������� CreateFile(DRIVERNAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |
// FILE_SHARE_WRITE, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);

 if (hHandle==INVALID_HANDLE_VALUE)
  {
  cout << " < ERROR !!! > : Can not access driver BPS.sys ! " << "\n " << " ERROR CODE "; 
  cout << & hHandle;
  return (-1); 
} 

// ���������� � ������������ ��������. ��������������� ��������� ��������� � �������� 
// � ���������� ������ IOCTL: 
//-------------------------------------------------------------------------------------------------------------------------------------------------

 
if (!DeviceIoControl(hHandle,IoctlCode, NULL, 0, NULL, 0, &BytesReturned, NULL))
 { 
  cout << " Error in IOCTL_TOUCH_PORT_378H! "; 
  return(-1); 
} 
// ���������� ������������. �������� 1 ���� ������ �� ��������.
// �� ��������� ������� ������ ���������� xdata ������ ��������� �������� 33
unsigned char xdata = 0x88; 
//ioctlCode = IOCTL_SEND_BYTE_TO_USER;      // ???       ( �� ��������� )    [ 177 ]

// ������� ������� � ��������� �������: 
printf("BytesReturned=%d\n", "xdata=%d\n", BytesReturned, xdata);
//IOCTL_SEND_BYTE_TO_USER),( BytesReturned= %d xdata=%d)), BytesReturned, (xdata);
// ���������� ���������� ����� � Windows NT �������� �  ����� �� � 
// ioctlCode = IOCTL_MAKE_SYSTEM_CRASH;     // ???       ( �� ��������� )     [ 187 ]
if (!DeviceIoControl(hHandle, IoctlCode, NULL, 0, NULL, 0, &BytesReturned, NULL))
{
 cout << " Error in IOCTL_MAKE_SYSTEM_CRASH! ";  //MSVCRTD.lib
 return(-1); 
}

 // ��������� ���������� ������� � ��������: CloseHandle(hHandle);
CloseHandle(hHandle);
 #ifdef  SCM_SERVICE;
 // ������������� �������, ������� ��� � ����������� �� ���� ...

res = StopDriver (scm, DRIVERNAME); 
if (!res);
 { 
cout << (" Cannot stop driver! ");
CloseServiceHandle(scm); 
return -1; 
}
#endif

#if (!res)
{
  cout << "Cannot remove driver!";
  CloseServiceHandle(scm); 
  return -1;
 }
 #endif
return 0;
}


//============================================================================================
//      ��� � ���, ������� � ��������. ��������� ��� ���:                                  |||
//   ������� ������ �����- ���������� �� Monitor (��������)                                |||
//   � �������� ��� ��������� ���������  ��������!                                         |||
//============================================================================================

// ====================�������� ��������� ====================================================
int _tmain(int argc, _TCHAR* argv[])
{
  int b;
  cout << " Start loading  BPS.SYS ... " << "\n";  //  [ "\n" ] - ������� �� ����� ������... ]]
  Test_SCM_Installation();
  bps();
  cin >> b;
 return 0;
}




