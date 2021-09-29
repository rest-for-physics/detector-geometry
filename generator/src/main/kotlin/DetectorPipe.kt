import space.kscience.gdml.*

enum class DetectorPipe(val mm: Double) {
    TotalLength(491.0),

    // Outside
    ChamberFlangeThickness(14.0), ChamberFlangeRadius(Chamber.SquareSide.mm / 2),
    TelescopeFlangeThickness(18.0), TelescopeFlangeRadius(150.0 / 2),
    Section2of2Length(150.0 - TelescopeFlangeThickness.mm),
    Section1of2Length(TotalLength.mm - TelescopeFlangeThickness.mm - ChamberFlangeThickness.mm - Section2of2Length.mm),
    OuterRadius1(92.0 / 2), OuterRadius2(108.0 / 2),
    Union1Z(ChamberFlangeThickness.mm / 2 + Section1of2Length.mm / 2),
    Union2Z(Union1Z.mm + Section1of2Length.mm / 2 + Section2of2Length.mm / 2),
    Union3Z(Union2Z.mm + Section2of2Length.mm / 2 + TelescopeFlangeThickness.mm / 2),

    // Inside
    InsideSection1of3Radius(43.0 / 2), InsideSection2of3Radius(68.0 / 2), InsideSection3of3Radius(85.0 / 2),
    InsideSectionTelescopeRadius(108.0 / 2),
    InsideCone1of3Length(21.65), InsideCone2of3Length(14.72), InsideCone3of3Length(9.0),
    InsideSection3of3Length(115.0 - InsideCone3of3Length.mm),
    InsideSection2of3Length(290.0 - InsideSection3of3Length.mm - InsideCone3of3Length.mm - InsideCone2of3Length.mm),
    InsideSection1of3Length(201.0 - InsideCone1of3Length.mm),
    InsideUnion1Z(InsideSection1of3Length.mm / 2 + InsideCone1of3Length.mm / 2),
    InsideUnion2Z(InsideUnion1Z.mm + InsideCone1of3Length.mm / 2 + InsideSection2of3Length.mm / 2),
    InsideUnion3Z(InsideUnion2Z.mm + InsideSection2of3Length.mm / 2 + InsideCone2of3Length.mm / 2),
    InsideUnion4Z(InsideUnion3Z.mm + InsideCone2of3Length.mm / 2 + InsideSection3of3Length.mm / 2),
    InsideUnion5Z(InsideUnion4Z.mm + InsideSection3of3Length.mm / 2 + InsideCone3of3Length.mm / 2),
    FillingOffsetWithPipe(InsideSection1of3Length.mm / 2 - ChamberFlangeThickness.mm / 2),

    // World
    ZinWorld(ChamberFlangeThickness.mm / 2 + Chamber.Height.mm / 2 + Chamber.CathodeTeflonDiskThickness.mm),
}

