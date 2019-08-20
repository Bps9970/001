
//========================================================================================================================================
// **************************** Консольное приложение для тестирования драйвера BPS.SYS ***********************************************
//========================================================================================================================================
#define DRIVERNAME _T("BPS.SYS")                                     //  Имя объекта драйвера и местоположение загружаемого файла 
#define DRIVERBINARY _T("C:\\DRV\\BPS.sys")                         //  Имя объекта драйвера и местоположение загружаемого файла 
#define DRIVERBINARY _T("C:\\DRV\\BPS.sys")                 //  Имя объекта драйвера и местоположение загружаемого файла 
#define DRIVERBINARY _T("C:\\DRV\\BPS.sys")                //  Имя объекта драйвера и местоположение загружаемого файла 
#define sc

//=====================================================================================================================================

// Заголовочные файлы, которые необходимы данной проге: #include <windows.h> #include <stdio.h> #include <winioctl.h> #include <tchar.h>
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

// Начинаем знакомиться с SCM — менеджером. Далее — функция установки драйвера на основе его вызовов. 

BOOL InstallDriver(SC_HANDLE scm, LPCTSTR DriverName, LPCTSTR driverExec)
{
 SC_HANDLE Service = CreateService (scm,DriverName,DriverName,  SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
 SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, driverExec, NULL ,NULL, NULL, NULL, NULL); 
if (Service == NULL) // неудачНЫЙ ЗАПУСК
 // открытый дескриптор к SCManager DriverName,
// имя сервиса — Primer DriverName, 
// для вывода на экран SERVICE_ALL_ACCESS, // желаемый доступ SERVICE_KERNEL_DRIVER,
// тип сервиса SERVICE_DEMAND_START, 
// тип запуска SERVICE_ERROR_NORMAL,
// указывает, как обрабатывается ошибка driverExec, 
// путь к бинарному файлу 
// Остальные параметры не используются – укажем NULL NULL, 
// Не определяем группу загрузки NULL, NULL, NULL, NULL); 
if (Service == NULL) 
// неудача 
{ 
DWORD err = GetLastError(); 
if (err == ERROR_SERVICE_EXISTS) 
 {
  /* уже установлен */
 }   // более серьезная ошибка: 
else 
     cout << ( "ERR: Can’t create service") <<  (err); 
     return FALSE;
 } 
CloseServiceHandle (Service);
//Check Also;
return TRUE; 
}


// Функция удаления драйвера на основе SCM вызовов 
BOOL RemoveDriver(SC_HANDLE scm, LPCTSTR DriverName) 
{ 
SC_HANDLE Service = OpenService (scm, DriverName, SERVICE_ALL_ACCESS); 
if (Service == NULL) return FALSE;
 BOOL ret = DeleteService (Service); 
if (!ret) 
{ /* неудача при удалении драйвера */ 
CloseServiceHandle (Service); 
} 
return ret;
} 


// Функция запуска драйвера на основе SCM вызовов 

BOOL StartDriver(SC_HANDLE scm, LPCTSTR DriverName)
{ 
SC_HANDLE Service = OpenService (scm, DriverName, SERVICE_ALL_ACCESS); 
if (Service == NULL) return FALSE; 
/* open failed */
 BOOL ret = StartService(Service, NULL,NULL);
 // дескриптор 0, // число аргументов NULL); // указатель на аргументы 
if (!ret) 
{ 
// неудача 
DWORD err = GetLastError(); 
if (err == ERROR_SERVICE_ALREADY_RUNNING)
{ 
ret = TRUE; 
// драйвер уже работает 
}
else  
{
cout << ("ERR: Can’t create service");     //  [  другие проблемы ... ]
CloseServiceHandle (Service);
{
return ret; 
}
} 
}
}

// Функция остановки драйвера на основе SCM вызовов

 // ================================================================================

//BOOL StopDriver(SC_HANDLE scm, LPCTSTR DriverName); 
//{
//SC_HANDLE Service = OpenService (scm, DriverName, SERVICE_ALL_ACCESS);
// if (Service == NULL) // невозможно выполнить остановку драйвера
 //{
 //Ловушка на маковода. Как работает уязвимость в клиенте GitHub для macOS
// В клиенте GitHub для macOS до версии 1.3.4 beta 1 нашлась возможность вызвать удаленное вы…
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




// Соберем вместе действия по установке, запуску, останову 
// и удалению драйвера (для учебных целей). Пользоваться этой функцией в данном примере нам не придется, 
// и  поэтому закомментируем ее. 

 void Test_SCM_Installation(void)  //Проверить
{ SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS); 
if(scm == NULL) 
// неудача 
{ 
// получаем код ошибки и ее текстовый эквивалент 
unsigned long err = GetLastError(); 
cout << " Code error[1] : " <<  (err) << "\n"; 
return;
 }
 BOOL res;
 res = InstallDriver(scm, DRIVERNAME, DRIVERBINARY); 
