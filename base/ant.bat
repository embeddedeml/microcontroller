@echo off
set CYGWIN=nodosfilewarning
set PATH=%PATH%;D:\usr\viseml\git\microcontroller\build\win
java -classpath "../build/ant/lib/ant-launcher.jar" -Dant.home="../build/ant/" -Dant.library.dir="../build/ant/lib" org.apache.tools.ant.launch.Launcher -Dostype="win" -Dproject.path="..\%1" %2
