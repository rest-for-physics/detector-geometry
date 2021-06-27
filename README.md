GDML files are inside the `experiment` folder both for `IAXO-D0` and `BabyIAXO` experiments. In the `macros` directory you will find some useful ROOT macros, such as to export a geometry in ROOT format or to visualize it.

You can also visualize the geometry with `ROOT` using these simple commands, where `Setup.gdml` is the main `gdml` file.

```
TGeoManager::Import("Setup.gdml");
gGeoManager->GetTopVolume()->Draw("ogl");
```

You may have a look to the REST-for-Physics [basic-geometries](https://github.com/rest-for-physics/basic-geometries) repository where you will find other basic examples.
