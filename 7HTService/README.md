### HTCOM
Для работы с dll, используйте в консоли, запущенной от администратора, команду
```
 	regsvr32 /i OS13_HTCOM.dll
```
в папке _/x64/Debug_

После чего можно использовать другие скомпилированные **.exe** файлы через _visual studio_

## Данный пунк, в коде уже соблюден но можете проверить
Для запуска **.exe** через консоль нужно изменить строчку в **OS13_HTCOM_LIB.cpp** в функции **RecordingWork** *(строка 374)*: 
```C++
	std::wstring filePath(L"..\\Records\\" + typeOperation + L"\\" + date);
```
На строку:
```C++
	std::wstring filePath(L"..\\..\\Records\\" + typeOperation + L"\\" + date);
```

Перед запуском службы нужно изменить **3** полных пути:
1 и 2 в файле Service.cpp строка номер 19, в данном случае нужно скопилировать проект **OS15_START** и используя проект **OS15_CREATE** создать **HT** хранилище с определенным именем, уже строку показанную ниже нужно заменить на пути к проекту **OS15_START** и к созданному раннее **хранилищу** (в примере я создавал хранилище с именем test.ht):
```C++
	std::wstring szExePath = L"T:\\C3S2\\OS\\HTService\\x64\\Debug\\OS15_START.exe T:\\C3S2\\OS\\HTService\\HT\\test.ht";
```
3 в файле OS15_StartStopService.cpp строка номер 6:
```C++
	#define SERVICEPATH L"T:\\C3S2\\OS\\HTService\\x64\\Debug\\OS15_HTService.exe"
```

### Для загрузки службы в систему используется проект OS15_StartStopService
Вызываете скомпилированный **.exe** данного проекта с флагом **/i**, после чего служба появится в списке служб Windows.
Что бы удалить данную службу из Windows, так же запусается данный проект только с флагом **/d**
