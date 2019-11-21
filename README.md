In order to visualize the geometry with `ROOT` one can use the following command, where `Setup.gdml` is the main `gdml` file.

```
TGeoManager::Import("Setup.gdml");gGeoManager->GetTopVolume()->Draw("ogl");
```

![alt text](/miscellaneous/pictures/reference.JPG "BabyIAXO/reference@688356a3")
![alt text](/miscellaneous/pictures/steel_pipe.JPG "BabyIAXO/with_steel_pipe@bf2eab49")
