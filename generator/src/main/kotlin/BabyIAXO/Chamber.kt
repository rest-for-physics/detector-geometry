package BabyIAXO

import Geometry

import space.kscience.gdml.*

open class Chamber : Geometry() {
    companion object Parameters {
        // Body
        const val Height: Double = 30.0
        const val Diameter: Double = 102.0
        const val BackplateThickness: Double = 15.0
        const val SquareSide: Double = 134.0
        const val TeflonWallThickness: Double = 1.0

        // Readout
        const val ReadoutKaptonThickness: Double = 0.5
        const val ReadoutCopperThickness: Double = 0.2
        const val ReadoutPlaneSide: Double = 60.0

        // Cathode
        const val CathodeTeflonDiskHoleRadius: Double = 15.0
        const val CathodeTeflonDiskThickness: Double = 5.0
        const val CathodeCopperSupportOuterRadius: Double = 45.0
        const val CathodeCopperSupportInnerRadius: Double = 8.5
        const val CathodeCopperSupportThickness: Double = 1.0
        const val CathodeWindowThickness: Double = 0.004
        const val CathodePatternDiskRadius: Double = 4.25
        const val CathodePatternLineWidth: Double = 0.3
    }

    override fun generate(gdml: Gdml): GdmlRef<GdmlAssembly> {
        val chamberVolume: GdmlRef<GdmlAssembly> by lazy {
            val chamberBodySolid = gdml.solids.subtraction(
                gdml.solids.box(
                    SquareSide,
                    SquareSide,
                    Height,
                    "chamberBodyBaseSolid"
                ), gdml.solids.tube(Diameter / 2, Height, "chamberBodyHoleSolid"),
                "chamberBodySolid"
            )
            val chamberBodyVolume = gdml.structure.volume(Materials.Copper.ref, chamberBodySolid, "chamberBodyVolume")

            val chamberBackplateSolid = gdml.solids.box(
                SquareSide,
                SquareSide,
                BackplateThickness,
                "chamberBackplateSolid"
            )
            val chamberBackplateVolume =
                gdml.structure.volume(Materials.Copper.ref, chamberBackplateSolid, "chamberBackplateVolume")

            val chamberTeflonWallSolid =
                gdml.solids.tube(Diameter / 2, Height, "chamberTeflonWallSolid") {
                    rmin = Diameter / 2 - TeflonWallThickness
                }
            val chamberTeflonWallVolume =
                gdml.structure.volume(Materials.Teflon.ref, chamberTeflonWallSolid, "chamberTeflonWallVolume")
            // readout
            val kaptonReadoutSolid = gdml.solids.box(
                SquareSide,
                SquareSide,
                ReadoutKaptonThickness,
                "kaptonReadoutSolid"
            )
            val kaptonReadoutVolume =
                gdml.structure.volume(Materials.Kapton.ref, kaptonReadoutSolid, "kaptonReadoutVolume")

            val copperReadoutSolid = gdml.solids.box(
                ReadoutPlaneSide,
                ReadoutPlaneSide,
                ReadoutCopperThickness,
                "copperReadoutSolid"
            )

            val copperReadoutVolume =
                gdml.structure.volume(Materials.Kapton.ref, copperReadoutSolid, "copperReadoutVolume")

            // cathode
            val cathodeTeflonDiskBaseSolid = gdml.solids.tube(
                SquareSide / 2,
                CathodeTeflonDiskThickness,
                "cathodeTeflonDiskBaseSolid"
            ) {
                rmin = CathodeTeflonDiskHoleRadius
            }

            val cathodeCopperDiskSolid = gdml.solids.tube(
                CathodeCopperSupportOuterRadius,
                CathodeCopperSupportThickness,
                "cathodeCopperDiskSolid"
            ) {
                rmin = CathodeCopperSupportInnerRadius
            }

            val cathodeTeflonDiskSolid =
                gdml.solids.subtraction(
                    cathodeTeflonDiskBaseSolid,
                    cathodeCopperDiskSolid,
                    "cathodeTeflonDiskSolid"
                ) {
                    position(z = -CathodeTeflonDiskThickness / 2 + CathodeCopperSupportThickness / 2)
                }

            val cathodeTeflonDiskVolume =
                gdml.structure.volume(Materials.Teflon.ref, cathodeTeflonDiskSolid, "cathodeTeflonDiskVolume") {}

            val cathodeWindowSolid =
                gdml.solids.tube(
                    CathodeTeflonDiskHoleRadius,
                    CathodeWindowThickness,
                    "cathodeWindowSolid"
                )
            val cathodeWindowVolume =
                gdml.structure.volume(Materials.Mylar.ref, cathodeWindowSolid, "cathodeWindowVolume")

            // cathode copper disk pattern
            val cathodePatternLineAux = gdml.solids.box(
                CathodePatternLineWidth,
                CathodeCopperSupportInnerRadius * 2,
                CathodeCopperSupportThickness,
                "cathodePatternLineAux"
            )
            val cathodePatternCentralHole = gdml.solids.tube(
                CathodePatternDiskRadius,
                CathodeCopperSupportThickness * 1.1, "cathodePatternCentralHole"
            )
            val cathodePatternLine =
                gdml.solids.subtraction(cathodePatternLineAux, cathodePatternCentralHole, "cathodePatternLine")

            val cathodePatternDisk = gdml.solids.tube(
                CathodePatternDiskRadius,
                CathodeCopperSupportThickness, "cathodePatternDisk"
            ) { rmin = CathodePatternDiskRadius - CathodePatternLineWidth }


            var cathodeCopperDiskSolidAux: GdmlRef<GdmlUnion> = GdmlRef("")

            for (i in 0..3) {
                cathodeCopperDiskSolidAux =
                    gdml.solids.union(
                        if (i > 0) cathodeCopperDiskSolidAux else cathodeCopperDiskSolid,
                        cathodePatternLine, "cathodeCopperDiskSolidAux$i"
                    ) {
                        rotation(x = 0, y = 0, z = 45 * i) {
                            unit = AUnit.DEG
                        }
                    }
            }

            val cathodeCopperDiskFinal =
                gdml.solids.union(cathodeCopperDiskSolidAux, cathodePatternDisk, "cathodeCopperDiskFinal.solid")

            val cathodeCopperDiskVolume =
                gdml.structure.volume(Materials.Copper.ref, cathodeCopperDiskFinal, "cathodeCopperDiskFinal")

            val cathodeFillingBaseSolid = gdml.solids.tube(
                CathodeTeflonDiskHoleRadius,
                CathodeTeflonDiskThickness,
                "cathodeFillingBaseSolid"
            )
            val cathodeFillingSolid =
                gdml.solids.subtraction(cathodeFillingBaseSolid, cathodeCopperDiskFinal, "cathodeFillingSolid") {
                    position(z = -CathodeTeflonDiskThickness / 2 + CathodeCopperSupportThickness / 2)
                }
            val cathodeFillingVolume =
                gdml.structure.volume(Materials.Vacuum.ref, cathodeFillingSolid, "cathodeFillingVolume") {}

            // gas
            val gasSolidOriginal = gdml.solids.tube(
                Diameter / 2 - TeflonWallThickness,
                Height, "gasSolidOriginal"
            )
            val gasSolidAux = gdml.solids.subtraction(gasSolidOriginal, copperReadoutSolid, "gasSolidAux") {
                position(z = -Height / 2 + ReadoutCopperThickness / 2)
                rotation(z = 45) { unit = AUnit.DEG }
            }
            val gasSolid =
                gdml.solids.subtraction(gasSolidAux, cathodeWindowSolid, "gasSolid") {
                    position(z = Height / 2 - CathodeWindowThickness / 2)
                }
            val gasVolume = gdml.structure.volume(Materials.Gas.ref, gasSolid, "gasVolume")

            return@lazy gdml.structure.assembly {
                name = "Chamber"
                physVolume(gasVolume, name = "gas")
                physVolume(chamberBackplateVolume, name = "chamberBackplate") {
                    position(
                        z = -Height / 2 - ReadoutKaptonThickness - BackplateThickness / 2
                    )
                }
                physVolume(chamberBodyVolume, name = "chamberBody")
                physVolume(chamberTeflonWallVolume, name = "chamberTeflonWall")
                physVolume(kaptonReadoutVolume, name = "kaptonReadout") {
                    position(z = -Height / 2 - ReadoutKaptonThickness / 2)
                }
                physVolume(copperReadoutVolume, name = "copperReadout") {
                    position(z = -Height / 2 + ReadoutCopperThickness / 2)
                    rotation(z = 45) { unit = AUnit.DEG }
                }
                physVolume(cathodeWindowVolume, name = "cathodeWindow") {
                    position(z = Height / 2 - CathodeWindowThickness / 2)
                }
                physVolume(cathodeTeflonDiskVolume, name = "cathodeTeflonDisk") {
                    position(z = Height / 2 + CathodeTeflonDiskThickness / 2)
                }
                physVolume(cathodeFillingVolume, name = "cathodeFilling") {
                    position(z = Height / 2 + CathodeTeflonDiskThickness / 2)
                }
                physVolume(cathodeCopperDiskVolume, name = "cathodeCopperDiskPattern") {
                    position(z = Height / 2 + CathodeCopperSupportThickness / 2)
                }
            }
        }

        return chamberVolume
    }

}