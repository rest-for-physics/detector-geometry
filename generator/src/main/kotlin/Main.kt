import space.kscience.gdml.*

import java.io.File

fun main() {

    val geometries = mapOf(
        "Setup" to Gdml {
            /*
            materials.apply {
                // need to add all reference inside the material recursively also
                val material = resolveMaterialByRef(Materials.Air)
                add(material)
            }
            */

            loadMaterialsFromUrl(materialsUrl) /* This adds all materials form the URL (we do not need them all) */

            val chamberVolume = chamber(this)
            val detectorPipeVolume = detectorPipe(this)
            val shieldingVolume = shielding(this)

            structure {
                val worldSize = 2000
                val worldBox = solids.box(worldSize, worldSize, worldSize, "worldBox")

                world = volume(Materials.Air.ref, worldBox, "world") {
                    physVolume(chamberVolume, name = "Chamber")
                    physVolume(detectorPipeVolume, name = "DetectorPipe") { position(z = DetectorPipe.ZinWorld.mm) }
                    physVolume(shieldingVolume, name = "Shielding")
                }
            }
        },
        "Chamber" to Gdml {
            loadMaterialsFromUrl(materialsUrl) /* This adds all materials form the URL (we do not need them all) */

            val chamberVolume = chamber(this)

            structure {
                val worldSize = 2000
                val worldBox = solids.box(worldSize, worldSize, worldSize, "worldBox")

                world = volume(Materials.Air.ref, worldBox, "world") {
                    physVolume(chamberVolume, name = "Chamber")
                }
            }
        },
        "Shielding" to Gdml {
            loadMaterialsFromUrl(materialsUrl) /* This adds all materials form the URL (we do not need them all) */

            val shieldingVolume = shielding(this)

            structure {
                val worldSize = 2000
                val worldBox = solids.box(worldSize, worldSize, worldSize, "worldBox")

                world = volume(Materials.Air.ref, worldBox, "world") {
                    physVolume(shieldingVolume, name = "Shielding")
                }
            }
        },
        "DetectorPipe" to Gdml {
            loadMaterialsFromUrl(materialsUrl) /* This adds all materials form the URL (we do not need them all) */

            val detectorPipeVolume = detectorPipe(this)

            structure {
                val worldSize = 2000
                val worldBox = solids.box(worldSize, worldSize, worldSize, "worldBox")

                world = volume(Materials.Air.ref, worldBox, "world") {
                    physVolume(detectorPipeVolume, name = "DetectorPipe")
                }
            }
        },
    )

    // Save all gdml files into "gdml" directory

    val outputDirectory = "gdml"

    val directory = File(".", outputDirectory)
    directory.deleteRecursively()
    directory.mkdir()

    for ((name, gdml) in geometries) {
        File("$outputDirectory/$name.gdml")
            .writeText(gdml.encodeToString())
    }

}