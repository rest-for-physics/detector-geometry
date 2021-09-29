import space.kscience.gdml.*

enum class Chamber(val mm: Double) {
    // Body + Backplate
    Height(30.0), Diameter(102.0),
    BackplateThickness(15.0), SquareSide(134.0),
    TeflonWallThickness(1.0),

    // Readout
    ReadoutKaptonThickness(0.5), ReadoutCopperThickness(0.2),
    ReadoutPlaneSide(60.0),

    // Cathode
    CathodeTeflonDiskHoleRadius(15.0), CathodeTeflonDiskThickness(5.0),
    CathodeCopperSupportOuterRadius(45.0), CathodeCopperSupportInnerRadius(8.5),
    CathodeCopperSupportThickness(1.0), CathodeWindowThickness(0.004),
    CathodePatternDiskRadius(4.25), CathodePatternLineWidth(0.3),
}

fun chamber(gdml: Gdml): GdmlRef<GdmlAssembly> {

    val chamberVolume: GdmlRef<GdmlAssembly> by lazy {
        val chamberBodySolid = gdml.solids.subtraction(
            gdml.solids.box(
                Chamber.SquareSide.mm,
                Chamber.SquareSide.mm,
                Chamber.Height.mm,
                "chamberBodyBaseSolid"
            ), gdml.solids.tube(Chamber.Diameter.mm / 2, Chamber.Height.mm, "chamberBodyHoleSolid"),
            "chamberBodySolid"
        )
        val chamberBodyVolume = gdml.structure.volume(Materials.Copper.ref, chamberBodySolid, "chamberBodyVolume")

        val chamberBackplateSolid = gdml.solids.box(
            Chamber.SquareSide.mm,
            Chamber.SquareSide.mm,
            Chamber.BackplateThickness.mm,
            "chamberBackplateSolid"
        )
        val chamberBackplateVolume =
            gdml.structure.volume(Materials.Copper.ref, chamberBackplateSolid, "chamberBackplateVolume")

        val chamberTeflonWallSolid =
            gdml.solids.tube(Chamber.Diameter.mm / 2, Chamber.Height.mm, "chamberTeflonWallSolid") {
                rmin = Chamber.Diameter.mm / 2 - Chamber.TeflonWallThickness.mm
            }
        val chamberTeflonWallVolume =
            gdml.structure.volume(Materials.Teflon.ref, chamberTeflonWallSolid, "chamberTeflonWallVolume")
        // readout
        val kaptonReadoutSolid = gdml.solids.box(
            Chamber.SquareSide.mm,
            Chamber.SquareSide.mm,
            Chamber.ReadoutKaptonThickness.mm,
            "kaptonReadoutSolid"
        )
        val kaptonReadoutVolume = gdml.structure.volume(Materials.Kapton.ref, kaptonReadoutSolid, "kaptonReadoutVolume")

        val copperReadoutSolid = gdml.solids.box(
            Chamber.ReadoutPlaneSide.mm,
            Chamber.ReadoutPlaneSide.mm,
            Chamber.ReadoutCopperThickness.mm,
            "copperReadoutSolid"
        )

        val copperReadoutVolume = gdml.structure.volume(Materials.Kapton.ref, copperReadoutSolid, "copperReadoutVolume")

        // cathode
        val cathodeTeflonDiskBaseSolid = gdml.solids.tube(
            Chamber.SquareSide.mm / 2,
            Chamber.CathodeTeflonDiskThickness.mm,
            "cathodeTeflonDiskBaseSolid"
        ) {
            rmin = Chamber.CathodeTeflonDiskHoleRadius.mm
        }

        val cathodeCopperDiskSolid = gdml.solids.tube(
            Chamber.CathodeCopperSupportOuterRadius.mm,
            Chamber.CathodeCopperSupportThickness.mm,
            "cathodeCopperDiskSolid"
        ) {
            rmin = Chamber.CathodeCopperSupportInnerRadius.mm
        }

        val cathodeTeflonDiskSolid =
            gdml.solids.subtraction(
                cathodeTeflonDiskBaseSolid,
                cathodeCopperDiskSolid,
                "cathodeTeflonDiskSolid"
            ) {
                position(z = -Chamber.CathodeTeflonDiskThickness.mm / 2 + Chamber.CathodeCopperSupportThickness.mm / 2)
            }

        val cathodeTeflonDiskVolume =
            gdml.structure.volume(Materials.Teflon.ref, cathodeTeflonDiskSolid, "cathodeTeflonDiskVolume") {}

        val cathodeWindowSolid =
            gdml.solids.tube(
                Chamber.CathodeTeflonDiskHoleRadius.mm,
                Chamber.CathodeWindowThickness.mm,
                "cathodeWindowSolid"
            )
        val cathodeWindowVolume = gdml.structure.volume(Materials.Mylar.ref, cathodeWindowSolid, "cathodeWindowVolume")

        // cathode copper disk pattern
        val cathodePatternLineAux = gdml.solids.box(
            Chamber.CathodePatternLineWidth.mm,
            Chamber.CathodeCopperSupportInnerRadius.mm * 2,
            Chamber.CathodeCopperSupportThickness.mm,
            "cathodePatternLineAux"
        )
        val cathodePatternCentralHole = gdml.solids.tube(
            Chamber.CathodePatternDiskRadius.mm,
            Chamber.CathodeCopperSupportThickness.mm * 1.1, "cathodePatternCentralHole"
        )
        val cathodePatternLine =
            gdml.solids.subtraction(cathodePatternLineAux, cathodePatternCentralHole, "cathodePatternLine")

        val cathodePatternDisk = gdml.solids.tube(
            Chamber.CathodePatternDiskRadius.mm,
            Chamber.CathodeCopperSupportThickness.mm, "cathodePatternDisk"
        ) { rmin = Chamber.CathodePatternDiskRadius.mm - Chamber.CathodePatternLineWidth.mm }


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
            Chamber.CathodeTeflonDiskHoleRadius.mm,
            Chamber.CathodeTeflonDiskThickness.mm,
            "cathodeFillingBaseSolid"
        )
        val cathodeFillingSolid =
            gdml.solids.subtraction(cathodeFillingBaseSolid, cathodeCopperDiskFinal, "cathodeFillingSolid") {
                position(z = -Chamber.CathodeTeflonDiskThickness.mm / 2 + Chamber.CathodeCopperSupportThickness.mm / 2)
            }
        val cathodeFillingVolume =
            gdml.structure.volume(Materials.Vacuum.ref, cathodeFillingSolid, "cathodeFillingVolume") {}

        // gas
        val gasSolidOriginal = gdml.solids.tube(
            Chamber.Diameter.mm / 2 - Chamber.TeflonWallThickness.mm,
            Chamber.Height.mm, "gasSolidOriginal"
        )
        val gasSolidAux = gdml.solids.subtraction(gasSolidOriginal, copperReadoutSolid, "gasSolidAux") {
            position(z = -Chamber.Height.mm / 2 + Chamber.ReadoutCopperThickness.mm / 2)
            rotation(z = 45) { unit = AUnit.DEG }
        }
        val gasSolid =
            gdml.solids.subtraction(gasSolidAux, cathodeWindowSolid, "gasSolid") {
                position(z = Chamber.Height.mm / 2 - Chamber.CathodeWindowThickness.mm / 2)
            }
        val gasVolume = gdml.structure.volume(Materials.Gas.ref, gasSolid, "gasVolume")

        return@lazy gdml.structure.assembly {
            name = "Chamber"
            physVolume(gasVolume, name = "gas")
            physVolume(chamberBackplateVolume, name = "chamberBackplate") {
                position(
                    z = -Chamber.Height.mm / 2 - Chamber.ReadoutKaptonThickness.mm - Chamber.BackplateThickness.mm / 2
                )
            }
            physVolume(chamberBodyVolume, name = "chamberBody")
            physVolume(chamberTeflonWallVolume, name = "chamberTeflonWall")
            physVolume(kaptonReadoutVolume, name = "kaptonReadout") {
                position(z = -Chamber.Height.mm / 2 - Chamber.ReadoutKaptonThickness.mm / 2)
            }
            physVolume(copperReadoutVolume, name = "copperReadout") {
                position(z = -Chamber.Height.mm / 2 + Chamber.ReadoutCopperThickness.mm / 2)
                rotation(z = 45) { unit = AUnit.DEG }
            }
            physVolume(cathodeWindowVolume, name = "cathodeWindow") {
                position(z = Chamber.Height.mm / 2 - Chamber.CathodeWindowThickness.mm / 2)
            }
            physVolume(cathodeTeflonDiskVolume, name = "cathodeTeflonDisk") {
                position(z = Chamber.Height.mm / 2 + Chamber.CathodeTeflonDiskThickness.mm / 2)
            }
            physVolume(cathodeFillingVolume, name = "cathodeFilling") {
                position(z = Chamber.Height.mm / 2 + Chamber.CathodeTeflonDiskThickness.mm / 2)
            }
            physVolume(cathodeCopperDiskVolume, name = "cathodeCopperDiskPattern") {
                position(z = Chamber.Height.mm / 2 + Chamber.CathodeCopperSupportThickness.mm / 2)
            }
        }
    }

    return chamberVolume
}

