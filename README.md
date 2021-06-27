GDML files are inside the `experiment` folder both for `IAXO-D0` and `BabyIAXO` experiments. In the `macros` directory you will find some useful ROOT macros, such as to export a geometry in ROOT format or to visualize it.

You can also visualize the geometry with `ROOT` using these simple commands, where `Setup.gdml` is the main `gdml` file.

```
TGeoManager::Import("Setup.gdml");
gGeoManager->GetTopVolume()->Draw("ogl");
```

Note: if the geometry contains many nested layers, you need to disable ROOT's [automatic visible depth](https://github.com/mipt-npm/gdml.kt/issues/30) to correctly visualize the geometry.

![BabyIAXO Reference Geometry (27/06/2021)](https://user-images.githubusercontent.com/35803280/123548792-9288b180-d766-11eb-839d-9a158c1971a4.png)

You may have a look to the REST-for-Physics [basic-geometries](https://github.com/rest-for-physics/basic-geometries) repository where you will find other basic examples.
