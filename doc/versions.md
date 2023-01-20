# Versions in ++C

This version format will be used all throughout the ++C ecosystem, so it is important to be familiar with this

## What is a breaking change?

A breaking change is a change in the code, that makes previous code, using the API, broken. There are two types of breaking changes: big ones and linking ones. A linking breaking change is a breaking change that only prevents the API to be linked to a project, requiring an older version, so a linking breaking change requires a recompilation. A big breaking change makes old code incompatible, so it requires any amount of tweaking to get the old code running again.

## What is an incremental change in ++C?

An incremental change is a change in the code base that doesn't affect compilation or linking of older code in no capacity. Such changes may expose new API, but never break old one.

## Segments

A version in ++C is made up of three segments: major (release), minor (breaking) and revision (incremental).

### Major (release)

The major segment is a 16-bit unsigned integer. It is used to represent big breaking changes, usually complete overhauls of the public API. This version segment has to be equal to the target version, so that the target is compatible. This version segment rarely changes.

### Minor (breaking)

The minor segment is a 16-bit unsigned integer. It is used to represent linking breaking changes. All code, compiled with a previous minor version must be compatible with the current minor version, but liking to a different minor version will be broken.

### Revision (incremental)

The revision segment is a 32-bit unsigned integer. It represents incremental changes. All code, compiled with a previous minor version must be compatible with the current minor version, and linking to a newer revision version must be possible.