fun detectorPipe(gdml: Gdml): GdmlRef<GdmlAssembly> {
    val detectorPipeVolume: GdmlRef<GdmlAssembly> by lazy {

        val detectorPipeChamberFlangeSolid = gdml.solids.tube(
            DetectorPipe.ChamberFlangeRadius.mm,
            DetectorPipe.ChamberFlangeThickness.mm,
            "detectorPipeChamberFlangeSolid"
        )
        val detectorPipeTelescopeFlangeSolid = gdml.solids.tube(
            DetectorPipe.TelescopeFlangeRadius.mm,
            DetectorPipe.TelescopeFlangeThickness.mm,
            "detectorPipeTelescopeFlangeSolid"
        )
        val detectorPipeSection1of2Solid =
            gdml.solids.tube(
                DetectorPipe.OuterRadius1.mm,
                DetectorPipe.Section1of2Length.mm,
                "detectorPipeSection1of2Solid"
            )
        val detectorPipeSection2of2Solid =
            gdml.solids.tube(
                DetectorPipe.OuterRadius2.mm,
                DetectorPipe.Section2of2Length.mm,
                "detectorPipeSection2of2Solid"
            )
        val detectorPipeAux1 =
            gdml.solids.union(detectorPipeChamberFlangeSolid, detectorPipeSection1of2Solid, "detectorPipeAux1")
            {
                position(z = DetectorPipe.Union1Z.mm)
            }
        val detectorPipeAux2 =
            gdml.solids.union(detectorPipeAux1, detectorPipeSection2of2Solid, "detectorPipeAux2")
            {
                position(z = DetectorPipe.Union2Z.mm)
            }
        val detectorPipeNotEmpty =
            gdml.solids.union(detectorPipeAux2, detectorPipeTelescopeFlangeSolid, "detectorPipeNotEmpty")
            {
                position(z = DetectorPipe.Union3Z.mm)
            }
        val detectorPipeInside1of3Solid = gdml.solids.tube(
            DetectorPipe.InsideSection1of3Radius.mm,
            DetectorPipe.InsideSection1of3Length.mm,
            "detectorPipeInside1of3Solid"
        )
        val detectorPipeInside2of3Solid = gdml.solids.tube(
            DetectorPipe.InsideSection2of3Radius.mm,
            DetectorPipe.InsideSection2of3Length.mm,
            "detectorPipeInside2of3Solid"
        )
        val detectorPipeInside3of3Solid = gdml.solids.tube(
            DetectorPipe.InsideSection3of3Radius.mm,
            DetectorPipe.InsideSection3of3Length.mm,
            "detectorPipeInside3of3Solid"
        )
        val detectorPipeInsideCone1of3Solid = gdml.solids.cone(
            DetectorPipe.InsideCone1of3Length.mm,
            DetectorPipe.InsideSection1of3Radius.mm, DetectorPipe.InsideSection2of3Radius.mm,
            "detectorPipeInsideCone1of3Solid"
        )
        val detectorPipeInsideCone2of3Solid = gdml.solids.cone(
            DetectorPipe.InsideCone2of3Length.mm,
            DetectorPipe.InsideSection2of3Radius.mm, DetectorPipe.InsideSection3of3Radius.mm,
            "detectorPipeInsideCone2of3Solid"
        )
        val detectorPipeInsideCone3of3Solid = gdml.solids.cone(
            DetectorPipe.InsideCone3of3Length.mm,
            DetectorPipe.InsideSection3of3Radius.mm, DetectorPipe.InsideSectionTelescopeRadius.mm,
            "detectorPipeInsideCone3of3Solid"
        )
        val detectorPipeInsideAux1 =
            gdml.solids.union(
                detectorPipeInside1of3Solid,
                detectorPipeInsideCone1of3Solid,
                "detectorPipeInsideAux1"
            ) {
                position(z = DetectorPipe.InsideUnion1Z.mm)
            }
        val detectorPipeInsideAux2 =
            gdml.solids.union(detectorPipeInsideAux1, detectorPipeInside2of3Solid, "detectorPipeInsideAux2") {
                position(z = DetectorPipe.InsideUnion2Z.mm)
            }
        val detectorPipeInsideAux3 =
            gdml.solids.union(
                detectorPipeInsideAux2,
                detectorPipeInsideCone2of3Solid,
                "detectorPipeInsideAux3"
            ) {
                position(z = DetectorPipe.InsideUnion3Z.mm)
            }
        val detectorPipeInsideAux4 =
            gdml.solids.union(detectorPipeInsideAux3, detectorPipeInside3of3Solid, "detectorPipeInsideAux4") {
                position(z = DetectorPipe.InsideUnion4Z.mm)
            }
        val detectorPipeInside =
            gdml.solids.union(detectorPipeInsideAux4, detectorPipeInsideCone3of3Solid, "detectorPipeInside") {
                position(z = DetectorPipe.InsideUnion5Z.mm)
            }
        val detectorPipeSolid =
            gdml.solids.subtraction(detectorPipeNotEmpty, detectorPipeInside, "detectorPipeSolid") {
                position(z = DetectorPipe.InsideSection1of3Length.mm / 2 - DetectorPipe.ChamberFlangeThickness.mm / 2)
            }
        val detectorPipeVolume = gdml.structure.volume(Materials.Copper.ref, detectorPipeSolid, "detectorPipeVolume")
        val detectorPipeFillingVolume =
            gdml.structure.volume(Materials.Vacuum.ref, detectorPipeInside, "detectorPipeFillingVolume")

        return@lazy gdml.structure.assembly {
            name = "DetectorPipe"
            physVolume(detectorPipeVolume) {
                name = "detectorPipe"
            }
            physVolume(detectorPipeFillingVolume) {
                name = "detectorPipeFilling"
                position { z = DetectorPipe.FillingOffsetWithPipe.mm }
            }
        }
    }

    return detectorPipeVolume
}
