import space.kscience.gdml.*

enum class Shielding(val mm: Double) {
    SizeXY(590.0), SizeZ(540.0),
    ShaftShortSideX(194.0), ShaftShortSideY(170.0),
    ShaftLongSide(340.0),
    DetectorToShieldingSeparation(-60.0), EnvelopeThickness(10.0),
    OffsetZ(DetectorToShieldingSeparation.mm + Chamber.Height.mm / 2 + Chamber.ReadoutKaptonThickness.mm + Chamber.BackplateThickness.mm),
}

fun shielding(gdml: Gdml): GdmlRef<GdmlAssembly> {
    val shieldingVolume: GdmlRef<GdmlAssembly> by lazy {
        val leadBoxSolid =
            gdml.solids.box(Shielding.SizeXY.mm, Shielding.SizeXY.mm, Shielding.SizeZ.mm, "leadBoxSolid")
        val leadBoxShaftSolid =
            gdml.solids.box(
                Shielding.ShaftShortSideX.mm,
                Shielding.ShaftShortSideY.mm,
                Shielding.ShaftLongSide.mm,
                "leadBoxShaftSolid"
            )
        val leadBoxWithShaftSolid =
            gdml.solids.subtraction(leadBoxSolid, leadBoxShaftSolid, "leadBoxWithShaftSolid") {
                position(z = Shielding.SizeZ.mm / 2 - Shielding.ShaftLongSide.mm / 2)
            }
        val leadShieldingVolume = gdml.structure.volume(Materials.Lead.ref, leadBoxWithShaftSolid, "ShieldingVolume")

        return@lazy gdml.structure.assembly {
            name = "Shielding"
            physVolume(leadShieldingVolume, name = "shielding20cm") {
                position(z = -Shielding.OffsetZ.mm)
            }
        }
    }

    return shieldingVolume
}
