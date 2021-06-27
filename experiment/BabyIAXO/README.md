Currently we're experimenting with using [gdml.kt](https://github.com/mipt-npm/gdml.kt) to generate the GDML files.
The GDML files would be generated from a Kotlin script (currently `main.kt`) that is easily configurable and human readable. All the GDML files inside the `gdml.kt` directory are produced using this kotlin file using different parameters.

It is not requiered for the users of this repository to use this tool to generate the files, they will be available in this repository as plain GDML files, which should be self contained (not referencing other XML files). All geometries are (currently only manually) tested for overlaps and other glitches, but the final user should do a basic check to see if there are any issues with the geometry. There are some macros provided that can be used for this.

