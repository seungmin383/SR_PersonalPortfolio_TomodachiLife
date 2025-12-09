@echo off

:: 이 배치파일이 있는 폴더로 이동
cd /d "%~dp0"

echo ===============================
echo  PNG -> DDS (BC7, Mipmaps) 변환 시작
echo ===============================

for %%f in (*.png) do (
    texconv.exe -f R8G8B8A8_UNORM -m 0 -o . "%%f"
)

echo ===============================
echo  변환 완료!
echo ===============================
pause
