@echo off
nuget.exe pack Digi21.Epsg.nuspec
nuget.exe sign Digi21.Epsg.22.0.0.nupkg -CertificateSubjectName "DREAMING WITH OBJECTS" -TimeStamper http://timestamp.comodoca.com?td=sha256.
