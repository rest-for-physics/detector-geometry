# Detector Geometry

This repository serves as a database of all serialized geometry files used in IAXO simulations (written in [gdml](https://indico.cern.ch/event/408139/contributions/979922/attachments/815913/1118019/GDML_CHEP06.pdf) format).

It also hosts software required to produce these files and some tools to validate or visualize them.

All gdml files are stored in the `gdml` directory and are all self-contained meaning they do not reference any other files and can be used directly in ROOT or Geant4.

### Visualization

In order to visualize a gdml file with root one can do the following (for a file named `Setup.gdml`):

```
TGeoManager::Import("Setup.gdml");
gGeoManager->GetTopVolume()->Draw("ogl");
```


Note: if the getGeometry contains many nested layers, you need to disable ROOT's [automatic visible depth](https://github.com/mipt-npm/gdml.kt/issues/30) to correctly visualize the getGeometry.

Additionally, the macro file `ViewGdml.C` from the `root` directory can be used to visualize the gdml. To use this macro to visualize some file named `Setup.gdml` one could do the following from a shell that has access to the `root` command (not from the root prompt itself!):

```
root 'ViewGdml.C("Setup.gdml")'
```
