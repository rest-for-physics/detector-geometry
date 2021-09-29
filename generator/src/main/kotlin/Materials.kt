import space.kscience.gdml.*

val materialsUrl = "https://raw.githubusercontent.com/lobis/geant4-materials-xml/main/materials.xml"

private val materialsGdml = Gdml {
    loadMaterialsFromUrl(materialsUrl)
}

private val materials = materialsGdml.materials

private fun resolve(tag: String): GdmlRef<GdmlMaterial> {
    return materials.get<GdmlMaterial>(tag)?.ref()
        ?: throw Exception("Material '$tag' does not exist! It should be present in $materialsUrl")
}

enum class Materials(val ref: GdmlRef<GdmlMaterial>) {
    Air(resolve("G4_AIR")),
    Gas(resolve("G4_Ar")),
    Vacuum(resolve("G4_Galactic")),
    Copper(resolve("G4_Cu")),
    Teflon(resolve("G4_TEFLON")),
    Kapton(resolve("G4_KAPTON")),
    Mylar(resolve("G4_MYLAR")),
    Lead(resolve("G4_Pb")),

}

fun resolveMaterialByRef(material: Materials): GdmlMaterial {
    return material.ref.resolve(materialsGdml)!!
}