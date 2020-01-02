@echo off
nuget.exe pack Digi21.Epsg.nuspec
nuget.exe sign Digi21.Epsg.17.0.0.nupkg -CertificatePath dwo.pfx -TimeStamper http://tsa.starfieldtech.com
