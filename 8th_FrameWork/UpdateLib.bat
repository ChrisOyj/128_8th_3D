

////xcopy	/옵션    .복사해야할 파일또는폴더			.복사받을 폴더. 

xcopy   /y		..\..\Engine\Bin\Engine.lib			..\..\Reference\Librarys\
xcopy   /y		..\..\Engine\ThirdPartyLib\*.lib	..\..\Reference\Librarys\
xcopy   /y		..\..\Engine\Bin\Engine.dll			..\..\Client\Bin\
xcopy   /y/s		..\..\Engine\Public\*.*			..\..\Reference\Headers\
xcopy   /y/s	..\..\Engine\Bin\ShaderFiles\*.*		..\..\Client\Bin\ShaderFiles\
