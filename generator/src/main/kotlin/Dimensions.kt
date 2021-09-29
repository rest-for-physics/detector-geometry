// vetoes
enum class VetoLengths(val mm: Double, val namePrefix: String) {
    LongSideStandard(800.0, "standard"), LongSideSmall(300.0, "small"), LongSideLarge(1500.0, "large"),
}

enum class Veto(val mm: Double) {
    Width(200.0), Thickness(50.0), LongSide(VetoLengths.LongSideStandard.mm),
    CaptureLayerThickness(1.0), WrappingThickness(1.0),
    SeparationAdjacent(5.0),
    FullThickness(Thickness.mm + 2 * WrappingThickness.mm + 2 * CaptureLayerThickness.mm),

    // light guide and PMT
    LightGuide1Length(130.0), LightGuide2Length(80.0),
    PhotomultiplierLength(233.5), PhotomultiplierDiameter(70.0),
}