// ошибка может быть не фатальной. Продолжаем:
  res = StartDriver (scm, DRIVERNAME); 
 if(res) 
   { // Здесь следует разместить функции работы с драйвером 
  //res = StopDriver (scm, DRIVERNAME);  // исправить
if(res) res = RemoveDriver (scm, DRIVERNAME); 
  } 
cout << " Code error[2] : " <<  (res) << "\n"; 
  CloseServiceHandle(scm);
  //cout << res; 
  return; 
}
 
//#define SCM_SERVICE ;
//===============================================================================================
// см. ниже  вводим элемент условной компиляции, при помощи
 // которого можно отключать использование SCM установки драйвера 
// в тексте данного приложения.
// Основная функция тестирующего приложения. 
// Здесь диагностике ошибочных ситуаций уделено очень мало внимания. 
// В реальных приложениях так делать  нельзя!
//===============================================================================================

BOOL  bps()
{
unsigned long IoctlCode; //= IOCTL_PRINT_DEBUG_ MESS;                            //IOCTL_PRINT_DEBUG_ ;    //  ??? ( не определен ) [ 157 ]
DWORD BytesReturned; // Переменная для хранения числа и  переданных байт ...
//#define SCM_SERVICE ; // используем сервис SCM для запуска драйвера.
SC_HANDLE scm;
#ifdef SCM_SERVICE 
int res; 
{
// получаем доступ к SCM : 
SC_HANDLE scm = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
 
if (scm == NULL)
{
   return -1; // неудача
}
   // делаем попытку установки драйвера 
 res = InstallDriver(scm, DRIVERNAME, DRIVERBINARY); 
  if (!res);
   {
       // неудача, но, быть может, драйвер уже установлен ?
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
     CloseServiceHandle(scm); // отключаемся от SCM 
    return -1; 
  }


#endif
HANDLE hHandle = CreateFile(DRIVERNAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | 
FILE_SHARE_WRITE, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);

 // Получаем доступ к драйверу CreateFile(DRIVERNAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |
// FILE_SHARE_WRITE, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);

 if (hHandle==INVALID_HANDLE_VALUE)
  {
  cout << " < ERROR !!! > : Can not access driver BPS.sys ! " << "\n " << " ERROR CODE "; 
  cout << & hHandle;
  return (-1); 
} 

// Приступаем к тестированию драйвера. Последовательно выполняем обращения к драйверу 
// с различными кодами IOCTL: 
//-------------------------------------------------------------------------------------------------------------------------------------------------

 
if (!DeviceIoControl(hHandle,IoctlCode, NULL, 0, NULL, 0, &BytesReturned, NULL))
 { 
  cout << " Error in IOCTL_TOUCH_PORT_378H! "; 
  return(-1); 
} 
// Продолжаем тестирование. Получаем 1 байт данных из драйвера.
// По окончании данного вызова переменная xdata должна содержать значение 33
unsigned char xdata = 0x88; 
//ioctlCode = IOCTL_SEND_BYTE_TO_USER;      // ???       ( не определен )    [ 177 ]

// Выводим мессагу в дебаговую консоль: 
printf("BytesReturned=%d\n", "xdata=%d\n", BytesReturned, xdata);
//IOCTL_SEND_BYTE_TO_USER),( BytesReturned= %d xdata=%d)), BytesReturned, (xdata);
// Выполнение следующего теста в Windows NT приведет к  краху ОС и 
// ioctlCode = IOCTL_MAKE_SYSTEM_CRASH;     // ???       ( не определен )     [ 187 ]
if (!DeviceIoControl(hHandle, IoctlCode, NULL, 0, NULL, 0, &BytesReturned, NULL))
{
 cout << " Error in IOCTL_MAKE_SYSTEM_CRASH! ";  //MSVCRTD.lib
 return(-1); 
}

 // Закрываем дескриптор доступа к драйверу: CloseHandle(hHandle);
CloseHandle(hHandle);
 #ifdef  SCM_SERVICE;
 // Останавливаем драйвер, удаляем его и отключаемся от него ...

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
//      Вот и все, компиль и тестируй. Напоминаю еще раз:                                  |||
//   запусти вьювер дебаг- информации из Monitor (например)                                |||
//   и посмотри все дебаговые сообщения  драйвера!                                         |||
//============================================================================================

// ====================Основная программа ====================================================
int _tmain(int argc, _TCHAR* argv[])
{
  int b;
  cout << " Start loading  BPS.SYS ... " << "\n";  //  [ "\n" ] - перевод на новую строку... ]]
  Test_SCM_Installation();
  bps();
  cin >> b;
 return 0;
}




