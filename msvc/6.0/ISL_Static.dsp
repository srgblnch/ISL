# Microsoft Developer Studio Project File - Name="ISL_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ISL_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ISL_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ISL_Static.mak" CFG="ISL_Static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ISL_Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ISL_Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ISL_Static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ISL_Static___Win32_Release"
# PROP BASE Intermediate_Dir "ISL_Static___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ISL\static\release"
# PROP Intermediate_Dir "ISL\static\release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Z7 /O2 /I "..\..\include" /I "..\..\src" /I "$(SOLEIL_ROOT)\sw-support\OpenCV\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\static\msvc-6.0\libISL.lib"

!ELSEIF  "$(CFG)" == "ISL_Static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ISL_Static___Win32_Debug"
# PROP BASE Intermediate_Dir "ISL_Static___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ISL\static\debug"
# PROP Intermediate_Dir "ISL\static\debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /Z7 /Od /I "..\..\include" /I "..\..\src" /I "$(SOLEIL_ROOT)\sw-support\OpenCV\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "ISL_ENABLE_LOG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\static\msvc-6.0\libISLd.lib"

!ENDIF 

# Begin Target

# Name "ISL_Static - Win32 Release"
# Name "ISL_Static - Win32 Debug"
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
# End Target
# End Project
