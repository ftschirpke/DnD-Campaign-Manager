#define X_OWNED_CONTENT_PIECES(X)                                                                                      \
    X(Character, CHARACTER, character, character, characters, Characters)                                              \
    X(Class, CLASS, class, cls, classes, Classes)                                                                      \
    X(Subclass, SUBCLASS, subclass, subclass, subclasses, Subclasses)                                                  \
    X(Species, SPECIES, species, species, species, Species)                                                            \
    X(Subspecies, SUBSPECIES, subspecies, subspecies, subspecies, Subspecies)                                          \
    X(Item, ITEM, item, item, items, Items)                                                                            \
    X(Spell, SPELL, spell, spell, spells, Spells)                                                                      \
    X(Choosable, CHOOSABLE, choosable, choosable, choosables, Choosables)

#define X_FEATURES(X)                                                                                                  \
    X(Feature, FEATURE, feature, feature, features, Features)                                                          \
    X(ClassFeature, CLASS_FEATURE, class_feature, class_feature, class_features, ClassFeatures)                        \
    X(SubclassFeature, SUBCLASS_FEATURE, subclass_feature, subclass_feature, subclass_features, SubclassFeatures)

#define X_REFERENCE_CONTENT_PIECES(X) X_FEATURES(X)

#define X_CONTENT_PIECES(X)                                                                                            \
    X_OWNED_CONTENT_PIECES(X)                                                                                          \
    X_REFERENCE_CONTENT_PIECES(X)
