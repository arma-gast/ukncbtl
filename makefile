# VS2008 nmake makefile
# Run from the Visual Studio 2008 Command Prompt

# Tools
WIXROOT=C:\bin\wix

# Project directories
WIXBIN=.\src\UKNCBTL.WiX\bin

# File masks
LIBDSKFILES=.\lib\disks\*.dsk
SETUPWIXOBJFILES=.\src\UKNCBTL.WiX\*.wixobj
SETUPMSIFILE=.\src\UKNCBTL.WiX\UkncBtl.msi


build:	build-main build-rt11dsk build-setup


build-main:
	vcbuild /rebuild src\UKNCBTL\UKNCBTL.vcproj Product

build-rt11dsk:
	vcbuild /rebuild src\rt11dsk\rt11dsk.vcproj Release

prepare-setup:
	copy .\src\UKNCBTL\Product\UKNCBTL.exe $(WIXBIN)
	copy .\src\rt11dsk\Release\rt11dsk.exe $(WIXBIN)
	copy .\lib\roms\uknc_rom.bin $(WIXBIN)
	copy .\lib\roms\romctr_basic.bin $(WIXBIN)
	copy $(LIBDSKFILES) $(WIXBIN)
	copy .\docs\README.txt $(WIXBIN)
	copy "$(VCINSTALLDIR)\redist\x86\Microsoft.VC90.CRT\msvcr90.dll" $(WIXBIN)
	copy "$(VCINSTALLDIR)\redist\x86\Microsoft.VC90.CRT\Microsoft.VC90.CRT.manifest" $(WIXBIN)

build-setup:  clean-setup prepare-setup
	cd .\src\UKNCBTL.WiX
	$(WIXROOT)\candle.exe -nologo Generic.wxs
	$(WIXROOT)\light.exe -nologo Generic.wixobj $(WIXROOT)\wixui.wixlib -out UkncBtl.msi -loc WixUI_ru-ru.wxl -loc UkncBtl_ru-ru.wxl
	cd ..\..
	del /Q $(SETUPWIXOBJFILES)
	if not exist .\build mkdir .\build
	copy .\src\UKNCBTL.WiX\UkncBtl.msi .\build
	copy .\docs\README.txt .\build

clean:	clean-setup
	if exist .\build rmdir /S/Q .\build

clean-setup:
	del /Q $(SETUPWIXOBJFILES)
	del /Q $(SETUPMSIFILE)

