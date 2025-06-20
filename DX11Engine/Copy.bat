// 사용할 명령어	옵션			원본 파일의 위치			사본을 저장할 위치

xcopy			/y			.\Engine\Default\epch.h		.\Reference\Header\
xcopy			/y			.\Engine\Header\*.h			.\Reference\Header\
xcopy			/y			.\Engine\Header\*.hpp		.\Reference\Header\

xcopy			/y			.\Engine\Bin\Engine.dll		.\Client\Bin\
xcopy			/y			.\Engine\Bin\Engine.lib		.\Reference\Library\