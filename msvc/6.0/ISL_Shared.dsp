# Microsoft Developer Studio Project File - Name="ISL_Shared" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ISL_Shared - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ISL_Shared.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ISL_Shared.mak" CFG="ISL_Shared - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ISL_Shared - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ISL_Shared - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ISL_Shared - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ISL_Shared___Win32_Release"
# PROP BASE Intermediate_Dir "ISL_Shared___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ISL\shared\release"
# PROP Intermediate_Dir "ISL\shared\release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ISL_BUILD" /D "ISL_DLL" /YX /FD /c
# ADD CPP /MD /W3 /GR /GX /Z7 /O2 /I "..\..\include" /I "..\..\src" /I "$(SOLEIL_ROOT)\sw-support\OpenCV\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ISL_BUILD" /D "ISL_DLL" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /dll /pdb:"../../bin/msvc-6.0/libISL.pdb" /machine:I386 /out:"../../bin/msvc-6.0/libISL.dll" /implib:"../../lib/shared/msvc-6.0/libISL.lib"
# SUBTRACT LINK32 /verbose /pdb:none

!ELSEIF  "$(CFG)" == "ISL_Shared - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ISL\shared\debug"
# PROP Intermediate_Dir "ISL\shared\debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ISL_BUILD" /D "ISL_DLL" /D "ISL_ENABLE_LOG" /YX /FD /GZ /c
# ADD CPP /MDd /W3 /GR /GX /Z7 /Od /I "..\..\include" /I "..\..\src" /I "$(SOLEIL_ROOT)\sw-support\OpenCV\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ISL_BUILD" /D "ISL_DLL" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /dll /incremental:no /pdb:"../../bin/msvc-6.0/libISLd.pdb" /debug /machine:I386 /out:"../../bin/msvc-6.0/libISLd.dll" /implib:"../../lib/shared/msvc-6.0/libISLd.lib" /pdbtype:sept
# SUBTRACT LINK32 /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "ISL_Shared - Win32 Release"
# Name "ISL_Shared - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Group "src_calib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\calib\Calib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\Engine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\Engine.h
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\Engine.i
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\PatternAnalyzer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\PatternAnalyzer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\PatternAnalyzer.i
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\PerspEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\PerspEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\PerspEngine.i
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\TsaiEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\TsaiEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\src\calib\TsaiEngine.i
# End Source File
# End Group
# Begin Group "src_blob"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\blob\Blob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\blob\BlobComparator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\blob\BlobList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\blob\BlobMeasure.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\blob\Criteria.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\blob\Filter.cpp
# End Source File
# End Group
# Begin Group "src_geometry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\geometry\Ellipse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\geometry\Point.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\geometry\Rectangle.cpp
# End Source File
# End Group
# Begin Group "src_statistics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\statistics\AccurateCentroid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\GaussianFit1D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\GaussianFit2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\Histogram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\LineProfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\LMOptim.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\Moments.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\NoiseEstimator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\PrincipalAxis.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\statistics\Projections.cpp
# End Source File
# End Group
# Begin Group "src_movie"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_FFmpeg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_FFmpeg.h
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_GraphicFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_GraphicFile.h
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_GraphicFileASCII.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_GraphicFileASCII.h
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_GraphicFileISL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_GraphicFileISL.h
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_GraphicFileRAS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriter_GraphicFileRAS.h
# End Source File
# Begin Source File

SOURCE=..\..\src\movie\MovieWriterFactory.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\AutoROI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\BeamBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Exception.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\InternalErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\InternalErrorHandler.h
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Group "isl"

# PROP Default_Filter ""
# Begin Group "calib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\isl\calib\Calib.h
# End Source File
# End Group
# Begin Group "blob"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\isl\blob\Blob.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\Blob.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\BlobComparator.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\BlobComparator.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\BlobList.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\BlobMeasure.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\Criteria.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\Criteria.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\Filter.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\Filter.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\Operator.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\blob\Operator.i
# End Source File
# End Group
# Begin Group "geometry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\isl\geometry\Ellipse.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\geometry\Ellipse.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\geometry\Point.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\geometry\Point.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\geometry\Point.t
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\geometry\Rectangle.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\geometry\Rectangle.i
# End Source File
# End Group
# Begin Group "statistics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\isl\statistics\AccurateCentroid.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\GaussianFit1D.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\GaussianFit1D.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\GaussianFit2D.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\GaussianFit2D.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\Histogram.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\Histogram.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\LineProfile.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\LineProfile.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\LMOptim.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\LMOptim.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\Moments.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\Moments.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\NoiseEstimator.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\NoiseEstimator.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\PrincipalAxis.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\PrincipalAxis.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\Projections.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\statistics\Projections.i
# End Source File
# End Group
# Begin Group "movie"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\isl\movie\MovieWriter.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\include\isl\AutoROI.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\AutoROI.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\BeamBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\BeamBox.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\Constants.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\ErrorHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\Exception.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\Exception.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\Image.h
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\Image.i
# End Source File
# Begin Source File

SOURCE=..\..\include\isl\ISLConfig.h
# End Source File
# End Group
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE="$(SOLEIL_ROOT)\sw-support\OpenCV\lib\shared\msvc-6.0\highgui.lib"
# End Source File
# Begin Source File

SOURCE="$(SOLEIL_ROOT)\sw-support\OpenCV\lib\shared\msvc-6.0\cv.lib"
# End Source File
# Begin Source File

SOURCE="$(SOLEIL_ROOT)\sw-support\OpenCV\lib\shared\msvc-6.0\cxcore.lib"
# End Source File
# End Group
# End Target
# End Project
