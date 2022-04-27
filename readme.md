----

# Turn RPG system #

> * Kokku Software Engineer Applicant Test
> * Current Version: 1.0.1

----

### TODO List: ###

> * [x] This application is an auto-battle RPG, that has a "grid" with cells/tiles/boxes organized as a matrix.
> * [x] This grid provides the length for the x and y-axis.
> * [x] There is also a Character with a Name, Health, BaseDamage, DamageMultiplier, current cell/box, target, and an Index.
> * [x] Each team should have one move per turn (except when the move places the character in attack range of an opposing team character)
> * [x] The game should work with a "battlefield" of any size, including a non-square matrix, for example 6x6 or 6x10 tiles in the grid.
> * [x] Make sure all the variables in CHARACTER are engaged in a code feature.
> * [x] The game should inform the player when the battle is over and which team has been declared victorious.
> * [x] The battlefield should only be reprinted/redrawn if a player makes a move.
> * [x] Each character should look for a possible target and attack it when this is viable and if not, move closer into attack range.
> * [x] Each candidate must also implement one of the following extra features in the application, to be selected depending on their month of birth.
> * [x] Characters cannot be on the same tile at the same time.

----

### Change log: ###

> version 1.0.1:

> > * Correction of readme.md file (incorrect markdown data).

> version 1.0.0:

> > * Creation of a readme.md file to keep the version/project information
> > * Changes to the folder/files organization (added a source folder to hold files).
> > * Changed `includes` in .cpp files to match the new folder organization.
> > * Included a .gitignore file to the project main folder.
> > * Renamed file AutoBattleC++.cpp to main.cpp and moved it to the src root folder.

> version 0.0.0:
    
> > * Creation of the project.