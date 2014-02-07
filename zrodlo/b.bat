@set "bak1=%include%"
@set "bak2=%lib%"

@set "include=%include%C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include;"
@set "lib=%lib%C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86;"

cls
cl -EHsc -W1 gl.cpp ^
-link -out:w.exe -nologo

@set "include=%bak1%"
@set "lib=%bak2%"