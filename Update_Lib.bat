// 명령어			옵션			원본 파일이 있는 위치			사본 파일을 저장할 위치

xcopy			/y			.\Engine\Bin\Debug\Engine.dll		.\Client\Bin\Debug\
xcopy			/y			.\Engine\Bin\Debug\Engine.lib		.\UpdateSDK\Lib\
xcopy			/y/s		.\Engine\Public\*.*					.\UpdateSDK\Inc\