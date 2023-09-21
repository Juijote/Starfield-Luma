@echo off

set dxc=..\tools\dxc_2023_08_14\bin\x64\dxc.exe

for /f "delims=" %%x in (target_folder.txt) do set target_folder=%%x

if "%target_folder%" == "" (
  echo target folder isn't set in "target_folder.txt"!
  echo single line with the folder with no quotation marks and a trailing \
  echo example: C:\Games\Starfield\Data\shadersfx\
  pause
  goto :eof
)

if not exist "%target_folder%" (
  echo target folder doesn't exist!
  echo creating it...
  mkdir "%target_folder%"
)

"%dxc%" "HDRComposite\HDRComposite_ps.hlsl" ^
        -T ps_6_6 -E PS -D APPLY_MERGED_COLOR_GRADING_LUT -Fo ^
        "%target_folder%HDRComposite\HDRComposite_E00FF1A_ps.bin"

"%dxc%" "HDRComposite\HDRComposite_ps.hlsl" ^
        -T ps_6_6 -E PS -Fo ^
        "%target_folder%HDRComposite\HDRComposite_600FF1A_ps.bin"

"%dxc%" "HDRComposite\HDRComposite_ps.hlsl" ^
        -T ps_6_6 -E PS -D APPLY_BLOOM -Fo ^
        "%target_folder%HDRComposite\HDRComposite_700FF1A_ps.bin"

"%dxc%" "HDRComposite\HDRComposite_ps.hlsl" ^
        -T ps_6_6 -E PS -D APPLY_BLOOM -D APPLY_MERGED_COLOR_GRADING_LUT -Fo ^
        "%target_folder%HDRComposite\HDRComposite_F00FF1A_ps.bin"

"%dxc%" "ScaleformComposite\ScaleformComposite_FFAA_ps.hlsl" ^
        -T ps_6_6 -E PS -Fo ^
        "%target_folder%ScaleformComposite\ScaleformComposite_FFAA_ps.bin"

"%dxc%" "ColorGradingMerge\ColorGradingMerge_FF81_cs.hlsl" ^
        -T cs_6_6 -E main -Fo ^
        "%target_folder%ColorGradingMerge\ColorGradingMerge_FF81_cs.bin"

"%dxc%" "BinkMovie\BinkMovie_FFAB_ps.hlsl" ^
        -T ps_6_6 -E PS -Fo ^
        "%target_folder%BinkMovie\BinkMovie_FFAB_ps.bin"

"%dxc%" "FilmGrain\FilmGrain_FF75_ps.hlsl" ^
        -T ps_6_6 -E main -Fo ^
        "%target_folder%FilmGrain\FilmGrain_FF75_ps.bin"

"%dxc%" "PostSharpen\PostSharpen_FF9A_ps.hlsl" ^
        -T ps_6_6 -E main -Fo ^
        "%target_folder%PostSharpen\PostSharpen_FF9A_ps.bin"

"%dxc%" "ContrastAdaptiveSharpening\ContrastAdaptiveSharpening_100FF94_cs.hlsl" ^
        -T cs_6_6 -E main -Fo ^
        "%target_folder%ContrastAdaptiveSharpening\ContrastAdaptiveSharpening_100FF94_cs.bin"

pause