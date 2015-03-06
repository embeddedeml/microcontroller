param(
[string]$project,
[string]$target
)

$port = ""

$outFile = ..\build\win\listComPorts.exe
foreach ($outFileLine in $outFile) {
  if($outFileLine -match ".*(COM[0-9]+).*Arduino.*")
  {
    $port = $matches[1]
  }
}

$AllArgs = @('-classpath', '"../build/ant/lib/ant-launcher.jar"', 
             '-Dant.home="../build/ant/"', 
             '-Dant.library.dir="../build/ant/lib"', 
             'org.apache.tools.ant.launch.Launcher',
             "-Dproject.path=`"..\$project`"",
             "-Davrdude.port=`"$port`"",
             $target)
 
& 'java' $AllArgs